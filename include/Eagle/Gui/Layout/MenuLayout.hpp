



#ifndef MenuLayout_HPP
#define MenuLayout_HPP




#include "Eagle/Gui/Layout/GridLayout.hpp"


enum MENU_LAYOUT_DIRECTION {
   MENU_HORIZONTAL = 0,
   MENU_VERTICAL = 1
};


class MenuLayout : public GridLayout {
   
   MENU_LAYOUT_DIRECTION layout_direction;

   int menu_size;
   
public :
   
   MenuLayout();
   
   
   void ResizeMenu(int new_menu_size);
   
   
};






#endif // MenuLayout_HPP





