#pragma once

#include <stdint.h>
#include "_def.h"

/* Window events */
#define WC_EVENT_SCROLL    0x3
#define WC_EVENT_KEYPRESS  0x4
#define WC_EVENT_EXPOSE    0x5
#define WC_EVENT_WINDOW_RESIZE 0x6
#define WC_EVENT_CHAR          0x7
#define WC_EVENT_BUTTONPRESS   0x8

#define WC_KEY_SHIFT_MASK 1
#define WC_KEY_CONTROL_MASK (1 << 1)
#define WC_KEY_CAPS_LOCK_MASK (1 << 2)

typedef void(*wpfnEventCallback)(int /* event */,
                                 void* /* data */,
                                 uint64_t /* lparam */,
                                 uint64_t /* rparam */);

struct wWindow;
typedef struct wWindow wWindow;

_EXTERN_C_OR_NOTHING

wWindow* wCreateWindow(const char* title, 
                       int width, int height);
void wWindowSubscribeToEvent(wWindow* w, int event,
                             wpfnEventCallback callbk, void* data);
void wWindowRunForever(wWindow* d);
void wDestroyWindow(wWindow* d);

_RIGHT_BRACE_OR_NOTHING
