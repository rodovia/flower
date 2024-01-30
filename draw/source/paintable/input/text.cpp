#include "text.h"
#include "css.h"
#include "source/drawable.h"
#include "source/navigator.h"
#include "windowcreator/key.h"
#include <cairo.h>
#include <cctype>
#include <pango/pango-font.h>

draw::CInput_Text::CInput_Text(draw::rectangle pos, std::string placeholder)
    : m_Placeholder(placeholder, { 238, 238, 238 }),
      m_ShouldPlaceholderAppear(true),
      m_Background({ 255, 255, 255 }, pos)
{
    auto metrics = pango_font_get_metrics(NULL, NULL);
    m_CharactersFit = pos.Width / (pango_font_metrics_get_approximate_char_width(metrics) / PANGO_SCALE);
    pango_font_metrics_unref(metrics);

    m_Placeholder.SetText(placeholder.substr(0, m_CharactersFit));
    CTextBuilder bl;
    bl.SetText(m_Buffer)
      ->SetColor({ 0, 0, 0 })
      ->SetSize(12)
      ->SetRectangle(pos, true)
      ->Manufacture(m_Text);
    this->SetRectangle(pos);
    m_Placeholder.SetRectangle(pos);
}

void draw::CInput_Text::Paint(const PainterState& state)
{
    m_Background.Paint(state);
    if (m_ShouldPlaceholderAppear)
        m_Placeholder.Paint(state);
    else
        m_Text->Paint(state);
}

void draw::CInput_Text::ThinkKeyPress(uint16_t key, const PainterState& ps)
{
    if (std::isalnum(key) 
     || std::ispunct(key) 
     || std::isspace(key))
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

    m_ShouldPlaceholderAppear = false;
    auto len = 0;
    if (m_Buffer.size() >= m_CharactersFit)
        len = m_Buffer.size() - m_CharactersFit;
    m_Text->SetText(m_Buffer.substr(len, m_CharactersFit));
    this->Paint(ps);
}

void draw::CInput_Text::ThinkButtonPress(int button, int x, int y, 
                                         navigator::CNavigator* navigator)
{
    navigator->SetFocus(this);
}
