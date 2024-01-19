/* css_units.h
   Purpose: CSS units and values definitions */

#pragma once

#include "css.h"
#include "html_node.h"
#include <cfloat>

namespace html
{

struct dom_element_node;

}

namespace css
{

/* Absolute distance units */
enum css_distance_units : int
{
    kCssDistanceUnitCentimeter,
    kCssDistanceUnitMillimeter,
    kCssDistanceUnitQuaterMillimeter,
    kCssDistanceUnitInch,
    kCssDistanceUnitPica,
    kCssDistanceUnitPoint,
    kCssDistanceUnitPixel /*< Reference unit */
};

constexpr float GetValueInReferenceUnits(css_distance_units Du)
{
    constexpr float pxInCm = 96.0f / 2.54;

    if (Du == kCssDistanceUnitCentimeter)
        return pxInCm;
    else if (Du == kCssDistanceUnitInch)
        return pxInCm / 10;
    else if (Du == kCssDistanceUnitQuaterMillimeter)
        return pxInCm / 40;
    else if (Du == kCssDistanceUnitInch)
        return 96.0f;
    else if (Du == kCssDistanceUnitPica)
        return 96.0f / 6;
    else if (Du == kCssDistanceUnitPoint)
        return 96.0f / 72;

    /* Pixel */
    return 1;
}

struct css_relative_units_base
{
    css_relative_units_base()
        : FontSize(FLT_MIN),
          CharacterAdvance(FLT_MIN),
          PercentageReference(0)
    {}

    float FontSize; /* for 'em' */
    float CharacterAdvance; /* for 'ch' and 'ic' */

    double PercentageReference; /* what '%' refers to. */
};

float EnsureReferenceUnit(const css_basic_value& bv, 
                          css_relative_units_base& base, 
                          std::shared_ptr<html::dom_node> node);
void HandleRelativeUnit(css_relative_units_base& base, 
                        std::shared_ptr<html::dom_node> node);

}
