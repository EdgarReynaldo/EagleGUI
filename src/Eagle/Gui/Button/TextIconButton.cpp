



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
   EagleColor c = GetColor(FGCOL);
   if (state != BUTTON_UP) {
      if (state == BUTTON_DOWN) {
         c = GetColor(BGCOL);
      }
      else if (state == BUTTON_HOVER_UP) {
         c = GetColor(HLCOL);
      }
      else if (state == BUTTON_HOVER_DOWN) {
         c = GetColor(MGCOL);
      }
   }
   c.fa = c.fa*tint.fa;
   c.fr = c.fr*tint.fr;
   c.fg = c.fg*tint.fg;
   c.fb = c.fb*tint.fb;
   EagleImage* image = original_images[(int)state];
   EAGLE_ASSERT(image && image->Valid());

   win->DrawImageAligned(image , InnerArea().MovedBy(Pos2I(xpos,ypos)) , HALIGN_LEFT , VALIGN_CENTER , c);
}



void TextIconButton::DisplayText(EagleGraphicsContext* win , int xpos , int ypos , EagleColor textcolor) {
   EagleImage* image = original_images[0];
   win->DrawTextString(label_font , label , InnerArea().X() + xpos + image->W() , InnerArea().CY() + ypos , textcolor , HALIGN_LEFT , VALIGN_CENTER);
}



