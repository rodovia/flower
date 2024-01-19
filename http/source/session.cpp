#include "session.h"
#include "socket.h"
#include "source/async/future.h"
#include "source/codec.h"
#include "source/request.h"

#include <asm-generic/errno.h>
#include <pthread.h>
#include <assert.h>
#include <netdb.h>
#include <memory>

#if defined(__linux__)
#define pthread_set_name_np pthread_setname_np 
#elif defined(__NetBSD__)
#define pthread_set_name_np(T, n) pthread_setname_np(T, n, nullptr)
#endif

template<class _Key, class _Ty>
static inline _Ty GetValue(std::multimap<_Key, _Ty> map, _Key name)
{
    return map.find(name)->second;
}

static std::unique_ptr<http::ISocket> 
GetSocketForHost(const http::url& url) noexcept
{
    addrinfo* info;
    addrinfo* link;
    addrinfo hint = {0};
    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_STREAM;

    assert(url.Scheme == "http" 
#ifdef HTTP_WANT_TLS
        || url.Scheme == "https"
#endif
        );

    int r = getaddrinfo(url.Host.c_str(), url.Scheme.c_str(), &hint, &info);
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
    if (url.Scheme == "https")
    {
        inf = new http::CEncryptedSocket();
        inf->SetHostName(url.Host);
        inf->SetHandle(sock);
    }
#endif

    if (url.Scheme == "http")
    {
        inf = new http::CHttpSocket();
        inf->SetHandle(sock);
    }

    freeaddrinfo(info);
    return std::unique_ptr<http::ISocket>(inf);
}

http::CHttpSession::CHttpSession(std::string host, std::string_view scheme)
    : m_Host(host),
      m_RequestThread(0),
      m_Scheme(scheme)
{

}

http::either<http::response, int> 
http::CHttpSession::PerformRequest(std::string path, 
                                   header_map headers)
{
    headers.emplace("Host", m_Host);
    headers.emplace("Connection" , "keep-alive");
    headers.emplace("User-Agent", "Mozilla/5.0 (Linux) FlowerIsBetter! (0.1)");

    if (m_RequestThread == 0)
    {
        m_RequestThread = this->CreateRequestThread();
    }

    pthread_mutex_lock(&m_RequestQueueMutex);
    m_RequestQueue.push(request_entry { path, headers });
    pthread_cond_signal(&m_RequestQueueCvar);
    pthread_mutex_unlock(&m_RequestQueueMutex);

    return m_Future.GetResult();
}

pthread_t http::CHttpSession::CreateRequestThread()
{
    pthread_condattr_t condattrib;
    pthread_condattr_init(&condattrib);
    pthread_condattr_setclock(&condattrib, CLOCK_MONOTONIC);
    pthread_cond_init(&m_RequestQueueCvar, &condattrib);
    pthread_mutex_init(&m_RequestQueueMutex, nullptr);

    pthread_t thread;
    pthread_create(&thread, nullptr, &CHttpSession::RequestThreadThunk, this);
    pthread_set_name_np(thread, "HTTP Session");
    return thread;
}

void* http::CHttpSession::RequestThreadThunk(void* cx)
{
    struct timespec timeout;
    timeout.tv_nsec = 0;
    timeout.tv_sec = 60;

    auto cls = reinterpret_cast<CHttpSession*>(cx);
    request_entry req;

    if (!cls->m_Socket.get())
    {
        http::url url;
        url.Host = cls->m_Host;
        url.Scheme = cls->m_Scheme;
        cls->m_Socket = GetSocketForHost(url);
    }

    while (true)
    {
        pthread_mutex_lock(&cls->m_RequestQueueMutex);
        while(cls->m_RequestQueue.empty())
        {
            if (pthread_cond_timedwait(&cls->m_RequestQueueCvar, 
                                       &cls->m_RequestQueueMutex, 
                                       &timeout) == ETIMEDOUT)
            {
                pthread_mutex_unlock(&cls->m_RequestQueueMutex);
                pthread_mutex_destroy(&cls->m_RequestQueueMutex);
                pthread_cond_destroy(&cls->m_RequestQueueCvar);
                cls->m_Socket.reset();
                cls->m_RequestThread = 0;
                return nullptr;
            };
        }
            

        req = cls->m_RequestQueue.back();
        cls->m_RequestQueue.pop();

        pthread_mutex_unlock(&cls->m_RequestQueueMutex);
        cls->PerformRequestInner(req);
    }
}

void http::CHttpSession::PerformRequestInner(const http::request_entry& req)
{
    using namespace std::string_literals;

    auto heading = http::CreateHeadingWith(kHttpVersion11, req.Path, req.Headers);
    std::printf("heading=%s\n", heading.c_str());
    m_Socket->Write(heading.data(), heading.size());

    int status;
    std::string response;
    size_t rlength;
    while(true)
    {
        rlength = response.size();
        response.resize(rlength + 1024);
        rlength = m_Socket->Read(&response[rlength], 1024);
        if (rlength < 1024) break;
    }

    response.shrink_to_fit();

    header_map rep;
    int headersize = http::ParserHeadersStatus(response, rep, status);
    std::string body = response.substr(headersize);

    if (status == 301 || status == 302)
    {
        auto loc = rep.find("location")->second;
        http::url url(loc);
        m_Host = url.Host;
        if (!this->UpgradeSocket(loc))
        {
            /* Trigger a negative result to the future */
        }

        request_entry rent;
        rent.Headers = req.Headers;
        rent.Path = url.Path;
        rent.Headers.erase("Host");
        rent.Headers.emplace("Host"s, m_Host);

        return this->PerformRequestInner(rent);
    }

    if (rep.contains("transfer-encoding") 
     && GetValue(rep, "transfer-encoding"s) == "chunked")
    {
        /* Because... TCP, a chunk can sneak inside 
           response buffer. */
        this->ReadChunked(body);
    }

    if (rep.contains("connection")
     && GetValue(rep, "connection"s) == "close")
    {
        m_Socket.reset(); /* BUG: the socket will not be recreated 
                                  at the next request. */
    }

    http::response resp;
    resp.StatusCode = status;
    resp.Body = body;
    resp.Headers = std::move(rep);
    std::printf("Statuscode=%i\n", status);
    m_Future.SetPositiveResult(resp);
}

void http::CHttpSession::ReadChunked(std::string_view bodyRemainder)
{
    int length;
    std::string body, rawresp;
    http::ReadChunkedContent(bodyRemainder, body);

    size_t rlength;
    while(true)
    {
        rlength = rawresp.size();
        rawresp.resize(rlength + 1024);
        rlength = m_Socket->Read(&rawresp[rlength], 1024);

        int length = http::ReadChunkedContent(&rawresp[rlength], body);
        if (length == 0 || rlength <= 0) break;
    }
}

bool http::CHttpSession::UpgradeSocket(std::string_view newScheme)
{
    if (m_Scheme == "https" && newScheme == "http")
    {
        std::printf("HTTP downgrade\n");
        return false; /* HTTP downgrade */
    }

    http::url url;
    url.Scheme = m_Scheme;
    url.Host = m_Host;
    url = url.HandleRelative(newScheme);

    m_Socket.reset();
    m_Socket = GetSocketForHost(url);
    return true;
}
