
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
 *    Copyright 2009-2018+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */
 
 
 #include "Eagle/Gui/WidgetBase2.hpp"
 #include "Eagle/Gui/WidgetHandler2.hpp"
 #include "Eagle/Timer.hpp"


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





/// ---------------------     WIDGETBASE      --------------------------



void WIDGETBASE::RaiseWidgetEvent(WidgetMsg wmsg) {
   QueueUserMessage(wmsg);
   EagleEvent ee = MakeEagleEvent(wmsg);
   EmitEvent(ee , 0);
}



void WIDGETBASE::QueueUserMessage(WidgetMsg wmsg) {
   if (parent) {
      parent->QueueUserMessage(wmsg);
   }
}



int WIDGETBASE::HandleEvent(EagleEvent ee) {
   if (ee.type == EAGLE_EVENT_TIMER) {
      Update(ee.timer.eagle_timer_source->SPT());
   }
   return (PrivateCheckInputs() | PrivateHandleEvent(ee));
}



void WIDGETBASE::Update(double dt) {
   PrivateUpdate(dt);
}



void WIDGETBASE::Display(EagleGraphicsContext* win , int xpos , int ypos) {
   PrivateDisplay(win , xpos , ypos);
   ClearRedrawFlag();
}




WIDGETAREA& WIDGETBASE::EditArea() {
   if (handler) {
      handler->MakeAreaDirty(warea.OuterArea());
   }
   SetRedrawFlag();
   return warea;
}





