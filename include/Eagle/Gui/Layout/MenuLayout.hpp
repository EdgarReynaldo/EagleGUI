



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


class MenuItemBase : public WidgetBase {
   
protected :
   friend class MenuBase;
   
   MenuBase* mparent;
   MenuBase* submenu;
   bool submenu_open;
   int submenu_openhour;/// 0 to 7 indicating clock direction to open menu at
   
   double hover_duration;
   double hover_trigger;
   
   
   
   void SyncSubMenuPosition();
   
   virtual void OnActivation();
   virtual void OnToggled();
   virtual void OnSubMenuClosed();
   virtual void OnSubMenuOpened();
   
   /// WidgetBase
   virtual int PrivateHandleEvent(EagleEvent ee);///< Virtual function that controls how widgets react to events
   virtual int PrivateUpdate(double dt);///< Virtual function that controls what happens when time passes for a widget
   /// Private interface, override to define behavior

   /// Callbacks, override and/or call if you need to
   virtual void OnAreaChanged();///< Override to react to changes in this widget's area
   virtual void OnAttributeChanged(const ATTRIBUTE& a , const VALUE& v);///< Override to react to attribute changes
   virtual void OnFlagChanged(WIDGET_FLAGS f , bool on);///< Override to react to flag changes
   virtual void OnColorChanged();///< Override to react to color changes

   virtual void OnAreaChanged();
   
   
   
   
public :
   
   MenuItemBase();
   MenuItemBase() :
         WidgetBase("MenuItemBase" , "Menu item"),
         mparent(0),
         submenu(0),
         submenu_open(false),
         submenu_openhour(3/* SE */),
         hover_duration(-1.0),
         hover_trigger(0.25)
   {
      
   }
   
   void OpenSubMenu();
   void CloseSubMenu();
   
   void SetOpenHour(unsigned int h);

   unsigned int OpenHour();///< 0 to 7 clockwise from N
};



class MenuBase : public GridLayout , public EagleEventListener {
   
protected :
   MenuItemBase* open_item;
   std::vector<MenuItemBase*> items;



   virtual void ReserveSlots(int nslots);

   if (e.type == EAGLE_EVENT_MOUSE_AXES) {
      if (OuterArea().Contains(e.mouse.x , e.mouse.y)) {
            
      }
   }


public :
   MenuBase();
   MenuBase() :
         GridLayout("Menu Base" , "Menu"),
         EagleEventListener(),
         open_item(0),
         items()
   {}
   virtual ~MenuBase() {}

   /// LayoutBase
   virtual void PlaceWidget(WidgetBase* w , int slot) override;
   virtual int AddWidget(WidgetBase* w);
   virtual void InsertWidget(WidgetBase* w , int insertion_slot);

   virtual void RemoveWidget(WidgetBase* widget);   
   
   virtual void EraseWidgetSlot(int slot);
   
   /// EagleEventListener
   virtual void RespondToEvent(EagleEvent e , EagleThread* thread = MAIN_THREAD)=0;
void MenuBase::RespondToEvent(EagleEvent e , EagleThread* thread) {
   /// A menu has to respond to certain events from its menu item widgets
   (void)thread;
   
   if (e.type == EAGLE_EVENT_WIDGET) {
      MenuItemBase* mitem = dynamic_cast<MenuItemBase*>(e.widget.from);
      if (e.widget.msgs == MENU_ITEM_TOGGLED) {
            
      }
      else if (e.widget.msgs == MENU_ITEM_ACTIVATED) {
            
      }
   }
   
   
}
   
   
      
   void CloseAllSubMenus();
   
   virtual void Open();
   virtual void Close();
};





#endif // MenuLayoutBase_HPP






