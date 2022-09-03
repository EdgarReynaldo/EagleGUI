
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
 * @file MenuItem.hpp
 * @brief Interface for menu items in Eagle
 */


#ifndef MENU_ITEM_HPP
#define MENU_ITEM_HPP


#include "Eagle/Gui/Menu/SimpleMenu.hpp"
#include "Eagle/Events.hpp"
#include "Eagle/Gui/Layout/Layout.hpp"

#include <memory>

#include "Eagle/Gui/Button/GuiButton.hpp"


class BasicCheckBox;
class BasicText;
class BasicScrollButton;
class BasicButton;
class SimpleTable;

/**! @class ClassicMenuItemLayout
 *   @brief A classic menu item layout including a checkbox, description text, key text, and a scroll button to open sub menus
 */

class ClassicMenuItemLayout : public LayoutBase , public MenuItemBase , public EagleEventListener {

protected :
   SIMPLE_MENU_ITEM item;/// Our init data
   
   std::unique_ptr<BasicCheckBox>          cbox;/// Checkbox for toggle buttons
   std::unique_ptr<BasicText>         dtext;/// Text button to display description of command
   std::unique_ptr<BasicText>         ktext;/// Label to tell what key to press
   std::unique_ptr<BasicScrollButton> hbtn;/// MENU_BTN for hover trigger to open sub menu
   
   MenuBase* submenu;
   
   std::unique_ptr<BasicButton> menu_button;
   std::unique_ptr<SimpleTable> item_layout;

   
   
   /// EagleEventListener
   virtual void RespondToEvent(EagleEvent e , EagleThread* thread = MAIN_THREAD);

   friend class ClassicMenu;
   
public :
   
   ClassicMenuItemLayout(SIMPLE_MENU_ITEM* mitem = 0);

   virtual ~ClassicMenuItemLayout ();

   void Clear();

   void SetItemInfo(SIMPLE_MENU_ITEM* mitem);

   /// LayoutBase
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight);
   
   
   void Toggle();
   void Activate();
   bool HasSubMenu();
   void OpenSubMenu();
   void CloseSubMenu();
   
   virtual void SetSubMenu(MenuBase* smenu);
};




/**! @class ClassicMenuBarItem
 *   @brief Interface for a classic menu bar item in Eagle
 */

class ClassicMenuBarItem : public MenuItemBase , public GuiButton {
protected :
   SIMPLE_MENU_BAR_ITEM item;

   virtual void QueueUserMessage(const WidgetMsg& msg);
   
public :
   
   ClassicMenuBarItem();
   void SetItem(SIMPLE_MENU_BAR_ITEM mbitem);

   virtual void Toggle();
   virtual void Activate();
   virtual bool HasSubMenu();
   virtual void OpenSubMenu();
   virtual void CloseSubMenu();
   virtual void SetSubMenu(MenuBase* smenu);
};





#endif // MENU_ITEM_HPP
