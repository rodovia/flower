#include "chrome.h"
#include "configuration.h"
#include "source/layout/layout.h"
#include "source/navigator.h"
#include "source/paintable/address_bar.h"
#include "source/rectangle.h"
#include "source/text.h"
#include <format>
#include <memory>
#include <vector>

#define TAB_WIDTH 120
#define TAB_HEIGHT 30

static std::string GetStringUrl(http::url& url)
{
    return url.Scheme + "://" + url.Host + url.Path;
}

draw::CChrome::CChrome(std::vector<navigator::document>* tabs,
                       navigator::CNavigator* navig,
                       size_t selectedTab)
    : m_Tabs(tabs),
      m_Navigator(navig),
      m_SelectedTab(selectedTab),
      m_AddressBar(nullptr)
{
    cfg::GetColor("navigator.chrome.inactiveTabColour", m_InactiveTabColor);
    cfg::GetColor("navigator.chrome.activeTabColour", m_ActiveTabColor);
    cfg::GetColor("navigator.chrome.backgroundColour", m_ChromeColor);

    m_Position.Height = TAB_HEIGHT + 35;
    m_Position.Width = 640;
    this->ComputeLayout();
}

void draw::CChrome::ComputeLayout()
{
    m_DisplayList.clear();

    /* URL bar drawing */
    {
        std::shared_ptr<CRectangle> urlrect;
        std::shared_ptr<CAddressBar> urlwriter;
        
        rectangle pos;
        pos.Width = 640;
        pos.Height = TAB_HEIGHT + 35;
        pos.X = 0;
        pos.Y = 0;
        urlrect = std::make_shared<CRectangle>(m_ChromeColor, pos, false);

        m_AddressBar = std::make_shared<CAddressBar>(this, GetStringUrl((*m_Tabs)[m_SelectedTab].Url),
                                                     css::css_color{169, 169, 169});
        pos.Y = pos.Height - 35;
        pos.X = 10;
        m_AddressBar->SetRectangle(pos);
        m_DisplayList.push_back(urlrect);
    }

    {
        rectangle rpos, tpos;
        std::shared_ptr<CTextWriter> wr;
        std::shared_ptr<CRectangle> drect;
        for (int i = 0; i < m_Navigator->GetTabs().size(); i++)
        {
            auto doc = m_Navigator->GetTabs().at(i);
            rpos.X = 30 + (TAB_WIDTH * i);
            rpos.Y = 0;
            rpos.Width = TAB_WIDTH;
            rpos.Height = TAB_HEIGHT;
            auto color = i == m_SelectedTab ? m_ActiveTabColor : m_InactiveTabColor;
            drect = std::make_shared<CRectangle>(color, rpos, false);

            wr = std::make_shared<CTextWriter>(doc.Title.empty() ? "(no name)" : doc.Title);
            tpos.Y = 5;
            tpos.X = rpos.X + 2;
            tpos.Height = wr->GetCachedHeight();
            tpos.Width = wr->GetCachedWidth();
            wr->SetRectangle(tpos);

            m_DisplayList.push_back(drect);
            m_DisplayList.push_back(wr);
        }
    }

    /* New tab button */
    {
        constexpr int margin = 5;
        rectangle pos;
        pos.Y = 0;
        pos.X = margin;
        pos.Width  = 30 - margin;
        pos.Height = 30 - margin;

        std::shared_ptr<CRectangle> ntrect;
        ntrect = std::make_shared<CRectangle>(m_ChromeColor, pos, false);

        std::shared_ptr<CTextWriter> nttext;
        nttext = std::make_shared<CTextWriter>("+");
        pos.Y = margin;
        pos.X = pos.Width / 2;
        pos.Width = nttext->GetCachedWidth();
        pos.Height = nttext->GetCachedHeight();
        
        m_DisplayList.push_back(ntrect);
        m_DisplayList.push_back(nttext);
    }

}

void draw::CChrome::SetSelectedTab(size_t sel)
{
    m_SelectedTab = sel;
    this->ComputeLayout();
    m_Navigator->PaintCurrentTab();
}

void draw::CChrome::PaintChrome(const PainterState& ps)
{
    auto painter = ps;
    painter.TopOffset = 0;
    for (auto& bar : m_DisplayList)
    {
        bar->Paint(painter);
    }
    m_AddressBar->Paint(painter);
}

draw::rectangle draw::CChrome::GetPosition() const
{
    return m_Position;
}

void draw::CChrome::ThinkClick(int x, int y)
{
    if (x <= 30 && y <= 25)
    {
        m_Navigator->CreateTab();
        return;
    }

    if (m_AddressBar->GetRectangle().InsideRect(x, y))
    {
        m_AddressBar->ClearBuffer();
        m_Navigator->SetFocus(m_AddressBar.get());
        return;
    }

    for (int i = 1; i < m_Navigator->GetTabs().size() + 1; i++)
    {
        if (x <= 30 + (TAB_WIDTH * i) && y <= TAB_HEIGHT)
        {
            m_Navigator->SetCurrentTab(i - 1);
            return;
        }
    }
}
