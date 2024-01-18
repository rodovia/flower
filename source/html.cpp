#include "html.h"
#include "html_node.h"
#include "css.h"
#include "source/request.h"
#include "source/session_manager.h"
#include "source/url.h"
#include <assert.h>

#include <algorithm>
#include <ctime>
#include <string_view>
#include <vector>

static constexpr const std::string_view inherited[] = 
{
    "color", "font-family", "font-size", "padding-left",
    "padding-right", "padding-top", "padding-bottom"
};

static constexpr const std::string_view selfclosingTags[] = 
{
    "area", "base", "br", "col", "embed", "hr", "img", "input",
    "link", "meta", "param", "source", "track", "wbr",
};

static std::vector<std::string> SplitString(std::string_view orig, char sep)
{
    size_t index = 0;
    size_t begin = 0;
    std::vector<std::string> ret;
    while (index <= orig.size())
    {
        if (orig[index] == sep || orig[index] == '\0')
        {
            std::string ps = std::string(orig.substr(begin, index));
            ret.push_back(ps);

            if (orig[index] == '\0') break;
            index++; /* skip the separator */
            begin = index;
        }

        index++;
    }

    return ret;
}

std::string_view html::TrimString(std::string_view str, std::string_view whitespace)
{
    const auto strb = str.find_first_not_of(whitespace);
    if (strb == std::string::npos)
        return ""; // no content

    const auto stre = str.find_last_not_of(whitespace);
    const auto strrange = stre - strb + 1;

    return str.substr(strb, strrange);
}

void html::dom_node::CreateStyle(const std::vector<css::css_rule>& rules)
{
    if (Parent.get() != nullptr)
    {
        for (auto& s : inherited)
        {
            std::string st = std::string(s);
            if (!Parent->Style.contains(st))
            {
                continue;
            }
            
            Style[st] = Parent->Style[st];
        }
    }

    for (auto& rule : rules)
    {
        if (Type != kDomNodeTypeElement) break;
        if (!rule.first.Matches(static_cast<dom_element_node&>(*this))) continue;

        for (auto& prop : rule.second)
        {
            Style.insert_or_assign(prop.first, prop.second);
        }
    }

    std::map<std::string, std::string>::iterator style;
    if (Type == kDomNodeTypeElement)
    {
        auto& elem = static_cast<dom_element_node&>(*this);
        if ((style = elem.Attributes.find("style")) != elem.Attributes.end())
        {
            css::CCascadingParser cp(style->second, {});
            auto parsed = cp.ParseProperties();
            for (auto& prop : parsed)
            {
                Style.insert_or_assign(prop.first, prop.second);
            }
        }
    }
    
    css::css_map::iterator padding;
    if ((padding = Style.find("margin")) != Style.end())
    {
        auto val = static_cast<const css::css_string&>(*padding->second);
        auto parts = SplitString(val.String, ' ');
        switch (parts.size())
        {
        /*    If there is only one component value... */
        case 1:
        {
            /* ...it applies to all sides. */
            Style["margin-left"] = std::make_shared<css::css_string>(parts[0]);
            Style["margin-right"] = std::make_shared<css::css_string>(parts[0]);
            Style["margin-top"] = std::make_shared<css::css_string>(parts[0]);
            Style["margin-bottom"] = std::make_shared<css::css_string>(parts[0]);
            break;
        }

        /*    If there are two values... */
        case 2:
        {
            /* 	...the top and bottom padding are set to the first value... */
            Style["margin-top"] = std::make_shared<css::css_string>(parts[0]);
            Style["margin-bottom"] = std::make_shared<css::css_string>(parts[0]);

            /*  ...and the right and left padding are set to the second. */
            Style["margin-right"] = std::make_shared<css::css_string>(parts[1]);
            Style["margin-left"] = std::make_shared<css::css_string>(parts[1]);
            break;
        }

        /* 	If there are three values... */
        case 3:
        {
            /* ...the top is set to the first value... */
            Style["margin-top"] = std::make_shared<css::css_string>(parts[0]);

            /* ...the left and right are set to the second... */
            Style["margin-left"] = std::make_shared<css::css_string>(parts[1]);
            Style["margin-right"] = std::make_shared<css::css_string>(parts[1]);
            
            /* 	...and the bottom is set to the third. */
            Style["margin-bottom"] = std::make_shared<css::css_string>(parts[2]);
            break;
        }

        /* 	If there are four values... */
        case 4:
        {
            /* 	...they apply to the top... */
            Style["margin-top"] = std::make_shared<css::css_string>(parts[0]);

            /* ...right... */
            Style["margin-right"] = std::make_shared<css::css_string>(parts[1]);

            /* ...bottom... */
            Style["margin-bottom"] = std::make_shared<css::css_string>(parts[2]);

            /* ...and left, respectively. */
            Style["margin-left"] = std::make_shared<css::css_string>(parts[3]);
            break;
        }

        /* If there there are more than 4 or less than 1 value... */
        default:
            /* ...the declaration will be ignored. */
            break;
        };
    }

    for (auto& ch : Children)
    {
        ch->CreateStyle(rules);
    }
}

/* vvvvv CHTMLParser vvvvv */

html::CHTMLParser::CHTMLParser(std::string input, http::url base)
    : m_Input(input),
      m_BaseUrl(base)
{}

std::shared_ptr<html::dom_node> html::CHTMLParser::Parse()
{
    std::string text;
    bool inAngle = false;
    for (const auto& c : m_Input)
    {
        if (c == '<')
        {
            inAngle = true;
            if (!text.empty())
                this->AddText(std::move(text));
        }
        else if (c == '>')
        {
            inAngle = false;
            this->AddTag(std::move(text));
            text.clear();
        }
        else
        {
            if (c != '\n')
                text += c;
        }
    }

    return this->ClosePendingNodes();
}

void html::CHTMLParser::AddText(std::string text)
{
    if (text.find_first_not_of(' ') == std::string::npos)
        return;
    
    text = TrimString(text);
    if (text.empty()) return;

    std::shared_ptr<dom_node>& parent = m_Unfinished.back();
    parent->Children.push_back(std::make_shared<dom_text_node>(text, parent));
}

void html::CHTMLParser::AddTag(std::string tag)
{
    if (tag.starts_with('/'))
    {
        if (m_Unfinished.size() <= 1) 
            return; /* Leave it to ClosePendingNodes */
        std::shared_ptr<html::dom_node> finished = m_Unfinished.back();
        m_Unfinished.pop_back();

        if (finished->Type == kDomNodeTypeElement)
        {
            this->HandleSpecialElement(static_cast<dom_element_node&>(*finished));
        }

        std::shared_ptr<dom_node> parent = m_Unfinished.back();
        finished->Parent = parent;
        parent->Children.push_back(finished);
        return;
    }
    
    if (tag[0] == '!' || tag == "--") // Ignore DOCTYPE, comment begin
    {                                 //   and comment end
        return;
    }
    
    auto elem = std::make_shared<dom_element_node>(tag);
    elem->Tag = this->SplitAttributes(*elem, tag);
    
    std::map<std::string, std::string>::iterator style;
    for (auto& rule : m_StyleRules)
    {
        if (rule.first.Matches(*elem))
        {
            for (auto& prop : rule.second)
            {
                elem->Style.insert_or_assign(prop.first, prop.second);
            }
        }
    }
    
    if ((style = elem->Attributes.find("style")) != elem->Attributes.end())
    {
        css::CCascadingParser cp(style->second, m_BaseUrl);
        auto parsed = cp.ParseProperties();
        for (auto& prop : parsed)
        {
            elem->Style.insert_or_assign(prop.first, prop.second);
        }
    }

    auto sft = std::find(std::begin(selfclosingTags), std::end(selfclosingTags),
                                elem->Tag);
    if (sft != std::end(selfclosingTags))
    {
        auto& parent = m_Unfinished.back();
        this->HandleSpecialElement(*elem);
        parent->Children.push_back(elem);
        return;
    }

    m_Unfinished.push_back(elem);
}

std::string_view html::CHTMLParser::SplitAttributes(html::dom_element_node& node,
                                                    std::string_view text)
{
    size_t tagidx;
    static const char tagseps[] = { '\t', '\n', '\f', ' ' };
    for (const char t : tagseps)
    {
        tagidx = text.find_first_of(t);
        if (tagidx == std::string_view::npos)
            continue;
    }

    if (tagidx == std::string_view::npos)
    {
        std::printf("%s\n", text.data());
        return text;
    }

    size_t begin = 0,
           index = 0;
    std::vector<std::string_view> parts;
    while (index <= text.size())
    {
        if (text[index] == ' ' 
         || text[index] == '\0'
         || (text[index - 1] == '"' 
             && std::isalpha(text[index])))
        {
            parts.push_back(TrimString(text.substr(begin, index - begin)));
            begin = index;
        }

        if (text[index] == '"')
        {
            index++;
            while(text[index] != '"') index++;
        }

        index++;
    }

    std::string attribname, attribvalue;
    for(int i = 1; i < parts.size(); i++)
    {
        size_t equal, equalend;
        auto& p = parts[i];
        if ((equal = p.find('=')) != std::string_view::npos)
        {
            for (int i = 0; i < equal; i++)
            {
                auto c = p[i];
                attribname += c;
            }

            equal++;
            equalend = p.size();
            if (p[equal] == '"' || p[equal] == '\'')
            {
                equal++;
                equalend--;
            }

            for (int i = equal; i < equalend; i++)
            {
                auto c = p[i];
                attribvalue += c;
            }
        }
        else {
            attribname = p;
        }

        node.Attributes.emplace(attribname, attribvalue);
        attribname.clear(); 
        attribvalue.clear();
    }

    return parts[0];
}

std::shared_ptr<html::dom_node> 
html::CHTMLParser::ClosePendingNodes()
{
    std::shared_ptr<dom_node> node, parent;
    while (m_Unfinished.size() > 1)
    {
        node = m_Unfinished.back();
        m_Unfinished.pop_back();
        parent = m_Unfinished.back();
        node->Parent = parent;
        parent->Children.push_back(node);
    }

    node = m_Unfinished.back();
    m_Unfinished.pop_back();
    return node;
}

void html::CHTMLParser::GetStyleSheetRules(std::vector<css::css_rule>& rules)
{
    for (auto& r : m_StyleRules)
    {
        rules.push_back(r);
    }
}

/* Handler for link and style elements. */ 
void html::CHTMLParser::HandleSpecialElement(const dom_element_node& node)
{
    if (node.Tag == "style")
    {
        if (node.Children.size() < 1)
        {
            std::printf("Style tag with no children\n");
            return;
        }

        dom_text_node& nd = static_cast<dom_text_node&>(*node.Children[0]);
        css::CCascadingParser casc(nd.Text, m_BaseUrl);
        m_StyleRules = casc.ParseBody();
    }
    else if (node.Tag == "link")
    {
        std::string rel;
        if (GetAttribValue(node.Attributes, "rel", rel)
         && rel == "stylesheet")
        {
            std::string href;
            http::url url;
            GetAttribValue(node.Attributes, "href", href);
            url = m_BaseUrl.HandleRelative(href);

            auto response = http::Fetch(url).GetLeft();
            if (response->StatusCode == 200)
            {
                css::CCascadingParser parser(response->Body, url);
                auto bdy = parser.ParseBody();
                for (auto& rule : bdy)
                {
                    m_StyleRules.push_back(rule);
                }
            }
        }
    }
    else if (node.Tag == "title")
    {
        assert(node.Children.size() >= 1 && node.Children[0]->Type == kDomNodeTypeText);
        m_Title = static_cast<dom_text_node&>(*node.Children[0]).Text;
    }
}

std::string html::CHTMLParser::GetTitle() const
{
    std::printf("m_Title=%s\n", m_Title.c_str());
    return m_Title;
}
