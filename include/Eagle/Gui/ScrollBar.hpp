



#ifndef ScrollBar_HPP
#define ScrollBar_HPP


#include "Eagle/Gui/WidgetBase.hpp"

class BasicScroller;
class BasicScrollButton;
#include "Eagle/Gui/Scroller.hpp"
#include "Eagle/Gui/Button/ScrollButton.hpp"


class BasicScrollBar : public WidgetBase {
   
protected:
   
   BasicScroller basic_scroller;
   BasicScrollButton basic_scroll_button_up_or_left;
   BasicScrollButton basic_scroll_button_down_or_right;
   
   BasicScroller* scroller;
   BasicScrollButton* up_or_left_button;
   BasicScrollButton* down_or_right_button;
   
   bool is_horizontal;
   
   
   
   void ResetHandleArea();
   void ResetScrollBarArea();
   void CheckRedraw();
   
   
   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);

public :
   BasicScrollBar();
   BasicScrollBar(std::string name);
   
   virtual void QueueUserMessage(const WidgetMsg& wmsg);

   void SetScrollWidgets(BasicScroller* basic_scroller , 
                         BasicScrollButton* basic_up_or_left_button , BasicScrollButton* basic_down_or_right_button);

   void SetScrollLength(int max_scroll);
   void SetScrollPercent(float new_percent);
   void SetScroll(int value);
   void ScrollBy(int amt);
   void ScrollBy(float percent);

   void SetScrollDirection(bool is_horizontal_scroller);
   void SetupView(int total_length_of_view , int actual_length_in_view);

   virtual void SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout = true);
      
   int GetScrollValue() {return scroller->GetScrollValue();}
   
};


#endif // ScrollBar_HPP

