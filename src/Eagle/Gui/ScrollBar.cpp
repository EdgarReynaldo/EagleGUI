
/**
 *
 *     _______       ___       ____      __       _______
 *    /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *    \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *     \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *      \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *       \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *        \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */


#include "Eagle/Gui/ScrollBar.hpp"





void BasicScrollBar::ResetHandleArea() {
   scroller->ResetHandleArea();
   SetRedrawFlag();
}



void BasicScrollBar::ResetScrollBarArea() {
   Rectangle inner = InnerArea();
   if (is_horizontal) {
      Rectangle left_button_area  = LayoutArea(inner , LayoutRectangle(0.0 , 0.0 , 0.1 , 1.0));
      Rectangle right_button_area = LayoutArea(inner , LayoutRectangle(0.9 , 0.0 , 0.1 , 1.0));
      Rectangle scroll_area       = LayoutArea(inner , LayoutRectangle(0.1 , 0.0 , 0.8 , 1.0));
      up_or_left_button->SetWidgetArea   (left_button_area  , false);
      down_or_right_button->SetWidgetArea(right_button_area , false);
      scroller->SetWidgetArea            (scroll_area       , false);
   }
   else {
      Rectangle up_button_area   = LayoutArea(inner , LayoutRectangle(0.0 , 0.0 , 1.0 , 0.1));
      Rectangle down_button_area = LayoutArea(inner , LayoutRectangle(0.0 , 0.9 , 1.0 , 0.1));
      Rectangle scroll_area      = LayoutArea(inner , LayoutRectangle(0.0 , 0.1 , 1.0 , 0.8));
      up_or_left_button->SetWidgetArea   (up_button_area   , false);
      down_or_right_button->SetWidgetArea(down_button_area , false);
      scroller->SetWidgetArea            (scroll_area      , false);
   }
   ResetHandleArea();
}



int BasicScrollBar::PrivateHandleEvent(EagleEvent e) {
   int ret = DIALOG_OKAY;
   ret |= up_or_left_button->HandleEvent(e);
   ret |= down_or_right_button->HandleEvent(e);
   if (!(ret & DIALOG_INPUT_USED)) {
      ret |= scroller->HandleEvent(e);
   }
   return ret;
}



int BasicScrollBar::PrivateCheckInputs() {
   return DIALOG_OKAY;
}



void BasicScrollBar::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   scroller->Display(win,xpos,ypos);
   up_or_left_button->Display(win,xpos,ypos);
   down_or_right_button->Display(win,xpos,ypos);
}



int BasicScrollBar::PrivateUpdate(double tsec) {
   int ret = DIALOG_OKAY;
   ret |= scroller->Update(tsec);
   ret |= up_or_left_button->Update(tsec);
   ret |= down_or_right_button->Update(tsec);
   return ret;
}



void BasicScrollBar::QueueUserMessage(const WidgetMsg& wmsg) {

   /// Messages from our scroll buttons and scroller
   const WidgetMsg scroll_msg(scroller , TOPIC_SCROLL_WIDGET , SCROLL_VALUE_CHANGED);
   const WidgetMsg ul_button_msg_click(up_or_left_button , TOPIC_BUTTON_WIDGET , BUTTON_CLICKED);
   const WidgetMsg ul_button_msg_held(up_or_left_button , TOPIC_BUTTON_WIDGET , BUTTON_HELD);
   const WidgetMsg dr_button_msg_click(down_or_right_button , TOPIC_BUTTON_WIDGET , BUTTON_CLICKED);
   const WidgetMsg dr_button_msg_held(down_or_right_button , TOPIC_BUTTON_WIDGET , BUTTON_HELD);

   /// Scroll message that we send out
   const WidgetMsg scrollbar_msg(this , TOPIC_SCROLL_WIDGET , SCROLL_VALUE_CHANGED);

   if ((wmsg == scroll_msg) ||
       (wmsg == ul_button_msg_click) ||
       (wmsg == ul_button_msg_held) ||
       (wmsg == dr_button_msg_click) ||
       (wmsg == dr_button_msg_held)) {
      SetRedrawFlag();
      RaiseEvent(scrollbar_msg);
      return;
   }
   WidgetBase::QueueUserMessage(wmsg);
}



void BasicScrollBar::OnAreaChanged() {
   ResetScrollBarArea();
}



BasicScrollBar::BasicScrollBar(std::string objclass , std::string objname) :
      WidgetBase(objclass , objname),
      basic_scroller(),
      basic_scroll_button_up_or_left(),
      basic_scroll_button_down_or_right(),
      scroller(),
      up_or_left_button(),
      down_or_right_button()
{
   SetScrollWidgets(SHAREDOBJECT<BasicScroller>() ,
                    SHAREDOBJECT<BasicScrollButton>() ,
                    SHAREDOBJECT<BasicScrollButton>() );
}



void BasicScrollBar::SetScrollWidgets(SHAREDOBJECT<BasicScroller> pbasic_scroller ,
                                      SHAREDOBJECT<BasicScrollButton> pbasic_up_or_left_button , 
                                      SHAREDOBJECT<BasicScrollButton> pbasic_down_or_right_button) {


   if (scroller) {
      scroller->SetParent(0);
   }
   if (up_or_left_button) {
      up_or_left_button->SetParent(0);
      up_or_left_button->SetScrollBar(0);
   }
   if (down_or_right_button) {
      down_or_right_button->SetParent(0);
      down_or_right_button->SetScrollBar(0);
   }
   
   scroller = pbasic_scroller;
   if (!scroller) {
      scroller = StackObject(&basic_scroller);
   }

   up_or_left_button = pbasic_up_or_left_button;
   if (!up_or_left_button) {
      up_or_left_button = StackObject(&basic_scroll_button_up_or_left);
   }

   down_or_right_button = pbasic_down_or_right_button;
   if (!down_or_right_button) {
      down_or_right_button = StackObject(&basic_scroll_button_down_or_right);
   }
   
   scroller->SetParent(this);
   up_or_left_button->SetParent(this);
   down_or_right_button->SetParent(this);
   
   up_or_left_button->SetScrollBar(this);
   down_or_right_button->SetScrollBar(this);

   ResetScrollBarArea();
   SetBgRedrawFlag();
}


/**
void BasicScrollBar::SetScrollLength(int max_scroll) {
   scroller->SetScrollLength(max_scroll);
   ResetHandleArea();
   SetRedrawFlag();
}
//*/


void BasicScrollBar::SetScrollPercent(float new_percent) {
   scroller->SetScrollPercent(new_percent);
   SetRedrawFlag();
}



void BasicScrollBar::SetScroll(int value) {
   scroller->SetScroll(value);
   SetRedrawFlag();
}



void BasicScrollBar::ScrollBy(int amt) {
   scroller->ScrollBy(amt);
   SetRedrawFlag();
}



void BasicScrollBar::ScrollBy(float percent) {
   scroller->ScrollBy(percent);
   SetRedrawFlag();
}



void BasicScrollBar::SetScrollDirection(bool is_horizontal_scrollbar) {
   is_horizontal = is_horizontal_scrollbar;

   scroller->SetScrollDirection(is_horizontal);
   up_or_left_button->SetScrollDirection(true , is_horizontal);
   down_or_right_button->SetScrollDirection(false , is_horizontal);

   ResetScrollBarArea();
}



void BasicScrollBar::SetupView(int total_length_of_view , int actual_length_in_view) {
   scroller->SetupView(total_length_of_view , actual_length_in_view);
   ResetHandleArea();

   int scroll_by_value = actual_length_in_view/2;
   up_or_left_button->SetIncrement(scroll_by_value);
   down_or_right_button->SetIncrement(scroll_by_value);

   SetRedrawFlag();
}



   


   
   
   
   
   
