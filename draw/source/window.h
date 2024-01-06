/* window.h
   Purpose: Everything boils down here. */

#pragma once

#include "drawable.h"
#include "scrollbar.h"
#include "source/layout.h"
#include "source/navigator.h"

#include <string_view>
#include <vector>
#include <memory>

#include <windowcreator/window.h>
#include <X11/Xlib.h>
#include <cairo/cairo.h>

namespace draw
{

enum scroll_direction
{
    kScrollDirectionUp,
    kScrollDirectionDown,
};

class CWindow
{
public:
    CWindow(std::string_view title, int w, int h);
    ~CWindow();

    void RunLoop();

private:
    static void ThinkScroll(int ev, void* tt, uint64_t lparam, uint64_t rparam);
    static void ThinkExpose(int ev, void* tt, uint64_t lparam, uint64_t rparam);
    static void ThinkKeyPress(int ev, void* tt, uint64_t lparam, uint64_t rparam);
    static void ThinkButtonPress(int ev, void* tt, uint64_t lparam, uint64_t rparam);

    int m_Width, m_Height;
    cairo_surface_t* m_Surface;
    PainterState m_State;
    wWindow* m_Window;

    std::unique_ptr<navigator::CNavigator> m_Navigator;
};

}
