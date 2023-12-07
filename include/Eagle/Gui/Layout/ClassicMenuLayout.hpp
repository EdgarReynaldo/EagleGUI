
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
 *    Copyright 2009-2023+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file MenuLayout.hpp
 * @brief The interface for menu layouts in Eagle
 */


#ifndef MenuLayout_HPP
#define MenuLayout_HPP


#include "Eagle/Gui/Layout/GridLayout.hpp"
#include "Eagle/Gui/Menu/SimpleMenu.hpp"

#include <string>



/**! @enum MENU_LAYOUT_DIRECTION
 *   @brief Determines the direction of the classic menu layout
 */
enum MENU_LAYOUT_DIRECTION {
   MENU_HORIZONTAL = 0,///< Layout this menu horizontally
   MENU_VERTICAL = 1   ///< Layout this menu vertically
};



/**! @class ClassicMenuLayout
 *   @brief A classic axis aligned menu layout, ready to use
 */

class ClassicMenuLayout : public MenuBase , public GridLayout {
   
   MENU_LAYOUT_DIRECTION layout_direction;

   int menu_size;
   
public :
   
   ClassicMenuLayout(std::string objname = "Nemo");
   
   virtual void Resize(unsigned int nsize);///< Resize the menu

   
   
   
   /// Classic menu options
   
   void ResizeMenu(int new_menu_size , MENU_LAYOUT_DIRECTION new_direction = MENU_VERTICAL);///< Resize the menu, and change its direction
   
   MENU_LAYOUT_DIRECTION Direction() {return layout_direction;}///< Returns the direction this menu is laid out in
   


   virtual void OpenMe();
   virtual void CloseMe();
   virtual bool IsOpen();
   
   virtual void SetSubMenu(int index , MenuBase* smenu);
};


/**! @class ClassicMenuBarLayout
 *   @brief A classic menu bar, laid out horizontally
 *
 */

class ClassicMenuBarLayout : public ClassicMenuLayout {
   
   
   virtual int PrivateHandleEvent(EagleEvent ee);
   
public :
   ClassicMenuBarLayout(std::string objname = "Nemo");

   virtual void Resize(unsigned int nsize);///< Resize the menu


};




#endif //MenuLayout_HPP

