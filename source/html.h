/* html.h
   Purpose: Defines HTML parser stuff
*/

#pragma once

#include "css.h"
#include "flower_http.h"
#include <memory>
#include <map>
#include <vector>
#include <string_view>
#include <string>

#ifndef __FLOWER_HTML_H
#define __FLOWER_HTML_H

namespace html
{

struct dom_node;

inline bool GetAttribValue(const std::map<std::string, std::string>& map,
                           std::string name,
                           std::string& result)
{
    std::map<std::string, std::string>::const_iterator iter;
    if ((iter = map.find(name)) != map.end())
    {
        result = iter->second;
        return true;
    }

    return false;
}

std::string_view TrimString(std::string_view str, std::string_view whitespace = " \t");

class CHTMLParser
{
public:
    CHTMLParser(std::string input, http::url base);
    std::shared_ptr<html::dom_node> Parse();
    void GetStyleSheetRules(std::vector<css::css_rule>& ssr);
    std::string GetTitle() const;

private:
    void AddText(std::string text);
    void AddTag(std::string text);
    std::string_view SplitAttributes(dom_element_node& node, 
                                     std::string_view tag);
    std::shared_ptr<html::dom_node> ClosePendingNodes();
    void HandleSpecialElement(const dom_element_node& node);

    std::string m_Title;
    http::url m_BaseUrl;
    std::vector<css::css_rule> m_StyleRules;
    std::vector<std::shared_ptr<dom_node>> m_Unfinished;
    std::string m_Input;
};

}

#endif /* ifdef __FLOWER_HTML_H */
