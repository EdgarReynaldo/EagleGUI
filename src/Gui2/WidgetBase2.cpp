
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



int WIDGETBASE::PrivateHandleEvent(EagleEvent ee) {
   (void)ee;
}



int WIDGETBASE::PrivateCheckInputs() {
   return 0;
}



void WIDGETBASE::PrivateUpdate(double dt) {
   (void)dt;
}



void WIDGETBASE::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   (void)win;
   (void)xpos;
   (void)ypos;
}



void WIDGETBASE::QueueUserMessage(WidgetMsg wmsg) {
   if (wparent) {
      wparent->QueueUserMessage(wmsg);
   }
}



void WIDGETBASE::OnAreaChanged() {
   (void)0;
}



void WIDGETBASE::OnAttributeChanged(const ATTRIBUTE& a , const VALUE& v) {
   (void)a;
   (void)v;
}



void WIDGETBASE::OnFlagChanged(WIDGET_FLAG f , bool on) {
   (void)f;
   (void)on;
}



void WIDGETBASE::OnSelfAreaChange(WIDGETAREA new_widget_area) {
   if (whandler) {
      whandler->MakeAreaDirty(warea);
   }
   warea = new_widget_area;
   SetBgRedrawFlag();
   OnAreaChanged();
}



void WIDGETBASE::OnSelfAttributeChange(ATTRIBUTE a , VALUE v) {
   wattributes[a] = v;
   OnAttributeChange(a,v);
}



void WIDGETBASE::OnSelfFlagChange(WidgetFlags new_widget_flags) {
   unsigned int diff = FlagDiff(wflags , new_widget_flags);
   if (diff & (VISIBLE | HOVER | HASFOCUS)) {
      new_widget_flags.AddFlag(NEEDS_REDRAW);
      
   }
   wflags.SetNewFlags(new_widget_flags);
   unsigned int cflags = wflags.ChangedFlags()
      for (unsigned int i = 1 ; i < NUM_WIDGET_FLAGS ; ++i) {
         /// If a flag changes, call its setter
         unsigned int flag = 1 << i;
         bool change = cflags & flag;
         if (change) {
            OnFlagChanged(flag , wflags.FlagOn(flag));
         }
      }
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
   wpainter.PaintBackground(win , this);
   PrivateDisplay(win , xpos , ypos);
   if (wflags.FlagSet(HASFOCUS)) {
      wpainter.PaintFocus(win , this);
   }
   ClearRedrawFlag();
}



WidgetFlags Flags() {
   return wflags;
}



VALUE GetAttributeValue(ATTRIBUTE a) {
   if (wattributes.find(a) != wattributes.end()) {
      return wattributes[a];
   }
   return "";
}



WIDGETAREA GetWidgetArea() {
   return warea;
}



void SetWidgetFlags(WidgetFlags flags) {
   OnSelfFlagChange(flags);
}


   
bool SetAttribute(ATTRIBUTE a , VALUE v) {
   OnSelfAttributeChange(a,v);
}


   
void SetWidgetArea(WIDGETAREA warea) {
   OnSelfAreaChange(warea);
}



void WIDGETBASE::SetRedrawFlag() {
   wflags.AddFlag(NEEDS_REDRAW);
}



void WIDGETBASE::ClearRedrawFlag() {
   wflags.RemoveFlag(NEEDS_REDRAW);
}


