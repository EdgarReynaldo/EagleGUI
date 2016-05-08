



#ifndef Scroller_HPP
#define Scroller_HPP


#include "Eagle/Gui/WidgetBase.hpp"


class Scroller : public WidgetBase {
   
protected :
   
   WidgetArea scroller_area;
   
   LayoutRectangle scroll_handle_area;

   int scroll_max;
   int scroll_val;
   float scroll_percent;

   int total_length;
   int length_in_view;
   
   bool horizontal_scroller;
   
public :
   
   
   void SetScrollPercent(float new_percent) {}
   
};






#endif // Scroller_HPP


