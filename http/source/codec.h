/* codec.h
   Purpose: HTTP-compliant string creation */

#pragma once

#include "source/session.h"
#include <string>

namespace http
{

enum http_version
{
    kHttpVersion10, /* HTTP/1.0 */
    kHttpVersion11  /* HTTP/1.1 */
};

std::string CreateHeadingWith(http_version version,
                              std::string_view path, 
                              const header_map& headermap);

int ParserHeadersStatus(std::string_view origin, 
                        http::header_map& headers,
                        int& status);

int ReadChunkedContent(std::string_view buffer, std::string& result);

}
