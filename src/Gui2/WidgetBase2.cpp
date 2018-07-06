
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
 
 
#include "Eagle/Gui2/WidgetBase2.hpp"
#include "Eagle/Gui2/WidgetHandler2.hpp"
#include "Eagle/Timer.hpp"



/// ---------------------     WIDGETBASE      --------------------------



void WIDGETBASE::RaiseWidgetEvent(WidgetMsg wmsg) {
   QueueUserMessage(wmsg);
   EagleEvent ee = MakeEagleEvent(wmsg);
   EmitEvent(ee , 0);
}



int WIDGETBASE::PrivateHandleEvent(EagleEvent ee) {
   (void)ee;
   return DIALOG_OKAY;
}



int WIDGETBASE::PrivateCheckInputs() {
   return DIALOG_OKAY;
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



void WIDGETBASE::OnFlagChanged(WIDGET_FLAGS f , bool on) {
   (void)f;
   (void)on;
}



void WIDGETBASE::OnColorChanged() {
   SetRedrawFlag();
}



void WIDGETBASE::OnSelfAreaChanged(WIDGETAREA new_widget_area) {
   if (whandler) {
      whandler->MakeAreaDirty(warea.OuterArea());
   }
   warea = new_widget_area;
   SetBgRedrawFlag();
   OnAreaChanged();
}



void WIDGETBASE::OnSelfAttributeChanged(ATTRIBUTE a , VALUE v) {
   wattributes[a] = v;
   OnAttributeChanged(a,v);
}



void WIDGETBASE::OnSelfFlagChanged(WidgetFlags new_widget_flags) {
   unsigned int diff = FlagDiff(wflags , new_widget_flags);
   if (diff & (VISIBLE | HOVER | HASFOCUS)) {
      new_widget_flags.AddFlag(NEEDS_REDRAW);
   }
   wflags.SetNewFlags(new_widget_flags);
   unsigned int cflags = wflags.ChangedFlags();
   for (unsigned int i = 1 ; i < NUM_WIDGET_FLAGS ; ++i) {
      /// If a flag changes, call its setter
      unsigned int flag = 1 << i;
      bool change = cflags & flag;
      if (change) {
         OnFlagChanged((WIDGET_FLAGS)flag , wflags.FlagOn((WIDGET_FLAGS)flag));
      }
   }
}



void WIDGETBASE::OnSelfColorChanged(std::shared_ptr<WidgetColorset> cset) {
   wcolors = cset;
   OnColorChanged();
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
   WidgetPainterBase* wp = wpainter.Painter();
   if (wp) {
      wp->PaintWidgetBackground(win , this);
   }
   PrivateDisplay(win , xpos , ypos);
   if (wp && wflags.FlagOn(HASFOCUS)) {
      wp->PaintWidgetFocus(win , this);
   }
   ClearRedrawFlag();
}



void WIDGETBASE::SetAttribute(const ATTRIBUTE& a , const VALUE& v) {
   OnSelfAttributeChanged(a,v);
}


   
void WIDGETBASE::SetWidgetArea(WIDGETAREA area) {
   OnSelfAreaChanged(area);
}



void WIDGETBASE::SetWidgetFlags(WidgetFlags flags) {
   OnSelfFlagChanged(flags);
}


   
void WIDGETBASE::SetWidgetColorset(std::shared_ptr<WidgetColorset> cset) {
   OnSelfColorChanged(cset);
}



void WIDGETBASE::SetWidgetColorset(const WidgetColorset& cset) {
   SetWidgetColorset(std::shared_ptr<WidgetColorset>(new WidgetColorset(cset)));
}



void WIDGETBASE::SetWidgetPainter(const WidgetPainter& wp) {
   wpainter = wp;
   SetBgRedrawFlag();
}



void WIDGETBASE::UnsetWidgetPainter() {
   wpainter.Reset();
}



bool WIDGETBASE::HasAttribute(const ATTRIBUTE& a) const {
   return wattributes.HasAttribute(a);
}



bool WIDGETBASE::InheritsAttribute(const ATTRIBUTE& a) const {
   if (HasAttribute(a)) {return false;}
   WIDGETBASE* w = wparent;
   while (w) {
      if (w->HasAttribute(a)) {return true;}
      w = w->wparent;
   }
   if (wlayout && wlayout->HasAttribute(a)) {return true;}
   if (whandler && whandler->HasAttribute(a)) {return true;}
   if (ATTRIBUTEVALUEMAP::GlobalAttributeMap()->HasAttribute(a)) {return true;}
   return false;
}



bool WIDGETBASE::AttributeIsSet(const ATTRIBUTE& a) const {
   return HasAttribute(a) || InheritsAttribute(a);
}



void WIDGETBASE::RemoveAttribute(const ATTRIBUTE& a) {
   if (HasAttribute(a)) {
      wattributes.RemoveAttribute(a);
   }
}



VALUE WIDGETBASE::GetAttributeValue(const ATTRIBUTE& a) const {
   if (HasAttribute(a)) {
      return wattributes[a];
   }
   WIDGETBASE* parent = wparent;
   while (parent) {
      if (parent->HasAttribute(a)) {
         return parent->GetAttributeValue(a);
      }
      parent = parent->wparent;
   }
   if (wlayout && wlayout->HasAttribute(a)) {
      return wlayout->GetAttributeValue(a);
   }
   if (whandler && whandler->HasAttribute(a)) {
      return whandler->GetAttributeValue(a);
   }
   if (ATTRIBUTEVALUEMAP::GlobalAttributeMap()->HasAttribute(a)) {
      return ATTRIBUTEVALUEMAP::GlobalAttributeMap()->GetAttributeValue(a);
   }
   return "";
}



EagleColor WIDGETBASE::GetColor(WIDGETCOLOR wc) {
   std::string cname = WidgetColorName(wc);
   if (HasAttribute(cname)) {
      return EagleColor(GetAttributeValue(cname));
   }
   std::shared_ptr<WidgetColorset> cset = WidgetColors();
   if (cset) {
      return (*cset)[wc];
   }
   std::shared_ptr<WidgetColorset> cset2 = ColorRegistry::GlobalColorRegistry()->GetDefaultColorset();
   return (*cset2)[wc];
}



WIDGETAREA WIDGETBASE::GetWidgetArea() {
   return warea;
}



WidgetFlags WIDGETBASE::Flags() {
   return wflags;
}



std::shared_ptr<WidgetColorset> WIDGETBASE::WidgetColors() {
   if (HasAttribute("Colorset")) {
      return std::shared_ptr(new WidgetColorset(GetColorsetByName(GetAttributeValue("Colorset"))));
   }
   if (wcolors) {return wcolors;}
   if (wparent && wparent->WidgetColors()) {return wparent->WidgetColors();}
   if (wlayout && wlayout->WidgetColors()) {return wlayout->WidgetColors();}
   if (whandler && whandler->WidgetColors()) {return whandler->WidgetColors();}
   return 0;
}



void WIDGETBASE::SetRedrawFlag() {
   wflags.AddFlag(NEEDS_REDRAW);
}



void WIDGETBASE::SetBgRedrawFlag() {
   wflags.AddFlags(NEEDS_BG_REDRAW | NEEDS_REDRAW);
}



void WIDGETBASE::ClearRedrawFlag() {
   wflags.RemoveFlags(NEEDS_BG_REDRAW | NEEDS_REDRAW);
}






