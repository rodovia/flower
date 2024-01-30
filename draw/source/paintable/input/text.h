/* text.h
   Purpose: Implementation of text fields (<input> with text type) */
#pragma once

#include "source/drawable.h"
#include "source/layout/rectangle.h"
#include "source/rectangle.h"
#include "source/text.h"

namespace draw
{

class CInput_Text : public IDrawable
{
public:
    /*    placeholder is a "ghost text" which disappears
       when clicked, usually giving a hint about the
       field's purpose. */
    CInput_Text(draw::rectangle pos, std::string placeholder = "");

    void ThinkKeyPress(uint16_t key, const PainterState& ps) override;
    void ThinkButtonPress(int button, int x, int y, navigator::CNavigator* navigator) override;
    void Paint(const PainterState& ps) override;
    
    constexpr int GetHeight() override
    {
        return m_Rectangle.Height;
    }
private:
    CTextWriter m_Placeholder;
    std::shared_ptr<CTextWriter> m_Text;
    std::string m_Buffer;
    CRectangle m_Background;
    size_t m_CharactersFit;
    bool m_ShouldPlaceholderAppear;
};

}

