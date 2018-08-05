


/*
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
 *    EAGLE
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2018+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */


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
   
   MenuLayout(std::string objname = "Nemo");
   
   
   void ResizeMenu(int new_menu_size , MENU_LAYOUT_DIRECTION new_direction = MENU_VERTICAL);
   
   MENU_LAYOUT_DIRECTION Direction() {return layout_direction;}
   
};






#endif // MenuLayout_HPP




