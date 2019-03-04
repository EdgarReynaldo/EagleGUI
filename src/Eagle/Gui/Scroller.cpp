



#include "Eagle/Gui/Scroller.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/GraphicsContext.hpp"


const int TOPIC_SCROLL_WIDGET = NextFreeTopicId();

const int SCROLL_VALUE_CHANGED = 0;



REGISTERED_WIDGET_MESSAGE(TOPIC_SCROLL_WIDGET , SCROLL_VALUE_CHANGED);


double SCROLLER_REPEAT_DELAY = 0.5;
double SCROLLER_NUM_REPEAT_PER_SEC = 16;



void BasicScroller::ResetHandleArea() {
   float handle_length_percent = (float)length_in_view/total_length;

   float start_of_handle_area = handle_length_percent/2.0f;

   float available_area_percent = 1.0f - handle_length_percent;
   
   float start_val = start_of_handle_area + scroll_percent*available_area_percent - handle_length_percent/2.0f;
   if (horizontal_scroller) {
      scroll_handle_layout = LayoutRectangle(start_val , 0.0f , handle_length_percent , 1.0f);
   }
   else {
      scroll_handle_layout = LayoutRectangle(0.0f , start_val , 1.0f , handle_length_percent);
   }
   scroll_handle_area = LayoutArea(scroller_area.InnerArea() , scroll_handle_layout);
   
   SetBgRedrawFlag();
}
   


int BasicScroller::PrivateHandleEvent(EagleEvent e) {
   int retmsg = DIALOG_OKAY;
   if (e.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN && e.mouse.button == 1) {
      int msx = e.mouse.x;
      int msy = e.mouse.y;
      if (InnerArea().Contains(msx,msy)) {
         retmsg |= DIALOG_TAKE_FOCUS;
         if (scroll_handle_area.Contains(msx,msy)) {
            mouse_drag_handle = true;
            start_mouse_x = msx;
            start_mouse_y = msy;
            start_percent = scroll_percent;
         }
         else {
            down = true;
            repeat_elapsed = repeat_previous = 0.0;
            float percent_to_scroll_by = ((float)length_in_view/total_length)/2.0f;
            if (horizontal_scroller) {
               if (msx < scroll_handle_area.X()) {
                  /// Scroll left, by 50% of the view length
                  ScrollBy(-percent_to_scroll_by);
                  repeat_scroll_increment = -length_in_view/2;
               }
               else if (msx > scroll_handle_area.BRX()) {
                  /// Scroll right, by 50% of the view length
                  ScrollBy(percent_to_scroll_by);
                  repeat_scroll_increment = length_in_view/2;
               }
            }
            else {/// Vertical scroller
               if (msy < scroll_handle_area.Y()) {
                  /// Scroll up
                  ScrollBy(-percent_to_scroll_by);
                  repeat_scroll_increment = -length_in_view/2;
               }
               else if (msy > scroll_handle_area.BRY()) {
                  ScrollBy(percent_to_scroll_by);
                  repeat_scroll_increment = length_in_view/2;
               }
            }
         }
      }
   }
   if (e.type == EAGLE_EVENT_MOUSE_AXES) {
      if (mouse_drag_handle) {
         int msx = e.mouse.x;
         int msy = e.mouse.y;
         int dx = msx - start_mouse_x;
         int dy = msy - start_mouse_y;
         int delta = horizontal_scroller?dx:dy;
         int length = 0;
         if (horizontal_scroller) {
            length = (scroller_area.InnerArea().W() - scroll_handle_area.W());
         }
         else if (!horizontal_scroller) {
            length = (scroller_area.InnerArea().H() - scroll_handle_area.H());
         }
         if (length == 0) {length = 1;}
         float percent = (float)delta/length;
         SetScrollPercent(start_percent + percent);
      }
   }
   if (e.type == EAGLE_EVENT_MOUSE_BUTTON_UP) {
      if (e.mouse.button == 1) {
         down = false;
         if (mouse_drag_handle) {
            mouse_drag_handle = false;
         }
      }
   }
   return retmsg;
}



int BasicScroller::PrivateCheckInputs() {
   return DIALOG_OKAY;
}


void BasicScroller::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   ///void WidgetBorderPainterShadow(EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos);
#warning TODO : FIXME : NEED PAINTER FUNCS
///   ContrastBorderBackgroundPainter painter;
   
   WidgetColorset wc = WidgetColors();
   
///   painter.PaintBackground(win , scroller_area , wc , xpos , ypos);
   Rectangle r = scroller_area.InnerArea();
   r.MoveBy(xpos,ypos);
   win->DrawFilledRectangle(r , wc[SDCOL]);
   Rectangle r2 = scroll_handle_area;
   r2.MoveBy(xpos,ypos);
   win->DrawFilledRectangle(r2 , wc[FGCOL]);
}



int BasicScroller::PrivateUpdate(double tsec) {
   (void)tsec;
   if (down) {
      repeat_elapsed += tsec;
      if (repeat_elapsed >= repeat_delay) {
         if (repeat_previous < repeat_delay) {
            repeat_previous = repeat_delay;
         }
         double dt = repeat_elapsed - repeat_previous;
         int num_repeat = dt/repeat_rate;
         for (int i = 0 ; i < num_repeat ; ++i) {
            ScrollBy(repeat_scroll_increment);
///            RaiseEvent(WidgetMsg(this , TOPIC_SCROLL_WIDGET , SCROLL_VALUE_CHANGED));
         }
         repeat_previous += (double)num_repeat*repeat_rate;
         EAGLE_ASSERT(repeat_previous <= repeat_elapsed);
      }
   }
   return DIALOG_OKAY;
}



void BasicScroller::OnAreaChanged() {
   scroller_area.SetOuterArea(InnerArea());
   ResetHandleArea();
}



BasicScroller::BasicScroller(std::string objclass , std::string objname) :
      WidgetBase(objclass , objname),
      scroller_area(),
      scroll_handle_layout(0.0f,0.0f,1.0f,1.0f),
      scroll_handle_area(),
      scroll_max(1),
      scroll_val(0),
      scroll_percent(0.0f),
      total_length(1),
      length_in_view(0),
      horizontal_scroller(true),
      mouse_drag_handle(false),
      start_mouse_x(0),
      start_mouse_y(0),
      start_percent(0.0f),
      down(false),
      repeat_delay(SCROLLER_REPEAT_DELAY),
      repeat_rate(1.0/SCROLLER_NUM_REPEAT_PER_SEC),
      repeat_elapsed(0.0),
      repeat_previous(0.0)
{
   scroller_area.SetWidgetArea(GetWidgetArea().SetBoxAreaContractFromOuter(BOX_TYPE_MARGIN , 5,5,5,5));
}



void BasicScroller::SetScrollLength(int max_scroll) {
   if (max_scroll < 1) {max_scroll = 1;}
   scroll_max = max_scroll;
   SetScrollPercent(scroll_percent);
}



void BasicScroller::SetScrollPercent(float new_percent) {
   if (new_percent < 0.0f) {new_percent = 0.0f;}
   if (new_percent >= 1.0f) {new_percent = 1.0f;}
   scroll_percent = new_percent;
   scroll_val = (int)(scroll_percent*scroll_max);
   ResetHandleArea();
   SetRedrawFlag();
   RaiseEvent(WidgetMsg(this , TOPIC_SCROLL_WIDGET , SCROLL_VALUE_CHANGED));
}


   
void BasicScroller::SetScroll(int value) {
   scroll_val = value;
   if (scroll_val < 0) {scroll_val = 0;}
   if (scroll_val > scroll_max) {scroll_val = scroll_max;}
   scroll_percent = (float)scroll_val/scroll_max;
   ResetHandleArea();
   SetRedrawFlag();
   RaiseEvent(WidgetMsg(this , TOPIC_SCROLL_WIDGET , SCROLL_VALUE_CHANGED));
}


   
void BasicScroller::ScrollBy(int amt) {
   SetScroll(scroll_val + amt);
}



void BasicScroller::ScrollBy(float percent) {
   SetScrollPercent(scroll_percent + percent);
}



void BasicScroller::SetScrollDirection(bool is_horizontal_scroller) {
   horizontal_scroller = is_horizontal_scroller;
   ResetHandleArea();
   SetBgRedrawFlag();
}
   


void BasicScroller::SetupView(int total_length_of_view , int actual_length_in_view) {
   if (total_length_of_view < 1) {total_length_of_view = 1;}
   if (actual_length_in_view < 0) {actual_length_in_view = 0;}
   total_length = total_length_of_view;
   length_in_view = actual_length_in_view;

   SetScrollPercent(0.0f);
}




