#pragma once

#include "blend2d.h"
#include <pango/pango-font.h>

typedef struct PangoBlend2DFont
{
    PangoFont base;
    BLFontCore font;
    BLFontFaceCore face;
    PangoFontDescription* desc;
} PangoBlend2DFont;

typedef struct PangoBlend2DFontClass
{
    PangoFontClass base;
} PangoBlend2DFontClass;

void _pango_b2d_font_set_face(PangoBlend2DFont*, BLFontFaceCore, float size);
