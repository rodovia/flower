/* session_manager.h 
   Purpose: Manage various HTTP sessions */

#pragma once

#include "session.h"
#include "async/future.h"
#include "request.h"
#include <vector>

namespace http
{

class CSessionManager
{
public:
    static CSessionManager& GetInstance()
    {
        static CSessionManager man;
        return man;
    }

    std::shared_ptr<CHttpSession> FillSession(const http::url& origin);

private:
    std::vector<std::shared_ptr<CHttpSession>> m_Sessions;
};

http::either<response, int> Fetch(const http::url& origin, header_map headers = {});

}

