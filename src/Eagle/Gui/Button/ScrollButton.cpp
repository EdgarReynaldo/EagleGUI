
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
   const Rectangle r = InnerArea();
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
   if ((BasicButton*)scroll_button == &our_basic_button) {
      ResetTriangle();
   }
}



int BasicScrollButton::PrivateHandleEvent(EagleEvent e) {
   return scroll_button->HandleEvent(e);
}




void BasicScrollButton::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   if (scroll_button == &our_basic_button) {
      Triangle t = our_click_area;
      t.MoveBy(InnerArea().X() + xpos , InnerArea().Y() + ypos);
      WidgetColorset wc = WidgetColors();
      if (Flags().FlagOn(HOVER)) {
         if (scroll_button->Up()) {
            t.Fill(win , wc[HVRCOL]);
            t.Draw(win , 1.5 , wc[HLCOL]);
         }
         else {
            t.Fill(win , wc[BGCOL]);
            t.Draw(win , 1.5 , wc[FGCOL]);
         }
      }
      else {
         if (scroll_button->Up()) {
            t.Fill(win , wc[MGCOL]);
            t.Draw(win , 1.5 , wc[HLCOL]);
         }
         else {
            t.Fill(win , wc[BGCOL]);
            t.Draw(win , 1.5 , wc[FGCOL]);
         }
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



void BasicScrollButton::OnAreaChanged() {
   scroll_button->SetWidgetArea(GetWidgetArea() , false);
   SyncButtonArea();
}



void BasicScrollButton::OnAttributeChanged(const ATTRIBUTE& a , const VALUE& v) {
   scroll_button->SetAttribute(a , v);
}



void BasicScrollButton::OnFlagChanged(WIDGET_FLAGS f , bool on) {
   (void)f;
   (void)on;
   scroll_button->SetWidgetFlags(Flags());
}


/**
void BasicScrollButton::OnColorChanged() {
   scroll_button->SetWidgetColorset(GetWidgetColorset());/// Shouldn't need to do this, scroll_button inherits the colors from us
}
//*/


BasicScrollButton::BasicScrollButton(std::string objclass , std::string objname) :
      WidgetBase(objclass , objname),
      our_basic_button("BSB::our_basic_button"),
      scroll_button(),
      our_scrollbar(0),
      increment(1),
      scroll_up_or_left(true),
      scroll_horizontal(false),
      our_click_area()
{
   Reset();
}



void BasicScrollButton::UseBasicButton() {
   UseButton(&our_basic_button);
   ResetTriangle();
   our_basic_button.SetClickArea(&our_click_area , false);
}




void BasicScrollButton::UseButton(BasicButton* button) {
   if (!button) {
      UseBasicButton();
      return;
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








