#pragma once

#include "private_font.h"
#include <glib-object.h>
#include <pango/pango-font.h>
#include <pango/pango-types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Fontmap */
typedef struct PangoBlend2DFontMap PangoBlend2DFontMap;
extern GType pango_b2d_fontmap_get_type();
PangoBlend2DFontMap* pango_b2d_fontmap_create();
PangoFont* pango_b2d_fontmap_load_font(PangoFontMap* fontmap, 
                                       PangoContext* context, 
                                       const PangoFontDescription* desc);

/* Font family */
extern GType pango_b2d_font_family_get_type();

/* Font */
extern GType pango_b2d_font_get_type();
typedef struct PangoBlend2DFont PangoBlend2DFont;
PangoBlend2DFont* pango_b2d_font_new();

#ifdef __cplusplus
} /* extern "C" */
#endif