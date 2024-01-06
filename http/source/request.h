#pragma once

#include "source/socket.h"
#include "url.h"
#include <stdint.h>

#include <memory>
#include <utility>
#include <vector>

namespace http 
{

using header_ent = std::pair<std::string, std::string>;

struct response
{
    uint16_t StatusCode;
    std::vector<header_ent> Headers;
    std::string Body;
};

class CRequestClient
{
public:
    CRequestClient(struct url url);
    response Perform() noexcept;
    void AddHeader(header_ent ent) noexcept;
    void SetUrl(struct url url) noexcept;

private:
    http::response HandleResponse(std::string_view buffer) noexcept;
    size_t HandleHeaders(std::string_view buffer, response& rp) noexcept;
    std::unique_ptr<ISocket> GetSocketForHost() noexcept;
    http::response ReadFile(std::string_view filename);

    url m_Url;
    int m_RedirectionLevel;
    std::vector<header_ent> m_Headers;
    std::vector<char> m_Response;
};

}
