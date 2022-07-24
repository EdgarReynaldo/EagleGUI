



#ifndef MenuLayoutBase_HPP
#define MenuLayoutBase_HPP



extern const unsigned int TOPIC_MENU_MESSAGE;/// = NextFreeTopicId()


/**! @enum MENU_MESSAGES
 *   @brief Messages sent when a menu opens or closes and when an item activates or toggles
 */


enum MENU_MESSAGES {
   MENU_CLOSED = 0,
   MENU_OPENED = 1,
   MENU_ITEM_ACTIVATED = 2,///< For spring buttons
   MENU_ITEM_TOGGLED = 3   ///< For toggle buttons
   
};


REGISTER_WIDGET_MESSAGE(TOPIC_MENU_MESSAGE , MENU_OPENED);
REGISTER_WIDGET_MESSAGE(TOPIC_MENU_MESSAGE , MENU_CLOSED);
REGISTER_WIDGET_MESSAGE(TOPIC_MENU_MESSAGE , MENU_ITEM_ACTIVATED);
REGISTER_WIDGET_MESSAGE(TOPIC_MENU_MESSAGE , MENU_ITEM_TOGGLED);


class MenuBase;
class WidgetBase;

class MenuItemBase , public EagleEventListener , EagleEventSource {
   
protected :
   friend class MenuBase;
   friend class WidgetBase;
   
   WidgetBase* ourwidget;
   MenuBase*   parentmenu;
   MenuBase*   submenu;

   bool submenu_open;
   int  submenu_openhour;/// 0 to 7 indicating clock direction to open menu at
   
   
   
   void SyncSubMenuPosition();
   
   virtual void OnActivation();
   virtual void OnToggled();
   virtual void OnSubMenuClosed();
   virtual void OnSubMenuOpened();
   
   
public :
   
   MenuItemBase(WidgetBase* widget , LayoutBase* smenu);
   void CloseMenu();
   void Clear();
   void SetWidgetAndMenu(WidgetBase* widget , LayoutBase* smenu);


   MenuItemBase(WidgetBase* widget , LayoutBase* smenu) :
         EagleEventListener(),
         EagleEventSource(),
         ourwidget(0),
         parentmenu(0),
         submenu(0),
         submenu_open(false),
         submenu_openhour(3/* SE */),
   {
      SetWidgetAndMenu(widget , smenu);
   }
   
   
   void OpenSubMenu();
   void CloseSubMenu();
   
   void SetOpenHour(unsigned int h);

   unsigned int OpenHour();///< 0 to 7 clockwise from N
};



class LayoutBase;

class MenuBase : public EagleEventListener {
   
protected :
   
   friend class LayoutBase;
   
   LayoutBase* ourlayout;
   MenuItemBase* open_item;

public :
   MenuBase(LayoutBase* layout);
   MenuBase(LayoutBase* layout) {
         EagleEventListener(),
         ourlayout(0),
         open_item(0)
   {
      SetLayout(layout);
   }
   
   virtual ~MenuBase() {}   
      
   void CloseOtherMenus(MenuItemBase* mitem);
   void CloseAllSubMenus();
   void OpenMenu();
   void CloseMenu();




   bool IsOpen();
   bool IsClosed();
};























class MenuTreeLayoutBase : public LayoutBase {
protected :
   
   void LayoutMenuTree()=0;
   
public :
   MenuBase* rootmenu;
   std::vector<MenuItemBase*> items;
   
};

class MenuTreeLayout : public MenuTreeLayoutBase {
   
protected :
   
   void LayoutMenuTree() override ;
void MenuTreeLayout::LayoutMenuTree() {
   
}
public :
   
   
   
};



#endif // MenuLayoutBase_HPP










