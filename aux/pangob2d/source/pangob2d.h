#pragma once

#include <glib-object.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Fontmap */
typedef struct PangoBlend2DFontMap PangoBlend2DFontMap;
GType pango_b2d_fontmap_get_type();
PangoBlend2DFontMap* pango_b2d_fontmap_create();

/* Font family */
GType pango_b2d_font_family_get_type();

#ifdef __cplusplus
} /* extern "C" */
#endif