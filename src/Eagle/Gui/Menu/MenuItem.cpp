
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
 * @file MenuItem.cpp
 * @brief Implementation of a classic menu item in Eagle
 */



#include "Eagle/Gui/Menu/MenuItem.hpp"
#include "Eagle/Gui/Button/CheckBox.hpp"
#include "Eagle/Gui/Text/BasicText.hpp"
#include "Eagle/Gui/Button/BasicButton.hpp"
#include "Eagle/Gui/Button/ScrollButton.hpp"
#include "Eagle/Gui/Layout/SimpleTable.hpp"

#include <cstring>

/// ------------------------     ClassicMenuItemLayout     --------------------------------



void ClassicMenuItemLayout::RespondToEvent(EagleEvent e , EagleThread* thread) {
   (void)thread;
   e.source = this;
   EmitEvent(e , thread);/// Pass our message on to our handler


/**
   if (e.type == EAGLE_EVENT_WIDGET) {
      if (e.widget.from == cbox.get() || e.widget.from == menu_button.get()) {
         if (e.widget.topic == BUTTON_TOGGLED) {
            Toggle();
         }
         else if (e.widget.topic == BUTTON_CLICKED) {
            Activate();
         }
      }
      if (e.widget.from == hbtn.get()) {
         /// open sub menu we've reached our hover trigger
         if (e.widget.topic == TOPIC_BUTTON_WIDGET && e.widget.msgs == BUTTON_HOVER_TRIGGER) {
            OpenSubMenu();
         }
      }
   }
*/
}



ClassicMenuItemLayout::ClassicMenuItemLayout(SIMPLE_MENU_ITEM* mitem) :
   LayoutBase("Menu item layout" , "Nemo"),
   EagleEventListener(),
   item(),
   cbox(),
   dtext(),
   ktext(),
   hbtn(),
   submenu(0),
   menu_button(),
   item_layout()
{
   SetItemInfo(mitem);
}



ClassicMenuItemLayout::~ClassicMenuItemLayout () {
   Clear();
}



void ClassicMenuItemLayout::Clear() {
   memset(&item , 0 , sizeof(SIMPLE_MENU_ITEM));
   StopListening();
   ClearWidgets();
   item_layout.reset();
   cbox.reset();
   dtext.reset();
   ktext.reset();
   hbtn.reset();
   menu_button.reset();
   submenu = 0;
}



void ClassicMenuItemLayout::SetItemInfo(SIMPLE_MENU_ITEM* mitem) {
   Clear();
   if (!mitem) {
      return;
   }
   item = *mitem;
   
   Resize(2);
   
   menu_button.reset(new BasicButton("Menu item button" , "menu_button"));
   menu_button->SetButtonState(!mitem->down , false);
   menu_button->SetZOrder(ZORDER_PRIORITY_LOW);
   menu_button->SetInputGroup(mitem->hotkey);
   menu_button->SetButtonType(mitem->batype);
   item_layout.reset(new SimpleTable("Menu item layout"));

   PlaceWidget(menu_button.get(), 0);

   menu_button->SetZOrder(ZORDER_PRIORITY_LOW);
   PlaceWidget(item_layout.get() , 1);
   
   item_layout->Resize(std::vector<float>({0.1 , 0.4 , 0.4 , 0.1}) , std::vector<float>({1.0}));

   cbox.reset(new BasicCheckBox("cbox"));
   dtext.reset(new BasicText("dtext"));
   ktext.reset(new BasicText("ktext"));
   hbtn.reset(new BasicScrollButton("Menu hover button" , "hbtn"));
   
   cbox->SetImages(mitem->image_up , mitem->image_dn , mitem->image_up , mitem->image_dn);
   cbox->SetChecked(mitem->down);
   
   dtext->SetupText(mitem->description , mitem->text_font);
   ktext->SetupText(mitem->key_text    , mitem->text_font);
   
   dtext->Realign(HALIGN_LEFT , VALIGN_CENTER , 2 , 0);
   ktext->Realign(HALIGN_RIGHT , VALIGN_CENTER , 2 , 0);
   
   hbtn->SetScrollDirection(false , true);
   
   if (mitem->image_up && mitem->image_dn) {
      item_layout->PlaceWidget(cbox.get() , 0);
   }
   item_layout->PlaceWidget(dtext.get() , 1);
   item_layout->PlaceWidget(ktext.get() , 2);
   if (mitem->batype == MENU_BTN) {
      item_layout->PlaceWidget(hbtn.get() , 3);
   }

   ListenTo(cbox.get());
   ListenTo(dtext.get());
   ListenTo(ktext.get());
   ListenTo(hbtn.get());
   ListenTo(menu_button.get());
   
}



Rectangle ClassicMenuItemLayout::RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) {
   (void)widget_slot;
   (void)newx;
   (void)newy;
   (void)newwidth;
   (void)newheight;
   return InnerArea();
}



void ClassicMenuItemLayout::Toggle() {
   cbox->SetButtonState(!menu_button->Up() , false);
   RaiseEvent(WidgetMsg(this , TOPIC_MENU , MENU_ITEM_TOGGLED));
}



void ClassicMenuItemLayout::Activate() {
   if (item.batype == MENU_BTN) {
      OpenSubMenu();
   }
   else {
      RaiseEvent(WidgetMsg(this , TOPIC_MENU , MENU_ITEM_ACTIVATED));
   }
}



bool ClassicMenuItemLayout::HasSubMenu() {
   return (bool)submenu;
}



void ClassicMenuItemLayout::OpenSubMenu() {
   if (item.batype == MENU_BTN && HasSubMenu()) {
      submenu->OpenMe();
   }
   RaiseEvent(WidgetMsg(this , TOPIC_MENU , MENU_OPENED));
}



void ClassicMenuItemLayout::CloseSubMenu() {
   if (HasSubMenu()) {
      submenu->CloseMe();
   }
}



void ClassicMenuItemLayout::SetSubMenu(MenuBase* smenu) {
   submenu = smenu;
}



/// -----------------------     ClassicMenuBarItem     ----------------------------------



void ClassicMenuBarItem::QueueUserMessage(const WidgetMsg& msg) {
   const WidgetMsg gbtnmsg1(this , TOPIC_BUTTON_WIDGET , BUTTON_TOGGLED);
   const WidgetMsg gbtnmsg2(this , TOPIC_BUTTON_WIDGET , BUTTON_CLICKED);
   if (msg == gbtnmsg1) {
      Toggle();
   }
   else if (msg == gbtnmsg2) {
      Activate();
   }
   else {
      WidgetBase::QueueUserMessage(msg);
   }
}



ClassicMenuBarItem::ClassicMenuBarItem() :
      MenuItemBase(),
      GuiButton("ClassicMenuBarItem" , "gbtn"),
      item()
{
   
}



void ClassicMenuBarItem::SetItem(SIMPLE_MENU_BAR_ITEM mbitem) {
   item = mbitem;
   SetInputGroup(item.hotkey);
   SetFont(item.text_font);
   SetLabel(item.guitext);
   SetButtonType(RECTANGLE_BTN , TOGGLE_BTN , BUTTON_CLASS_HOVER);
}



void ClassicMenuBarItem::Toggle() {
   WidgetMsg msg(this , TOPIC_MENU , MENU_ITEM_TOGGLED);
   if (Up()) {
      CloseSubMenu();
   }
   else {
      OpenSubMenu();
   }
   RaiseEvent(msg);
}



void ClassicMenuBarItem::Activate() {
   WidgetMsg msg(this , TOPIC_MENU , MENU_ITEM_ACTIVATED);
   OpenSubMenu();
   RaiseEvent(msg);
}



bool ClassicMenuBarItem::HasSubMenu() {
   return (bool)this->submenu;
}



void ClassicMenuBarItem::OpenSubMenu() {
   if (HasSubMenu()) {
      submenu->OpenMe();
   }
}



void ClassicMenuBarItem::CloseSubMenu() {
   if (HasSubMenu()) {
      submenu->CloseMe();
   }
}



void ClassicMenuBarItem::SetSubMenu(MenuBase* smenu) {
   submenu = smenu;
   if (submenu) {
      submenu->CloseMe();
   }
}

