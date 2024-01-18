#include "request.h"
#include "source/session.h"
#include "source/socket.h"
#include "url.h"

#include <charconv>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <memory>
#include <sstream>
#include <algorithm>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <assert.h>

#include <format>

http::CRequestClient::CRequestClient(http::url url)
    : m_Url(url),
      m_RedirectionLevel(0)
{}

http::response http::CRequestClient::Perform() noexcept
{
    if (m_Url.Scheme == "file")
    {
        return this->ReadFile(m_Url.Host + m_Url.Path);
    }

    {
        size_t dataptr;
        std::unique_ptr<ISocket> sock = this->GetSocketForHost();
        std::string contents = std::format("GET {} HTTP/1.1\r\nHost: {}\r\nConnection: close\r\n", 
                                m_Url.Path, m_Url.Host);

        sock->Write(contents.c_str(), contents.size());    
        for (header_ent& header : m_Headers)
        {
            contents = std::format("{}: {}\r\n", header.first, header.second);
            sock->Write(contents.c_str(), contents.size());
        }
        sock->Write("\r\n", 2);

        while (true)
        {
            dataptr = m_Response.size();
            m_Response.resize(dataptr + 1024);
            int bytesre = sock->Read(&m_Response[dataptr], 1024);
            if (bytesre <= 0)
            {
                break;
            }
        }

        m_Response.shrink_to_fit();
    }

    response re = this->HandleResponse(m_Response.data());
    std::printf("re.StatusCode = %i\n", re.StatusCode);
    if (re.StatusCode == 301)
    {
        if (m_RedirectionLevel > 5)
        {
            return this->ReadFile("../redir.html");
        }
/*
        if (re.Headers.contains("Location"))
        {
            auto loc = re.Headers.at("Location");
            m_RedirectionLevel++;
            
            CRequestClient client(*this);
            this->SetUrl(m_Url.HandleRelative(loc));
            std::printf("redir to %s://%s\n", m_Url.Scheme.c_str(), m_Url.Host.c_str());
            m_Response.clear();
            auto cl = this->Perform();
            return cl;
        }
*/
    }

    return re;
}

std::unique_ptr<http::ISocket> 
http::CRequestClient::GetSocketForHost() noexcept
{
    addrinfo* info;
    addrinfo* link;
    addrinfo hint = {0};
    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_STREAM;

    assert(m_Url.Scheme == "http" 
#ifdef HTTP_WANT_TLS
        || m_Url.Scheme == "https"
#endif
        );

    int r = getaddrinfo(m_Url.Host.c_str(), m_Url.Scheme.c_str(), &hint, &info);
    link = info;
    int sock;
    while (link != nullptr)
    {
        sock = socket(link->ai_family, link->ai_socktype, link->ai_protocol);
        if (sock == -1)
            continue;

        if (!connect(sock, link->ai_addr, link->ai_addrlen))
            break;

        close(sock);
        link = link->ai_next;
    }

    http::ISocket* inf;

#ifdef HTTP_WANT_TLS
    if (m_Url.Scheme == "https")
    {
        inf = new http::CEncryptedSocket();
        inf->SetHostName(m_Url.Host);
        inf->SetHandle(sock);
    }
#endif

    if (m_Url.Scheme == "http")
    {
        inf = new http::CHttpSocket();
        inf->SetHandle(sock);
    }

    freeaddrinfo(info);
    return std::unique_ptr<ISocket>(inf);
}

http::response http::CRequestClient::HandleResponse(std::string_view buffer) noexcept
{
    response resp;
    uint16_t statuscode;
    int result = buffer.compare("HTTP/1.");
    if (result == 0)
    {
        std::printf("Not an HTTP result\n");
    }

    auto scdata = buffer.substr(9);
    std::from_chars(scdata.data(), scdata.data() + 3, statuscode);

    size_t headeridx = buffer.find_first_of("\r\n") + 2;
    auto headerbegin = buffer.substr(headeridx);
    size_t headerSize = this->HandleHeaders(buffer, resp);
    resp.Body = m_Response.substr(headerSize + headeridx, m_Response.size() - (headerSize + headeridx));

    resp.StatusCode = statuscode;
    return resp;
}

void http::CRequestClient::AddHeader(http::header_ent header) noexcept
{
    m_Headers.push_back(header);
}

void http::CRequestClient::SetUrl(struct url url) noexcept
{
    m_Url = url;
}

size_t http::CRequestClient::HandleHeaders(std::string_view buffer,
                                         http::response& rp) noexcept
{
    auto headerbegin = buffer.substr(buffer.find_first_of("\r\n") + 2);
    bool lookingForKey = true;
    size_t index = 0, begin = 0;
    size_t sequentialCrlfs = 0;
    std::string hdname, hdval;

    while(index < headerbegin.size())
    {
        if (headerbegin[index] == ':' && lookingForKey)
        {
            sequentialCrlfs = 0;
            hdname.clear();
            if (headerbegin[begin] == '\n') 
                begin++;

            hdname = headerbegin.substr(begin, index - begin);
            begin = index + 2; /* skip the ": " part  */
            lookingForKey = false;
        }

        if (headerbegin[index] == '\n' && !lookingForKey)
        {
            if (sequentialCrlfs > 1)
            {
                break;
            }

            hdval.clear();
            hdval = headerbegin.substr(begin, index - begin - 1);
            begin = index;
            rp.Headers.emplace(hdname, hdval);

            lookingForKey = true;
            sequentialCrlfs++;
        }

        index++;
    }

    for (auto& h : rp.Headers)
    {
        std::printf("h.name = %s, h.val = %s\n", h.first.c_str(), h.second.c_str());
    }

    return index;
}

http::response http::CRequestClient::ReadFile(std::string_view filename)
{
    using namespace std::string_literals;

    char* bd;
    int readb;
    size_t fileSize;
    std::string files, body;
    files.reserve(10);

    struct stat st;
    int result = stat(filename.data(), &st);
    if (result == -1)
    {
        if (errno == ENOENT)
        {
            std::printf("Request to read file %s failed: ENOENT\n", filename.data());
            http::response rep;
            rep.StatusCode = 404;
            rep.Body = "";
            rep.Headers = {};
            return rep;
        }
    }

    FILE* fi = fopen(filename.data(), "r");
    fseek(fi, 0, SEEK_END);
    fileSize = ftell(fi);
    fseek(fi, 0, SEEK_SET);

    bd = new char[fileSize + 1];
    readb = fread(bd, 1, fileSize, fi);
    if (readb != fileSize)
    {
        printf("CRequestClient::ReadFile: did not read entire file!"
                        " (%lu (total) != %i (read))\n", 
            fileSize, readb);
    }

    body = std::string(bd, fileSize);
    delete[] bd;
    fclose(fi);
    std::to_chars(files.data(), files.data() + files.capacity(), fileSize);
    http::response rep;
    rep.Body = body;
    rep.StatusCode = 200;
    return rep;
}
