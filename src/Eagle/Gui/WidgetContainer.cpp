
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




#include "Eagle/Gui/WidgetContainer.hpp"
#include "Eagle/Gui/WidgetBase.hpp"




WIDGETCONTAINER::WIDGETCONTAINER() :
   cmap()
{}



WIDGETCONTAINER::~WIDGETCONTAINER() {
   cmap.clear();
}



EAGLE_ID WIDGETCONTAINER::Register(SHAREDWIDGET sw) {
   /// TODO : FIXME : This will crash if sw is null or invalid
   if (!sw) {
      return EAGLE_ID_INVALID;
   }
   EAGLE_ID id = sw->GetEagleId();
   cmap[id] = sw;
   return id;
}



void WIDGETCONTAINER::Remove(EAGLE_ID id) {
   CMIT it = cmap.find(id);
   if (it != cmap.end()) {
      cmap.erase(it);
   }
}



void WIDGETCONTAINER::Clear() {
   cmap.clear();
}



SHAREDWIDGET WIDGETCONTAINER::FindById(EAGLE_ID id) {
   if (cmap.find(id) != cmap.end()) {
      return cmap[id];
   }
   return SHAREDWIDGET();
}




