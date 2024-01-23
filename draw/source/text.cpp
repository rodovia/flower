#include "text.h"
#include "source/drawable.h"
#include "layout/layout.h"
#include <cairo.h>
#include <pango/pango-font.h>
#include <pango/pango-layout.h>
#include <pango/pango-types.h>
#include <pango/pangocairo.h>

draw::CTextWriter::CTextWriter(std::string text, css::css_color color)
    : CTextWriter(pango_font_description_from_string("Terminus 12pt"), 
                        text, 
                        color)
{}

draw::CTextWriter::CTextWriter(PangoFontDescription* font, std::string text, css::css_color color,
                               rectangle rect)
    : m_Font(std::move(font)),
      m_Text(text),
      m_Layout(nullptr),
      m_Color(color)
{
    if (m_Font == nullptr)
    {
       m_Font = pango_font_description_from_string("Terminus 12pt");  
    }

    this->SetRectangle(rect);
    auto fm = pango_cairo_font_map_get_default();
    auto ctx = pango_font_map_create_context(fm);
    auto ly = pango_layout_new(ctx);

    pango_layout_set_text(ly, m_Text.data(), m_Text.length());
    pango_layout_set_font_description(ly, m_Font);
    pango_layout_get_pixel_extents(ly, nullptr, &m_BoxSizes);


    g_object_unref(ly);
    g_object_unref(ctx);
}

void draw::CTextWriter::CreateLayout(cairo_t* cr)
{
    m_Layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(m_Layout, m_Text.data(), m_Text.length());
    pango_layout_set_font_description(m_Layout, m_Font);
}

void draw::CTextWriter::Paint(const draw::PainterState& pt)
{
    cairo_t* ctx = cairo_create(pt.Surface);
    if (m_Layout == nullptr)
        this->CreateLayout(ctx);

    cairo_move_to(ctx, m_Rectangle.X, m_Rectangle.Y + pt.TopOffset + -pt.ScrollOffset);
    pango_cairo_update_layout(ctx, m_Layout);
    cairo_set_source_rgba(ctx, m_Color.Red, m_Color.Green, m_Color.Blue, m_Color.Alpha);
    pango_cairo_show_layout(ctx, m_Layout);
    cairo_destroy(ctx);
}

void draw::CTextWriter::Repaint(const draw::PainterState& pt)
{
    this->Paint(pt);
}

int draw::CTextWriter::GetHeight()
{
    if (m_Layout == nullptr)
        return this->GetCachedHeight();

    PangoRectangle rect;
    pango_layout_get_pixel_extents(m_Layout, nullptr, &rect);
    return rect.height;
}

int draw::CTextWriter::GetCachedHeight()
{
    return m_BoxSizes.height;
}

int draw::CTextWriter::GetCachedWidth()
{
    return m_BoxSizes.width;
}

void draw::CTextWriter::SetText(std::string_view text)
{
    m_Text = text;
    if (m_Layout != nullptr)
    {
        pango_layout_set_text(m_Layout, m_Text.data(), m_Text.length());
        pango_layout_get_pixel_extents(m_Layout, nullptr, &m_BoxSizes);
    }
}
