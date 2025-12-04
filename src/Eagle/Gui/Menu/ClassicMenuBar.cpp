
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
#include "Eagle/Gui/WidgetHandler.hpp"



/// ------------------------------     ClassicMenuBar     -------------------------------



void ClassicMenuBar::RespondToEvent(EagleEvent e , EagleThread* thread) {
   (void)thread;

   for (unsigned int i = 0 ; i < mbitems.size() ; ++i) {
      ClassicMenuBarItem* mbitem = mbitems[i];
      if (e.source == mbitem) {
         citem = mbitem;
         if (e.type == EAGLE_EVENT_WIDGET) {
            if (e.widget.topic == TOPIC_MENU) {
               if (e.widget.msgs == MENU_ITEM_TOGGLED) {
                  if (mbitem->Up()) {
                     mbitem->CloseSubMenu();
                  }
                  else {
                     mbitem->OpenSubMenu();
                     CloseOtherMenus(mbitem);
                  }
               }
               else if (e.widget.msgs == MENU_ITEM_ACTIVATED) {
                  mbitem->OpenSubMenu();
                  CloseOtherMenus(mbitem);
               }
            }
         }
      }
   }

}



void ClassicMenuBar::OnFlagChanged(WIDGET_FLAGS f , bool on) {
   if ((f & HASFOCUS) && !on) {
      CloseMe();
   }
}



int ClassicMenuBar::PrivateHandleEvent(EagleEvent e) {
   if (toggle_key) {
      ToggleOpen();
   }
   if (e.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN) {
      if (IsOpen()) {
         bool subtreecontains = false;
         std::vector<Rectangle> tree;
         tree.push_back(OuterArea());
         std::vector<Rectangle> subtreearea = SubTreeArea();
         for (unsigned int i = 0 ; i < subtreearea.size() ; ++i) {
            Rectangle r = subtreearea[i];
            if (r.Contains(e.mouse.x , e.mouse.y)) {
               subtreecontains = true;
            }
         }
         if (OuterArea().Contains(e.mouse.x , e.mouse.y)) {
            subtreecontains = true;
         }
         if (!subtreecontains) {
            CloseMe();
         }
      }
   }
   return DIALOG_OKAY;
}



ClassicMenuBar::ClassicMenuBar() :
      ClassicMenuBarLayout("Menubar"),
      EagleEventListener(),
      bitems(),
      mbitems(),
      citem(0),
      open(false),
      radio(),
      toggle_key(Input(KB , PRESS , EAGLE_KEY_ALT))
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
   radio.Clear();
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
         mbitems[i]->SetParent(this);
         radio.AddRadioButton(mbitems[i]);
   }
   radio.SelectButton(0);
}



void ClassicMenuBar::OpenMe() {
   open = true;
   RootHandler()->GiveWidgetFocus(this , true);
   SetRedrawFlag();
}



void ClassicMenuBar::CloseMe() {
   open = false;
   if (citem) {
      citem->Deactivate();
      citem = 0;
      radio.SelectButton(0);
   }
   if (RootHandler()) {RootHandler()->GiveWidgetFocus(0 , true);}
   SetRedrawFlag();
}



bool ClassicMenuBar::IsOpen() {
   return open;
}



bool ClassicMenuBar::ToggleOpen() {
   if (!IsOpen()) {
      OpenMe();
   }
   else {
      CloseMe();
   }
   return IsOpen();
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



std::vector<Rectangle> ClassicMenuBar::SubTreeArea() {
   std::vector<Rectangle> tree;
   tree.push_back(OuterArea());
   if (citem) {
      std::vector<Rectangle> subtree = citem->SubTreeArea();
      for (unsigned int j = 0 ; j < subtree.size() ; ++j) {
         tree.push_back(subtree[j]);
      }
   }
   return tree;
}




