/* css_selector.h
   Purpose:  */

#pragma once

#include <memory>
#include <algorithm>
#include <string>
#include <vector>

namespace html
{

struct dom_element_node;

}

namespace css
{

enum selector_type
{
    kCssSelectorTypeTag,
    kCssSelectorTypeClassIdTag,
    kCssSelectorTypeDescendant,
};

struct basic_selector
{
    int Type;
    int Priority;

    constexpr basic_selector(int type, int prio)
        : Type(type), 
          Priority(prio)
    {}

    virtual bool Matches(const html::dom_element_node& elem) = 0;
    virtual ~basic_selector() {}
};

/* Selector for plain tags.

    An example of tag selector is: `pre { ... }`. */
struct tag_selector : public basic_selector
{
    std::string Tag;

    constexpr tag_selector(std::string tag)
        : Tag(tag),
          basic_selector(kCssSelectorTypeTag, 1)
    {}

    bool Matches(const html::dom_element_node& elem) override;
};

/* A selector which uses HTML attribs `class` and `id` 
   (and optionally, an tag) to match elements */
struct class_selector : public basic_selector
{
    std::string Tag;
    std::vector<std::string> Classes;

    class_selector(std::string selector);
    bool Matches(const html::dom_element_node& elem) override;
};

struct descendant_selector : public basic_selector
{
    std::vector<std::shared_ptr<basic_selector>> Selectors;
    
    descendant_selector(std::string_view select);
    bool Matches(const html::dom_element_node& elem) override;
};

struct selector_collection
{
    std::vector<std::shared_ptr<basic_selector>> Selectors;
    int Priority;

    constexpr selector_collection(
        std::vector<std::shared_ptr<basic_selector>> sel = {})
        : Selectors(sel),
          Priority(0)
    {}

    inline bool Matches(const html::dom_element_node& elem) const
    {
        return std::any_of(Selectors.begin(), Selectors.end(),
                    [&](const std::shared_ptr<basic_selector>& s)
                    { 
                        return s->Matches(elem);
                    });
    }

    constexpr int GetPriority()
    {
        if (Priority == 0)
        {
            for (auto& s : Selectors)
            {
                Priority += s->Priority;
            }
        }

        return Priority;
    }

};

}
