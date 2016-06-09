

#include "Eagle/Gui/Layout/MenuLayout.hpp"



MenuLayout::MenuLayout() :
      GridLayout(StringPrintF("MenuLayout object at %p" , this)),
      layout_direction(MENU_VERTICAL),
      menu_size(0)
{}



void MenuLayout::ResizeMenu(int size) {
   switch(layout_direction) {
   case MENU_HORIZONTAL :
      ResizeGrid(menu_size , 1);
      break;
   case MENU_VERTICAL :
      ResizeGrid(1,menu_size);
      break;
   default :
      throw EagleError("Unknown MENU_LAYOUT_DIRECTION");
   }
}






