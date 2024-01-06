/* navigator.h
   Purpose: Tabbed browsing support */

#pragma once 

#include "drawable.h"
#include "source/paintable/chrome.h"
#include "source/paintable/address_bar.h"
#include <cairo.h>
#include <memory>
#include <vector>

namespace navigator
{

struct document
{
    document(http::url url);
    void Paint(const draw::PainterState&);

    http::url Url;
    std::string Title;
    std::vector<std::shared_ptr<draw::IDrawable>> DisplayList;
};

class CNavigator
{
public:
    CNavigator(cairo_surface_t* surface);
    CNavigator() = default;
    CNavigator(CNavigator&&) = default;
    
    void PaintCurrentTab();
    void ThinkMousePress(int x, int y);
    void ThinkKeyPress(uint16_t key);
    void SetCurrentTab(size_t index);
    void CreateTab(http::url to);
    void SetFocus(draw::IDrawable* focus);
    void LoadUntoCurrentTab(http::url to);
    std::vector<document> GetTabs() const;

private:
    draw::IDrawable* m_Focus;
    std::unique_ptr<draw::CChrome> m_Chrome;
    draw::PainterState m_Painter;
    std::vector<document> m_Tabs;
    size_t m_CurrentTabIndex;
};

}