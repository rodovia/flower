#pragma once

#include "blend2d.h"
#include <pango/pango.h>

typedef struct PangoBlend2DFontFamily
{
    PangoFontFamily base;
    BLStringCore familyName;
    BLArrayCore faces;
    BLFontManagerCore* fman;
} PangoBlend2DFontFamily;

typedef struct PangoBlend2DFontFamilyClass
{
    PangoFontFamilyClass base;
} PangoBlend2DFontFamilyClass;

PangoBlend2DFontFamily* _pango_b2d_font_family_new(const char* name, size_t nameLength, 
                                                   BLFontManagerCore* fman);

