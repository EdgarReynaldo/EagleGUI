
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


#include "Eagle/Gui/Layout/ClassicMenuLayout.hpp"
#include "Eagle/Gui/Button/ScrollButton.hpp"
#include "Eagle/Gui/Button/CheckBox.hpp"
#include "Eagle/Gui/Layout/SimpleTable.hpp"
#include "Eagle/Gui/Text/BasicText.hpp"
#include "Eagle/StringWork.hpp"

#include "Eagle/Events.hpp"


#include <cstring>


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



void ClassicMenuBar::CloseOtherMenus(ClassicMenuBarItem* exclude) {
   for (unsigned int i = 0 ; i < mbitems.size() ; ++i) {
      ClassicMenuBarItem* mbitem = mbitems[i];
      if (mbitem && mbitem != exclude) {
         mbitem->CloseSubMenu();
      }
   }
}









