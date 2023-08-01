



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



TextIconButton::TextIconButton(EagleFont* font , std::string text , std::string classname , std::string objname) :
      IconButton(classname , objname),
      label(text),
      label_font(font)
{}



void TextIconButton::DisplayIcon(EagleGraphicsContext* win , BUTTON_STATE state , int xpos , int ypos) {
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



void TextIconButton::DisplayText(EagleGraphicsContext* win , int xpos , int ypos , EagleColor tint) {
   Transform t = win->GetTransformer()->GetViewTransform();
   
   t.Scale(1 , //0.8*InnerArea().W()/label_font->Width(label),
           0.8*InnerArea().H()/label_font->Height());
   t.Translate(xpos + InnerArea().CX() , ypos + InnerArea().CY());
   
   win->PushViewTransform(t);
   
   win->DrawTextString(label_font , label , 0 , 0 , tint , HALIGN_CENTER , VALIGN_CENTER);
   
   win->PopViewTransform();
}



