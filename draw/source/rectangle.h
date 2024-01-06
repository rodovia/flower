/* rectangle.h 
   Purpose: DRAW A RECTANGLE INSIDE THE DOCUMENT */

#pragma once

#include "css.h"
#include "drawable.h"
#include <tuple>

namespace draw
{

class CRectangle : public IDrawable
{
public:
    CRectangle() = default;
    CRectangle(css::css_color clor, rectangle rect, bool affectedByScroll = true);

    int GetHeight() override;
    void Paint(const PainterState& pt) override;

private:
    css::css_color m_Color;
    bool m_ShouldScroll;
};

}