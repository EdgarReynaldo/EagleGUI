



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
   
   virtual void RespondToEvent(EagleEvent e , EagleThread* thread = MAIN_THREAD);
void MenuItemBase::RespondToEvent(EagleEvent e , EagleThread* thread) {
   /// React to messages from our widgets, specifically buttons
   if (e.type == EAGLE_EVENT_WIDGET) {
      if (e.widget.topic == TOPIC_BUTTON_WIDGET) {
         if (e.widget.from == ourwidget) {
            /// We got a button message from our widget
            if (e.widget.msgs == BUTTON_TOGGLED) {
               EmitEvent(this , TOPIC_MENU , MENU_ITEM_TOGGLED);
            }
            else if (e.widget.msgs == BUTTON_CLICKED) {
               EmitEvent(this , TOPIC_MENU , MENU_ITEM_ACTIVATED);
            }
            else if (e.widget.msgs == BUTTON_HOVER_TRIGGER) {
               
            }
         }
      }
      else if (e.widget.topic == TOPIC_MENU) {
         if (e.widget.from == submenu) {
            if (e.widget.msgs == MENU_OPENED) {
               EmitEvent(e , 0);/// Pass menu open message up the chain to our parent so they can close other sub menus
            }
         }
      }
   }
}
   
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




   virtual void RespondToEvent(EagleEvent e , EagleThread* thread = MAIN_THREAD);
void RespondToEvent(EagleEvent e , EagleThread* thread) {
   (void)thread;
   
}


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










