/* layout.h
   Purpose: Converts nodes from html to
            IDrawables */

#pragma once

#include <vector>
#include <memory>

#include "css.h"
#include "html.h"
#include "html_node.h"
#include "rectangle.h"

namespace draw
{

class IDrawable;

enum layout_node_type
{
    kLayoutNodeTypeGeneric,
    kLayoutNodeTypeBlock,
    kLayoutNodeTypeDocument,
};

enum layout_mode : unsigned short
{
    kLayoutModeNotComputed,
    kLayoutModeNone,
    kLayoutModeInline,
    kLayoutModeBlock,

    kLayoutModeTable,
    kLayoutModeTableRow,
    kLayoutModeTableRowGroup,
    kLayoutModeTableHeader,
    kLayoutModeTableFooter,
    kLayoutModeTableColumn,
    kLayoutModeTableColumnGroup,
    kLayoutModeTableCell,
    kLayoutModeTableCaption,

    kLayoutModeInput,
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
    void CreateBoxModel();
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
