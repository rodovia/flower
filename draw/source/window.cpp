#include "window.h"
#include "scrollbar.h"
#include "source/navigator.h"
#include "windowcreator/window.h"
#include "windowcreator/x11.h"
#include <cairo-deprecated.h>

#define XK_MISCELLANY
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysymdef.h>
#include <X11/XKBlib.h>
#include <X11/Xatom.h>

#include <memory.h>
#include <cairo.h>
#include <cairo/cairo-xlib.h>
#include <stdio.h>

#define SCROLL_RATE 20

draw::CWindow::CWindow(std::string_view title, int width, int height)
    : m_Width(width),
      m_Height(height)
{
    m_Window = wCreateWindow(title.data(), m_Width, m_Height);
    auto dpy = wX11GetWindowDisplay(m_Window);
    auto winh = wX11GetWindowHandle(m_Window);
    m_Surface = cairo_xlib_surface_create(dpy, winh, 
                                          XDefaultVisual(dpy, 0), 
                                          width, height);
    cairo_xlib_surface_set_size(m_Surface, width, height); 
    m_Navigator = std::make_unique<navigator::CNavigator>(m_Surface);

    wWindowSubscribeToEvent(m_Window, WC_EVENT_EXPOSE, ThinkExpose, this);
    wWindowSubscribeToEvent(m_Window, WC_EVENT_SCROLL, ThinkScroll, this); 
    wWindowSubscribeToEvent(m_Window, WC_EVENT_KEYPRESS, ThinkKeyPress, this); 
    wWindowSubscribeToEvent(m_Window, WC_EVENT_BUTTONPRESS, ThinkButtonPress, this);
}

draw::CWindow::~CWindow()
{
    cairo_surface_destroy(m_Surface);
    wDestroyWindow(m_Window);
}

void draw::CWindow::RunLoop()
{
    wWindowRunForever(m_Window);
}

void draw::CWindow::ThinkExpose(int ev, void* tt, uint64_t lparam, uint64_t rparam)
{
    auto thiz = reinterpret_cast<CWindow*>(tt);
    thiz->m_Navigator->PaintCurrentTab();
}

void draw::CWindow::ThinkScroll(int ev, void* tt, uint64_t lparam, uint64_t rparam)
{
}

void draw::CWindow::ThinkKeyPress(int ev, void* tt, uint64_t lparam, uint64_t rp)
{
    auto keycode = rp == 0 ? (lparam & (0xFFFF << 15)) >> 15 : rp;
    auto thiz = reinterpret_cast<CWindow*>(tt);
    thiz->m_Navigator->ThinkKeyPress(static_cast<uint16_t>(keycode));
}

void draw::CWindow::ThinkButtonPress(int ev, void* tt, uint64_t lparam, uint64_t rparam)
{
    auto thiz = reinterpret_cast<CWindow*>(tt);
    int x = (rparam & ((uint64_t)0xFFFFFFFF << 32)) >> 32;
    int y = rparam & 0xFFFFFFFF;

    thiz->m_Navigator->ThinkMousePress(x, y);
}
