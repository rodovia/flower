#include <cairo.h>
#include <cstdio>
#include <pango/pango-font.h>
#include <pango/pango-fontmap.h>
#include "css.h"
#include "css/css_selector.h"
#include "html.h"
#include "html_node.h"
#include "flower_http.h"
#include "flower_draw.h"
#include "pangob2d.h"
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
    auto fm = (PangoFontMap*)pango_b2d_fontmap_create();
    auto f = pango_font_map_get_family(fm, "DejaVu Serif");
    auto ce = pango_font_family_get_face(f, nullptr);
    auto d = pango_font_face_describe(ce);
    

    http::Initialize();
    std::printf("Olá, mundo!\n");
    
    wInitialize();
    draw::CWindow wind("Flower", 640, 480);
    wind.RunLoop();
}