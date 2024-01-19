#pragma once

#include "source/socket.h"
#include "url.h"
#include <stdint.h>

#include <map>
#include <memory>
#include <utility>
#include <vector>

namespace http 
{

using header_ent = std::pair<std::string, std::string>;

struct response
{
    uint16_t StatusCode;
    std::multimap<std::string, std::string> Headers;
    std::string Body;

    inline response() = default;
    inline response(const response& rhs)
    {
        StatusCode = rhs.StatusCode;
        Headers.clear();
        Headers = rhs.Headers;
        Body = rhs.Body;
    }

    inline response& operator=(const response& rhs)
    {
        StatusCode = rhs.StatusCode;
        Headers = rhs.Headers;
        Body = rhs.Body;
        return *this;
    }

    inline response& operator=(response&& rhs)
    {
        std::swap(rhs.StatusCode, StatusCode);
        std::swap(rhs.Headers, Headers);
        std::swap(rhs.Body, Body);
        return *this;
    }

};

}
