/* scrollbar.h
   Purpose: Defines the scrollbar, a way of telling
            where you are inside the document.
*/

#pragma once

#include "source/drawable.h"
#include <memory>
#include <vector>

namespace draw
{

class CScrollBar : public IDrawable
{
public:
   CScrollBar(std::vector<std::shared_ptr<IDrawable>>& drawables);
   CScrollBar() = default;

   int GetHeight() override
   {
      return m_Height;
   }

   void Paint(const PainterState&) override;

private:
   int m_Height;
};

}
