



#include "Eagle/Gui/Button/TextIconButton.hpp"
#include "Eagle/Gui/Button/IconButton.hpp"
#include "Eagle/GraphicsContext.hpp"
#include "Eagle/Font.hpp"



void TextIconButton::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   DisplayIcon(win , btn_state , xpos , ypos);
   DisplayText(win , xpos , ypos , GetColor(TXTCOL));
}



void TextIconButton::OnFlagChanged(WIDGET_FLAGS f , bool on) {
   BasicButton::OnFlagChanged(f , on);
   if (f & NEEDS_REDRAW) {
      SetBgRedrawFlag();
   }
}



TextIconButton::TextIconButton(std::string classname , std::string objname) :
      IconButton(classname , objname),
      label(""),
      label_font(0)
{}



TextIconButton::TextIconButton(EagleFont* font , std::string text , std::string classname , std::string objname) :
      IconButton(classname , objname),
      label(text),
      label_font(font)
{}



void TextIconButton::SetupText(EagleFont* font , std::string text) {
   label_font = font;
   label = text;
}



void TextIconButton::DisplayIcon(EagleGraphicsContext* win , BUTTON_STATE state , int xpos , int ypos , EagleColor tint) {
   (void)tint;/// hack to avoid warning
   switch (state) {
   case BUTTON_UP :
      IconButton::DisplayIcon(win , state , xpos , ypos , GetColor(FGCOL));
      break;
   case BUTTON_DOWN :
      IconButton::DisplayIcon(win , state , xpos , ypos , GetColor(BGCOL));
      break;
   case BUTTON_HOVER_UP :
      IconButton::DisplayIcon(win , state , xpos , ypos , GetColor(HLCOL));
      break;
   case BUTTON_HOVER_DOWN :
      IconButton::DisplayIcon(win , state , xpos , ypos , GetColor(MGCOL));
      break;
   default :
      break;
   };
}



void TextIconButton::DisplayText(EagleGraphicsContext* win , int xpos , int ypos , EagleColor textcolor) {
   win->DrawTextString(label_font , label , InnerArea().X() + xpos , InnerArea().CY() + ypos , textcolor , HALIGN_LEFT , VALIGN_CENTER);
}



