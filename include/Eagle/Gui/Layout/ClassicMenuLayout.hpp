
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


extern const unsigned int TOPIC_MENU;

enum MENU_MESSAGES {
   
   MENU_OPENED = 0,
   MENU_CLOSED = 1,
   MENU_ITEM_ACTIVATED = 2,
   MENU_ITEM_TOGGLED = 3
   
};

REGISTER_WIDGET_MESSAGE(TOPIC_MENU_MESSAGE , MENU_OPENED);
REGISTER_WIDGET_MESSAGE(TOPIC_MENU_MESSAGE , MENU_CLOSED);
REGISTER_WIDGET_MESSAGE(TOPIC_MENU_MESSAGE , MENU_ITEM_ACTIVATED);
REGISTER_WIDGET_MESSAGE(TOPIC_MENU_MESSAGE , MENU_ITEM_TOGGLED);



#include "Eagle/Gui/Layout/GridLayout.hpp"




class MenuBase {
   
protected :
   
public :
   /// NO DATA
   virtual void OpenMe()=0;
   virtual void CloseMe()=0;
   virtual bool IsOpen()=0;

   bool IsClosed() {return !IsOpen();}
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
   
   void Toggle();
   void Activate();
   void OpenSubMenu();


   virtual void OpenMe();
   virtual void CloseMe();
   virtual bool IsOpen();
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

   virtual void OpenMe();
   virtual void CloseMe();

   virtual void Resize(unsigned int nsize);///< Resize the menu

};


#include "SimpleMenu.hpp"


class SIMPLE_MENU_ITEM {
public :
   BUTTON_ACTION_TYPE   batype;/// Spring / Toggle / Menu hover trigger
   bool              down;
   EagleImage*       image_up;
   EagleImage*       image_dn;
   EagleFont*        text_font;
   std::string       description;
   InputGroup        action_key;
   std::string       key_text;
   SIMPLE_MENU_ITEM* sub_menu;
};

class ClassicMenuItemLayout : public LayoutBase , public EagleEventListener {

protected :
   SIMPLE_MENU_ITEM* item;/// Our init data
   
   std::unique_ptr<CheckBox>          cbox;/// Checkbox for toggle buttons
   std::unique_ptr<BasicText>         dtext;/// Text button to display description of command
   std::unique_ptr<BasicText>         ktext;/// Label to tell what key to press
   std::unique_ptr<BasicScrollButton> hbtn;/// MENU_BTN for hover trigger to open sub menu
   
   LayoutBase* submenu;
   
   std::unique_ptr<BasicButton> menu_button;
   std::unique_ptr<SimpleTable> item_layout;

   
   
   /// EagleEventListener
   virtual void RespondToEvent(EagleEvent e , EagleThread* thread = MAIN_THREAD);

public :
   
   ClassicMenuItemLayout(SIMPLE_MENU_ITEM* item = 0);

   ~virtual ClassicMenuItemLayout () {Clear();}

   void Clear();

   void SetItemInfo(SIMPLE_MENU_ITEM* mitem);

   /// LayoutBase
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight);
   
};



/**! @class ClassicMenuBar @brief simple implentation of a classic menu bar
 */

class ClassicMenuBar : public ClassicMenuBarLayout {
protected :
   
public :
   
   
};

/**! @class ClassicMenu @brief simple implentation of a classic menu
 */
class ClassicMenu : public ClassicMenuLayout {
protected :
   
public :
   
   
};












#endif // MenuLayout_HPP









