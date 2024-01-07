#include "blend2d/fontface.h"
#include "private_font.h"

G_DEFINE_TYPE(PangoBlend2DFont, pango_b2d_font, pango_font_get_type());

void _pango_b2d_font_set_face(PangoBlend2DFont* font, BLFontFaceCore face)
{
    font->font = face;
}

static void
pango_b2d_font_constructed(GObject* obj)
{
    G_OBJECT_CLASS (pango_b2d_font_parent_class)->constructed(obj);
}

static void
pango_b2d_font_finalize(GObject *obj)
{
    PangoBlend2DFont* self = (PangoBlend2DFont*)(obj);
    G_OBJECT_CLASS (pango_b2d_font_parent_class)->finalize (obj);
}

static void
pango_b2d_font_class_init(PangoBlend2DFontClass* klass)
{
    GObjectClass* object_class = G_OBJECT_CLASS (klass);
    PangoFontFamilyClass* pangcl = PANGO_FONT_FAMILY_CLASS(klass);

    object_class->constructed = pango_b2d_font_constructed;
    object_class->finalize = pango_b2d_font_finalize;
}

static void
pango_b2d_font_init(PangoBlend2DFont* self)
{
    
}


