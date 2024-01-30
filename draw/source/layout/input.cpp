#include "input.h"
#include "html.h"

#include "source/paintable/input/text.h"

void draw::CreateInputDisplayList(layout_block_node* node)
{
    if (IntoElementPtr(node->Node)->Tag == "input")
    {
        std::string type;
        if (html::GetAttribValue(IntoElementPtr(node->Node)->Attributes, "type", type)
         && type == "text")
        {
            std::string placeholder;
            if (!html::GetAttribValue(IntoElementPtr(node->Node)->Attributes, 
                                      "placeholder", placeholder))
                placeholder = "Text";
            auto ptr = std::make_shared<CInput_Text>(node->Position, placeholder);
            node->DisplayList.push_back(ptr);
        }
    }
}
