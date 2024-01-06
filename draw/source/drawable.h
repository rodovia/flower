/* drawable.h
   Purpose: Common class for everything that
            can be painted inside the document 
*/

#pragma once

#include "source/layout.h"
#include <cairo.h>

namespace draw
{

struct PainterState
{
    PainterState()
        : Surface(nullptr),
          ScrollOffset(0)
    {
    }

    cairo_surface_t* Surface;
    int TopOffset;
    int ScrollOffset;
};

struct IDrawable
{
    virtual int GetHeight() = 0;
    virtual void Paint(const PainterState&) = 0;
    virtual void Repaint(const PainterState& pt)
    {
        this->Paint(pt);
    }

    virtual void SetRectangle(draw::rectangle rect)
    {
        m_Rectangle = rect;
    }

    virtual void ThinkKeyPress(uint16_t key, const PainterState& ps)
    {}

    inline rectangle GetRectangle()
    {
        return m_Rectangle;
    }

protected:
    draw::rectangle m_Rectangle;
};

}
