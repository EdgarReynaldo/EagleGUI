
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\____/     \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2023+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file SimpleMenu.hpp
 * @brief Base classes for menu work
 */

#ifndef SimpleMenu_HPP
#define SimpleMenu_HPP


#include "Eagle/Gui/Button/BasicButton.hpp"
#include "Eagle/InputHandler.hpp"
#include "Eagle/Area.hpp"



class EagleImage;
class EagleFont;

#include <string>
#include <vector>



class SIMPLE_MENU_ITEM {
public :
   BUTTON_ACTION_TYPE   batype;
   bool              down;
   EagleImage*       image_up;
   EagleImage*       image_dn;
   EagleFont*        text_font;
   std::string       description;
   InputGroup        hotkey;
   std::string       key_text;
};


class SIMPLE_MENU_BAR_ITEM {
public :
   EagleFont* text_font;
   std::string guitext;
   InputGroup hotkey;
};



extern const unsigned int TOPIC_MENU;


/**! @enum MENU_MESSAGES 
 *   @brief Simple enum to track menu messages
 */
enum MENU_MESSAGES {
   MENU_ITEM_ACTIVATED = 1,///< A menu item was activated
   MENU_ITEM_TOGGLED   = 2 ///< A menu item was toggled
};



REGISTER_WIDGET_MESSAGE(TOPIC_MENU , MENU_ITEM_ACTIVATED);
REGISTER_WIDGET_MESSAGE(TOPIC_MENU , MENU_ITEM_TOGGLED);



/**! @class MenuBase
 *   @brief The base class for a menu in Eagle
 */

class MenuBase {
   
protected :
   
public :
   virtual ~MenuBase() {}
   /// NO DATA
   virtual void OpenMe()=0;///< Open this menu
   virtual void CloseMe()=0;///< Close this menu
   virtual bool IsOpen()=0;///< True if open

   virtual void SetSubMenu(int index , MenuBase* smenu)=0;///< Allows you to set the sub menu for this menu's items
   
   bool IsClosed() {return !IsOpen();}///< True if closed
   
   virtual std::vector<Rectangle> SubTreeArea()=0;
};



/**! @class MenuItemBase
 *   @brief The base class for menu items in Eagle
 */

class MenuItemBase {
public :
   MenuBase* parentmenu;
   MenuBase* submenu;
   
   MenuItemBase() : parentmenu(0) , submenu(0) {}
   virtual ~MenuItemBase() {}
   
   virtual void Toggle()=0;
   virtual void Activate()=0;
   virtual void Deactivate()=0;
   virtual bool HasSubMenu()=0;
   virtual bool SubMenuOpen()=0;
   virtual void OpenSubMenu()=0;
   virtual void CloseSubMenu()=0;
   
   virtual void SetSubMenu(MenuBase* smenu)=0;
   
   virtual std::vector<Rectangle> SubTreeArea()=0;
};



#endif // SimpleMenu_HPP






