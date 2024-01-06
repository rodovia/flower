#pragma once

#include <string>

namespace http
{

struct url
{
    std::string Scheme;
    std::string Host;
    std::string Path;
    std::string Fragment;

    url() = default;
    url(std::string nonparsed);
};

}
