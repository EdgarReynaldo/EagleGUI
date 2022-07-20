



#include "Eagle/Gui/Layout/MenuLayout.hpp"



const unsigned int TOPIC_MENU = NextFreeTopicId();
const unsigned int TOPIC_MENU_MESSAGE = NextFreeTopicId();



REGISTERED_WIDGET_MESSAGE(TOPIC_MENU_MESSAGE , MENU_OPENED);
REGISTERED_WIDGET_MESSAGE(TOPIC_MENU_MESSAGE , MENU_CLOSED);
REGISTERED_WIDGET_MESSAGE(TOPIC_MENU_MESSAGE , MENU_ITEM_ACTIVATED);
REGISTERED_WIDGET_MESSAGE(TOPIC_MENU_MESSAGE , MENU_ITEM_TOGGLED);





void MenuItemBase::SyncSubMenuPosition() {
   if (submenu) {
      WIDGETAREA wa = submenu->GetWidgetArea();
      switch (submenu_openhour) {
      case 0 :
         /// N
         wa.MoveTo(OuterArea().TopCenter());
         wa.MoveBy(0 , -OuterArea().H());/// Bottom alignment to N side
         break;
      case 1 :
         /// NE
         wa.MoveTo(OuterArea().TopRight());
         wa.MoveBy(0 , -OuterArea().H());/// Bottom left alignment to NE corner
         break;
      case 2 :
         /// E
         wa.MoveTo(OuterArea().MiddleRight());
         wa.MoveBy(0 , -OuterArea().H()/2);/// Left vcenter alignment to E side
         break;
      case 3 :
         /// SE
         wa.MoveTo(OuterArea().BottomRight());
         /// Top left alignment to SE corner
         break;
      case 4 :
         /// S
         wa.MoveTo(OuterArea().BottomCenter());
         wa.MoveBy(-OuterArea().W()/2 , 0);/// Top align to S side
         break;
      case 5 :
         /// SW
         wa.MoveTo(OuterArea().BottomLeft());
         wa.MoveBy(-OuterArea().W() , 0);/// Top right align with SW corner
         break;
      case 6 :
         /// W
         wa.MoveTo(OuterArea().MiddleLeft());
         wa.MoveBy(-OuterArea().W() , -OuterArea().H()/2);
         break;
      case 7 :
         /// NW
         wa.MoveTo(OuterArea().TopLeft());
         wa.MoveBy(-OuterArea().W() , -OuterArea().H());
         break;
      default : break;
      }
      submenu->SetWidgetArea(wa);
   }
}



void MenuItemBase::OnActivation() {
   (void)0;
   WidgetMsg wmsg;
   wmsg.from = this;
   wmsg.topic = TOPIC_MENU_MESSAGE;
   wmsg.msgs = MENU_ITEM_ACTIVATED;
   EmitEvent(wmsg);
}



void MenuItemBase::OnToggled() {
   (void)0;
   WidgetMsg wmsg;
   wmsg.from = this;
   wmsg.topic = TOPIC_MENU_MESSAGE;
   wmsg.msgs = MENU_ITEM_TOGGLED;
   EmitEvent(wmsg);
}



void MenuItemBase::OnSubMenuClosed() {
   (void)0;
}



void MenuItemBase::OnSubMenuOpened() {
   (void)0;
   
}



void MenuItemBase::OnAreaChanged() {
   SyncSubMenuPosition();
}



int MenuItemBase::PrivateHandleEvent(EagleEvent ee) {
   
}



int MenuItemBase::PrivateUpdate(double dt) {
   if (Flags().FlagOn(HOVER)) {
      /// Count up with hover
      if (hover_duration + dt >= hover_trigger) {
         /// Threshold triggered, open submenu
         OpenSubMenu();
         hover_duration = 0.0;
      }
      else {
         hover_duration += dt;
      }
   }
   
}



void MenuItemBase::OpenSubMenu() {
   if (mparent) {
      mparent->CloseAllSubMenus();
   }
   if (submenu) {
      submenu->Open();
   }
}



void MenuItemBase::CloseSubMenu() {
   if (submenu) {submenu->Close();}
}



void MenuItemBase::SetOpenHour(unsigned int h) {
   submenu_openhour = h%8;
   SyncSubMenuPosition();
}
   
unsigned int MenuItemBase::OpenHour() {
   return submenu_openhour;
}



/// -------------------------------       MenuBase     -----------------------



void MenuBase::ReserveSlots(int nslots) {
   LayoutBase::ReserveSlots(nslots);
   items.resize(nslots);
}



void MenuBase::CloseAllSubMenus() {
   for (unsigned int i = 0 ; i < items.size() ; ++i) {
      items[i].CloseSubMenu();
   }
}



void MenuBase::Open() {
   SetWidgetFlags(Flags().AddFlag(VISIBLE));
   WidgetMsg wmsg;
   wmsg.from = this;
   wmsg.topic = TOPIC_MENU_MESSAGE;
   wmsg.msgs = MENU_OPENED;
   EmitEvent(wmsg);
}



void MenuBase::Close() {
   CloseAllSubMenus();
   SetWidgetFlags(Flags().RemoveFlag(VISIBLE));
   WidgetMsg wmsg;
   wmsg.from = this;
   wmsg.topic = TOPIC_MENU_MESSAGE;
   wmsg.msgs = MENU_CLOSED;
   EmitEvent(wmsg);
}



void MenuBase::PlaceWidget(WidgetBase* w , int slot) {
   CloseAllSubMenus();
   MenuItemBase* mitem = dynamic_cast<MenuItemBase*>(w);
   LayoutBase::PlaceWidget(w,slot);
   if (items[slot]) {
      StopListeningTo(items[slot]);
   }
   items[slot] = mitem;
   if (mitem) {
      ListenTo(mitem);
   }
}



int MenuBase::AddWidget(WidgetBase* w) {
   CloseAllSubMenus();
   items[LayoutBase::AddWidget(w)] = dynamic_cast<MenuItemBase*>(w);
   ListenTo(w);
}



void MenuBase::InsertWidget(WidgetBase* w , int insertion_slot) {
   CloseAllSubMenus();
   LayoutBase::InsertWidget(w , insertion_slot);
   ListenTo(w);
   items.resize(items.size() + 1);
   for (int i = items.size() - 1 ; i > insertion_slot ; --i) {
      items[i] = items[i-1];
   }
   items[insertion_slot] = dynamic_cast<MenuItemBase*>(w);
}



void MenuBase::RemoveWidget(WidgetBase* widget) {
   CloseAllSubMenus();
   StopListeningTo(widget);
   items[WidgetIndex(widget)] = 0;
   LayoutBase::RemoveWidget(widget);
}



void MenuBase::EraseWidgetSlot(int slot) {
   LayoutBase::EraseWidgetSlot(slot);
   if (items[slot]) {
      StopListeningTo(items[slot]);
   }
   for (int i = slot ; i < (int)items.size - 1 ; ++i) {
      items[i] = items[i+1];
   }
}




