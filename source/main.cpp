#include <cstdio>
#include <windowcreator.h>
#include "flower_draw.h"
#include "flower_http.h"

#define CONFIG_DEFINE_GLOBAL_VARIABLE
#include "configuration.h"

int main()
{
    cfg::ReadConfigurationFile();
    http::Initialize();
    std::printf("Olá, mundo!\n");

    wInitialize();
    draw::CWindow wind("Flower", 640, 480);
    wind.RunLoop();
}