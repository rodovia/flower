#include "css_selector.h"
#include "css.h"
#include "html_node.h"

css::class_selector::class_selector(std::string selector)
    : basic_selector(kCssSelectorTypeClassIdTag, 10)
{
    size_t elebeg = 0;
    size_t index = 0;
    char last = '\0';
    while (index < selector.size())
    {
        if (selector[index] == '.' || selector[index] == '#')
        {
            switch (last)
            {
            case '\0':
                Tag = selector.substr(elebeg, index);
                break;
            case '.':
                Classes.push_back(selector.substr(elebeg + 1, index - 1));
                break;
            case '#':
                std::printf("TODO: ID support\n");
                break;
            }
                
            last = selector[index];
            elebeg = index;
        }

        index++;
    }

    switch (last)
    {
    case '\0':
        Tag = selector.substr(elebeg, index - 1);
        break;
    case '.':
        Classes.push_back(selector.substr(elebeg + 1, index - 1));
        break;
    case '#':
        std::printf("TODO: ID support\n");
        break;
    }
}

bool css::class_selector::Matches(const html::dom_element_node& node)
{
    if (!Tag.empty() && node.Tag != Tag)
    {
        return false;
    }

    auto nodeclass = node.Attributes.find("class");
    if (nodeclass == node.Attributes.end() && !Classes.empty())
    {
        return false;
    }

    bool val = std::all_of(Classes.begin(), Classes.end(), [&](const std::string& cls)
    {
        auto cl = cls.substr(1);
        return nodeclass->second.find(cl) != std::string::npos;
    });
    return val;
}

bool css::tag_selector::Matches(const html::dom_element_node& node)
{
    return node.Tag == Tag || Tag == "*";
}

css::descendant_selector::descendant_selector(std::string_view select)
    : basic_selector(kCssSelectorTypeDescendant, 1)
{
    size_t index = 0;
    size_t begin = index;
    std::vector<std::shared_ptr<basic_selector>> sels;
    while (index <= select.size())
    {
        if (select[index] == ' ' || select[index] == '\0')
        {
            auto skip = select[begin] == ' ' ? 1 : 0;
            auto sub = select.substr(begin + skip, index);
            auto sel = css::CreateSelectorFor(sub);
            sels.push_back(sel);

            begin = index;
            if (select[index] == '\0') break;
        }

        index++;
    }

    Selectors = std::move(sels);
}

bool css::descendant_selector::Matches(const html::dom_element_node& node)
{
    auto mdl = &node;
    for (auto i = Selectors.rbegin(); i != Selectors.rend(); ++i)
    {
        auto itr = *i;
        /* It is safe to assume parents are elements, right? */
        if (!itr->Matches(static_cast<const html::dom_element_node&>(*mdl)))
        {
            return false;
        }

        mdl = static_cast<const html::dom_element_node*>(mdl->Parent.get());
        if (mdl == nullptr 
         && std::distance(i, Selectors.rend()) != 1)
        { 
            return false;
        }
    }

    return true;
}
