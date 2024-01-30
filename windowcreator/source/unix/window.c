#include <windowcreator/window.h>
#include <windowcreator/x11.h>
#include "globalstate.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define XK_MISCELLANY
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysymdef.h>
#include <X11/XKBlib.h>
#include <X11/Xatom.h>

extern _wGlobalState* _wc_globalState;

struct event_handler
{
    int type;
    void* data;
    wpfnEventCallback callback;
    struct event_handler* next;
};

struct wWindow
{
    XIC ictx;
    XIM imethod;
    Window wind;
    Display* display;
    struct event_handler* evh;
};

static void DestroyLinkedList(struct event_handler* eh)
{
    struct event_handler* tmp = eh;
    struct event_handler* nex;
    while (tmp->next != NULL)
    {
        nex = tmp->next;
        free(tmp);
        tmp = nex;
    }
}

static inline void RunEvent(struct event_handler* head, int type, 
                            uint64_t lparam, uint64_t rparam)
{
    if (head == NULL) return;

    struct event_handler* tmp = head;
    while(tmp != NULL)
    {
        if (tmp->type == type)
        {
            tmp->callback(type, tmp->data, lparam, rparam);
        }

        tmp = tmp->next;
    }
}

static void CreateInputContext(wWindow* window)
{
    if ((window->imethod = XOpenIM(window->display, NULL, NULL, NULL)) == NULL)
    {
        XSetLocaleModifiers("@im=");
        if ((window->imethod = XOpenIM(window->display, NULL, NULL, NULL)) == NULL )
        {
            printf("Could not create IM connection. "
                   "WC_EVENT_CHAR may not be dispatched correctly\n");
        }
    }

    window->ictx = XCreateIC(window->imethod, 
                             XNClientWindow, window->wind,
                             XNFocusWindow, window->wind,
                             XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
                             NULL);

    XSetICFocus(window->ictx);
}

wWindow* wCreateWindow(const char* title, 
                       int width, int height)
{
    wWindow* ret = calloc(1, sizeof(wWindow));
    ret->display = XOpenDisplay(NULL);
    ret->wind = XCreateSimpleWindow(ret->display, XDefaultRootWindow(ret->display),
                                    0, 0, width, height, 0, 0, 0);
    XChangeProperty(ret->display, ret->wind, XInternAtom(ret->display, "WM_NAME", 0),
                    XA_STRING, 8, PropModeReplace, (const uint8_t*)title, 
                    strlen(title));
    CreateInputContext(ret);
    return ret;
}

void wDestroyWindow(wWindow* d)
{
    if (d == NULL)
        return;

    XCloseIM(d->imethod);
    XDestroyWindow(d->display, d->wind);
    XCloseDisplay(d->display);
    DestroyLinkedList(d->evh);
    free(d);
}

void wWindowSubscribeToEvent(wWindow* w, int event, 
                             wpfnEventCallback callbk, 
                             void* data)
{
    if (w == NULL || callbk == NULL) return;

    struct event_handler* handler = calloc(1, sizeof(struct event_handler));
    handler->callback = callbk;
    handler->data = data;
    handler->type = event;

    if (w->evh == NULL)
    {
        w->evh = handler;
        return;
    }

    struct event_handler* tmp = w->evh;
    while(tmp->next != NULL)
    {
        tmp = tmp->next;
    }

    tmp->next = handler;
}

void wWindowRunForever(wWindow* d)
{
    if (d == NULL) return;

    XEvent event;
    XMapWindow(d->display, d->wind);
    XSelectInput(d->display, d->wind, KeyPressMask | KeyReleaseMask 
                                    | ButtonPressMask | ExposureMask
                                    | StructureNotifyMask);

    while(1)
    {
        XNextEvent(d->display, &event);
        if (XFilterEvent(&event, d->wind)) continue;
        switch (event.type)
        {
        case Expose:
        {
            if (event.xexpose.count != 0)
            {
                continue;
            }

            RunEvent(d->evh, WC_EVENT_EXPOSE, 0, 0);
            break;
        }
        case ButtonPress:
        {

            if (event.xbutton.button == 4
             || event.xbutton.button == 5)
            {
                int dir; /* 1 = up */
                if (event.xbutton.button == 4)
                    dir = 1;
                else if (event.xbutton.button == 5)
                    dir = 0;
                RunEvent(d->evh, WC_EVENT_SCROLL, dir, 0);
                break;
            }

            RunEvent(d->evh, WC_EVENT_BUTTONPRESS, 
                     event.xbutton.button, 
                     ((uint64_t)event.xbutton.x << 32) | event.xbutton.y);
            break;
        }
        case KeyPress:
        {
            KeySym ks = XkbKeycodeToKeysym(d->display, event.xkey.keycode, 0,
                                        event.xkey.state & ShiftMask ? 1 : 0);
            int subtr = ks > 0xff00 ? 0xff00 : 0;
            uint16_t i = _wc_globalState->keymaps[ks - subtr];
            uint64_t lp = (i << 15) | (1 << 10);
            uint64_t rp;

            printf("kp %i (char %c, ks %lu)\n", i, i, ks);
            if (subtr == 0)
            {
                rp = i;
                if (!(event.xkey.state & ShiftMask) &&
                    !(event.xkey.state & LockMask))
                {
                    rp = tolower(i);
                }

                int error, nb;
                char buffer[4];
                char* ch = buffer;
                if (d->ictx)
                {
                    nb = Xutf8LookupString(d->ictx, &event.xkey, 
                                        buffer, sizeof(buffer) - 1,
                                        NULL, &error);
                    if (error == XBufferOverflow)
                    {
                        ch = calloc(nb - 1, sizeof(char));
                        nb = Xutf8LookupString(d->ictx, &event.xkey, 
                                        ch, nb - 1,
                                        NULL, &error);
                    }

                    if (error == XLookupBoth || error == XLookupChars)
                    {
                        /*  
                            This function call assumes the called
                           LookupString above is Xutf8, given that the
                           maximum size of a UTF-8 char is
                           4 bytes. 
                            Beware when changing Xutf8 to
                           something such as Xmb 
                        */
                        memcpy(&rp, ch, nb - 1);
                    }
                    if (ch != buffer)
                    {
                        free(ch);
                    }
                }

                RunEvent(d->evh, WC_EVENT_CHAR, lp, rp);
            }
            else 
            {
                rp = 0;
            }

            RunEvent(d->evh, WC_EVENT_KEYPRESS, lp, rp);
            break;
        }
        }
    }
}

Window wX11GetWindowHandle(wWindow* window)
{
    return window->wind;
}

Display* wX11GetWindowDisplay(wWindow* w)
{
    return w->display;
}

void wX11ClearWindow(wWindow* w)
{
    XClearWindow(w->display, w->wind);
}
