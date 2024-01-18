#include "codec.h"
#include "source/session.h"
#include <charconv>

template<class _Iy, class _By>
constexpr _Iy max(_Iy X, _By y) 
{ 
    return static_cast<_Iy>((X) > (y) ? (X) : (y)); 
}

std::string http::CreateHeadingWith(http_version version, 
                                    std::string_view path,
                                    const header_map& headermap)
{
    std::string result = "GET ";
    result += path.data();

    switch(version)
    {
    case kHttpVersion10:
        result += " HTTP/1.0";
        break;
    case kHttpVersion11:
        result += " HTTP/1.1";
        break;
    }
    result += "\r\n\r\n";

    for (auto& h : headermap)
    {
        result += h.first + ": " + h.second + "\r\n";
    }

    result += "\r\n";
    return result;
}

int http::ParserHeadersStatus(std::string_view origin,
                              http::header_map& headers,
                              int& status)
{
    http::header_map map;
    size_t begin = origin.find_first_of("\r\n") + 2;
    size_t index = begin;

    while(index < origin.size())
    {
        if (origin[index] == '\n')
        {
            auto fld = origin.substr(begin, max(index - begin, 2) - 1);
            if (fld == "\r")
            {
                break;
            }

            auto sepidx = fld.find(':');
            std::string name = std::string(fld.substr(0, sepidx));
            std::string value = std::string(fld.substr(sepidx + 2));

            map.emplace(name, value);
            begin = index + 1;
        }

        index++;
    }
    headers = map;

    {
        int stat;
        constexpr std::string_view reference = "HTTP/1.0 ";
        std::string_view statusstr = origin.substr(reference.size(), 3);
        std::from_chars(statusstr.data(), statusstr.data() + 3, stat);
        status = stat;
    }

    return index;
}

int http::ReadChunkedContent(std::string_view buffer,
                             std::string& result)
{
    int finalLength = 0;
    int length = 0;
    size_t lengthtrack = 0;
    size_t index = 0;
    size_t begin = 0;
    bool lookingForLength = true;

    while(index < buffer.size())
    {
        if (buffer[index] == '\n' 
         && lookingForLength)
        {
            auto lenstr = buffer.substr(begin, index - begin);
            std::from_chars(lenstr.data(), lenstr.data() + lenstr.size(), length, 16);
            begin = index + 1;
            lookingForLength = false;

            if (length == 0) break;
        }

        if (lengthtrack >= length 
         && !lookingForLength)
        {
            auto cttstr = buffer.substr(begin, length);
            lengthtrack = 0;
            index += 3;
            begin = index;
            lookingForLength = true;
            
            result += cttstr;
            finalLength += length;
        }

        if (!lookingForLength) lengthtrack++;
        index++;
    }

    return finalLength;
}
