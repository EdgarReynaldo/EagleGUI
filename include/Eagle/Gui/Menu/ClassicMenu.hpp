
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
 * @file ClassicMenu.hpp
 * @brief The interface for a classic menu in Eagle
 */

#ifndef CLASSIC_MENU_HPP
#define CLASSIC_MENU_HPP


#include "Eagle/Gui/Menu/SimpleMenu.hpp"
#include "Eagle/Gui/Layout/MenuLayout.hpp"
#include "Eagle/GUi/Menu/MenuItem.hpp"
#include "Eagle/Events.hpp"

#include <vector>



/**! @class ClassicMenu @brief simple implentation of a classic menu
 */
class ClassicMenu : public ClassicMenuLayout , public EagleEventListener {
protected :
   std::vector<SIMPLE_MENU_ITEM> items;
   std::vector<ClassicMenuItemLayout*> mitems;
   bool open;
   ClassicMenuItemLayout* citem;/// current menu item
   
   virtual void RespondToEvent(EagleEvent e , EagleThread* thread = MAIN_THREAD);
   
   
   void CloseOtherMenus(ClassicMenuItemLayout* mitem);
   
public :
   
   ClassicMenu();
   
   void Clear();
   void SetItems(SIMPLE_MENU_ITEM* menu , int msize);
   
   /// MenuBase
   
   virtual void OpenMe();
   virtual void CloseMe();
   virtual bool IsOpen();

   virtual void SetSubMenu(int index , MenuBase* smenu);


};



#endif // CLASSIC_MENU_HPP






