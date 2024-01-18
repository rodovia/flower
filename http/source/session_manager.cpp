#include "session_manager.h"
#include "session.h"

#include <sys/stat.h>
#include <charconv>
#include <algorithm>

static http::response ReadFile(std::string_view filename)
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
        printf("ReadFile: did not read entire file!"
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

std::shared_ptr<http::CHttpSession>
http::CSessionManager::FillSession(const http::url& origin)
{
    auto sess = std::find_if(m_Sessions.begin(), m_Sessions.end(), 
                [&](const std::shared_ptr<CHttpSession>& sess)
                    { return sess->GetHost() == origin.Host; });
    
    if (sess == m_Sessions.end())
    {
        auto cre = std::make_shared<CHttpSession>(origin.Host, origin.Scheme);
        m_Sessions.push_back(cre);
        return cre;
    }

    return *sess;
}

http::either<http::response, int> 
http::Fetch(const http::url& origin, header_map headers)
{
    if (origin.Scheme == "file")
    {
        return ReadFile(origin.Host + origin.Path);
    }

    auto& ins = CSessionManager::GetInstance();
    auto sess = ins.FillSession(origin);
    return sess->PerformRequest(origin.Path, headers);
}
