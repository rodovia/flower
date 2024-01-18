/* session.h
   Purpose: keep-alive capability */

#pragma once

#include "request.h"
#include "url.h"
#include "socket.h"
#include "async/future.h"

#include <memory>
#include <map>
#include <queue>

namespace http
{

using header_map = std::multimap<std::string, std::string>;

struct request_entry
{
    std::string Path;
    header_map Headers;
};

class CHttpSession
{
public:
    CHttpSession(std::string host, std::string_view scheme = "http");

    constexpr std::string_view GetHost()
    {
        return m_Host;
    }

    either<response, int> PerformRequest(std::string path, 
                                         header_map headers = {});
    void SetUserAgent(std::string ua);
private:
    void ReadChunked(std::string_view bodyRemainder);
    void PerformRequestInner(const request_entry&);
    static void* RequestThreadThunk(void*);
    pthread_t CreateRequestThread();

    std::string m_Host;
    std::string_view m_Scheme;
    std::unique_ptr<ISocket> m_Socket;

    CFuture<response> m_Future;
    pthread_t m_RequestThread;
    pthread_mutex_t m_RequestQueueMutex;
    pthread_cond_t m_RequestQueueCvar;
    std::queue<request_entry> m_RequestQueue;
};

}

