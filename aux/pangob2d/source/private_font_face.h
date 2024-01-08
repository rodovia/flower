#pragma once

#include "blend2d.h"
#include <pango/pango-font.h>

typedef struct PangoBlend2DFontFace
{
    PangoFontFace base;
    BLFontFaceCore face;
    PangoFontDescription* desc;
} PangoBlend2DFontFace;

typedef struct PangoBlend2DFontFaceClass
{
    PangoFontFaceClass base;
} PangoBlend2DFontFaceClass;

extern GType pango_b2d_font_face_get_type();
void _pango_b2d_font_face_set_face(PangoBlend2DFontFace* ff,
                                   BLFontFaceCore face);
