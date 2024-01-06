/* text.h
   Purpose: Defines the text writer,
            made to write text everywhere. 
*/

#pragma once

#include "drawable.h"

#include <cairo.h>
#include <pango/pango-font.h>
#include <pango/pangocairo.h>
#include <string>
#include <vector>

namespace draw
{

class CTextWriter : public IDrawable
{
public:
    CTextWriter() = default;
    CTextWriter(std::string text, css::css_color color = { 255, 255, 255 });
    CTextWriter(PangoFontDescription* font, std::string text, 
                css::css_color color = { 255, 255, 255 }, rectangle rect = {});
    int GetCachedHeight();
    int GetCachedWidth();
    void CreateLayout(cairo_t* cr);
    void SetText(std::string_view text);

    int GetHeight() override;
    void Paint(const PainterState& ctx) override;
    void Repaint(const PainterState& ctx) override;
    
private:
    PangoRectangle m_BoxSizes;
    PangoFontDescription* m_Font;
    PangoLayout* m_Layout;

    std::string m_Text;
    css::css_color m_Color;
};

class CTextBuilder
{
public:
    CTextBuilder() = default;

    CTextBuilder* SetFont(PangoFontDescription* font)
    {
        m_Font = font;
        return this;
    }
    
    CTextBuilder* SetText(std::string text)
    {
        m_Text = text;
        return this;
    };

    CTextBuilder* SetColor(css::css_color color)
    {
        m_Color = color;
        return this;
    }

    /* overrideWithCached - If true, `rect.Width` and `.Height` are
        replaced with writer's cached height and width. */
    CTextBuilder* SetRectangle(rectangle rect, bool overrideWithCached = true)
    {
        m_Rectangle = rect;
        m_OverrideCache = overrideWithCached;
        return this;
    }

    CTextBuilder* SetSize(double size)
    {
        m_Size = size;
        return this;
    }

    void Manufacture(std::shared_ptr<class CTextWriter>& wri)
    {
        wri = std::make_shared<CTextWriter>(m_Font, m_Text, m_Color, rectangle{});
        if (m_OverrideCache)
        {
            m_Rectangle.Height = wri->GetCachedHeight();
            m_Rectangle.Width = wri->GetCachedWidth();
        }

        wri->SetRectangle(m_Rectangle);
    }

private:
    double m_Size;
    int m_Width, m_Height;
    bool m_OverrideCache;
    PangoFontDescription* m_Font;
    std::string m_Text;
    css::css_color m_Color;
    rectangle m_Rectangle;
};

}
