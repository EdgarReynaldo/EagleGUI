



#ifndef TextIconButton_HPP
#define TextIconButton_HPP


#include "Eagle/Gui/Button/IconButton.hpp"
#include "Eagle/Font.hpp"
#include <string>


class TextIconButton : public IconButton {
   
   std::string label;
   EagleFont* label_font;

   virtual int PrivateUpdate(double dt) {(void)dt;return DIALOG_OKAY;}
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   
   /// Private interface, override to define behavior

   /// Callbacks, overload if you need to
   virtual void OnFlagChanged(WIDGET_FLAGS f , bool on);
   
public :
   TextIconButton(EagleFont* font , std::string text , std::string classname = "TextIconButton" , std::string objname = "Nemo");

   void DisplayIcon(EagleGraphicsContext* win , BUTTON_STATE state , int xpos , int ypos);
   virtual void DisplayText(EagleGraphicsContext* win , int xpos , int ypos , EagleColor tint = EagleColor(255,255,255));
};


#endif // TextIconButton_HPP
