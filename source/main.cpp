#include <cairo.h>
#include <cstdio>
#include "css.h"
#include "css/css_selector.h"
#include "html.h"
#include "html_node.h"
#include "flower_http.h"
#include "flower_draw.h"
#include "source/async/future.h"
#include "source/codec.h"
#include "source/layout.h"
#include "source/request.h"
#include "source/session.h"
#include "windowcreator.h"
#include "windowcreator/window.h"

int main()
{
    http::Initialize();
    std::printf("Olá, mundo!\n");

    wInitialize();
    draw::CWindow wind("Flower", 640, 480);
    wind.RunLoop();
}