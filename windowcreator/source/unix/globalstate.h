#pragma once

#include <stdint.h>

typedef struct _wGlobalState
{
    uint16_t keymaps[300]; /*< X KEY MAPPING!!! If porting
                            to Wayland, remember to make an
                            separate array for translation. */
} _wGlobalState;
