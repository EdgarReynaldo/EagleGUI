
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
#include "Eagle/Gui/Button/CheckBox.hpp"
#include "Eagle/Gui/Layout/SimpleTable.hpp"
#include "Eagle/Gui/Text/BasicText.hpp"
#include "Eagle/StringWork.hpp"

#include "Eagle/Events.hpp"


#include <cstring>


const unsigned int TOPIC_MENU = NextFreeTopicId();



REGISTERED_WIDGET_MESSAGE(TOPIC_MENU , MENU_OPENED);
REGISTERED_WIDGET_MESSAGE(TOPIC_MENU , MENU_CLOSED);
REGISTERED_WIDGET_MESSAGE(TOPIC_MENU , MENU_ITEM_ACTIVATED);
REGISTERED_WIDGET_MESSAGE(TOPIC_MENU , MENU_ITEM_TOGGLED);



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



void ClassicMenuLayout::OpenMe() {
   ShowAndEnable();
}



void ClassicMenuLayout::CloseMe() {
   HideAndDisable();
}



bool ClassicMenuLayout::IsOpen() {
   return (Flags().FlagOn(ENABLED) && Flags().FlagOn(VISIBLE));
}



/// -------------------------     ClassicMenuBarLayout     ------------------------



int ClassicMenuBarLayout::PrivateHandleEvent(EagleEvent ee) {
   (void)ee;
   return DIALOG_OKAY;
}



ClassicMenuBarLayout::ClassicMenuBarLayout(std::string objname) : 
      ClassicMenuLayout(objname)
{}



void ClassicMenuBarLayout::Resize(unsigned int nsize) {
   ResizeGrid(nsize , 1);
   return;
}




/// ------------------------     ClassicMenuItemLayout     --------------------------------



void ClassicMenuItemLayout::RespondToEvent(EagleEvent e , EagleThread* thread) {
   (void)thread;
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
   item_layout.reset(new SimpleTable("Menu item layout"));

   PlaceWidget(menu_button.get(), 0);
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
   
   hbtn->SetScrollDirection(false , true);
   
   item_layout->PlaceWidget(cbox.get() , 0);
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
   submenu->CloseMe();
}



void ClassicMenuItemLayout::SetSubMenu(MenuBase* smenu) {
   submenu = smenu;
}



/// -----------------------     ClassicMenu     ----------------------------------



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
}



void ClassicMenuBarItem::Toggle() {
   WidgetMsg msg(this , TOPIC_MENU , MENU_ITEM_TOGGLED);
   RaiseEvent(msg);
}



void ClassicMenuBarItem::Activate() {
   WidgetMsg msg(this , TOPIC_MENU , MENU_ITEM_ACTIVATED);
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
}



/// ------------------------------     ClassicMenuBar     -------------------------------



void ClassicMenuBar::RespondToEvent(EagleEvent e , EagleThread* thread) {
   (void)thread;
/*
   for (int i = 0 ; i < mbitems.size() ; ++i) {
      ClassicMenuBarItem* mbitem = mbitems[i];
      if (e.source == mbitem) {
         if (e.type == EAGLE_EVENT_WIDGET) {
            if (e.widget.topic == TOPIC_MENU) {
               if (e.widget.msgs == MENU_ITEM_TOGGLED) {
                  if (mbitem->Up()) {
                     CloseAllMenus();
                  }
                  else {
                     mbitem->OpenSubMenu();
                     CloseOtherMenus();
                  }
               }
               else if (e.widget.msgs == MENU_ITEM_ACTIVATED) {
                  mbitem->OpenSubMenu();
                  CloseOtherMenus(mbitem);
               }
               else if (e.widget.msgs == MENU_OPENED) {
                  CloseOtherMenus(mbitem);
               }
               
            }
         }
      }
   }
*/
}



ClassicMenuBar::ClassicMenuBar() :
      ClassicMenuBarLayout("Menubar"),
      EagleEventListener(),
      bitems(),
      mbitems(),
      citem(0),
      open(false)
{}



void ClassicMenuBar::Clear() {
   open = false;
   StopListening();
   ClearWidgets();
   bitems.clear();
   for (unsigned int i = 0 ; i < mbitems.size() ; ++i) {
      delete mbitems[i];
   }
   mbitems.clear();
}



void ClassicMenuBar::SetBarItems(SIMPLE_MENU_BAR_ITEM* mbi , int nitems) {
   Clear();
   ResizeMenu(nitems , MENU_HORIZONTAL);
   bitems.resize(nitems);
   mbitems.resize(nitems,0);
   for (int i = 0 ; i < nitems ; ++i) {
         bitems[i] = mbi[i];
         mbitems[i] = new ClassicMenuBarItem();
         mbitems[i]->SetItem(bitems[i]);
         ListenTo(mbitems[i]);
         PlaceWidget(mbitems[i] , i);
   }
}



void ClassicMenuBar::OpenMe() {
   open = true;
   SetRedrawFlag();
}



void ClassicMenuBar::CloseMe() {
   open = false;
   SetRedrawFlag();
}



bool ClassicMenuBar::IsOpen() {
   return open;
}



void ClassicMenuBar::SetSubMenu(int index , MenuBase* smenu) {
   mbitems[index]->SetSubMenu(smenu);
}



/// -----------------------     ClassicMenu     ----------------------------------



void ClassicMenu::RespondToEvent(EagleEvent e , EagleThread* thread) {
   (void)thread;
   
   if (e.type == EAGLE_EVENT_WIDGET) {
      if (e.widget.topic == TOPIC_MENU) {
         ClassicMenuItemLayout* mitem = dynamic_cast<ClassicMenuItemLayout*>(e.widget.from);
         int index = -1;
         for (unsigned int i = 0 ; i < mitems.size() ; ++i) {
            ClassicMenuItemLayout* mi = mitems[i];
            if (mitem == mi) {
               index = i;
               break;
            }
         }
         if (index != -1) {
            /// This menu item is one of ours
            if (e.widget.msgs == MENU_ITEM_ACTIVATED) {
               if (mitem->HasSubMenu()) {
                  mitem->OpenSubMenu();
///                  CloseOtherMenus(mitem);
                  /// We don't need to close the other menus here - we will get a MENU_OPENED message and then they will close
               }
               else {
//                  RaiseEvent(WidgetMsg(this , TOPIC_MENU , MENU_ITEM_ACTIVATED));
               }
            }
            else if (e.widget.msgs == MENU_ITEM_TOGGLED) {
               e.source = this;
//               RaiseEvent(WidgetMsg(this , TOPIC_MENU , MENU_ITEM_TOGGLED));
            }
            else if (e.widget.msgs == MENU_OPENED) {
               CloseOtherMenus(mitem);
            }
         }
      }
   }
}



void ClassicMenu::CloseOtherMenus(ClassicMenuItemLayout* mitem) {
   for (unsigned int i = 0 ; i < mitems.size() ; ++i) {
      ClassicMenuItemLayout* m = mitems[i];
      if (m->HasSubMenu() && (m != mitem)) {
         m->CloseSubMenu();
      }
   }
}



ClassicMenu::ClassicMenu() :
      ClassicMenuLayout("Classic menu"),
      EagleEventListener(),
      items(),
      mitems(),
      open(false),
      citem(0)
{}



void ClassicMenu::Clear() {
   StopListening();
   ClearWidgets();
   items.clear();
   for (unsigned int i = 0 ; i < mitems.size() ; ++i) {
      delete mitems[i];
   }
   mitems.clear();
}



void ClassicMenu::SetItems(SIMPLE_MENU_ITEM* menu , int msize) {
   Clear();
   items.resize(msize);
   mitems.resize(msize , 0);
   Resize(msize);
   for (int i = 0 ; i < msize ; ++i) {
      items[i] = menu[i];
      mitems[i] = new ClassicMenuItemLayout(&items[i]);
      PlaceWidget(mitems[i] , i);
      ListenTo(mitems[i]);
   }
}



void ClassicMenu::OpenMe() {
   open = true;
   if (citem) {
      citem->OpenSubMenu();
   }
   SetRedrawFlag();
}



void ClassicMenu::CloseMe() {
   open = false;
   if (citem) {
      citem->CloseSubMenu();
   }
   SetRedrawFlag();
}



bool ClassicMenu::IsOpen() {
   return open;
}



void ClassicMenu::SetSubMenu(int index , MenuBase* smenu) {
   mitems[index]->SetSubMenu(smenu);
}










