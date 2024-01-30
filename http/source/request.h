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
    http::url Url; /* URL after redirections */

    inline response() = default;
};

}
