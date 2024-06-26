
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



#include "Eagle/Gui/Button/IconButton.hpp"
#include "Eagle/Font.hpp"
#include "Eagle/Gui/WidgetHandler.hpp"


IconButtonBase::IconButtonBase(std::string classname , std::string objname) :
      BasicButton(classname , objname)
{}



void IconButton::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   DisplayIcon(win , btn_state , xpos , ypos);
/**
   Rectangle r = click_rect;
   r.MoveBy(InnerArea().X() , InnerArea().Y());
   Rectangle r2 = InnerArea();
   
   int mx = RootGui()->GetMouseX();
   int my = RootGui()->GetMouseY();
      
   win->DrawRectangle(r , 7.0 , EagleColor(255,0,255));
   win->DrawRectangle(r2 , 3.0 , EagleColor(0,255,0));
   win->DrawTextString(win->DefaultFont() , StringPrintF("Mouse xy = %d,%d" , mx , my) ,
                        InnerArea().X() , InnerArea().BRY() + 10 , EagleColor(255,0,0));
//*/
}



void IconButton::OnAreaChanged() {
   click_rect.SetArea(0,0,InnerArea().W(),InnerArea().H());
}



IconButton::IconButton(std::string classname , std::string objname) :
      IconButtonBase(classname , objname),
      original_images(),
      click_rect()
{
   SetClickArea(&click_rect , false);
}



void IconButton::SetImages(EagleImage* upimage , EagleImage* downimage , EagleImage* hoverupimage , EagleImage* hoverdownimage) {
   original_images[BUTTON_UP] = upimage;
   original_images[BUTTON_DOWN] = downimage;
   original_images[BUTTON_HOVER_UP] = hoverupimage;
   original_images[BUTTON_HOVER_DOWN] = hoverdownimage;
   SetBgRedrawFlag();
}



void IconButton::DisplayIcon(EagleGraphicsContext* win , BUTTON_STATE state , int xpos , int ypos , EagleColor tint) {
   EagleImage* image = original_images[(int)state];
   EAGLE_ASSERT(image && image->Valid());
   
   
   win->DrawImageStretch(image , InnerArea().MovedBy(Pos2I(xpos,ypos)) , tint);
   
   
///   win->DrawTinted(image , InnerArea().X() + xpos , InnerArea().Y() + ypos , tint);
//   win->DrawTinted(image , InnerArea().X() + xpos , InnerArea().Y() + ypos , tint);
}



/// --------------------------     HoverIcon     -------------------------------------



void HoverIcon::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   if (Hover()) {
      if (Up()) {
         DisplayIcon(win , btn_state , xpos , ypos , GetColor(HLCOL));
      }
      else {
         DisplayIcon(win , btn_state , xpos , ypos , GetColor(MGCOL));
      }
   }
   else {
      if (Up()) {
         DisplayIcon(win , btn_state , xpos , ypos , GetColor(FGCOL));
      }
      else {
         DisplayIcon(win , btn_state , xpos , ypos , GetColor(BGCOL));
      }
   }
}




