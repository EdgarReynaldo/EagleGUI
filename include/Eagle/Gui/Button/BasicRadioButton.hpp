



#ifndef BasicRadioButton_HPP
#define BasicRadioButton_HPP



#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/Gui/Button/BasicButton.hpp"
#include "Eagle/Gui/Text/BasicText.hpp"


class BasicRadioButton : public WidgetBase {
   
protected :
   BasicButton basic_button;/// default button widget
   BasicText basic_text;/// default text widget
   
   BasicButton* our_button;/// Button widget in use
   BasicText* our_text;/// Text widget in use
   
   
      
   
   virtual int PrivateHandleEvent(EagleEvent ee) override;
   virtual int PrivateCheckInputs() override;
   virtual int PrivateUpdate(double dt) override;
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) override;

   virtual void OnAreaChanged() override;
   

   void ResetWidgetAreas();


public :
   BasicRadioButton(BasicButton* button_to_use = 0 , BasicText* text_to_use = 0 , std::string classname = "BasicRadioButton" , std::string objname = "Nemo");

   virtual void SetRedrawFlag() override;

   void SetWidgets(BasicButton* button_to_use , BasicText* text_to_use);
   
};




#endif // BasicRadioButton_HPP
