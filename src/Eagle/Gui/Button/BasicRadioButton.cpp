



#include "Eagle/Gui/Button/BasicRadioButton.hpp"




int BasicRadioButton::PrivateHandleEvent(EagleEvent ee) {
   int ret = our_button->HandleEvent(ee);
   if (ret != DIALOG_INPUT_USED) {
      ret = our_text->HandleEvent(ee);
   }
   return ret;
}



int BasicRadioButton::PrivateCheckInputs() {
   return DIALOG_OKAY;
}



int BasicRadioButton::PrivateUpdate(double dt) {
   our_button->Update(dt);
   our_text->Update(dt);
   return DIALOG_OKAY;
}



void BasicRadioButton::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   our_button->Display(win , xpos , ypos);
   our_text->Display(win , xpos , ypos);
}



void BasicRadioButton::OnAreaChanged() {
   ResetWidgetAreas();
}



void BasicRadioButton::ResetWidgetAreas() {
   Rectangle in = InnerArea();
   EAGLE_ASSERT(our_button);
   EAGLE_ASSERT(our_text);
   our_button->SetWidgetArea(Rectangle(in.X() , in.Y() , in.W()/5 , in.H()) , false);
   our_text->SetWidgetArea(Rectangle(in.X() + in.W()/5 , in.Y() , in.W()/4 , in.H()) , false);
}



BasicRadioButton::BasicRadioButton(BasicButton* button_to_use , BasicText* text_to_use , std::string classname , std::string objname) :
      WidgetBase(classname , objname),
      basic_button("BasicButton" , objname + " basic_button"),
      basic_text(objname + " basic_text"),
      our_button(0),
      our_text(0)
{
   SetWidgets(button_to_use , text_to_use);
}



void BasicRadioButton::SetWidgets(BasicButton* button_to_use , BasicText* text_to_use) {
   if (!button_to_use) {
      button_to_use = &basic_button;
   }
   if (!text_to_use) {
      text_to_use = &basic_text;
   }
   if (our_button) {
      our_button->SetParent(0);
      if (our_button != button_to_use) {
         button_to_use->SetButtonState(our_button->Hover() , our_button->Up() , false);
      }
   }
   if (our_text) {
      our_text->SetParent(0);
   }
   
   our_button = button_to_use;
   our_text = text_to_use;
   our_button->SetParent(this);
   our_text->SetParent(this);
   our_button->SetButtonType(TOGGLE_BTN);
   ResetWidgetAreas();
}
