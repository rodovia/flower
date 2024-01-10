/* layout.h
   Purpose: Converts nodes from html to
            IDrawables */

#pragma once

#include <vector>
#include "css.h"
#include "html.h"
#include "html_node.h"
#include <memory>

namespace draw
{

class IDrawable;

enum layout_node_type
{
    kLayoutNodeTypeGeneric,
    kLayoutNodeTypeBlock,
    kLayoutNodeTypeDocument,
};

enum layout_mode
{
    kLayoutModeNotComputed,
    kLayoutModeNone,
    kLayoutModeInline,
    kLayoutModeBlock,
};

struct rectangle
{
    unsigned int X, Y;
    unsigned int Width, Height;

    __consteval_or_nothing rectangle()
        : X(0), Y(0),
          Width(0), Height(0)
    {}

    __constexpr_or_nothing inline bool InsideRect(unsigned int x, unsigned int y)
    {
            return std::clamp(x, X, Width) == x
            && std::clamp(y, Y, Height + Y) == y;
    }
    
    __constexpr_or_nothing inline rectangle operator-(const rectangle& rhs)
    {
       rectangle ret = *this;
       ret.Height -= rhs.Height;
       ret.Width -= rhs.Width;
       ret.X -= rhs.X;
       ret.Y -= rhs.Y;
       return ret;
    }
    
    __constexpr_or_nothing inline void operator-=(const rectangle& rhs)
    {
       *this = operator-(rhs);
    }
    
    __constexpr_or_nothing inline rectangle operator+(const rectangle& rhs)
    {
       rectangle ret = *this;
       ret.Height += rhs.Height;
       ret.Width += rhs.Width;
       ret.X += rhs.X;
       ret.Y += rhs.Y;
       return ret;
    }
    
    __constexpr_or_nothing inline void operator+=(const rectangle& rhs)
    {
       *this = operator+(rhs);
    }
};

struct layout_generic_node
{
    rectangle Position;
    layout_generic_node* Parent;
    layout_generic_node* Previous;
    std::shared_ptr<html::dom_node> Node;
    std::vector<std::shared_ptr<layout_generic_node>> Children;
    std::vector<std::shared_ptr<IDrawable>> DisplayList;
    layout_mode LayoutMode;
    bool ForcedWidth, ForcedHeight;

    explicit layout_generic_node(layout_generic_node* parent,
                                std::shared_ptr<html::dom_node> node,
                                layout_generic_node* previous = nullptr,
                                std::vector<std::shared_ptr<layout_generic_node>> children = {})
        : Parent(parent),
          Previous(previous),
          Node(node),
          Children(children),
          LayoutMode(kLayoutModeNotComputed),
          ForcedWidth(parent != nullptr ? parent->ForcedWidth : false),
          ForcedHeight(parent != nullptr ? parent->ForcedHeight : false)
    {}

    virtual layout_node_type GetType() const
    {
        return kLayoutNodeTypeGeneric;
    }
    
    std::vector<std::shared_ptr<IDrawable>> CreateDisplayList();
    layout_mode GetLayoutMode();
};

struct layout_block_node : public layout_generic_node
{
    const layout_node_type Type = kLayoutNodeTypeBlock;
    using layout_generic_node::layout_generic_node;

    void Layout();
    void CreateDisplayList();

    virtual layout_node_type GetType() const override
    {
        return kLayoutNodeTypeBlock;
    }

private:
    void CreateText(html::dom_text_node* node);
    void PerformIntermdtLayout();
};

struct layout_document_node : public layout_generic_node
{   
    const layout_node_type Type = kLayoutNodeTypeDocument;

    layout_document_node(std::shared_ptr<html::dom_node> node,
                         layout_generic_node* previous = nullptr,
                         std::vector<std::shared_ptr<layout_generic_node>> children = {})
        : layout_generic_node(nullptr, node, previous, children)
    {}

    void GetDisplayList();
    std::shared_ptr<layout_block_node> CreateLayoutTree(int w = 640, int h = 480);
    virtual layout_node_type GetType() const override
    {
        return kLayoutNodeTypeDocument;
    }
};

void ExtendDisplayListFor(std::vector<std::shared_ptr<IDrawable>>& dpl, 
                          layout_generic_node& node);

}
