
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
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#include "Eagle/Gui/Button/IconButton.hpp"
#include "Eagle/Font.hpp"
#include "Eagle/Gui/WidgetHandler.hpp"


void IconButton::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   EagleImage* image = original_images[(int)btn_state];
   EAGLE_ASSERT(image && image->Valid());
   win->DrawStretchedRegion(image , 0 , 0 , image->W() , image->H() ,
                            InnerArea().X() + xpos , InnerArea().Y() + ypos , InnerArea().W() , InnerArea().H());
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
      BasicButton(classname , objname),
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








