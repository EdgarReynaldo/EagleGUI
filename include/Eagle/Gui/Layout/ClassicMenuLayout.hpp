
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
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file MenuLayout.hpp
 * @brief The interface for the MenuLayout class
 */

#ifndef MenuLayout_HPP
#define MenuLayout_HPP


#include "Eagle/Gui/Layout/GridLayout.hpp"



extern const unsigned int TOPIC_MENU;



enum MENU_MESSAGES {
   
   MENU_OPENED = 0,
   MENU_CLOSED = 1,
   MENU_ITEM_ACTIVATED = 2,
   MENU_ITEM_TOGGLED = 3
   
};



REGISTER_WIDGET_MESSAGE(TOPIC_MENU , MENU_OPENED);
REGISTER_WIDGET_MESSAGE(TOPIC_MENU , MENU_CLOSED);
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









/**! @enum MENU_LAYOUT_DIRECTION
 *   @brief Determines the direction of the classic menu layout
 */
enum MENU_LAYOUT_DIRECTION {
   MENU_HORIZONTAL = 0,///< Layout this menu horizontally
   MENU_VERTICAL = 1   ///< Layout this menu vertically
};


/**! @class ClassicMenuLayout
 *   @brief A classic axis aligned menu layout
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
   
   virtual void SetSubMenu(int index , MenuBase* smenu)=0;
};


/**! @class ClassicMenuBarLayout
 *   @brief A classic menu bar, laid out horizontally
 *
 *   TODO : COMPLETE ME
 *   Needs : Key shortcuts, hover, focus, etc...
 */

class ClassicMenuBarLayout : public ClassicMenuLayout {
   
   
   virtual int PrivateHandleEvent(EagleEvent ee);
   
public :
   ClassicMenuBarLayout(std::string objname = "Nemo");

   virtual void Resize(unsigned int nsize);///< Resize the menu

   virtual void OpenMe();
   virtual void CloseMe();

   virtual void SetSubMenu(int index , MenuBase* smenu)=0;
};


//#include "SimpleMenu.hpp"

#include "Eagle/Gui/Button/BasicButton.hpp"
#include "Eagle/Font.hpp"
#include "Eagle/Image.hpp"
#include "Eagle/InputHandler.hpp"


#include "Eagle/Gui/Menu/SimpleMenu.hpp"



class BasicCheckBox;
class BasicText;
class BasicScrollButton;
class BasicButton;
class SimpleTable;


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

public :
   
   ClassicMenuItemLayout(SIMPLE_MENU_ITEM* item = 0);

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



class ClassicMenuBarItem : public MenuItemBase , public GuiButton {
protected :
   SIMPLE_MENU_BAR_ITEM item;

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




/**! @class ClassicMenuBar @brief simple implentation of a classic menu bar
 */

class ClassicMenuBar : public ClassicMenuBarLayout , public MenuBase , public EagleEventListener {
protected :
   std::vector<SIMPLE_MENU_BAR_ITEM> bitems;
   std::vector<ClassicMenuBarItem> mbitems;
   ClassicMenuBarItem* citem;
   bool open;
public :
   
   ClassicMenuBar();
   ClassicMenuBar() :
         ClassicMenuBarLayout("Menubar"),
         MenuBase(),
         EagleEventListener(),
         bitems(),
         mbitems(),
         citem(0),
         open(false)
   {}
   
   virtual ~ClassicMenuBar() {Clear();}
   
   void Clear();
   void SetBarItems(SIMPLE_MENU_BAR_ITEM* mbi , int nitems);
   void SetSubMenu(int bar_item_index , ClassicMenu* menu);
   
   virtual void OpenMe();
   virtual void CloseMe();
   virtual bool IsOpen();

   virtual void SetSubMenu(int index , MenuBase* smenu);
};

/**! @class ClassicMenu @brief simple implentation of a classic menu
 */
class ClassicMenu : protected ClassicMenuLayout , public EagleEventListener {
protected :
   std::vector<SIMPLE_MENU_ITEM> items;
   std::vector<ClassicMenuItemLayout*> mitems;
   bool open;
   ClassicMenuItemLayout citem;/// current menu item
   
   virtual void RespondToEvent(EagleEvent e , EagleThread* thread = MAIN_THREAD);
   
   
   void CloseOtherMenus(ClassicMenuItemLayout* mitem);
   
public :
   
   ClassicMenu();
   
   void Clear();
   void SetItems(SIMPLE_MENU_ITEM* menu , int size);
   
   /// MenuBase
   
   virtual void OpenMe();
   virtual void CloseMe();
   virtual bool IsOpen();

   virtual void SetSubMenu(int index , MenuBase* smenu)p


};












#endif // MenuLayout_HPP









