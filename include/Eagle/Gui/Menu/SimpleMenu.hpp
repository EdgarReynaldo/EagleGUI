
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
 *    Copyright 2009-2022+ by Edgar Reynaldo
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

class EagleImage;
class EagleFont;

#include <string>


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



enum MENU_MESSAGES {
   MENU_ITEM_ACTIVATED = 1,
   MENU_ITEM_TOGGLED   = 2
};



REGISTER_WIDGET_MESSAGE(TOPIC_MENU , MENU_ITEM_ACTIVATED);
REGISTER_WIDGET_MESSAGE(TOPIC_MENU , MENU_ITEM_TOGGLED);




class MenuBase {
   
protected :
   
public :
   virtual ~MenuBase() {}
   /// NO DATA
   virtual void OpenMe()=0;
   virtual void CloseMe()=0;
   virtual bool IsOpen()=0;

   virtual void SetSubMenu(int index , MenuBase* smenu)=0;
   
   
   
   bool IsClosed() {return !IsOpen();}
};





class MenuItemBase {
public :
   MenuBase* parentmenu;
   MenuBase* submenu;
   
   virtual void Toggle()=0;
   virtual void Activate()=0;
   virtual bool HasSubMenu()=0;
   virtual void OpenSubMenu()=0;
   virtual void CloseSubMenu()=0;
   
   virtual void SetSubMenu(MenuBase* smenu)=0;
};










#endif // SimpleMenu_HPP
