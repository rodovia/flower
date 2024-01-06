#pragma once

#include "source/drawable.h"
#include "source/rectangle.h"
#include "source/text.h"

namespace draw
{

class CAddressBar : public IDrawable
{
public:
    CAddressBar(class CChrome* chrome, std::string initialtext, css::css_color bg);
    void ThinkKeyPress(uint16_t key, const PainterState& ps) override;
    void Paint(const PainterState& ps) override;
    void SetRectangle(rectangle rect) override;
    void ClearBuffer()
    {
        m_Buffer.clear();
        m_Writer.SetText(m_Buffer);
    }

    int GetHeight() override
    {
        return m_Writer.GetHeight();
    }

private:
    CChrome* m_Chrome;
    CRectangle m_Rectangle;
    CRectangle m_Background;
    CTextWriter m_Writer;
    std::string m_Buffer;
};

}
