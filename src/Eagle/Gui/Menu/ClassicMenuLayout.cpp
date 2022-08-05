
/**
 *
 *     _______       ___       ____      __       _______
 *    /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *    \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *     \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *      \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *       \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *        \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */


#include "Eagle/Gui/Layout/ClassicMenuLayout.hpp"
#include "Eagle/Gui/Button/ScrollButton.hpp"
#include "Eagle/StringWork.hpp"




const unsigned int TOPIC_MENU = NextFreeTopicId();



REGISTERED_WIDGET_MESSAGE(TOPIC_MENU_MESSAGE , MENU_OPENED);
REGISTERED_WIDGET_MESSAGE(TOPIC_MENU_MESSAGE , MENU_CLOSED);
REGISTERED_WIDGET_MESSAGE(TOPIC_MENU_MESSAGE , MENU_ITEM_ACTIVATED);
REGISTERED_WIDGET_MESSAGE(TOPIC_MENU_MESSAGE , MENU_ITEM_TOGGLED);



ClassicMenuLayout::ClassicMenuLayout(std::string objname) :
      GridLayout(0 , 0 , "ClassicMenuLayout" , objname),
      layout_direction(MENU_VERTICAL),
      menu_size(0)
{}



void ClassicMenuLayout::Resize(unsigned int nsize) {
   ResizeMenu(nsize , layout_direction);
   return;
}



void ClassicMenuLayout::ResizeMenu(int new_menu_size , MENU_LAYOUT_DIRECTION new_direction) {
   layout_direction = new_direction;
   std::vector<WidgetBase*> cvec = ChildrenVector();
   cvec.resize(new_menu_size);
   
   switch(layout_direction) {
   case MENU_HORIZONTAL :
      ResizeGrid(new_menu_size , 1);
      break;
   case MENU_VERTICAL :
      ResizeGrid(1,new_menu_size);
      break;
   default :
      throw EagleException("Unknown MENU_LAYOUT_DIRECTION");
   }
   
   SetWChildren(cvec);
   
   menu_size = GridLayout::GetLayoutSize();
}



void ClassicMenuLayout::Toggle() {
   EagleEvent e;
   e.type = EAGLE_EVENT_WIDGET;
   e.widget.from = this;
   e.widget.topic = TOPIC_MENU;
   e.widget.msgs = MENU_ITEM_TOGGLED;
}



void ClassicMenuLayout::Activate() {
   EagleEvent e;
   e.type = EAGLE_EVENT_WIDGET;
   e.widget.from = this;
   e.widget.topic = TOPIC_MENU;
   e.widget.msgs = MENU_ITEM_TOGGLED;
}



void ClassicMenuLayout::OpenSubMenu() {
   
}



void ClassicMenuLayout::OpenMe() {
   ShowAndEnable();
}



void ClassicMenuLayout::CloseMe() {
   HideAndDisable();
}



bool ClassicMenuLayout::IsOpen() {
   return (Flags().FlagOn(ENABLED) && Flags.FlagOn(VISIBLE));
}



/// -------------------------     ClassicMenuBarLayout     ------------------------



int ClassicMenuBarLayout::PrivateHandleEvent(EagleEvent ee) {
   (void)ee;
   return DIALOG_OKAY;
}



ClassicMenuBarLayout::ClassicMenuBarLayout(std::string objname) : 
      GridLayout(0,0,"ClassicMenuBarLayout" , objname)
{}



void ClassicMenuBarLayout::Resize(unsigned int nsize) {
   ResizeGrid(nsize , 1);
   return;
}




/// ------------------------     ClassicMenuItemLayout     --------------------------------



void ClassicMenuItemLayout::RespondToEvent(EagleEvent e , EagleThread* thread) {
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
}



ClassicMenuItemLayout::ClassicMenuItemLayout(SIMPLE_MENU_ITEM* item) :
   BasicButton("Menu item button" , "this"),
   EagleEventListener(),
   item(0),
   cbox(),
   icon(),
   btn(),
   ktext(),
   hbtn(),
   submenu(0),
   menu_button(),
   item_layout()
{
   SetItemInfo(item);
}



void ClassicMenuItemLayout::Clear() {
   item = 0;
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
   item = mitem;
   if (!item) {
      return;
   }
   
   Resize(2);
   
   menu_button.set(new BasicButton("Menu item button" , "menu_button"));
   menu_button->SetButtonState(!item->down , false);
   menu_button->SetZDepth(ZORDER_PRIORITY_LOW);
   item_layout.set(new SimpleTable("Menu item layout"));

   PlaceWidget(menu_button, 0);
   PlaceWidget(item_layout , 1);
   
   item_layout->Resize(std::vector<float>({0.1 , 0.4 , 0.4 , 0.1}) , std::vector<float>({1.0}));

   cbox.set(new BasicCheckBox("cbox"));
   dtext.set(new BasicText("dtext"));
   ktext.set(new BasicText("ktext"));
   hbtn.set(new BasicScrollButton("Menu hover button" , "hbtn"));
   
   cbox->SetImages(item->image_up , item->image_dn , item->image_up , item->image_dn);
   cbox->SetChecked(item->down);
   
   dtext.SetupText(item->text_font , item->description);
   ktext.SetupText(item->text_font , item->key_text);
   
   hbtn.SetScrollDirection(false , true);
   
   item_layout->PlaceWidget(cbox , 0);
   item_layout->PlaceWidget(dtext , 1);
   item_layout->PlaceWidget(ktext , 2);
   if (item->sub_menu) {
      item_layout->PlaceWidget(hbtn , 3);
   }

   ListenTo(cbox.get());
   ListenTo(gbtn.get());
   ListenTo(ktext.get());
   ListenTo(hbtn.get());
   ListenTo(menu_button.get());
   
}



Rectangle ClassicMenuItemLayout::RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) {
   (void)widget_slot;
   (void)newx;
   (void)newy;
   (void)neww;
   (void)newh;
   return InnerArea();
}











