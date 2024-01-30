#include "navigator.h"
#include "configuration.h"
#include "css.h"
#include "html.h"
#include "html_node.h"

#include "flower_http.h"
#include "source/drawable.h"
#include "layout/layout.h"
#include "source/paintable/chrome.h"
#include "source/request.h"
#include "source/session_manager.h"
#include <optional>
#include <vector>

static void SortRules(std::vector<css::css_rule>& rules)
{
    struct {
        constexpr bool operator()(const css::css_rule& a, const css::css_rule& b)
        {
            /* ...so we can call the const variant of GetPriority. */
            return a.first.GetPriority() < b.first.GetPriority();
        }
    } ruleless;

    /* Compute the priority first... */
    for (auto& c : rules) { c.first.GetPriority(); }
    std::stable_sort(rules.begin(), rules.end(), ruleless);
}

static std::vector<css::css_rule> GetBuiltinRules()
{
    std::string path;
    cfg::GetCanonicalPath("navigator.css.userAgentStylesheet", path);

    auto f = http::url(path);
    auto resp = http::Fetch(f).GetLeft();

    css::CCascadingParser cpar(resp->Body, f);
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
    auto response = http::Fetch(uri).GetLeft();
    std::vector<css::css_rule> rules = GetBuiltinRules();
    auto ps = html::CHTMLParser(response->Body, uri);
    auto dom = ps.Parse();

    Url = response->Url;
    Title = ps.GetTitle();
    ps.GetStyleSheetRules(rules);
    SortRules(rules);
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
    auto clicked = hitobj.back().get();
    
    while(clicked != nullptr)
    {
        if (clicked->Node->Type == html::kDomNodeTypeElement)
        {
            if (IntoElementPtr(clicked->Node)->Tag == "a"
             && IntoElementPtr(clicked->Node)->Attributes.contains("href"))
            {
                Navigator->CreateTab(
                    Url.HandleRelative(IntoElementPtr(clicked->Node)->Attributes.at("href")
                ));
                return;
            }

            std::string inptype;
            bool hasi = html::GetAttribValue(IntoElementPtr(clicked->Node)->Attributes, "type", inptype);
            if (IntoElementPtr(clicked->Node)->Tag == "input"
             && hasi)
            {
                auto& dr = clicked->DisplayList.back();
                dr->ThinkButtonPress(1, x - clicked->Position.X, y - clicked->Position.Y, Navigator);
            }

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

    this->CreateTab();
    m_Painter.TopOffset = 0;
    if (bool chromeDisplay; 
        cfg::g_Configuration.lookupValue("navigator.chrome.display", chromeDisplay)
     && chromeDisplay)
    {
        m_Chrome = std::make_unique<draw::CChrome>(&m_Tabs, this);
        m_Painter.TopOffset = m_Chrome->GetPosition().Height;
    }
    
    this->PaintCurrentTab();
}

void navigator::CNavigator::PaintCurrentTab()
{
    m_Tabs[m_CurrentTabIndex].Paint(m_Painter);
    if (m_Chrome)
        m_Chrome->PaintChrome(m_Painter);
}

void navigator::CNavigator::ThinkMousePress(int x, int y)
{
    auto chrome = m_Chrome ? m_Chrome->GetPosition() : draw::rectangle();
    if (m_Chrome && chrome.InsideRect(x, y))
    {
        m_Chrome->ThinkClick(x, y);
        return;
    }

    /* It must be inside the document. */
    m_Tabs[m_CurrentTabIndex].HandleClick(x, y - chrome.Height);
}

void navigator::CNavigator::CreateTab(http::url to)
{
    auto tab = navigator::document(to, this);
    m_Tabs.push_back(tab);
    this->SetCurrentTab(m_Tabs.size() - 1);
}

void navigator::CNavigator::CreateTab()
{
    std::string ntab;
    cfg::GetCanonicalPath("navigator.newTabPage", ntab);
    http::url uri = std::string(ntab);
    this->CreateTab(uri);
}

void navigator::CNavigator::SetCurrentTab(size_t index)
{
    m_CurrentTabIndex = index;
    if (m_Chrome)
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
