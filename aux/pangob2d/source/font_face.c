#include "blend2d/string.h"
#include "private_font_face.h"
#include <pango/pango-font.h>

G_DEFINE_TYPE(PangoBlend2DFontFace, pango_b2d_font_face, pango_font_face_get_type());

static PangoFontDescription*
pango_b2d_font_face_create_description(BLFontFaceCore face)
{
    PangoFontDescription* d = g_object_new(pango_font_description_get_type(), NULL);
    BLStringCore name;
    blStringInit(&name);

    blFontFaceGetFamilyName(&face, &name);
    pango_font_description_set_family(d, blStringGetData(&name));
    return d;
}

void _pango_b2d_font_face_set_face(PangoBlend2DFontFace* ff, BLFontFaceCore face)
{
    ff->face = face;
    ff->desc = pango_b2d_font_face_create_description(face);
}

static PangoFontDescription* 
pango_b2d_font_face_describe(PangoFontFace* face)
{
    return ((PangoBlend2DFontFace*)face)->desc;
}

static void
pango_b2d_font_face_constructed(GObject* obj)
{
    G_OBJECT_CLASS (pango_b2d_font_face_parent_class)->constructed(obj);
}

static void
pango_b2d_font_face_finalize(GObject *obj)
{
    PangoBlend2DFontFace* self = (PangoBlend2DFontFace*)(obj);
    G_OBJECT_CLASS (pango_b2d_font_face_parent_class)->finalize (obj);
}

static void
pango_b2d_font_face_class_init(PangoBlend2DFontFaceClass* klass)
{
    GObjectClass* object_class = G_OBJECT_CLASS (klass);
    PangoFontFaceClass* pangcl = PANGO_FONT_FACE_CLASS(klass);

    pangcl->describe = pango_b2d_font_face_describe;
    object_class->constructed = pango_b2d_font_face_constructed;
    object_class->finalize = pango_b2d_font_face_finalize;
}

static void
pango_b2d_font_face_init(PangoBlend2DFontFace* self)
{
    
}
