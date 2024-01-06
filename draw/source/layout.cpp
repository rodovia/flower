#include "layout.h"

#include "css/css_selector.h"
#include "css/css_units.h"
#include "html.h"
#include "html_node.h"
#include "css.h"

#include "source/drawable.h"
#include "source/rectangle.h"
#include "text.h"

#include <algorithm>
#include <memory>
#include <pango/pango-font.h>
#include <pango/pango-layout.h>

static bool HasCssProp(css::css_map& map, const std::string& prop, css::css_map::iterator& iter)
{
    bool has = map.contains(prop);
    iter = has ? map.find(prop) : map.end();
    return has;
}

static constexpr draw::layout_mode 
DisplayPropertyToLayoutType(const css::css_basic_value& bv)
{
    if (bv.Type != css::kCssValueTypeKeywordOrString)
    {
        std::printf("DisplayPropertyToLayoutType invalid bv type %i. Is it a bug?\n", bv.Type);
        return draw::kLayoutModeNone;
    }

    auto& str = static_cast<const css::css_string&>(bv);
    if (str.String == "block")
        return draw::kLayoutModeBlock;
    else if(str.String == "inline")
        return draw::kLayoutModeInline;
    else if (str.String == "none")
        return draw::kLayoutModeNone;

    std::printf("DisplayPropertyToLayoutType unsupported value %s\n", str.String.c_str());
    return draw::kLayoutModeNone;
}

void draw::ExtendDisplayListFor(std::vector<std::shared_ptr<IDrawable>>& dpl, 
                                layout_generic_node& node)
{
    auto dl = node.CreateDisplayList();
    for (auto& i : dl)
    {
        dpl.push_back(i);
    }

    for (auto& n : node.Children)
    {
        ExtendDisplayListFor(dpl, *n);
    }
}

std::vector<std::shared_ptr<draw::IDrawable>> 
draw::layout_generic_node::CreateDisplayList()
{
    css::css_map::iterator bgc;
    std::vector<std::shared_ptr<IDrawable>> cmds;
    if (HasCssProp(Node->Style, "padding-left", bgc))
    {
        auto ref = css::EnsureReferenceUnit(*bgc->second);
        Position.X -= ref;
        Position.Width += ref;
    }

    if (HasCssProp(Node->Style, "padding-right", bgc))
    {
        auto ref = css::EnsureReferenceUnit(*bgc->second);
        Position.Width += ref;
    }

    if (HasCssProp(Node->Style, "padding-bottom", bgc))
    {
        auto ref = css::EnsureReferenceUnit(*bgc->second);
        Position.Height += ref;
    }

    if (HasCssProp(Node->Style, "padding-top", bgc))
    {
        auto ref = css::EnsureReferenceUnit(*bgc->second);
        Position.Y -= ref;
        Position.Height += ref;
    }


    if ((bgc = Node->Style.find("background-color")) != Node->Style.end())
    {
        bool scroll = true;
        if (Node->Type == html::kDomNodeTypeElement)
        {
            auto& elem = static_cast<html::dom_element_node&>(*Node);
            if (elem.Tag == "body" || elem.Tag == "html")
            {
                scroll = false;
                Position.Y = 0;
                Position.Height = Parent->Position.Height;
                Position.Width = Parent->Position.Width;
            }
        }

        css::css_color color;
        auto val = bgc->second;
        css::EnsureColorCorrect(*val, color);
        cmds.push_back(std::make_shared<CRectangle>(color, Position, scroll));
    }

    if (this->GetLayoutMode() == kLayoutModeInline)
    {
        for (auto& a : DisplayList)
        {
            cmds.push_back(a);
        }
    }

    return cmds;
}

draw::layout_mode draw::layout_generic_node::GetLayoutMode()
{
    css::css_map::iterator dis;
    if ((dis = Node->Style.find("display")) != Node->Style.end())
    {
        auto& strt = static_cast<const css::css_string&>(*dis->second);
        return (LayoutMode = DisplayPropertyToLayoutType(strt));
    }

    if (Node->Type == html::kDomNodeTypeText)
    {
        return (LayoutMode = kLayoutModeInline);
    }

    if (!Children.empty())
    {
        return (LayoutMode = kLayoutModeBlock);
    }

    return (LayoutMode = kLayoutModeInline);
}

std::shared_ptr<draw::layout_block_node> 
draw::layout_document_node::CreateLayoutTree(int w, int h)
{
    auto child = std::make_shared<layout_block_node>(this, Node, nullptr, Children);
    Position.Height = h;
    Position.Width = w;
    Position.X = 0;
    Position.Y = 0;
    child->LayoutMode = kLayoutModeBlock;
    Children.push_back(child);
    child->Layout();

    std::vector<std::shared_ptr<draw::IDrawable>> dpl;
    draw::ExtendDisplayListFor(dpl, *child);
    child->DisplayList = dpl;
    return child;
}

void draw::layout_block_node::PerformIntermdtLayout()
{
    layout_generic_node* previous = nullptr;
    std::shared_ptr<layout_generic_node> node;

    for (auto& child : Node->Children)
    {
        node = std::make_shared<layout_block_node>(this, child, previous);
        Children.push_back(node);
        previous = node.get();
    }
}

void draw::layout_block_node::Layout()
{
    auto lm = this->GetLayoutMode();
    if (lm == kLayoutModeNone)
    {
        Position.Height = 0;
        Position.Width = 0;
        Position.X = 0;
        Position.Y = 0;
        return;
    }

    css::css_map::iterator mg;
    if (HasCssProp(Node->Style, "margin-left", mg))
    {
        auto ref = css::EnsureReferenceUnit(*mg->second);
        Position.X += ref;
    }

    if (HasCssProp(Node->Style, "margin-top", mg))
    {
        auto ref = css::EnsureReferenceUnit(*mg->second);
        Position.Y += ref;
    }

    if (HasCssProp(Node->Style, "margin-bottom", mg))
    {
        auto ref = css::EnsureReferenceUnit(*mg->second);
        Position.Height += ref;
    }

    if (HasCssProp(Node->Style, "margin-right", mg))
    {
        auto ref = css::EnsureReferenceUnit(*mg->second);
        Position.Width += ref;
    }

    css::css_map::iterator wi;
    if ((wi = Node->Style.find("width")) != Node->Style.end())
    {
        auto ref = css::EnsureReferenceUnit(*wi->second);
        Position.Width = static_cast<int>(ref);
        ForcedWidth = true;
    }

    if ((wi = Node->Style.find("height")) != Node->Style.end())
    {
        auto ref = css::EnsureReferenceUnit(*wi->second);
        Position.Height = static_cast<int>(ref);
        ForcedHeight = true;
    }

    this->PerformIntermdtLayout();
    Position.X += Parent->Position.X;
    Position.Y += Parent->Position.Y;
    if (!ForcedWidth)
    {
        Position.Width = Parent->Position.Width;
    }
    
    if (Previous != nullptr && Previous->LayoutMode != kLayoutModeInline)
    {
        Position.Y = Previous->Position.Y + Previous->Position.Height;
    }

    if (lm == kLayoutModeInline)
    {
        if (Node->Type == html::kDomNodeTypeElement)
        {
            Position.Width = 0;
            for (auto& c : Children)
            {
                auto stc = static_cast<layout_block_node*>(c.get());
                stc->Layout();
                Position.Width += stc->Position.Width;
            }
            if (Previous != nullptr)
                Position.X += Previous->Position.Width;
            return;
        }

        auto text = static_cast<html::dom_text_node*>(Node.get());
        this->CreateText(text);
        return;
    }
    else if (lm == kLayoutModeBlock)
    {
        std::printf("ForcedWidth=%i\n", ForcedWidth);
        int presumedw = ForcedWidth ? Position.Width : 0;
        for (auto& child : Children)
        {
            auto stc = static_cast<layout_block_node*>(child.get());
            stc->Layout();
            DisplayList.reserve(DisplayList.size() + 
                                    std::distance(stc->DisplayList.begin(), 
                                            stc->DisplayList.end()));
            DisplayList.insert(DisplayList.end(), stc->DisplayList.begin(),stc->DisplayList .end());

            if (child->Position.Width > presumedw)
            {
                presumedw = child->Position.Width;
            }

            if (!ForcedHeight)
                Position.Height += child->Position.Height;
        }

        Position.Width = presumedw;        
    }
}

void draw::layout_block_node::CreateText(html::dom_text_node* node)
{
    CTextBuilder bu;
    css::css_map::iterator color, ff;
    css::css_color cl = {0, 0, 0};
    if ((color = Node->Style.find("color")) != Node->Style.end())
    {
        css::EnsureColorCorrect(*color->second, cl);
    }
    bu.SetColor(cl);
    
    PangoFontDescription* desc = nullptr;
    if (HasCssProp(Node->Style, "font-family", ff))
    {
        PangoFontFamily* faml;
        css::GetFontForValue(*ff->second, faml);
        PangoFontFace* face = pango_font_family_get_face(faml, nullptr);
        desc = pango_font_face_describe(face);

        if (HasCssProp(Node->Style, "font-size", ff))
        {
            auto size = css::EnsureReferenceUnit(*ff->second);
            pango_font_description_set_absolute_size(desc, size * PANGO_SCALE);
        }
    }
    bu.SetFont(desc);

    std::shared_ptr<CTextWriter> dr;
    if (Previous != nullptr)
    {
        Position.X += Previous->Position.Width;
    }

    bu.SetRectangle(Position)
        ->SetText(node->Text);
    bu.Manufacture(dr);
    Position = dr->GetRectangle();
    DisplayList.push_back(dr);
}
