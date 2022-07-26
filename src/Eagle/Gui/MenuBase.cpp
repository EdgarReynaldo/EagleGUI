



#include "Eagle/Gui/Layout/MenuLayout.hpp"
#include "Eagle/Gui/MenuBase.hpp"
#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/Gui/Layout/Layout.hpp"


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



void MenuItemBase::CloseMenu() {
   if (submenu && submenu->IsOpen()) {
      submenu->Close();
   }
}



void MenuItemBase::Clear() {
   CloseMenu();
   if (ourwidget) {
      StopListeningTo(ourwidget);
      ourwidget = 0;
   }
   parentmenu = 0;
   submenu = 0;
   submenu_open = false;
}



void MenuItemBase::SetWidgetAndMenu(WidgetBase* widget , LayoutBase* smenu) {
   Clear();
   if (widget) {
      ourwidget = widget;
      ListenTo(ourwidget);
      parentmenu = widget->GetLayout();
      submenu = dynamic_cast<MenuBase*>(smenu);
   }
}



void MenuItemBase::OpenSubMenu() {
   if (submenu) {
      submenu->Open();
   }
}



void MenuItemBase::CloseSubMenu() {
   if (submenu) {
      submenu->Close();
   }
}



void MenuItemBase::SetOpenHour(unsigned int h) {
   submenu_openhour = h%8;
   SyncSubMenuPosition();
}
   
unsigned int MenuItemBase::OpenHour() {
   return submenu_openhour;
}



/// -------------------------------       MenuBase     -----------------------




void MenuBase::CloseOtherMenus(MenuItemBase* mitem) {
   EAGLE_ASSERT(ourlayout);
   EAGLE_ASSERT(mitem);
   std::vector<WidgetBase*> wchildren = ourlayout->WidgetChildren();
   for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
      MenuItemBase* item = dynamic_cast<MenuItemBase*>(wchildren[i]);
      if (item && mitem && item != mitem) {
         item->CloseSubMenu();
      }
   }
}



void MenuBase::CloseAllSubMenus() {
   EAGLE_ASSERT(ourlayout);
   std::vector<WidgetBase*> wchildren = ourlayout->WidgetChildren();
   for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
      MenuItemBase* item = dynamic_cast<MenuItemBase*>(wchildren[i]);
      if (item) {
         item->CloseSubMenu();
      }
   }
}



void MenuBase::OpenMenu() {
   EAGLE_ASSERT(ourlayout);
   ourlayout->SetWidgetFlags(Flags().AddFlag(VISIBLE));
   EagleEvent e;
   e.type = EAGLE_EVENT_WIDGET;
   e.source = this;
   e.widget.from = this;
   e.widget.msgs = MENU_OPENED;
   e.widget.topic = TOPIC_MENU_MESSAGE;
   EmitEvent(e,0);
}



void MenuBase::CloseMenu() {
   CloseAllSubMenus();
   EAGLE_ASSERT(ourlayout);
   ourlayout->SetWidgetFlags(Flags().RemoveFlag(VISIBLE));
   EagleEvent e;
   e.type = EAGLE_EVENT_WIDGET;
   e.source = this;
   e.widget.from = this;
   e.widget.msgs = MENU_CLOSED;
   e.widget.topic = TOPIC_MENU;
   EmitEvent(e,0);
}



bool MenuBase::IsOpen() {
   return (bool)open_item;
}



bool MenuBase::IsClosed() {
   return !IsOpen();
}








