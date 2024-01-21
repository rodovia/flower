#include "layout_table.h"
#include "html.h"
#include "html_node.h"
#include "source/layout.h"
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

static void ComputeRowLayout(std::shared_ptr<draw::layout_generic_node> node,
                             unsigned int& width, unsigned int& height)
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
    unsigned int rowspan = 1;
    unsigned int colspan = 1;
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
        cells.emplace_back(draw::rectangle{currentwidth, currentheight, colspan, rowspan},
                           std::static_pointer_cast<draw::layout_block_node>(child));

        if (&child == &node->Children.back())
            currentheight++;
    }

    /* Mapping cell coordinates into screen coordinates */
    /* TODO: replace all instances of ' + 5' with cell-spacing CSS property */
    size_t woffset = 0;
    height = layouth;
    for (auto& ce : cells)
    {
        ce.Node->Position.X = node->Position.X;
        if (ce.Node->Previous)
            ce.Node->Position.X = (ce.Node->Previous->Position.X - node->Position.X)
                                + ce.Node->Previous->Position.Width + 5;
        ce.Node->Position.Y = node->Position.Y;
        ce.Node->Layout();
        height = ce.Node->Position.Height;
    }
}

void draw::ComputeTableLayout(layout_block_node& node)
{
    node.Position.Width = 0;
    if (node.LayoutMode != kLayoutModeTable 
     && node.LayoutMode != kLayoutModeNotComputed)
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
    RecursiveLayoutMode(node);
    for(auto& child : node.Children)
    {
        if (child->LayoutMode == kLayoutModeTableRow)
        {
            if (child->Previous)
            {
                previoush = child->Previous->Position.Height + child->Previous->Position.Y;
            }

            std::printf("previoush %lu\n", previoush);
            std::printf("child->Pos.y = %i\n", child->Position.Y);
            child->Position.Y += previoush;
            ComputeRowLayout(child, child->Position.X, correctheight);
            child->Position.Y = correctheight;
            node.Position.Height += child->Position.Y;
            node.Position.Width += child->Position.X + 1;
        }
    }
}
