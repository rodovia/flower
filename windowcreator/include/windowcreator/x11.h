/* x11.h
   Purpose: Functions relevants to the X11 platform. */

#pragma once

#include "_def.h"

#include "window.h"
#include <X11/Xlib.h>

_EXTERN_C_OR_NOTHING

Window wX11GetWindowHandle(wWindow* w);
Display* wX11GetWindowDisplay(wWindow* w);
void wX11ClearWindow(wWindow*);

_RIGHT_BRACE_OR_NOTHING
