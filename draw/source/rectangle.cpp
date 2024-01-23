#include "rectangle.h"
#include "source/drawable.h"
#include "layout/layout.h"
#include <cairo.h>

draw::CRectangle::CRectangle(css::css_color color, draw::rectangle rect,
                             bool scroll)
    : m_Color(color),
      m_ShouldScroll(scroll)
{
    this->SetRectangle(rect);
}

void draw::CRectangle::Paint(const draw::PainterState& pt)
{
    int y = m_Rectangle.Y + pt.TopOffset + (m_ShouldScroll ? -pt.ScrollOffset : 0); 
    cairo_t* ctx = cairo_create(pt.Surface);
    cairo_set_source_rgba(ctx, m_Color.Red, m_Color.Green, m_Color.Blue, m_Color.Alpha);
    cairo_rectangle(ctx, m_Rectangle.X, y, 
                    m_Rectangle.Width, m_Rectangle.Height);
    cairo_fill(ctx);
    cairo_destroy(ctx);
}

int draw::CRectangle::GetHeight()
{
    return m_Rectangle.Height;
}
