#include <cairo.h>
#include <cstdio>
#include "css.h"
#include "css/css_selector.h"
#include "html.h"
#include "html_node.h"
#include "flower_http.h"
#include "flower_draw.h"
#include "source/layout.h"
#include "source/request.h"
#include "windowcreator.h"
#include "windowcreator/window.h"

static void __sortRules(std::vector<css::css_rule>& rules)
{
    struct {
        constexpr bool operator()(css::css_rule& a, css::css_rule& b)
        {
            return a.first.GetPriority() < b.first.GetPriority();
        }
    } ruleless;

    std::sort(rules.begin(), rules.end(), ruleless);
}

int main()
{
    http::Initialize();
    std::printf("Olá, mundo!\n");
    
    wInitialize();
    draw::CWindow wind("Flower", 640, 480);
    wind.RunLoop();
}