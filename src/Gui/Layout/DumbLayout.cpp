
/*
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
 *    EAGLE
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2014+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */




#include "Eagle/Gui/Layout/DumbLayout.hpp"

#include "Eagle/StringWork.hpp"



void DumbLayout::RepositionAllChildren() {
	return;// DUH WERE DUMB REMEMBER
}



void DumbLayout::RepositionChild(int slot) {
   (void)slot;
   return;/// DUH WERE DUMB REMEMBER
}



DumbLayout::DumbLayout() : 
   Layout(StringPrintF("Dumb layout at %p" , this))
{
   
}



DumbLayout::DumbLayout(std::string name) : 
   Layout(name)
{
   
}



DumbLayout::~DumbLayout() {
   ClearLayoutAndFreeWidgets();
   /// In case we go out of scope before our WidgetHandler
   DetachFromGui();
}













