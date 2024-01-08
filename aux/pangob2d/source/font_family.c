#include "blend2d/api.h"
#include "blend2d/array.h"
#include "blend2d/fontface.h"
#include "blend2d/fontmanager.h"
#include "blend2d/string.h"
#include "pangob2d.h"
#include <pango/pango-font.h>
#include <pango/pangofc-fontmap.h>
#include "private_font_face.h"
#include "private_font_family.h"

#define blArrayGet(A, t, i) (&((t*)blArrayGetData(A))[i])

G_DEFINE_TYPE(PangoBlend2DFontFamily, pango_b2d_font_family, pango_font_family_get_type());

PangoBlend2DFontFamily*
_pango_b2d_font_family_new(const char* name, size_t namel,
                           BLFontManagerCore* fman)
{
    if (namel == 0) namel = strlen(name);

    BLArrayCore array;
    PangoBlend2DFontFamily* fam = g_object_new(pango_b2d_font_family_get_type(), NULL);

    blStringInitWithData(&fam->familyName, name, namel);
    blArrayInit(&array, BL_OBJECT_TYPE_FONT_FACE);
    blFontManagerQueryFacesByFamilyName(fman, name, namel, &array);
    fam->faces = array;
    fam->fman = fman;
    return fam;
}

static PangoFontFace*
pango_b2d_font_family_get_face(PangoFontFamily* family, const char* name)
{
    PangoBlend2DFontFamily* self = (PangoBlend2DFontFamily*)family;
    if (!name) name = "Regular";

    BLStringCore fam;
    blStringInit(&fam);

    BLFontFaceInfo inf;

    BLStringCore facename;
    BLStringCore nfam;
    blStringInitWeak(&nfam, &self->familyName);
    blStringInitWithData(&facename, name, strlen(name));

    size_t nfamlen = blStringGetSize(&nfam);
    blStringInsertChar(&nfam, nfamlen, ' ', 1);
    nfamlen++;

    BLFontFaceCore ret;
    blFontFaceInit(&ret);
    BLFontQueryProperties prop = {};
    blStringInsertData(&nfam, nfamlen, name, strlen(name));
    blFontManagerQueryFace(self->fman, name, strlen(name), &prop, &ret);

    PangoFontFace* face = g_object_new(pango_b2d_font_face_get_type(), NULL);
    _pango_b2d_font_face_set_face((PangoBlend2DFontFace*)face, ret);
    return face;
}

static void
pango_b2d_font_family_constructed(GObject* obj)
{
    G_OBJECT_CLASS (pango_b2d_font_family_parent_class)->constructed(obj);
}

static void
pango_b2d_font_family_finalize(GObject *obj)
{
    PangoBlend2DFontFamily* self = (PangoBlend2DFontFamily*)(obj);
    blStringDestroy(&self->familyName);
    blArrayDestroy(&self->faces);
    G_OBJECT_CLASS (pango_b2d_font_family_parent_class)->finalize (obj);
}

static void
pango_b2d_font_family_class_init(PangoBlend2DFontFamilyClass* klass)
{
    GObjectClass* object_class = G_OBJECT_CLASS (klass);
    PangoFontFamilyClass* pangcl = PANGO_FONT_FAMILY_CLASS(klass);

    pangcl->get_face = pango_b2d_font_family_get_face;
    object_class->constructed = pango_b2d_font_family_constructed;
    object_class->finalize = pango_b2d_font_family_finalize;
}

static void
pango_b2d_font_family_init(PangoBlend2DFontFamily* self)
{
    
}
