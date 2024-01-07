#include <blend2d.h>
#include "blend2d/font.h"
#include "pangob2d.h"
#include "private_font_family.h"
#include <fontconfig/fontconfig.h>
#include <pango/pango-font.h>
#include <pango/pango-fontmap.h>
#include <pango/pango-types.h>

typedef struct PangoBlend2DFontMap
{
    PangoFontMap base;
    BLFontManagerCore* fontmg;
} PangoBlend2DFontMap;

typedef struct PangoBlend2DFontMapClass
{
    PangoFontMapClass klass;
    GArray* families;
} PangoBlend2DFontMapClass;

G_DEFINE_TYPE(PangoBlend2DFontMap, pango_b2d_fontmap, pango_font_map_get_type());

static GArray* _b2d_get_families()
{
    GArray* arr = g_array_new(false, true, sizeof(PangoBlend2DFontFamily));
    FcConfig* fc = FcInitLoadConfigAndFonts();
    FcPattern* pt = FcPatternCreate();
    FcObjectSet* st = FcObjectSetBuild(FC_FAMILY, NULL);
    FcFontSet* fonts = FcFontList(fc, pt, st);

    PangoBlend2DFontFamily* faml;
    for (int i = 0; i < fonts->nfont; ++i)
    {
        faml = g_object_new(pango_b2d_font_family_get_type(), NULL);
        g_array_append_val(arr, faml);
    }

    return arr;
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
    klass->families = _b2d_get_families();
}

static void
pango_b2d_fontmap_init(PangoBlend2DFontMap* self)
{
    self->fontmg = malloc(sizeof(*self->fontmg));
    blFontManagerInit(self->fontmg);
}

PangoBlend2DFontMap*
pango_b2d_fontmap_create()
{
    return g_object_new(pango_b2d_fontmap_get_type(), NULL);
}
