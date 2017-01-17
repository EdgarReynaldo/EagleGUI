

#include "Eagle/Gui/Button/ScrollButton.hpp"
#include "Eagle/Gui/ScrollBar.hpp"


void BasicScrollButton::Scroll() {
   if (!our_scrollbar) {return;}
   int scroll = (scroll_up_or_left?-increment:increment);
   our_scrollbar->ScrollBy(scroll);
}



void BasicScrollButton::Reset() {
   UseBasicButton();
}



void BasicScrollButton::ResetTriangle() {
   Triangle* t = &our_click_area;
   const Rectangle r = area.InnerArea();
   int x1 = 0;
   int y1 = 0;
   int x2 = 0;
   int y2 = 0;
   int x3 = 0;
   int y3 = 0;
   if (scroll_horizontal) {
      /// Left right scroller
      x1 = 0;
      y1 = r.H()/2;
      x2 = r.W();
      x3 = x2;
      y2 = 0;
      y3 = r.H();
      if (scroll_up_or_left) {
         /// Scroll button points left
         /// Already set up for triangle facing left
      }
      else {
         /// Scroll button points right
         /// Flip x coords
         x1 = r.W();
         x2 = r.W() - x2;
         x3 = x2;
      }
   }
   else {
      /// Up down scroller
      x1 = r.W()/2;
      y1 = 0;
      x2 = r.W();
      x3 = 0;
      y2 = r.H();
      y3 = y2;
      if (scroll_up_or_left) {
         /// Scroll button points up
         /// Already setup for triangle facing up.
      }
      else {
         /// Scroll button points down
         /// Flip y coords
         y1 = r.H();
         y2 = r.H() - y2;
         y3 = y2;
      }
   }
   t->SetPos(x1,y1,x2,y2,x3,y3);
}



void BasicScrollButton::SyncButtonArea() {
   scroll_button->WidgetBase::SetWidgetArea(InnerArea() , false);
   if (scroll_button == &our_basic_button) {
      ResetTriangle();
   }
}



void BasicScrollButton::UseBasicButton() {
   UseButton(&our_basic_button);
   ResetTriangle();
   our_basic_button.SetClickArea(&our_click_area , false);
}




void BasicScrollButton::UseButton(BasicButton* button) {
   if (!button) {
      UseBasicButton();
   }
   EAGLE_ASSERT(button);
   scroll_button = button;
   scroll_button->SetParent(this);
   scroll_button->SetButtonType(SPRING_BTN);
   SyncButtonArea();
}



void BasicScrollButton::SetScrollBar(BasicScrollBar* scrollbar) {
   our_scrollbar = scrollbar;
}



void BasicScrollButton::SetIncrement(int new_increment) {
   increment = new_increment;
}



void BasicScrollButton::SetScrollDirection(bool up_or_left , bool horizontal) {
   scroll_up_or_left = up_or_left;
   scroll_horizontal = horizontal;
   if (scroll_button == &our_basic_button) {
      ResetTriangle();
   }
}



int BasicScrollButton::PrivateHandleEvent(EagleEvent e) {
   return scroll_button->HandleEvent(e);
}




int BasicScrollButton::PrivateCheckInputs() {
   return DIALOG_OKAY;
}



void BasicScrollButton::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   if (scroll_button == &our_basic_button) {
      Triangle t = our_click_area;
      t.MoveBy(InnerArea().X() + xpos , InnerArea().Y() + ypos);
      if (scroll_button->Up()) {
         t.Fill(win , WCols()[MGCOL]);
         t.Draw(win , 3.0 , WCols()[HLCOL]);
      }
      else {
         t.Fill(win , WCols()[BGCOL]);
         t.Draw(win , 3.0 , WCols()[FGCOL]);
      }
   }
   else {
      scroll_button->Display(win,xpos,ypos);
   }
   scroll_button->ClearRedrawFlag();
   ClearRedrawFlag();
}



int BasicScrollButton::PrivateUpdate(double tsec) {
   return scroll_button->Update(tsec);
}



void BasicScrollButton::QueueUserMessage(const WidgetMsg& wmsg) {

   const WidgetMsg clickmessage(scroll_button , TOPIC_BUTTON_WIDGET , BUTTON_CLICKED);
   const WidgetMsg heldmessage(scroll_button , TOPIC_BUTTON_WIDGET , BUTTON_HELD);

   const WidgetMsg releasemessage(scroll_button , TOPIC_BUTTON_WIDGET , BUTTON_RELEASED);

   if ((wmsg == clickmessage) || (wmsg == heldmessage)) {
      Scroll();
   }

   if (wmsg == clickmessage) {
      RaiseEvent(WidgetMsg(this , TOPIC_BUTTON_WIDGET , BUTTON_CLICKED));
   }
   else if (wmsg == heldmessage) {
      RaiseEvent(WidgetMsg(this , TOPIC_BUTTON_WIDGET , BUTTON_HELD));
   }
   else if (wmsg == releasemessage) {
      RaiseEvent(WidgetMsg(this , TOPIC_BUTTON_WIDGET , BUTTON_RELEASED));
   }
   WidgetBase::QueueUserMessage(wmsg);
}

      


void BasicScrollButton::SetColorset(const WidgetColorset& colors , bool set_descendants_colors) {
   scroll_button->SetColorset(colors , set_descendants_colors);
}



void BasicScrollButton::SetPrivateColorset(const WidgetColorset& colors) {
   scroll_button->SetPrivateColorset(colors);
}



void BasicScrollButton::UseColorset(bool use_public_colorset) {
   scroll_button->UseColorset(use_public_colorset);
}



void BasicScrollButton::UsePrivateColorset(bool use_priv_colorset) {
   scroll_button->UsePrivateColorset(use_priv_colorset);
}




void BasicScrollButton::SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout) {
   WidgetBase::SetWidgetArea(xpos,ypos,width,height,notify_layout);
   SyncButtonArea();
}



void BasicScrollButton::SetMarginsContractFromOuter(int left , int right , int top , int bottom) {
   WidgetBase::SetMarginsContractFromOuter(left,right,top,bottom);
   SyncButtonArea();
}



