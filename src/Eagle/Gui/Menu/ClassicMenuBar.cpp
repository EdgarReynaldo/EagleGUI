
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
 * @file ClassicMenuBar.cpp
 * @brief Implementation for the classic menu bar in Eagle
 */

 
#include "Eagle/Gui/Menu/ClassicMenuBar.hpp"





/// ------------------------------     ClassicMenuBar     -------------------------------



void ClassicMenuBar::RespondToEvent(EagleEvent e , EagleThread* thread) {
   (void)thread;

   for (unsigned int i = 0 ; i < mbitems.size() ; ++i) {
      ClassicMenuBarItem* mbitem = mbitems[i];
      if (e.source == mbitem) {
         if (e.type == EAGLE_EVENT_WIDGET) {
            if (e.widget.topic == TOPIC_MENU) {
               if (e.widget.msgs == MENU_ITEM_TOGGLED) {
                  if (mbitem->Up()) {
//                     mbitem->CloseSubMenu();
                  }
                  else {
                     mbitem->OpenSubMenu();
                     CloseOtherMenus(mbitem);
                  }
               }
               else if (e.widget.msgs == MENU_ITEM_ACTIVATED) {
//                  mbitem->OpenSubMenu();
                  CloseOtherMenus(mbitem);
               }
            }
         }
      }
   }

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



void ClassicMenuBar::CloseOtherMenus(ClassicMenuBarItem* exclude) {
   for (unsigned int i = 0 ; i < mbitems.size() ; ++i) {
      ClassicMenuBarItem* mbitem = mbitems[i];
      if (mbitem != exclude) {
         mbitem->CloseSubMenu();
      }
   }
}





