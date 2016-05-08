



#ifndef ScrollBar_HPP
#define ScrollBar_HPP


#include "Eagle/Gui/WidgetBase.hpp"

class Scroller;
class ScrollButton;


class ScrollBar : public WidgetBase {
   
protected:
   
   Scroller* scroller;
   ScrollButton* up_or_left_button;
   ScrollButton* down_or_right_button;
   
   
public:
   
   void ScrollBy(int amt) {}
   
   
   
};





#endif // ScrollBar_HPP

