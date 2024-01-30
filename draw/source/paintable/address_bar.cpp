#include "address_bar.h"
#include "chrome.h"
#include "source/drawable.h"
#include "source/navigator.h"
#include "windowcreator/key.h"
#include <cctype>

draw::CAddressBar::CAddressBar(draw::CChrome* chrome, std::string initialtext, css::css_color color)
    : m_Chrome(chrome),
      m_Buffer(initialtext),
      m_Writer(initialtext, css::css_color(120, 120, 233)),
      m_Rectangle(color, {}, false),
      m_Background(css::css_color(255, 255, 255), {}, false)
{}

void draw::CAddressBar::ThinkKeyPress(uint16_t key, const PainterState& ps)
{
    if (std::isalnum(key) || std::ispunct(key))
        m_Buffer += key;
    
    if (key == WC_KEY_BACKSPACE)
    {
        if (m_Buffer.size() < 1)
        {
            return;
        }
        
        m_Buffer.pop_back();
        m_Buffer.pop_back();
    }

    if (key == WC_KEY_RETURN)
    {
        auto navi = m_Chrome->GetNavigator();
        m_Writer.SetText("");
        navi->LoadUntoCurrentTab(m_Buffer);
        return;
    }

    m_Writer.SetText(m_Buffer);
    this->SetRectangle(IDrawable::m_Rectangle);
    this->Paint(ps);
}

void draw::CAddressBar::Paint(const PainterState& ps)
{
    auto painter = ps;
    painter.TopOffset = 0;

    m_Rectangle.Paint(painter);
    m_Background.Paint(painter);
    m_Writer.Paint(painter);
}

void draw::CAddressBar::SetRectangle(rectangle rect)
{
    rect.Height = m_Writer.GetHeight();
    rect.Width = 640 - rect.X;

    IDrawable::SetRectangle(rect);
    m_Rectangle.SetRectangle(rect);
    m_Writer.SetRectangle(rect);

    constexpr int padding = 5;
    rect.Height += padding;
    rect.Y -= 2;
    rect.Width += padding - 3;
    rect.X -= 2;
    m_Background.SetRectangle(rect);
}
