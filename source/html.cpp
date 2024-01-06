#include "html.h"
#include "html_node.h"
#include "css.h"
#include "source/request.h"
#include <assert.h>

#include <algorithm>
#include <ctime>
#include <string_view>
#include <vector>

static constexpr const std::string_view inherited[] = 
{
    "color", "font-family", "font-size"
};

static constexpr const std::string_view selfclosingTags[] = 
{
    "area", "base", "br", "col", "embed", "hr", "img", "input",
    "link", "meta", "param", "source", "track", "wbr",
};

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
            css::CCascadingParser cp(style->second);
            auto parsed = cp.ParseProperties();
            for (auto& prop : parsed)
            {
                Style.insert_or_assign(prop.first, prop.second);
            }
        }
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
        css::CCascadingParser cp(style->second);
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
        return text;
    }

    size_t equidx, fdidx, attribvidx;
    std::string_view attribname;
    std::string attribvalue;
    auto attribs = text.substr(tagidx + 1);
    while (true)
    {
        attribs = TrimString(attribs);
        equidx = attribs.find('=');
        attribname = attribs.substr(0, equidx);
        fdidx = attribs.find(' ', equidx);

        if (equidx == std::string_view::npos)
        {
            /* Attribute has no value. */
            node.Attributes.insert(std::make_pair(attribname, ""));
            if (fdidx == std::string_view::npos)
            {
                break;
            }
            
            attribs = attribs.substr(fdidx);

            continue;
        }
        
        if (attribs[equidx + 1] == '"')
        {
            /* Attribute's value is quoted. */
            std::string_view attr = attribs.substr(equidx + 2);
            for (const char& c : attr)
            {
                if (c == '"')
                {
                    break;
                }
                attribvalue += c;
            }

            node.Attributes.insert(std::make_pair(attribname, attribvalue));
        }

        /* Attribute's value is not quoted
           TODO: add sanity checks if invalid chars
           are found. The parser may be confused. */
        // attribvalue = attribs.substr(equidx, fdidx - equidx);

        if (fdidx == std::string_view::npos)
        {
            break;
        }

        attribs = attribs.substr(fdidx);
        attribvalue.clear();
    }

    return (tagidx == std::string_view::npos) ? text : text.substr(0, tagidx);
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
        dom_text_node& nd = static_cast<dom_text_node&>(*node.Children[0]);
        css::CCascadingParser casc(nd.Text);
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
            url = m_BaseUrl;
            if (href[0] != '/' 
             && href.find("://") == std::string::npos)
            {
                href.insert(0, "/");
            }

            url.Path = href;
            if (href.starts_with("http"))
            {
                url = href;
            }

            http::CRequestClient client(url);
            auto response = client.Perform();
            if (response.StatusCode == 200)
            {
                css::CCascadingParser parser(response.Body);
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
    return m_Title;
}
