#include <blend2d.h>
#include "blend2d/font.h"
#include "blend2d/fontface.h"
#include "blend2d/fontmanager.h"
#include "pangob2d.h"
#include "private_font.h"
#include "private_font_family.h"
#include <fontconfig/fontconfig.h>
#include <pango/pango-font.h>
#include <pango/pango-fontmap.h>
#include <pango/pango-types.h>

typedef struct PangoBlend2DFontMap
{
    PangoFontMap base;
    BLFontManagerCore fontmg;
} PangoBlend2DFontMap;

typedef struct PangoBlend2DFontMapClass
{
    PangoFontMapClass klass;
    FcConfig* fontconfig;
    bool familiesdirty;
    GArray* families;
} PangoBlend2DFontMapClass;

G_DEFINE_TYPE(PangoBlend2DFontMap, pango_b2d_fontmap, pango_font_map_get_type());

PangoFont* pango_b2d_fontmap_load_font(PangoFontMap* fontmap, 
                                       PangoContext* context, 
                                       const PangoFontDescription* desc)
{
    PangoBlend2DFontMap* map = (PangoBlend2DFontMap*)fontmap;
    PangoFontMask mask = pango_font_description_get_set_fields(desc);
    BLFontFaceCore cr;
    BLFontQueryProperties props = {0};

    const char* ff = "\0";
    size_t size = 12;
    if (mask & PANGO_FONT_MASK_FAMILY)
        ff = pango_font_description_get_family(desc);

    /* Blend2D uses the same values as Pango. */
    if (mask & PANGO_FONT_MASK_STYLE)    
        props.style = pango_font_description_get_style(desc);
    if (mask & PANGO_FONT_MASK_STRETCH)
        props.stretch = pango_font_description_get_stretch(desc);

    if (mask & PANGO_FONT_MASK_SIZE)
    {
        float pred = pango_font_description_get_size(desc);
        if (pango_font_description_get_size_is_absolute(desc))
        {
            pred /= PANGO_SCALE;
            pred *= (72.0f / 96.0f);
        }
        size = pred;
    }

    blFontFaceInit(&cr);
    blFontManagerQueryFace(&map->fontmg, ff, size, &props, &cr);

    PangoBlend2DFont* font = g_object_new(pango_b2d_font_get_type(), NULL);
    _pango_b2d_font_set_face(font, cr, size);
    return (PangoFont*)font;
}

static void
pango_b2d_fontmap_constructed(GObject* obj)
{
    G_OBJECT_CLASS (pango_b2d_fontmap_parent_class)->constructed(obj);
}

static void
pango_b2d_fontmap_finalize(GObject *obj)
{
    PangoBlend2DFontMap* self = (PangoBlend2DFontMap*)(obj);
    G_OBJECT_CLASS (pango_b2d_fontmap_parent_class)->finalize (obj);
}

void
pango_b2d_fontmap_class_init(PangoBlend2DFontMapClass* klass)
{
    GObjectClass* object_class = G_OBJECT_CLASS (klass);
    PangoFontMapClass* pangcl = PANGO_FONT_MAP_CLASS(klass);

    object_class->constructed = pango_b2d_fontmap_constructed;
    object_class->finalize = pango_b2d_fontmap_finalize;

    klass->klass.load_font = pango_b2d_fontmap_load_font;

    klass->fontconfig = FcInitLoadConfigAndFonts();
    klass->families = g_array_new(true, true, sizeof(PangoBlend2DFontFamily));
}

static void
pango_b2d_fontmap_init(PangoBlend2DFontMap* self)
{
    blFontManagerInit(&self->fontmg);
}

PangoBlend2DFontMap*
pango_b2d_fontmap_create()
{
    return g_object_new(pango_b2d_fontmap_get_type(), NULL);
}
