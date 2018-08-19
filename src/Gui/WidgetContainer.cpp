



#include "Eagle/Gui/WidgetContainer.hpp"
#include "Eagle/Gui/WidgetBase.hpp"




WIDGETCONTAINER::WIDGETCONTAINER() :
   cmap()
{}



WIDGETCONTAINER::~WIDGETCONTAINER() {
   cmap.clear();
}



EAGLE_ID WIDGETCONTAINER::Register(SHAREDWIDGET sw) {
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




