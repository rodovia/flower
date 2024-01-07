#include "pangob2d.h"
#include <pango/pango-font.h>
#include <pango/pangofc-fontmap.h>
#include "private_font_family.h"

G_DEFINE_TYPE(PangoBlend2DFontFamily, pango_b2d_font_family, pango_font_family_get_type());

static void
pango_b2d_font_family_constructed(GObject* obj)
{
    G_OBJECT_CLASS (pango_b2d_font_family_parent_class)->constructed(obj);
}

static void
pango_b2d_font_family_finalize(GObject *obj)
{
    PangoBlend2DFontFamily* self = (PangoBlend2DFontFamily*)(obj);
    G_OBJECT_CLASS (pango_b2d_font_family_parent_class)->finalize (obj);
}

static void
pango_b2d_font_family_class_init(PangoBlend2DFontFamilyClass* klass)
{
    GObjectClass* object_class = G_OBJECT_CLASS (klass);
    PangoFontFamilyClass* pangcl = PANGO_FONT_FAMILY_CLASS(klass);

    object_class->constructed = pango_b2d_font_family_constructed;
    object_class->finalize = pango_b2d_font_family_finalize;
}

static void
pango_b2d_font_family_init(PangoBlend2DFontFamily* self)
{
    
}
