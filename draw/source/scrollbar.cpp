#include "scrollbar.h"
#include "source/drawable.h"
#include <cairo.h>
#include <vector>

draw::CScrollBar::CScrollBar(std::vector<std::shared_ptr<IDrawable>>& drawables)
{
    int mx = 0;
    for (auto& d : drawables)
    {
        if (d->GetHeight() > mx)
        {
            mx = d->GetHeight();
        }
    }

    m_Height = mx;
}

void draw::CScrollBar::Paint(const PainterState& pt)
{
    cairo_t* ctx = cairo_create(pt.Surface);
    
    cairo_set_source_rgb(ctx, 0.0, 1.0f, 0.0);
    cairo_rectangle(ctx, 600, 0, 20, 480);
    cairo_fill(ctx);
    
    cairo_set_source_rgb(ctx, 0.1f, 0.6, 1.0f);
    cairo_rectangle(ctx, 600, ((double)pt.ScrollOffset / 100) * m_Height, 
                    10, 50);
    cairo_fill(ctx);
    cairo_destroy(ctx);
}
