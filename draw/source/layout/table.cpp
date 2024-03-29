#include "table.h"
#include "html.h"
#include "html_node.h"
#include "layout.h"
#include <charconv>
#include <memory>

struct cell
{
    draw::rectangle Position;
    std::shared_ptr<draw::layout_block_node> Node;
};

static void RecursiveLayoutMode(draw::layout_generic_node& node)
{
    node.GetLayoutMode();    
    for (auto& child : node.Children)
    {
        RecursiveLayoutMode(*child);
    }
}

static void GetRowColumnSpan(std::shared_ptr<html::dom_node> node,
                             unsigned int& rows, unsigned int& columns)
{
    std::string colspanstr,
                rowspanstr;
    bool has = html::GetAttribValue(IntoElementPtr(node)->Attributes, "colspan", colspanstr);
    if (!has) 
        colspanstr = "1";

    has = html::GetAttribValue(IntoElementPtr(node)->Attributes, "rowspan", colspanstr);
    if (!has) 
        rowspanstr = "1";

    std::from_chars(colspanstr.data(), colspanstr.data() + colspanstr.size(), columns);
    std::clamp(columns, 1U, 1000U);

    std::from_chars(rowspanstr.data(), rowspanstr.data() + rowspanstr.size(), rows);
    std::clamp(rows, 0U, 65534U);
}

static void ComputeRowLayout(std::shared_ptr<draw::layout_generic_node> node,
                             unsigned int& width, unsigned int& height,
                             draw::rectangle& rowunits)
{
    size_t layoutw = width, 
           layouth = height;
    std::vector<cell> cells;
    unsigned int currentheight = 0;
    unsigned int currentwidth = 0;
    if (currentheight == height)
        height++;
            
    if (node->Children.empty())
        return;

    auto hasassign = [&](const cell& c) -> bool 
                     { return currentheight ==  c.Position.Y && currentwidth == c.Position.X; };
    unsigned int rowspan;
    unsigned int colspan;
    GetRowColumnSpan(node->Node, rowspan, colspan);

    for (auto& child : node->Children)
    {
        while (currentwidth < width 
            && std::find_if(cells.begin(), cells.end(), hasassign) != cells.end())
        {
            currentwidth++;
        }

        currentwidth++;
        if (currentwidth == width) 
            width++;

        if (width < currentwidth + colspan) 
            width = currentwidth + colspan;
        if (height < currentheight + rowspan) 
            height = currentheight + rowspan;
        cells.emplace_back(draw::rectangle{width, height, colspan, rowspan},
                           std::static_pointer_cast<draw::layout_block_node>(child));

        if (&child == &node->Children.back())
            currentheight++;
    }
    rowunits = { currentwidth, currentheight, colspan, rowspan };

    /* Mapping cell coordinates into screen coordinates */
    /* TODO: replace all instances of ' + 5' with cell-spacing CSS property */
    size_t woffset = 0;
    height = layouth;
    for (auto& ce : cells)
    {
        std::printf("ce={ h %i, w %i, x %i, y %i }\n", ce.Position.Height, ce.Position.Width, 
                                                    ce.Position.X, ce.Position.Y);
        
        ce.Node->Position.X = node->Position.X;
        if (ce.Node->Previous)
            ce.Node->Position.X = (ce.Node->Previous->Position.X - node->Position.X)
                                + ce.Node->Previous->Position.Width + 5;
        ce.Node->Position.Y = node->Position.Y;
        ce.Node->Layout();
        height = ce.Node->Position.Height;
    }
}

static void ComputeRowGroupLayout(std::shared_ptr<draw::layout_generic_node> node,
                                  const draw::rectangle& start)
{
    unsigned int previoush;
    unsigned int correctheight;
    draw::rectangle unused;
    for (auto& child : node->Children)
    {
        if (child->LayoutMode == draw::kLayoutModeTableRow)
        {
            if (child->Previous)
            {
                previoush = child->Previous->Position.Height + child->Previous->Position.Y;
            }

            child->Position.Y += previoush;
            ComputeRowLayout(child, child->Position.X, correctheight, unused);
            child->Position.Y = correctheight;
            node->Position.Height += correctheight;
        }
    }
/*
    if (node->Position.Height > node->Parent->Position.Height)
    {

    }
*/
}

void draw::ComputeTableLayout(layout_block_node& node)
{
    bool seenCaption = false;
    node.Position.Width = 0;
    if (node.LayoutMode != kLayoutModeTable)
    {
        std::printf("ComputeTableLayout: not a table\n");
        return;
    }

    if (node.Children.empty())
    {
        return;
    }
    
    size_t previoush = 0;
    unsigned int correctheight = 0;
    draw::rectangle row;
    RecursiveLayoutMode(node);
    for(auto& child : node.Children)
    {
        if (child->LayoutMode == kLayoutModeTableCaption && !seenCaption)
        {
            std::static_pointer_cast<draw::layout_block_node>(child)->Layout();
            seenCaption = true;
        }

        if (child->LayoutMode == kLayoutModeTableRow)
        {
            if (child->Previous)
            {
                previoush = child->Previous->Position.Height + child->Previous->Position.Y;
                std::printf("previoush = %lu\n", previoush);
            }

            child->Position.Y += previoush;
            ComputeRowLayout(child, child->Position.X, correctheight, row);
            child->Position.Y = correctheight;
            node.Position.Height += child->Position.Y;
            node.Position.Width += child->Position.X + 1;
        }

        if (child->LayoutMode == kLayoutModeTableRowGroup)
        {
            ComputeRowGroupLayout(child, row);
        }
    }
}
