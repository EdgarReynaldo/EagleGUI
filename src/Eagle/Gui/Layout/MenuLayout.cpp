
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


#include "Eagle/Gui/Layout/MenuLayout.hpp"
#include "Eagle/StringWork.hpp"



ClassicMenuLayout::ClassicMenuLayout(std::string objname) :
      GridLayout(0 , 0 , "MenuLayout" , objname),
      layout_direction(MENU_VERTICAL),
      menu_size(0)
{}



void ClassicMenuLayout::Resize(unsigned int nsize) {
   ResizeMenu(nsize , layout_direction);
   return;
}



void ClassicMenuLayout::ResizeMenu(int new_menu_size , MENU_LAYOUT_DIRECTION new_direction) {
   layout_direction = new_direction;
   std::vector<WidgetBase*> cvec = ChildrenVector();
   cvec.resize(new_menu_size);
   
   switch(layout_direction) {
   case MENU_HORIZONTAL :
      ResizeGrid(new_menu_size , 1);
      break;
   case MENU_VERTICAL :
      ResizeGrid(1,new_menu_size);
      break;
   default :
      throw EagleException("Unknown MENU_LAYOUT_DIRECTION");
   }
   
   SetWChildren(cvec);
   
   menu_size = GridLayout::GetLayoutSize();
}




/// -------------------------     ClassicMenuBarLayout     ------------------------



int ClassicMenuBarLayout::PrivateHandleEvent(EagleEvent ee) {
   (void)ee;
   return DIALOG_OKAY;
}



ClassicMenuBarLayout::ClassicMenuBarLayout(std::string objname) : 
      GridLayout(0,0,"ClassicMenuBarLayout" , objname)
{}



void ClassicMenuBarLayout::Resize(unsigned int nsize) {
   ResizeGrid(nsize , 1);
   return;
}
