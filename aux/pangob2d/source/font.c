#include "blend2d/font.h"
#include "blend2d/api.h"
#include "blend2d/fontface.h"
#include "blend2d/string.h"
#include "private_font.h"
#include <pango/pango-font.h>

G_DEFINE_TYPE(PangoBlend2DFont, pango_b2d_font, pango_font_get_type());

static PangoFontDescription* 
_pango_b2d_font_create_description(PangoBlend2DFont* font)
{
    BLStringCore family;
    blStringInit(&family);
    blFontFaceGetFamilyName(&font->face, &family);

    PangoFontDescription* de = pango_font_description_new();
    pango_font_description_set_family(de, blStringGetData(&family));
    blStringDestroy(&family);
    return de;
}

void _pango_b2d_font_set_face(PangoBlend2DFont* font, BLFontFaceCore face, float size)
{
    font->face = face;
    blFontCreateFromFace(&font->font, &font->face, size);
    font->desc = _pango_b2d_font_create_description(font);
}

static PangoFontDescription*
pango_b2d_font_describe(PangoFont* self)
{
    return ((PangoBlend2DFont*)self)->desc;
}

static PangoFontMetrics*
pango_b2d_font_get_metrics(PangoFont* font, PangoLanguage* language)
{
    PangoBlend2DFont* self = (PangoBlend2DFont*)font;

    BLFontMetrics met;
    blFontGetMetrics(&self->font, &met);
    PangoFontMetrics* fm = g_object_new(pango_font_metrics_get_type(), NULL);

    fm->approximate_char_width = met.xMax;
    fm->approximate_digit_width = met.xMax;
    fm->ascent = met.vAscent;
    fm->descent = met.vDescent;
    fm->height = met.capHeight;
    fm->strikethrough_position = met.strikethroughPosition;
    fm->strikethrough_thickness = met.strikethroughThickness;
    return fm;
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
    g_object_unref(self->desc);
    blFontFaceDestroy(&self->face);
    blFontDestroy(&self->font);
    G_OBJECT_CLASS (pango_b2d_font_parent_class)->finalize (obj);
}

static void
pango_b2d_font_class_init(PangoBlend2DFontClass* klass)
{
    GObjectClass* object_class = G_OBJECT_CLASS (klass);
    PangoFontClass* pangcl = PANGO_FONT_CLASS(klass);

    pangcl->describe = pango_b2d_font_describe;
    pangcl->get_metrics = pango_b2d_font_get_metrics;
    object_class->constructed = pango_b2d_font_constructed;
    object_class->finalize = pango_b2d_font_finalize;
}

static void
pango_b2d_font_init(PangoBlend2DFont* self)
{
    self->desc = NULL;
}


