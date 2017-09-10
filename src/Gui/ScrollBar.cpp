


#include "Eagle/Gui/ScrollBar.hpp"





void BasicScrollBar::ResetHandleArea() {
   scroller->ResetHandleArea();
   SetRedrawFlag();
}



void BasicScrollBar::ResetScrollBarArea() {
   if (is_horizontal) {
      LayoutRectangle left_button_layout(0.0,0.0,0.1,1.0);
      LayoutRectangle right_button_layout(0.9,0.0,0.1,1.0);
      LayoutRectangle scroller_layout(0.1,0.0,0.8,1.0);
      up_or_left_button->WidgetBase::SetWidgetArea(LayoutArea(InnerArea() , left_button_layout) , false);
      down_or_right_button->WidgetBase::SetWidgetArea(LayoutArea(InnerArea() , right_button_layout) , false);
      scroller->WidgetBase::SetWidgetArea(LayoutArea(InnerArea() , scroller_layout) , false);
   }
   else {
      LayoutRectangle up_button_layout(0.0,0.0,1.0,0.1);
      LayoutRectangle down_button_layout(0.0,0.9,1.0,0.1);
      LayoutRectangle scroller_layout(0.0,0.1,1.0,0.8);
      up_or_left_button->WidgetBase::SetWidgetArea(LayoutArea(InnerArea() , up_button_layout) , false);
      down_or_right_button->WidgetBase::SetWidgetArea(LayoutArea(InnerArea() , down_button_layout) , false);
      scroller->WidgetBase::SetWidgetArea(LayoutArea(InnerArea() , scroller_layout) , false);
   }
   ResetHandleArea();
}



void BasicScrollBar::CheckRedraw() {
   if ((scroller->Flags() & NEEDS_REDRAW) ||
       (up_or_left_button->Flags() & NEEDS_REDRAW) ||
       (down_or_right_button->Flags() & NEEDS_REDRAW)) {
      SetRedrawFlag();
   }
   if ((scroller->Flags() & NEEDS_BG_REDRAW) ||
       (up_or_left_button->Flags() & NEEDS_BG_REDRAW) ||
       (down_or_right_button->Flags() & NEEDS_BG_REDRAW)) {
      SetBgRedrawFlag();
   }
}



int BasicScrollBar::PrivateHandleEvent(EagleEvent e) {
   int ret = DIALOG_OKAY;
   ret |= scroller->HandleEvent(e);
   ret |= up_or_left_button->HandleEvent(e);
   ret |= down_or_right_button->HandleEvent(e);
///   CheckRedraw();
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
///   CheckRedraw();
   return ret;
}



BasicScrollBar::BasicScrollBar(std::string objclass , std::string objname) :
      WidgetBase(objclass , objname),
      basic_scroller(),
      basic_scroll_button_up_or_left(),
      basic_scroll_button_down_or_right(),
      scroller(0),
      up_or_left_button(0),
      down_or_right_button(0)
{
   SetScrollWidgets(&basic_scroller , &basic_scroll_button_up_or_left , &basic_scroll_button_down_or_right);
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



void BasicScrollBar::SetScrollWidgets(BasicScroller* pbasic_scroller , BasicScrollButton* pbasic_up_or_left_button , BasicScrollButton* pbasic_down_or_right_button) {
   scroller = pbasic_scroller;

   scroller = &basic_scroller;
   if (pbasic_scroller) {
      scroller = pbasic_scroller;
   }

   up_or_left_button = &basic_scroll_button_up_or_left;
   if (pbasic_up_or_left_button) {
      up_or_left_button = pbasic_up_or_left_button;
   }

   down_or_right_button = &basic_scroll_button_down_or_right;
   if (pbasic_down_or_right_button) {
      down_or_right_button = pbasic_down_or_right_button;
   }
   
   scroller->SetParent(this);
   up_or_left_button->SetParent(this);
   down_or_right_button->SetParent(this);
   
   up_or_left_button->SetScrollBar(this);
   down_or_right_button->SetScrollBar(this);

   ResetScrollBarArea();
   SetBgRedrawFlag();
}



void BasicScrollBar::SetScrollLength(int max_scroll) {
   scroller->SetScrollLength(max_scroll);
   ResetHandleArea();
   SetRedrawFlag();
}



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



void BasicScrollBar::SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout) {
   WidgetBase::SetWidgetArea(xpos,ypos,width,height,notify_layout);
   ResetScrollBarArea();
   SetRedrawFlag();
}
   


   
   
   
   
   
