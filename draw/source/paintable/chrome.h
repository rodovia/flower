/* chrome.h
   Purpose: Chrome definitions */

/* Not really a paintable (does not inherit from IDrawable), but 
   it paints onto the screen anyway */

#pragma once

#include "css.h"
#include "source/drawable.h"
#include "source/layout/layout.h"
#include "source/paintable/address_bar.h"
#include <vector>

namespace navigator
{

struct document;
class CNavigator;

}

namespace draw
{

class CChrome
{
public:
    CChrome() = default;

    CChrome(std::vector<navigator::document>* tabs, navigator::CNavigator* navig,
            size_t selectedTab = 0);
    draw::rectangle GetPosition() const;
    void SetSelectedTab(size_t selectedTab);
    void PaintChrome(const PainterState&);
    void ThinkClick(int x, int y);
    navigator::CNavigator* GetNavigator()
    {
        return m_Navigator;
    }

private:
    void ComputeLayout();

    css::css_color m_ChromeColor;
    css::css_color m_ActiveTabColor;
    css::css_color m_InactiveTabColor;

    navigator::CNavigator* m_Navigator;
    std::shared_ptr<CAddressBar> m_AddressBar;
    size_t m_SelectedTab;
    draw::rectangle m_Position;
    std::vector<std::shared_ptr<IDrawable>> m_DisplayList;
    std::vector<navigator::document>* m_Tabs;
};

}