#include "url.h"
#include <algorithm>
#include <cstdio>
#include <iterator>
#include <string>
#include <string_view>

http::url::url(std::string ur)
{
    int schemepos,
        schemeend,
        pathpos;

    schemepos = ur.find_first_of("://");
    if (schemepos == std::string::npos)
    {
        ur = ur.insert(0, "http://");
        schemepos = ur.find_first_of("://");
    }

    schemeend = schemepos + 3;
    pathpos = ur.find_first_of('/', schemeend);
    if (pathpos == std::string::npos)
    {
        ur.insert(ur.length(), "/");
        pathpos = ur.find_first_of('/', schemeend);
    }

    std::string subscheme = ur.substr(0, schemepos);
    std::string subhost = ur.substr(schemeend, pathpos - schemeend);
    std::string subpath = ur.substr(pathpos);
    Host = subhost;
    Scheme = subscheme;
    Path = subpath;

    if (Scheme == "file")
    {
        auto hostpos = Path.find_last_of('/');
        Host = Path.substr(0, hostpos);
        Path = Path.substr(hostpos);
    }
}

http::url http::url::HandleRelative(std::string_view href)
{
    std::string link = std::string(href);
    http::url url = *this;
    if (href[0] != '/' 
     && href.find("://") == std::string::npos)
    {
        link.insert(0, "/");
    }

    url.Path = link;
    if (link.starts_with("http"))
    {
        url = http::url(link);
    }

    return url;
}
