
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
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file BasicTextButtonLayout.cpp
 * @brief The implementation for the BasicTextButton class
 */

 
#include "Eagle/Gui/Layout/BasicTextButtonLayout.hpp"




BasicTextButton::BasicTextButton() :
      LayoutBase("BasicTextButtonLayout" , "BasicTextButton"),
      basic_button("BasicButton" , "BasicTextButton"),
      basic_text("BasicText"),
      our_button(0),
      our_text(0)
{
   Resize(2);
   SetOurButton(&basic_button);
   SetOurText(&basic_text);
   SetZOrder(ZValue());
}



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
   our_button = button;
   PlaceWidget(our_button , 0);
   our_button->SetZOrder(ZValue() + 1);
}



void BasicTextButton::SetOurText(BasicText* text) {
   if (!text) {
      text = &basic_text;
   }
   our_text = text;
   PlaceWidget(our_text , 1);
   our_text->SetZOrder(ZValue() + 2);
}



void BasicTextButton::SetupText(std::string text , EagleFont* font , HALIGNMENT halign , VALIGNMENT valign , int hpadding , int vpadding , int lspacing) {
   our_text->SetupText(text , font , halign , valign , hpadding , vpadding , lspacing);
}



BasicButton* BasicTextButton::GetOurButton() {
   return our_button;
}



BasicText* BasicTextButton::GetOurText() {
   return our_text;
}



void BasicTextButton::SetPreferredSize(int pw , int ph) {
   our_text->SetPreferredSize(pw,ph);
   our_button->SetPreferredSize(our_text->PreferredWidth() + 4 , our_text->PreferredHeight() + 4);
   WidgetBase::SetPreferredSize(our_text->PreferredWidth() + 4 + OuterArea().W() - InnerArea().W() ,
                                our_text->PreferredHeight() + 4 + OuterArea().H() - InnerArea().H());
}

