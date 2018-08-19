

#include "Eagle/Gui/Layout/MenuLayout.hpp"
#include "Eagle/StringWork.hpp"



MenuLayout::MenuLayout(std::string objname) :
      GridLayout(0 , 0 , "MenuLayout" , objname),
      layout_direction(MENU_VERTICAL),
      menu_size(0)
{}



void MenuLayout::ResizeMenu(int new_menu_size , MENU_LAYOUT_DIRECTION new_direction) {
   layout_direction = new_direction;
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
   
   menu_size = GridLayout::GetLayoutSize();
}






