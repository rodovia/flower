#include "navigator.h"
#include "css.h"
#include "html.h"
#include "html_node.h"

#include "flower_http.h"
#include "source/drawable.h"
#include "source/paintable/chrome.h"
#include "source/request.h"

static std::vector<css::css_rule> GetBuiltinRules()
{
    auto f = http::url("file:///home/miguelrodovia/dev/flower/html.css");
    http::CRequestClient req(f);
    auto resp = req.Perform();

    css::CCascadingParser cpar(resp.Body);
    return cpar.ParseBody();
}

navigator::document::document(http::url uri)
    : Url(uri)
{
    std::vector<css::css_rule> rules = GetBuiltinRules();

    auto request = http::CRequestClient(Url);
    auto response = request.Perform();
    auto ps = html::CHTMLParser(response.Body, Url);
    Title = ps.GetTitle();
    auto dom = ps.Parse();
    ps.GetStyleSheetRules(rules);
    dom->CreateStyle(rules);

    auto layoutn = std::make_shared<draw::layout_document_node>(dom);
    auto txt = layoutn->CreateLayoutTree();
    DisplayList = txt->DisplayList;
}

void navigator::document::Paint(const draw::PainterState& ps)
{
    for (auto& dr : DisplayList)
    {
        dr->Paint(ps);
    }
}

navigator::CNavigator::CNavigator(cairo_surface_t* surface)
    : m_CurrentTabIndex(0),
      m_Focus(nullptr)
{
    m_Painter.Surface = surface;
    m_Painter.ScrollOffset = 0;

    http::url uri = std::string("file:///home/miguelrodovia/dev/flower/orig.html");
    auto tab = navigator::document(uri);
    m_Tabs.push_back(tab);

    m_Chrome = std::make_unique<draw::CChrome>(&m_Tabs, this);
    m_Painter.TopOffset = m_Chrome->GetPosition().Height;
    this->PaintCurrentTab();
}

void navigator::CNavigator::PaintCurrentTab()
{
    m_Tabs[m_CurrentTabIndex].Paint(m_Painter);
    m_Chrome->PaintChrome(m_Painter);
}

void navigator::CNavigator::ThinkMousePress(int x, int y)
{
    if (m_Chrome->GetPosition().InsideRect(x, y))
    {
        m_Chrome->ThinkClick(x, y);
    }
}

void navigator::CNavigator::CreateTab(http::url to)
{
    auto tab = navigator::document(to);
    m_Tabs.push_back(tab);
    this->SetCurrentTab(m_Tabs.size() - 1);
}

void navigator::CNavigator::SetCurrentTab(size_t index)
{
    m_CurrentTabIndex = index;
    m_Chrome->SetSelectedTab(index);
}

std::vector<navigator::document> 
navigator::CNavigator::GetTabs() const
{
    return m_Tabs;
}

void navigator::CNavigator::SetFocus(draw::IDrawable* focus)
{
    m_Focus = focus;
}

void navigator::CNavigator::ThinkKeyPress(uint16_t key)
{
    if (m_Focus != nullptr)
    {
        m_Focus->ThinkKeyPress(key, m_Painter);
    }
}

void navigator::CNavigator::LoadUntoCurrentTab(http::url to)
{
    auto& ct = m_Tabs[m_CurrentTabIndex];
    ct.DisplayList.clear();
    ct.Title.clear();
    
    ct = document(to);
    this->PaintCurrentTab();
    m_Focus = nullptr;
}
