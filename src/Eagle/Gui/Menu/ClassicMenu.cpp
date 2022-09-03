
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
 * @file ClassicMenu.cpp
 * @brief Implementation for the classic menu in Eagle
 */

#include "Eagle/Gui/Menu/ClassicMenu.hpp"
#include "Eagle/Gui/Layout/MenuLayout.hpp"
#include "Eagle/Events.hpp"

#include <cstring>


#include "Eagle/Gui/Button/BasicButton.hpp"
#include "Eagle/Gui/Button/CheckBox.hpp"
#include "Eagle/Gui/Button/ScrollButton.hpp"



/// -----------------------     ClassicMenu     ----------------------------------



void ClassicMenu::RespondToEvent(EagleEvent e , EagleThread* thread) {
   (void)thread;
   
   if (e.type == EAGLE_EVENT_WIDGET) {
      if (e.widget.topic == TOPIC_BUTTON_WIDGET) {
         ClassicMenuItemLayout* mitem = dynamic_cast<ClassicMenuItemLayout*>(e.source);
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
            if (e.widget.from == dynamic_cast<WidgetBase*>(mitem->cbox.get())) {
               /// Our checkbox was toggled
               mitem->menu_button->SetButtonState(false , !mitem->cbox->Up() , false);
            }
            else if (e.widget.from == dynamic_cast<WidgetBase*>(mitem->menu_button.get())) {
               /// Menu button was activated or toggled
               if (e.widget.msgs == BUTTON_CLICKED) {
                  mitem->OpenSubMenu();
                  CloseOtherMenus(mitem);
               }
               else if (e.widget.msgs == BUTTON_TOGGLED) {
                  mitem->cbox->SetButtonState(false , !mitem->menu_button->Up() , false);
                  if (mitem->menu_button->Up()) {
                     mitem->CloseSubMenu();
                  }
                  else {
                     mitem->OpenSubMenu();
                     CloseOtherMenus(mitem);
                  }
               }
            }
            else if (e.widget.from == dynamic_cast<WidgetBase*>(mitem->hbtn.get())) {
               /// Our hover button was activated
               if (e.widget.msgs == BUTTON_HOVER_TRIGGER) {
                  mitem->OpenSubMenu();
                  CloseOtherMenus(mitem);
               }
            }
         }
      }
      RaiseEvent(WidgetMsg(e.widget.from , e.widget.topic , e.widget.msgs));
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
   ClassicMenuLayout::OpenMe();
   open = true;
   if (citem) {
      citem->OpenSubMenu();
   }
   SetRedrawFlag();
}



void ClassicMenu::CloseMe() {
   ClassicMenuLayout::CloseMe();
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



void ClassicMenu::CloseOtherMenus(ClassicMenuItemLayout* exclude) {
   for (unsigned int i = 0 ; i < mitems.size() ; ++i) {
      ClassicMenuItemLayout* item = mitems[i];
      if (item != exclude) {
         item->CloseSubMenu();
      }
   }
}








