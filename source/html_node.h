/* html_node.h
   Purpose: defines DOM nodes */

#pragma once

#include <memory>
#include <map>
#include <vector>
#include <string_view>
#include <string>
#include "css.h"

namespace html
{

enum dom_node_type : unsigned int
{
    kDomNodeTypeElement,
    kDomNodeTypeText
};

struct dom_node
{
    css::css_map Style;
    dom_node_type Type;
    std::vector<std::shared_ptr<dom_node>> Children;
    std::shared_ptr<dom_node> Parent;

    dom_node()
        : Type(kDomNodeTypeElement)
    {}

    dom_node(dom_node_type type,
             std::vector<std::shared_ptr<dom_node>> children = {},
             std::shared_ptr<dom_node> parent = {})
        : Type(type),
          Children(children),
          Parent(parent)
    {}

    void CreateStyle(const std::vector<css::css_rule>& rules);
};

struct dom_text_node : public dom_node
{
    std::string Text;

    dom_text_node(std::string text,
        std::shared_ptr<dom_node> parent = {})
        : dom_node(kDomNodeTypeText, {}, parent),
          Text(text)
    {}
};

struct dom_element_node : public dom_node
{
    std::string Tag;
    std::map<std::string, std::string> Attributes;

    dom_element_node(std::string tag, 
                    std::vector<std::shared_ptr<dom_node>> children = {},
                    std::shared_ptr<dom_node> parent = {})
        : dom_node(kDomNodeTypeElement, children),
          Tag(tag)
    {}
};

}
