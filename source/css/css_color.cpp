#include <algorithm>
#include <string_view>
#include <unordered_map>

#include "css.h"
#include <cmath>

#define min(X, y) ((X < y) ? X : y)

/* Colors safe for the web. */
static const std::unordered_map<std::string_view, css::css_color> 
websafeColors =
{
    /* CSS Color Module Level 3, 4.1 "Basic color keywords" */
    { "black", { 0, 0, 0 } },
    { "silver", { 192, 192, 192 } },
    { "gray", { 128, 128, 128 } },
    { "white", { 255, 255, 255 } },
    { "maroon", { 128, 0, 0 } },
    { "red", { 255, 0, 0 } },
    { "purple", { 128, 0, 128 } },
    { "fuchsia", { 255, 0, 255 } },
    { "green", { 0, 128, 0 } },
    { "lime", { 0, 255, 0 } },
    { "olive", { 128, 128, 0 } },
    { "yellow", { 255, 255, 0 } },
    { "navy", { 0, 0, 128 } },
    { "blue", { 0, 0, 255 } },
    { "teal", { 0, 128, 128 } },
    { "aqua", { 0, 255, 255 } },

    /* CSS Color Module Level 3, 4.3 "Extended color keywords" */
    { "aliceblue", { 240, 248, 255 } },
    { "antiquewhite", { 250, 235, 215 } },
    { "aqua", { 0, 255, 255 } },
    { "aquamarine", { 127, 255, 212 } },
    { "azure", { 240, 255, 255 } },
    { "beige", { 245, 245, 220 } },
    { "bisque", { 255, 228, 196 } },
    { "blanchedalmond", { 255, 235, 205 } },
    { "blueviolet", { 138, 43, 226 } },
    { "brown", { 165, 42, 42 } },
    { "burlywood", { 222, 184, 135 } },
    { "cadetblue", { 95, 158, 160 } },
    { "chartreuse", { 127, 255, 0 } },
    { "chocolate", { 210, 105, 30 } },
    { "coral", { 255, 127, 80 } },
    { "cornflowerblue", { 100, 149, 237 } },
    { "cornsilk", { 100, 149, 237 } },
    { "crimson", { 220, 20, 60 } },
    { "cyan", { 0, 255, 255 } },
    { "darkblue", { 0, 0, 139 } },
    { "darkcyan", { 0, 139, 139 } },
    { "darkgoldenrod", { 184, 134, 11 } },
    { "darkgray", { 169, 169, 169 } },
    { "darkgreen", { 0, 100, 0 } },
    { "darkgrey", { 169, 169, 169 } },
    { "darkkakhi", { 189, 183, 107 } },
    { "darkmagenta", { 139, 0, 139 } },
    { "darkolivegreen", { 85, 107, 47 } },
    { "darkorange", { 255, 140, 0 } },
    { "darkorchid", { 153, 50, 204 } },
    { "darkred", { 139, 0, 0 } },
    { "darksalmon", { 233, 150, 122 } },
    { "darkseagreen", { 143, 188, 143 } },
    { "darkslateblue", { 72, 61, 139 } },
    { "darkslategray", { 47, 49, 49 } },
    { "darkslategrey", { 47, 49, 49 } },
    { "darkturquoise", { 0, 206, 206 } },
    { "darkviolet", { 148, 0, 211 } },
    { "deeppink", { 255, 20, 147 } },
    { "deepskyblue", { 0, 191, 255 } },
    { "dimgray", { 105, 105, 105 } },
    { "dimgrey", { 105, 105, 105 } },
    { "dimgray", { 105, 105, 105 } },
    { "dodgerblue", { 30, 144, 255 } },
    { "firebrick", { 178, 34, 34 } },
    { "floralwhite", { 255, 250, 240 } },
    { "forestgreen", { 34, 139, 34 } },
    { "fuchsia", { 255, 0, 255 } },
    { "gainsboro", { 220, 220, 220 } },
    { "gold", { 255, 215, 0 } },
    { "goldenrod", { 210, 165, 32 } },
    { "gray", { 128, 128, 128 } },
    { "greenyellow", { 173, 255, 47 } },
    { "grey", { 128, 128, 128 } },
    { "honeydew", { 240, 255, 240 } },
    { "hotpink", { 255, 105, 180 } },
    { "indianred", { 205, 92, 92 } },
    { "indigo", { 75, 0, 130 } },
    { "ivory", { 255, 255, 240 } },
    { "khaki", { 240, 230, 140 } },
    { "lavender", { 230, 230, 250 } },
    { "lavenderblush", { 255, 240, 245 } },
    { "lawngreen", { 124, 252, 0 } },
    { "lemonchiffon", { 255, 250, 205 } },
    { "lightblue", { 173, 216, 230} },
    { "lightcoral", { 240, 128, 128 } },
    { "lightcyan", { 224, 255, 255 } },
    { "lightgoldenrodyellow", { 250, 250, 210 } },
    { "lightgray", { 211, 211, 211 } },
    { "lightgreen", { 144, 238, 144 } },
    { "lightgrey", { 255, 182, 193 } },
    { "lightpink", { 255, 182, 193 } },
    { "lightsalmon", { 255, 160, 122 } },
    { "lightseagreen", { 32, 178, 170 } },
    { "lightskyblue", { 135, 206, 250 } },
    { "lightslategray", { 119, 136, 153 } },
    { "lightslategrey", { 119, 136, 153 } },
    { "lightsteelblue", { 176, 196, 222 } },
    { "lightyellow", { 255, 255, 224 } },
    { "lime", { 0, 255, 0 } },
    { "limegreen", { 50, 205, 50 } },
    { "linen", { 250, 240, 230 } },
    { "magenta", { 255, 0, 255 } },
    { "mediumaquamarine", { 102, 205, 170 } },
    { "mediumblue", { 0, 0, 205 } },
    { "mediumorchid", { 186, 85, 211 } },
    { "mediumpurple", { 147, 112, 219 } },
    { "mediumseagreen", { 60, 179, 113 } },
    { "mediumslateblue", { 123, 104, 238 } },
    { "mediumspringgreen", { 0, 250, 154 } },
    { "mediumturquoise", { 72, 209, 204 } },
    { "mediumvioletred", { 199, 21, 133 } },
    { "midnightblue", { 25, 25, 112 } },
    { "mintcream", { 245, 255, 250 } },
    { "mistyrose", { 225, 228, 181 } },
    { "moccasin", { 225, 228, 181 } },
    { "navajowhite", { 255, 222, 173 } },
    { "oldlace", { 253, 245, 230 } },
    { "olivedrab", { 107, 142, 35 } },
    { "orange", { 255, 165, 0 } },
    { "orangered", { 255, 69, 0 } },
    { "orchid", { 218, 112, 214 } },
    { "palegoldenrod", { 238, 232, 170 } },
    { "palegreen", { 152, 251, 152 } },
    { "paleturquoise", { 175, 238, 238 } },
    { "palevioletred", { 219, 112, 147 } },
    { "papayawhip", { 255, 239, 213 } },
    { "peachpuff", { 255, 218, 185 } },
    { "peru", { 205, 133, 63 } },
    { "pink", { 255, 192, 203 } },
    { "plum", { 221, 160, 221 } },
    { "powderblue", { 176, 223, 230 } },
    { "rebeccapurple", { 102, 51, 153 } }, /* Defined in CSS Color Level 4, 6.1 "Named Colors" */
    { "rosybrown", { 188, 143, 143 } },
    { "royalblue", { 65, 105, 225 } },
    { "saddlebrown", { 139, 69, 19 } },
    { "salmon", { 250, 128, 114 } },
    { "sandybrown", { 244, 164, 96 } },
    { "seagreen", {46, 139, 87 } },
    { "seashell", { 255, 245, 238 } },
    { "sienna", { 160, 82, 45 } },
    { "skyblue", { 135, 206, 235 } },
    { "slateblue", { 106, 90, 205 } },
    { "slategray", { 112, 128, 144 } },
    { "slategrey", { 112, 128, 144 } },
    { "snow", { 255, 250, 250 } },
    { "springgreen", { 0, 255, 127 } },
    { "steelblue", { 70, 130, 180 } },
    { "tan", { 210, 180, 140 } },
    { "teal", { 0, 128, 128 } },
    { "thistle", { 216, 191, 216 } },
    { "tomato", { 255, 99, 71 } },
    { "turquoise", { 64, 224, 208 } },
    { "violet", { 238, 130, 238 } },
    { "wheat", { 245, 222, 179 } },
    { "whitesmoke", { 245, 245, 245 } },
    { "yellow", { 255, 255, 0 } },
    { "yellowgreen", { 154, 205, 50 } }
};

static constexpr bool __between(int x, int y, int val)
{
    return ((unsigned int)(val - x) <= y);
}

/* Converts a named CSS color to a css_color
   (or does nothing, if the basic_value is already color-like) */
bool css::EnsureColorCorrect(const css_basic_value& bv, css_color& cls)
{
    if (bv.Type == kCssValueTypeColor)
    {
        cls = static_cast<const css_color&>(bv);
        return true;
    }

    const css_string& st = static_cast<const css_string&>(bv);
    auto clit = websafeColors.find(st.String);
    if (clit == websafeColors.end())
    {
        return false;
    }

    cls = clit->second;
    return true;
}

/* Adapted from Wikipedia. See 
    <https://en.wikipedia.org/wiki/HSL_and_HSV#HSL_to_RGB> 
    
    TODO: Provide sanity checks? */
void css::ConvertHslRgb(std::array<float, 3>& output)
{
    std::array<float, 3> prergb;
    auto& h = output[0], 
        & s = output[1], 
        & l = output[2];

    s /= 100;
    l /= 100;

    auto chroma = (1 - std::abs((2 * l) - 1)) * s;
    auto hue = h / 60;
    auto x = chroma * (1 - std::abs((int)hue % 2 - 1));

    if (__between(0, 1, hue)) prergb = { chroma, x, 0 };
    if (__between(1, 2, hue)) prergb = { x, chroma, 0 };
    if (__between(2, 3, hue)) prergb = { 0, chroma, x };
    if (__between(3, 4, hue)) prergb = { 0, x, chroma };
    if (__between(4, 5, hue)) prergb = { x, 0, chroma };
    if (__between(5, 6, hue)) prergb = { chroma, 0, x };

    auto m = l - (chroma / 2);
    output[0] = (prergb[0] + m) * 255;
    output[1] = (prergb[1] + m) * 255;
    output[2] = (prergb[2] + m) * 255;
}
