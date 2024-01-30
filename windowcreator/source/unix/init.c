#include "windowcreator/init.h"
#include "windowcreator/key.h"
#include "globalstate.h"
#include <X11/Xlib.h>
#include <locale.h>

#define XK_MISCELLANY
#include <X11/keysym.h>
#include <stdlib.h>
#include <stdio.h>

_wGlobalState* _wc_globalState = NULL;

static inline void RegisterKeys()
{
#define _(X) (X - 0xff00)
    _wc_globalState->keymaps[_(XK_BackSpace)] = WC_KEY_BACKSPACE;
    _wc_globalState->keymaps[_(XK_Tab)] = WC_KEY_BACKSPACE;
    _wc_globalState->keymaps[_(XK_Return)] = WC_KEY_RETURN;
    _wc_globalState->keymaps[_(XK_Home)] = WC_KEY_HOME;
    _wc_globalState->keymaps[_(XK_Left)] = WC_KEY_ARROW_LEFT;
    _wc_globalState->keymaps[_(XK_Right)] = WC_KEY_ARROW_RIGHT;
    _wc_globalState->keymaps[_(XK_Up)] = WC_KEY_ARROW_UP;
    _wc_globalState->keymaps[_(XK_Down)] = WC_KEY_ARROW_DOWN;
    _wc_globalState->keymaps[_(XK_Prior)] = WC_KEY_UNKNOWN; /* I don't know */
    _wc_globalState->keymaps[_(XK_Page_Up)] = WC_KEY_PGUP;
    _wc_globalState->keymaps[_(XK_Page_Down)] = WC_KEY_PGDN;
    _wc_globalState->keymaps[_(XK_End)] = WC_KEY_END;
    _wc_globalState->keymaps[_(XK_Insert)] = WC_KEY_INSERT;
    _wc_globalState->keymaps[_(XK_Num_Lock)] = WC_KEY_NUMLK;
    
    _wc_globalState->keymaps[_(XK_F1)] = WC_KEY_F1;
    _wc_globalState->keymaps[_(XK_F2)] = WC_KEY_F2;
    _wc_globalState->keymaps[_(XK_F3)] = WC_KEY_F3;
    _wc_globalState->keymaps[_(XK_F4)] = WC_KEY_F4;
    _wc_globalState->keymaps[_(XK_F5)] = WC_KEY_F5;
    _wc_globalState->keymaps[_(XK_F6)] = WC_KEY_F6;
    _wc_globalState->keymaps[_(XK_F7)] = WC_KEY_F7;
    _wc_globalState->keymaps[_(XK_F8)] = WC_KEY_F8;
    _wc_globalState->keymaps[_(XK_F9)] = WC_KEY_F9;
    _wc_globalState->keymaps[_(XK_F10)] = WC_KEY_F10;
    _wc_globalState->keymaps[_(XK_F11)] = WC_KEY_F11;
    _wc_globalState->keymaps[_(XK_F12)] = WC_KEY_F12;
    /* Xlib defines all the way up to F35, but it is unnecessary, IMO. */

#undef _
    _wc_globalState->keymaps[XK_0] = WC_KEY_0;
    _wc_globalState->keymaps[XK_1] = WC_KEY_1;
    _wc_globalState->keymaps[XK_2] = WC_KEY_2;
    _wc_globalState->keymaps[XK_3] = WC_KEY_3;
    _wc_globalState->keymaps[XK_4] = WC_KEY_4;
    _wc_globalState->keymaps[XK_5] = WC_KEY_5;
    _wc_globalState->keymaps[XK_6] = WC_KEY_6;
    _wc_globalState->keymaps[XK_7] = WC_KEY_7;
    _wc_globalState->keymaps[XK_8] = WC_KEY_8;
    _wc_globalState->keymaps[XK_9] = WC_KEY_9;
    _wc_globalState->keymaps[XK_colon] = WC_KEY_COLON;
    _wc_globalState->keymaps[XK_semicolon] = WC_KEY_SEMICOLON;
    _wc_globalState->keymaps[XK_less] = WC_KEY_LEFT_CHREVON;
    _wc_globalState->keymaps[XK_greater] = WC_KEY_RIGHT_CHREVON;

    _wc_globalState->keymaps[XK_A] = WC_KEY_A;
    _wc_globalState->keymaps[XK_B] = WC_KEY_B;
    _wc_globalState->keymaps[XK_C] = WC_KEY_C;
    _wc_globalState->keymaps[XK_D] = WC_KEY_D;
    _wc_globalState->keymaps[XK_E] = WC_KEY_E;
    _wc_globalState->keymaps[XK_F] = WC_KEY_F;
    _wc_globalState->keymaps[XK_G] = WC_KEY_G;
    _wc_globalState->keymaps[XK_H] = WC_KEY_H;
    _wc_globalState->keymaps[XK_I] = WC_KEY_I;
    _wc_globalState->keymaps[XK_J] = WC_KEY_J;
    _wc_globalState->keymaps[XK_K] = WC_KEY_K;
    _wc_globalState->keymaps[XK_L] = WC_KEY_L;
    _wc_globalState->keymaps[XK_M] = WC_KEY_M;
    _wc_globalState->keymaps[XK_N] = WC_KEY_N;
    _wc_globalState->keymaps[XK_O] = WC_KEY_O;
    _wc_globalState->keymaps[XK_P] = WC_KEY_P;
    _wc_globalState->keymaps[XK_Q] = WC_KEY_Q;
    _wc_globalState->keymaps[XK_R] = WC_KEY_R;
    _wc_globalState->keymaps[XK_S] = WC_KEY_S;
    _wc_globalState->keymaps[XK_T] = WC_KEY_T;
    _wc_globalState->keymaps[XK_U] = WC_KEY_U;
    _wc_globalState->keymaps[XK_V] = WC_KEY_V;
    _wc_globalState->keymaps[XK_W] = WC_KEY_W;
    _wc_globalState->keymaps[XK_X] = WC_KEY_X;
    _wc_globalState->keymaps[XK_Y] = WC_KEY_Y;
    _wc_globalState->keymaps[XK_Z] = WC_KEY_Z;

    _wc_globalState->keymaps[XK_a] = WC_KEY_A;
    _wc_globalState->keymaps[XK_b] = WC_KEY_B;
    _wc_globalState->keymaps[XK_c] = WC_KEY_C;
    _wc_globalState->keymaps[XK_d] = WC_KEY_D;
    _wc_globalState->keymaps[XK_e] = WC_KEY_E;
    _wc_globalState->keymaps[XK_f] = WC_KEY_F;
    _wc_globalState->keymaps[XK_g] = WC_KEY_G;
    _wc_globalState->keymaps[XK_h] = WC_KEY_H;
    _wc_globalState->keymaps[XK_i] = WC_KEY_I;
    _wc_globalState->keymaps[XK_j] = WC_KEY_J;
    _wc_globalState->keymaps[XK_k] = WC_KEY_K;
    _wc_globalState->keymaps[XK_l] = WC_KEY_L;
    _wc_globalState->keymaps[XK_m] = WC_KEY_M;
    _wc_globalState->keymaps[XK_n] = WC_KEY_N;
    _wc_globalState->keymaps[XK_o] = WC_KEY_O;
    _wc_globalState->keymaps[XK_p] = WC_KEY_P;
    _wc_globalState->keymaps[XK_q] = WC_KEY_Q;
    _wc_globalState->keymaps[XK_r] = WC_KEY_R;
    _wc_globalState->keymaps[XK_s] = WC_KEY_S;
    _wc_globalState->keymaps[XK_t] = WC_KEY_T;
    _wc_globalState->keymaps[XK_u] = WC_KEY_U;
    _wc_globalState->keymaps[XK_v] = WC_KEY_V;
    _wc_globalState->keymaps[XK_w] = WC_KEY_W;
    _wc_globalState->keymaps[XK_x] = WC_KEY_X;
    _wc_globalState->keymaps[XK_y] = WC_KEY_Y;
    _wc_globalState->keymaps[XK_z] = WC_KEY_Z;

    _wc_globalState->keymaps[XK_period] = WC_KEY_PERIOD;
    _wc_globalState->keymaps[XK_slash] = WC_KEY_FORWARD_SLASH;
}

void wInitialize()
{
    if (!getenv("DISPLAY"))
    {
        printf("Cannot initialize WindowCreator: No display\n");        
        exit(1);
    }

    setlocale(LC_ALL, "");
    char* vl = XSetLocaleModifiers("");
    _wc_globalState = calloc(1, sizeof(_wGlobalState));
    RegisterKeys();
}
