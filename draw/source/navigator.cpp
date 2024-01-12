#include "navigator.h"
#include "css.h"
#include "html.h"
#include "html_node.h"

#include "flower_http.h"
#include "source/drawable.h"
#include "source/layout.h"
#include "source/paintable/chrome.h"
#include "source/request.h"
#include <vector>

static std::vector<css::css_rule> GetBuiltinRules()
{
    auto f = http::url("file:///home/miguelrodovia/dev/flower/html.css");
    http::CRequestClient req(f);
    auto resp = req.Perform();

    css::CCascadingParser cpar(resp.Body);
    return cpar.ParseBody();
}

static auto GetLinearList(
    std::shared_ptr<draw::layout_generic_node> root,
    std::vector<std::shared_ptr<draw::layout_generic_node>>& list) -> decltype(list)
{
    list.push_back(root);
    for (auto& child : root->Children)
    {
        GetLinearList(child, list);
    }

    return list;
}

navigator::document::document(http::url uri, CNavigator* navigator)
    : Url(uri),
      Navigator(navigator)
{
    std::vector<css::css_rule> rules = GetBuiltinRules();

    auto request = http::CRequestClient(Url);
    auto response = request.Perform();
    auto ps = html::CHTMLParser(response.Body, Url);
    auto dom = ps.Parse();
    Title = ps.GetTitle();
    ps.GetStyleSheetRules(rules);
    dom->CreateStyle(rules);

    auto layoutn = std::make_shared<draw::layout_document_node>(dom);
    LayoutRoot = layoutn->CreateLayoutTree();
    DisplayList = LayoutRoot->DisplayList;
}

void navigator::document::Paint(const draw::PainterState& ps)
{
    for (auto& dr : DisplayList)
    {
        dr->Paint(ps);
    }
}

void navigator::document::HandleClick(int x, int y)
{
    std::vector<std::shared_ptr<draw::layout_generic_node>> objects;
    std::vector<std::shared_ptr<draw::layout_generic_node>> hitobj;
    objects = GetLinearList(LayoutRoot, objects);
    hitobj.reserve(objects.size());
    
    for (auto ob : objects)
    {
        if (ob->Position.InsideRect(x, y))
        {
            hitobj.push_back(ob);
        }
    }
    
    if (hitobj.empty()) return;
    auto clicked = hitobj.back().get()->Node;
    
    while(clicked != nullptr)
    {
        if (clicked->Type == html::kDomNodeTypeElement
         && IntoElement(*clicked).Tag == "a"
         && IntoElement(*clicked).Attributes.contains("href"))
        {
            Navigator->CreateTab(
                Url.HandleRelative(IntoElement(*clicked).Attributes.at("href")
            ));
            return;
        }
        clicked = clicked->Parent;
    }
}

navigator::CNavigator::CNavigator(cairo_surface_t* surface)
    : m_CurrentTabIndex(0),
      m_Focus(nullptr)
{
    m_Painter.Surface = surface;
    m_Painter.ScrollOffset = 0;

    http::url uri = std::string("file:///home/miguelrodovia/dev/flower/orig.html");
    auto tab = navigator::document(uri, this);
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
        return;
    }

    /* It must be inside the document. */
    m_Tabs[m_CurrentTabIndex].HandleClick(x, y - m_Chrome->GetPosition().Height);
}

void navigator::CNavigator::CreateTab(http::url to)
{
    auto tab = navigator::document(to, this);
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
    
    ct = document(to, this);
    this->PaintCurrentTab();
    m_Focus = nullptr;
}
