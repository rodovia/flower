/* css_color.h
   Purpose: Functions used for color mgmt */

#pragma once

#include <array>
#include <stdint.h>

namespace css
{

struct css_basic_value;
struct css_color;

bool EnsureColorCorrect(const css_basic_value& bv, css_color& cls);
void ConvertHslRgb(std::array<float, 3>& output);

}
