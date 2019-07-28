
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file MenuLayout.hpp
 * @brief The interface for the MenuLayout class
 */

#ifndef MenuLayout_HPP
#define MenuLayout_HPP



#include "Eagle/Gui/Layout/GridLayout.hpp"


/**! @enum MENU_LAYOUT_DIRECTION
 *   @brief Determines the direction of the menu layout
 */
enum MENU_LAYOUT_DIRECTION {
   MENU_HORIZONTAL = 0,///< Layout this menu horizontally
   MENU_VERTICAL = 1   ///< Layout this menu vertically
};


/**! @class ClassicMenuLayout
 *   @brief A classic axis aligned menu layout
 */

class ClassicMenuLayout : public GridLayout {
   
   MENU_LAYOUT_DIRECTION layout_direction;

   int menu_size;
   
public :
   
   ClassicMenuLayout(std::string objname = "Nemo");
   
   virtual void Resize(unsigned int nsize);///< Resize the menu

   
   
   
   /// Classic menu options
   
   void ResizeMenu(int new_menu_size , MENU_LAYOUT_DIRECTION new_direction = MENU_VERTICAL);///< Resize the menu, and change its direction
   
   MENU_LAYOUT_DIRECTION Direction() {return layout_direction;}///< Returns the direction this menu is laid out in
   
};


/**! @class ClassicMenuBarLayout
 *   @brief A classic menu bar, laid out horizontally
 *
 *   TODO : COMPLETE ME
 *   Needs : Key shortcuts, hover, focus, etc...
 */

class ClassicMenuBarLayout : public GridLayout {
   
   
   virtual int PrivateHandleEvent(EagleEvent ee);
   
public :
   ClassicMenuBarLayout(std::string objname = "Nemo");

   virtual void Resize(unsigned int nsize);///< Resize the menu

};




#endif // MenuLayout_HPP



