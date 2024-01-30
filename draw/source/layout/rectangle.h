/* rectangle.h
   Purpose: Specify regions relative to something easily */
#pragma once

#include "css.h"

namespace draw
{

struct rectangle
{
    unsigned int X, Y;
    unsigned int Width, Height;

    __consteval_or_nothing rectangle()
        : X(0), Y(0),
          Width(0), Height(0)
    {}

    __constexpr_or_nothing rectangle(unsigned int x, unsigned int y,
                                     unsigned int w, unsigned int h)
        : X(x), Y(y),
          Width(w), Height(h)
    {}

    __constexpr_or_nothing inline bool InsideRect(unsigned int x, unsigned int y)
    {
            return std::clamp(x, X, Width) == x
                && std::clamp(y, Y, Height + Y) == y;
    }
    
    __constexpr_or_nothing inline rectangle operator-(const rectangle& rhs)
    {
       rectangle ret = *this;
       ret.Height -= rhs.Height;
       ret.Width -= rhs.Width;
       ret.X -= rhs.X;
       ret.Y -= rhs.Y;
       return ret;
    }
    
    __constexpr_or_nothing inline void operator-=(const rectangle& rhs)
    {
       *this = operator-(rhs);
    }
    
    __constexpr_or_nothing inline rectangle operator+(const rectangle& rhs)
    {
       rectangle ret = *this;
       ret.Height += rhs.Height;
       ret.Width += rhs.Width;
       ret.X += rhs.X;
       ret.Y += rhs.Y;
       return ret;
    }
    
    __constexpr_or_nothing inline void operator+=(const rectangle& rhs)
    {
       *this = operator+(rhs);
    }
};

}
