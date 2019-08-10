
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file PopupWindow.cpp
 * @brief Implementations for the various popup types
 */

#include "Eagle/Gui/PopupWindow.hpp"
#include "Eagle/Gui/TextWidgets.hpp"
 
 
void PopupWindow::Hide() {
   our_window->HideWindow();
}



void PopupWindow::Show() {
   our_window->ShowWindow();
   our_system->GetWindowManager()->GiveWindowFocus(our_window->GetEagleId());
}



SHAREDOBJECT<PopupWindow> CreatePopupTextWindow(int x , int y , int flags , std::string message , EagleFont* font) {
   EAGLE_ASSERT(font);
   BasicText text;
   text.SetupText(HALIGN_CENTER , VALIGN_CENTER , 5 , 5 , font->Height()/2 , message , font);
   text.ShrinkWrap();
   
   SHAREDOBJECT<PopupWindow> popup = HeapObject(new PopupWindow(x , y , text.OuterArea().W() , text.OuterArea().H() , flags));
   popup->AddWidget(&text);
   return popup;
}
