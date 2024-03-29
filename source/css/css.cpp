#include "css.h"
#include "configuration.h"
#include "css/css_selector.h"

#include <byteswap.h>
#include <endian.h>
#include <algorithm>
#include <cctype>
#include <charconv>
#include <limits>
#include <memory>
#include <pango/pango-fontmap.h>
#include <pango/pangocairo.h>
#include <string_view>
#include <unordered_map>
#include <frozen/unordered_map.h>
#include <frozen/string.h>

#include "css/css_units.h"
#include "html.h"
#include "html_node.h"
#include "source/request.h"
#include "source/session_manager.h"

static constinit frozen::unordered_map<frozen::string, int, 14> valuetype = {
    { "cm", css::kCssDistanceUnitCentimeter },
    { "mm", css::kCssDistanceUnitMillimeter },
    { "Q", css::kCssDistanceUnitQuaterMillimeter },
    { "in", css::kCssDistanceUnitInch },
    { "pc", css::kCssDistanceUnitPica },
    { "pt", css::kCssDistanceUnitPoint },
    { "px", css::kCssDistanceUnitPixel },

    { "xx-small", css::kCssFontSizeXxSmall },
    { "x-small", css::kCssFontSizeXSmall },
    { "small", css::kCssFontSizeSmall },
    { "medium", css::kCssFontSizeMedium },
    { "large", css::kCssFontSizeLarge },
    { "x-large", css::kCssFontSizeXLarge },
    { "xx-large", css::kCssFontSizeXxLarge }
};

static constexpr float GetFontSizeFactor(css::css_font_size_units abs)
{
    float factor = 1.0f;
    switch (abs)
    {
    case css::kCssFontSizeXxSmall:
        factor = 0.6f;
        break;
    case css::kCssFontSizeXSmall:
        factor = 0.75f;
        break;
    case css::kCssFontSizeSmall:
        factor = 0.88f;
        break;
    case css::kCssFontSizeLarge:
        factor = 1.2f;
        break;
    case css::kCssFontSizeXLarge:
        factor = 2.0f;
        break;
    case css::kCssFontSizeXxLarge:
        factor = 3.0f;
        break;
    default:
        break;
    }

    return factor;
}

static constexpr int GetAbsoluteFontSize(std::string_view unit)
{
    if (unit == "xx-small")
        return css::kCssFontSizeXxSmall;
    if (unit == "x-small")
        return css::kCssFontSizeXSmall;
    if (unit == "small")
        return css::kCssFontSizeSmall;
    if (unit == "medium")
        return css::kCssFontSizeMedium;
    if (unit == "large")
        return css::kCssFontSizeLarge;
    if (unit == "x-large")
        return css::kCssFontSizeXLarge;
    if (unit == "xx-large")
        return css::kCssFontSizeXxLarge;
    
    return 0;
}

static constexpr bool __between(int x, int y, double val)
{
    return ((unsigned int)(val - x) <= y);
}

static constexpr int
GetAbsoluteFontSizeComputed(double computed)
{
    float cmpxxs = computed * GetFontSizeFactor(css::kCssFontSizeXxSmall);
    float cmpxs = computed * GetFontSizeFactor(css::kCssFontSizeXSmall);
    float cmps = computed * GetFontSizeFactor(css::kCssFontSizeSmall);
    float cmpl = computed * GetFontSizeFactor(css::kCssFontSizeLarge);
    float cmpxl = computed * GetFontSizeFactor(css::kCssFontSizeXLarge);
    float cmpxxl = computed * GetFontSizeFactor(css::kCssFontSizeXxLarge);

    if (cmpxxs < computed < cmpxs)
        return css::kCssFontSizeXxSmall;
    else if (cmpxs < computed < cmps)
        return css::kCssFontSizeXSmall;
    else if (cmps < computed < cmpl)
        return css::kCssFontSizeSmall;
    else if (cmpl < computed < cmpxl)
        return css::kCssFontSizeLarge;
    else if (cmpxl < computed < cmpxxl)
        return css::kCssFontSizeXLarge;
    else if (cmpxxl < computed)
        return css::kCssFontSizeXxLarge;
}

/* Used by rgb() and hsl() */
template<class _Ty = uint16_t, size_t _Ns = 3>
static constexpr std::array<_Ty, _Ns> 
GetTuple3(std::string_view functionName, 
          std::string_view value)
{
    char c;
    std::array<_Ty, _Ns> ret;
    bool usesModernSyntax = true;
    int begin = functionName.size() + 1; /* skip the inital function name */
    size_t index = begin, emplacementIndex = 0;
    while(index < value.size())
    {
        c = value[index];
        if (c == ',')
            usesModernSyntax = false;

        if (c == ','
         || c == ')'
         || (usesModernSyntax && c == ' '))
        {
            _Ty numeric = 0;
            if (value[begin] == ',' || value[begin] == ' ') 
                begin++;
            
            auto sub = value.find_first_not_of(' ', begin);
            auto val = value.substr(sub, index - begin);
            std::from_chars(val.data(), val.data() + val.size(), numeric);

            ret.at(emplacementIndex) = numeric;
            emplacementIndex++;

            begin = index;
            if (c == ')') 
                break;
        }

        index++;
    }

    return ret;
}

bool css::GetFontForValue(const css_basic_value& bv, PangoFontFamily*& faml)
{
    if (bv.Type != kCssValueTypeKeywordOrString)
    {
        std::printf("GetFontForValue with bv not a string. Is this a bug?\n");
        return false;
    }

    auto& strv = static_cast<const css_string&>(bv);
    std::string fontname;
    std::string_view str = strv.String;
    size_t index = 0,
           begin = 0;

    while (index <= str.size())
    {
        if (str[index] == ',' || str[index] == '\0')
        {
            int commaskip = (str[index] == ',' ? 1 : 0);
            fontname = str.substr(begin, index - commaskip); /* Do not include the comma 
                                                                       or the null terminator. */
            auto fm = pango_cairo_font_map_get_default();
            if (fontname == "serif"
             || fontname == "cursive"
             || fontname == "fantasy"
             || fontname == "monospace"
             || fontname == "sans-serif")
            {
                if (fontname == "sans-serif") fontname = "sansSerif";
                cfg::g_Configuration.lookupValue("navigator.css.genericFonts." + fontname, fontname);
            }

            auto ff = pango_font_map_get_family(fm, fontname.c_str());
            if (ff != nullptr)
            {
                faml = ff;
                return true;    
            }

            begin = index;
        }

        index++;
    }

    return false;
}

float css::EnsureReferenceUnit(const css_basic_value &bv,
                               css_relative_units_base& base, 
                               std::shared_ptr<html::dom_node> node)
{
    if (bv.Type != kCssValueTypeKeywordOrString)
    {
        return 0.0f;
    }

    auto st = static_cast<const css_string&>(bv);
    auto string = st.String;
    size_t index = 0;

    char c;
    while(index < string.size())
    {
        c = string[index];
        if (!std::isdigit(c) && c != '.')
        {
            break;
        }

        index++;
    }

    float numb;
    auto number = string.substr(0, index);
    auto unstr = string.substr(index);
    auto unit = std::string_view(unstr);
    if (unit == "auto")
    {
        return 0;
    }

    std::from_chars(number.data(), number.data() + number.size(), numb);

    if (unit == "em" 
     || unit == "%"
     || unit == "smaller"
     || unit == "larger"
     || GetAbsoluteFontSize(unit) != 0)
    {
        HandleRelativeUnit(base, node);
        if (unit == "em")
            return base.FontSize * numb;
        if (unit == "%")
            return base.PercentageReference * (numb / 100);
        
        if (unit == "smaller"
         || unit == "larger")
        {
            float factor = 1.0f;
            if (unit == "smaller")
                factor = 0.6f;

            if (unit == "larger")
                factor = 1.2f;

            return base.FontSize * factor;
        }
            
        auto abs = GetAbsoluteFontSize(unit);
        float factor = 1.0f;
    }

    if (unit.empty()) return numb;

    auto val = valuetype.at(unit);
    auto ref = GetValueInReferenceUnits((css_distance_units)val);
    return ref * numb;
}

void css::HandleRelativeUnit(css_relative_units_base& base, 
                              std::shared_ptr<html::dom_node> node)
{
    if (base.FontSize == FLT_MIN)
    {
        if (node->Parent == nullptr)
            base.FontSize = 12.0f;
        else 
            base.FontSize = EnsureReferenceUnit(*node->Parent->Style.at("font-size"),
                                                base, node->Parent);
    }

    if (base.CharacterAdvance == FLT_MIN)
    {
        PangoFontFamily* family;
        auto fam = node->Style.at("font-family");
        GetFontForValue(*fam, family);

    }
}

css::CCascadingParser::CCascadingParser(std::string input, http::url base)
    : m_Index(0),
      m_Input(input),
      m_Base(base)
{

}

void css::CCascadingParser::HandleWhitespace()
{
    while(m_Index < m_Input.size() && std::isspace(m_Input[m_Index]))
    {
        m_Index++;
    }
    this->HandleComment();
}

void css::CCascadingParser::HandleComment()
{
    if (m_Input[m_Index + 1] == '*' 
     && m_Input[m_Index] == '/')
    {
        m_Index += 2;
        while (m_Index < m_Input.size() 
            && (m_Input[m_Index + 1] != '/' 
             && m_Input[m_Index] != '*')) m_Index++;
        m_Index += 2;
    }

    if (std::isspace(m_Input[m_Index]))
    {
        this->HandleWhitespace();
    }
}

std::string css::CCascadingParser::HandleName(bool allowSpaces)
{
    static const std::string_view symbols = "#-.%,()/\"";
    auto start = m_Index;
    if (m_Input[start] == '\"')
    {
        return this->HandleQuotedName();
    }

    while (m_Index < m_Input.size())
    {
        if (std::isalnum(m_Input[m_Index])
            || (allowSpaces && std::isspace(m_Input[m_Index]))
            || symbols.find(m_Input[m_Index]) != std::string_view::npos)
        {
            m_Index++;
        }
        else 
        {
            break;
        }
    }

    auto st = std::string(m_Input.data() + start, m_Index - start);
    return st;
}

bool css::CCascadingParser::ExpectLiteral(char lt)
{
    if ((m_Index < m_Input.size()) && (m_Input[m_Index] != lt))
    {
        std::printf("Unexpected literal %c, expected %c\n", m_Input[m_Index], lt);
        return false;
    }

    m_Index++;
    return true;
}

css::css_map css::CCascadingParser::ParseProperties()
{
    css_kv kv;
    css_map vals;
    while (m_Index < m_Input.size() && m_Input[m_Index] != '}')
    {
        kv = this->HandlePair();
        vals.insert(kv);

        this->HandleWhitespace();
        this->ExpectLiteral(';');
        this->HandleWhitespace();
    }

    return vals;
}

css::css_color 
css::CCascadingParser::HandleColor(std::string_view value)
{
    if (value[0] == '#')
    {
        auto nohash = value.substr(1);
        if (nohash.size() != 3 || nohash.size() != 6)
        {
            /* TODO: ERROR */
        }

        if (nohash.size() == 6)
        {
            uint32_t cvt = 0;
            uint16_t result;
            std::from_chars(nohash.data(), nohash.data() + 6, cvt, 16);
            return css_color(cvt);
        }
    }

    if (value.starts_with("rgb("))
    {
        auto tup = GetTuple3("rgb", value);
        return css_color(std::get<0>(tup), 
                         std::get<1>(tup),
                         std::get<2>(tup));
    }

    if (value.starts_with("hsl("))
    {
        auto hsl = GetTuple3<float>("hsl", value);
        ConvertHslRgb(hsl);
        return css_color(std::get<0>(hsl), 
                         std::get<1>(hsl),
                         std::get<2>(hsl));
    }

    return css_color(0, 0, 0, 1);
}

css::css_kv css::CCascadingParser::HandlePair()
{
    uint32_t clr;
    std::string name, value;

    this->HandleWhitespace();
    name = std::string(this->HandleName());
    this->HandleWhitespace();
    bool exp = this->ExpectLiteral(':');
    if (!exp)
    {
        value = "";
        return std::make_pair(name, std::make_shared<css_string>(value));
    }

    this->HandleWhitespace();
    value = this->HandleName(true);

    if (value[0] == '#' 
     || value.starts_with("rgb(")
     || value.starts_with("hsl("))
    {
        css_color col = this->HandleColor(value);
        return std::make_pair(name, std::make_shared<css_color>(col));
    }

    css_string str = css_string(value);
    return std::make_pair(name, std::make_shared<css_string>(str));
}

css::selector_collection 
css::CCascadingParser::HandleSelector()
{
    size_t begin = m_Index;
    std::string selector;
    css::selector_collection col;
    while (m_Index < m_Input.size() && m_Input[m_Index] != '{')
    {
        if (m_Input[m_Index] == ',') /* Another selector for the collection */
        {
            selector = m_Input.substr(begin, m_Index - begin);
            selector = html::TrimString(selector, " ");
            m_Index++;
            begin = m_Index;

            auto mk = CreateSelectorFor(selector);
            col.Selectors.push_back(mk);
            selector.clear();
            continue;
        }

        m_Index++;
    }

    selector = m_Input.substr(begin, m_Index - begin);
    selector = html::TrimString(selector, " \n\r\t");
    auto mk = CreateSelectorFor(selector);
    col.Selectors.push_back(mk);
    return col;
}

std::vector<css::css_rule>
css::CCascadingParser::ParseBody()
{
    css_map mp;
    std::vector<css::css_rule> rules;
    css::selector_collection selectors;
    while (m_Index < m_Input.size())
    {
        this->HandleWhitespace();
        if (m_Input[m_Index] == '@')
        {
            this->HandleAtRule();
            continue;
        }

        selectors = this->HandleSelector();
        this->ExpectLiteral('{');
        mp = this->ParseProperties();
        this->ExpectLiteral('}');
        rules.push_back(std::make_pair(selectors, mp));
    }

    return rules;
}

std::shared_ptr<css::basic_selector> 
css::CreateSelectorFor(std::string_view selector)
{
    if (selector.find('#') != std::string_view::npos 
     || selector.find('.') != std::string_view::npos)
    {
        return std::make_shared<css::class_selector>(std::string(selector));
    }

    if (selector.find(' ') != std::string_view::npos)
    {
        return std::make_shared<css::descendant_selector>(selector);
    }

    return std::make_shared<css::tag_selector>(std::string(selector));
}

void css::CCascadingParser::HandleAtRule()
{
    if (!this->ExpectLiteral('@'))
    {
        std::printf("HandleAtRule called but current char is not at!\n");
        return;
    }

    size_t begin = m_Index++;
    while(m_Index < m_Input.size() && m_Input[m_Index] != ' ') m_Index++;
    
    std::string rulename = m_Input.substr(begin, m_Index - begin);
    if (rulename == "media")
    {
        /* This entire logic just to ignore the declaration. */
        int level = 1;
        while(m_Index < m_Input.size() && m_Input[m_Index] != '{') m_Index++;
        while(m_Index < m_Input.size() && (m_Input[m_Index] != '}' && level > 0))
        {
            if (m_Input[m_Index] == '{')
            {
                level++;
            }

            if (m_Input[m_Index] == '}')
            {
                level--;
                if (level == 1)
                    level = 0;
            }

            m_Index++;
        }
    }
    else if (rulename == "import")
    {
        this->HandleWhitespace();
        begin = m_Index++;

        while(m_Index < m_Input.size() && m_Input[m_Index] != '"') 
            m_Index++;

        std::string check = m_Input.substr(begin, m_Index - begin);
        if (check == "url(")
            begin += 5;

        m_Index++;
        while(m_Index < m_Input.size() && m_Input[m_Index] != '"') 
            m_Index++;

        check = m_Input.substr(begin, m_Index - begin);
        auto url = m_Base.HandleRelative(check);

        auto res = http::Fetch(url).GetLeft();
        if (res->StatusCode == 200)
        {
            m_Input.insert(m_Index, res->Body);
        }
    }
}

std::string css::CCascadingParser::HandleQuotedName()
{
    if (m_Input[m_Index] == '\"')
    {
        m_Index++;
    }

    size_t begin = m_Index;
    while (m_Index < m_Input.size())
    {
        if (m_Input[m_Index] == '\"' 
         && m_Input[m_Index - 1] != '\\')
        {
            break;
        }

        m_Index++;
    }

    return m_Input.substr(begin, m_Index - begin);
}
