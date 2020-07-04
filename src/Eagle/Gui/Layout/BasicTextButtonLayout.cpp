
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2020+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file BasicTextButtonLayout.cpp
 * @brief The implementation for the BasicTextButton class
 */

 
#include "Eagle/Gui/Layout/BasicTextButtonLayout.hpp"




void BasicTextButton::SetZOrder(int zpriority) {
   WidgetBase::SetZOrder(zpriority);
   our_button->SetZOrder(ZValue() + 1);
   our_text->SetZOrder(ZValue() + 2);
}



Rectangle BasicTextButton::RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) {
   (void)widget_slot;
   (void)newx;
   (void)newy;
   (void)newwidth;
   (void)newheight;
   return InnerArea();
}



void BasicTextButton::Resize(unsigned int nsize) {
   (void)nsize;
   LayoutBase::Resize(2);
}



void BasicTextButton::SetOurButton(BasicButton* button) {
   if (!button) {
      button = &basic_button;
   }
   PlaceWidget(button , 0);
   button->SetZOrder(ZValue() + 1);
   our_button = button;
}



void BasicTextButton::SetOurText(BasicText* text) {
   if (!text) {
      text = &basic_text;
   }
   PlaceWidget(text , 0);
   text->SetZOrder(ZValue() + 2);
}



BasicButton* BasicTextButton::GetOurButton() {
   return our_button;
}



BasicText* BasicTextButton::GetOurText() {
   return our_text;
}




