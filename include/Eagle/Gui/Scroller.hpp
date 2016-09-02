



#ifndef Scroller_HPP
#define Scroller_HPP


#include "Eagle/Gui/WidgetBase.hpp"

#include <string>



extern const int TOPIC_SCROLL_WIDGET;

extern const int SCROLL_VALUE_CHANGED;



REGISTER_WIDGET_MESSAGE(TOPIC_SCROLL_WIDGET , SCROLL_VALUE_CHANGED);

extern double SCROLLER_REPEAT_DELAY;
extern double SCROLLER_NUM_REPEAT_PER_SEC;

class BasicScroller : public WidgetBase {

public :
   /// DIAGNOSTIC purposes only
   float GetStartPercent() {return start_percent;}
   float GetScrollPercent() {return scroll_percent;}
   
protected :
   
   WidgetArea scroller_area;
   
   LayoutRectangle scroll_handle_layout;
   Rectangle scroll_handle_area;

   int scroll_max;
   int scroll_val;
   float scroll_percent;

   int total_length;
   int length_in_view;
   
   bool horizontal_scroller;

   bool mouse_drag_handle;
   int start_mouse_x;
   int start_mouse_y;
   int start_percent;
   
   bool down;
   int repeat_scroll_increment;
   double repeat_delay;
   double repeat_rate;
   double repeat_elapsed;
   double repeat_previous;





   friend class BasicScrollBar;
   void ResetHandleArea();

   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);

public :
   BasicScroller();
   BasicScroller(std::string name);
   
   void SetScrollLength(int max_scroll);
   void SetScrollPercent(float new_percent);
   void SetScroll(int value);
   void ScrollBy(int amt);
   void ScrollBy(float percent);

   void SetScrollDirection(bool is_horizontal_scroller);
   void SetupView(int total_length_of_view , int actual_length_in_view);

   virtual void SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout = true);
   
   int GetScrollValue() {return scroll_val;}
   
   
   
};






#endif // Scroller_HPP


