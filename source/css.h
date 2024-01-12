/* css.h
   Purpose: defines stuff for parsing Cascading Style Sheets (CSS) */

#pragma once

#include <cstring>
#include <pango/pango-font.h>
#include <string>
#include <stdint.h>
#include <map>
#include <utility>
#include <vector>
#include <memory>

#include "css/css_selector.h"
#include "css/css_color.h"
#include "flower_http.h"

/* Workaround GCC bug 98752 (wrong error when consteval ctor).
  Seems to be fixed in GCC 14 (TODO: verify) */
#if defined(__GNUC__) 
#if __GNUC__ >= 14
#   define __consteval_or_nothing consteval
#   define __constexpr_or_nothing constexpr
#else
#   define __consteval_or_nothing
#   define __constexpr_or_nothing
#endif
#else
#   define __consteval_or_nothing consteval
#   define __constexpr_or_nothing constexpr
#endif

namespace css
{

enum css_value_type : unsigned short
{
    kCssValueTypeUnknown,
    kCssValueTypeKeywordOrString,
    kCssValueTypeInteger,
    kCssValueTypeColor
};

struct css_basic_value
{
    __consteval_or_nothing css_basic_value(css_value_type type = kCssValueTypeUnknown)
                : Type(type)
    {}

    css_value_type Type;
};

struct css_int : public css_basic_value
{
    __consteval_or_nothing css_int(int integer)
                : Integer(integer),
                css_basic_value(kCssValueTypeInteger)
    {}

    int Integer;
};

struct css_string : public css_basic_value
{
    __constexpr_or_nothing css_string(std::string string)
        : css_basic_value(kCssValueTypeKeywordOrString),
          String(string)
    {}

    std::string String;
};

struct css_color : public css_basic_value
{
    __consteval_or_nothing css_color()
        : css_basic_value(kCssValueTypeColor)
    {};

    explicit __constexpr_or_nothing css_color(uint32_t i32)
        : css_color((i32 & 0xFF0000) >> 16, (i32 & 0x00FF00) >> 8, i32 & 0x0000FF)
    {}

    __constexpr_or_nothing css_color(uint8_t red, uint8_t green, 
                        uint8_t blue, float alpha = 1.0f)
        : Red(static_cast<float>(red) / 255),
          Green(static_cast<float>(green) / 255),
          Blue(static_cast<float>(blue) / 255),
          Alpha(alpha),
          css_basic_value(kCssValueTypeColor)
    {};

    float Red, Green, Blue;
    float Alpha;
};

bool GetFontForValue(const css_basic_value& bv, PangoFontFamily*& faml);

using css_kv = std::pair<std::string, std::shared_ptr<css_basic_value>>;
using css_map = std::map<std::string, std::shared_ptr<css_basic_value>>;
using css_rule = std::pair<selector_collection, css_map>;

std::shared_ptr<basic_selector> CreateSelectorFor(std::string_view selector);

class CCascadingParser
{
public:
    CCascadingParser(std::string input, http::url base);
    std::vector<css_rule> ParseBody();
    css_map ParseProperties();

private:
    bool ExpectLiteral(char lit);
    void HandleAtRule();
    void HandleWhitespace();
    void HandleComment();
    css_color HandleColor(std::string_view value);
    css_kv HandlePair();
    std::string HandleName(bool allowSpaces = false);
    std::string_view HandleValue();
    selector_collection HandleSelector();
    
    http::url m_Base;
    std::string m_Input;
    uint32_t m_Index;
};

}