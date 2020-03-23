
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
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */

 
#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/Gui/WidgetHandler.hpp"
#include "Eagle/Timer.hpp"



const unsigned int TOPIC_DIALOG = NextFreeTopicId();



REGISTERED_WIDGET_MESSAGE(TOPIC_DIALOG , DIALOG_I_MOVED);



/// ---------------------     WidgetBase      --------------------------



void WidgetBase::RaiseEvent(WidgetMsg wmsg) {
   QueueUserMessage(wmsg);
   EagleEvent ee = MakeEagleEvent(wmsg);
   EmitEvent(ee , 0);
}



int WidgetBase::PrivateHandleEvent(EagleEvent ee) {
   (void)ee;
   return DIALOG_OKAY;
}



int WidgetBase::PrivateCheckInputs() {
   return DIALOG_OKAY;
}



int WidgetBase::PrivateUpdate(double dt) {
   (void)dt;
   return DIALOG_OKAY;
}



void WidgetBase::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   (void)win;
   (void)xpos;
   (void)ypos;
}



void WidgetBase::OnAreaChanged() {
   WidgetMsg msg(this , TOPIC_DIALOG , DIALOG_I_MOVED);
   EagleEvent e = MakeEagleEvent(msg);
   EmitEvent(e , 0);
   (void)0;
}



void WidgetBase::OnAttributeChanged(const ATTRIBUTE& a , const VALUE& v) {
   (void)a;
   (void)v;
}



void WidgetBase::OnFlagChanged(WIDGET_FLAGS f , bool on) {
   (void)f;
   (void)on;
}



void WidgetBase::OnColorChanged() {
   SetRedrawFlag();
}



void WidgetBase::OnSelfAreaChanged(WIDGETAREA new_widget_area) {
   if (whandler) {
      /// Make the old widget area dirty
      whandler->MakeAreaDirty(warea.OuterArea());
   }
   warea = new_widget_area;
   SetBgRedrawFlag();
   OnAreaChanged();
}



void WidgetBase::OnSelfAttributeChanged(ATTRIBUTE a , VALUE v) {
   wattributes[a] = v;
   if (a.compare("Colorset") == 0) {
      wcolors = GetColorsetByName(GetAttributeValue("Colorset"));
   }
   OnAttributeChanged(a,v);
}



void WidgetBase::OnSelfFlagChanged(WidgetFlags new_widget_flags) {
   unsigned int diff = FlagDiff(wflags , new_widget_flags);
   if (diff & VISIBLE) {
      new_widget_flags.AddFlag(NEEDS_BG_REDRAW);
   }
   if (diff & (HOVER | HASFOCUS)) {
      new_widget_flags.AddFlag(NEEDS_BG_REDRAW);
   }
   if (new_widget_flags.FlagOn(NEEDS_BG_REDRAW)) {
      new_widget_flags.AddFlag(NEEDS_REDRAW);
   }
   wflags.SetNewFlags(new_widget_flags);

   unsigned int cflags = wflags.ChangedFlags();

   if (!cflags) {return;}/// Nothing changed

   for (unsigned int i = 1 ; i < NUM_WIDGET_FLAGS ; ++i) {
      /// If a flag changes, call its setter
      unsigned int flag = 1 << i;
      bool change = cflags & flag;
      if (change) {
         OnFlagChanged((WIDGET_FLAGS)flag , wflags.FlagOn((WIDGET_FLAGS)flag));
      }
   }
   
   if (cflags & NEEDS_REDRAW) {
      SetRedrawFlag();
   }
   if ((cflags & NEEDS_BG_REDRAW) || (cflags & VISIBLE)) {
      if (whandler) {
         whandler->MakeAreaDirty(OuterArea());
      }
   }

   if (!wparent) {return;}

   if (cflags & HASFOCUS) {
      wparent->SetFocusState(wflags.FlagOn(HASFOCUS));
   }
   if (cflags & HOVER) {
      wparent->SetHoverState(wflags.FlagOn(HOVER));
   }
}



void WidgetBase::OnSelfColorChanged(SHAREDOBJECT<WidgetColorset> cset) {
   wcolors = cset;
   OnColorChanged();
}



WidgetBase::~WidgetBase() {
   if (wlayout) {
      wlayout->RemoveWidget(this);
   }
   widgets.Clear();
}



int WidgetBase::HandleEvent(EagleEvent ee) {
   if (ee.type == EAGLE_EVENT_TIMER) {
///      Update(ee.timer.eagle_timer_source->SPT());
   }
   
   return (PrivateCheckInputs() | PrivateHandleEvent(ee));
}



int WidgetBase::Update(double dt) {
   return PrivateUpdate(dt);
}



void WidgetBase::Display(EagleGraphicsContext* win , int xpos , int ypos) {
   if (Flags().FlagOn(VISIBLE)) {
      WidgetPainter wp = GetWidgetPainter();
      
      EagleImage* img = win->GetDrawingTarget();
      
      Rectangle cliprect = OuterArea().MovedBy(Pos2I(xpos,ypos));
      
      Clipper clip(img , cliprect);
      
      if (wp) {
         wp->GetPainterReady(win , this , xpos , ypos);
         wp->PaintBackground();
         PrivateDisplay(win , xpos , ypos);
         if (wflags.FlagOn(HASFOCUS)) {
            wp->PaintFocus();
         }
      }
      else {
         PrivateDisplay(win , xpos , ypos);
      }
   }
   ClearRedrawFlag();
}



void WidgetBase::QueueUserMessage(const WidgetMsg& wmsg) {
   if (wparent) {
      wparent->QueueUserMessage(wmsg);
   }
}



void WidgetBase::SetAttributes(const AttributeValueMap& avmap) {
   const ATTVALMAP& avpairs = avmap.GetAttributeValueMap();
   ATTVALMAP::const_iterator it = avpairs.begin();
   while (it != avpairs.end()) {
      SetAttribute(it->first , it->second);
      ++it;
   }
}



void WidgetBase::SetAttribute(const ATTRIBUTE& a , const VALUE& v) {
   OnSelfAttributeChanged(a,v);
}


   
void WidgetBase::SetWidgetArea(WIDGETAREA area , bool notify_layout) {
   if (wlayout && notify_layout) {
      area.SetOuterArea(wlayout->RequestWidgetArea(this , area.OuterArea()));
   }
   OnSelfAreaChanged(area);
}



void WidgetBase::SetWidgetArea(Rectangle oarea , bool notify_layout) {
   WidgetArea wa = GetWidgetArea();
   if (wlayout && notify_layout) {
      wa.SetOuterArea(wlayout->RequestWidgetArea(this , oarea));
   }
   else {
      wa.SetOuterArea(oarea);
   }
   OnSelfAreaChanged(wa);
}



void WidgetBase::SetWidgetFlags(WidgetFlags flags) {
   OnSelfFlagChanged(flags);
}



void WidgetBase::SetHoverState(bool hover) {
   SetWidgetFlags(Flags().SetNewFlag(HOVER , hover));
}



void WidgetBase::SetFocusState(bool focus) {
   SetWidgetFlags(Flags().SetNewFlag(HASFOCUS , focus));
}


  
void WidgetBase::SetVisibleState(bool visible) {
   SetWidgetFlags(Flags().SetNewFlag(VISIBLE , visible));
}



void WidgetBase::SetEnabledState(bool enabled) {
   SetWidgetFlags(Flags().SetNewFlag(ENABLED , enabled));
}



void WidgetBase::SetWidgetColorset(SHAREDOBJECT<WidgetColorset> cset) {
   OnSelfColorChanged(cset);
}



void WidgetBase::SetWidgetColorset(const WidgetColorset& cset) {
   SetWidgetColorset(HeapObject(new WidgetColorset(cset)));
}



void WidgetBase::UnsetWidgetColorset() {
   SetWidgetColorset(SHAREDOBJECT<WidgetColorset>());
}



void WidgetBase::SetWidgetPainter(const WidgetPainter& wp) {
   wpainter = wp;
   SetBgRedrawFlag();
}



void WidgetBase::UnsetWidgetPainter() {
   wpainter.reset();
}



void WidgetBase::SetZOrder(int zpriority) {
   zdepth = zpriority;
   SetBgRedrawFlag();
}



bool WidgetBase::HasAttribute(const ATTRIBUTE& a) const {
   return wattributes.HasAttribute(a);
}



bool WidgetBase::InheritsAttribute(const ATTRIBUTE& a) const {
   if (HasAttribute(a)) {return false;}
   WidgetBase* w = wparent;
   while (w) {
      if (w->HasAttribute(a)) {return true;}
      w = w->wparent;
   }
   if (wlayout && wlayout->HasAttribute(a)) {return true;}
   if (whandler && whandler->HasAttribute(a)) {return true;}
   if (AttributeValueMap::GlobalAttributeMap()->HasAttribute(a)) {return true;}
   return false;
}



bool WidgetBase::AttributeIsSet(const ATTRIBUTE& a) const {
   return HasAttribute(a) || InheritsAttribute(a);
}



void WidgetBase::RemoveAttribute(const ATTRIBUTE& a) {
   if (HasAttribute(a)) {
      wattributes.RemoveAttribute(a);
   }
}



VALUE WidgetBase::GetAttributeValue(const ATTRIBUTE& a) const {
   if (HasAttribute(a)) {
      return wattributes.GetDefinedAttributeValue(a);
   }
   WidgetBase* parent = wparent;
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
   if (AttributeValueMap::GlobalAttributeMap()->HasAttribute(a)) {
      return AttributeValueMap::GlobalAttributeMap()->GetAttributeValue(a);
   }
   return "";
}



WIDGETAREA WidgetBase::GetWidgetArea() const {
   return warea;
}



Rectangle WidgetBase::OuterArea() const {
   return warea.OuterArea();
}



Rectangle WidgetBase::InnerArea() const {
   return warea.InnerArea();
}



WidgetFlags WidgetBase::Flags() const {
   return wflags;
}



SHAREDOBJECT<WidgetColorset> WidgetBase::GetWidgetColorset() const {
   if (wcolors) {return wcolors;}
   if ((bool)wparent && wparent->GetWidgetColorset()) {return wparent->GetWidgetColorset();}
   if ((bool)wlayout && wlayout->GetWidgetColorset()) {return wlayout->GetWidgetColorset();}
   if ((bool)whandler && whandler->GetWidgetColorset()) {return whandler->GetWidgetColorset();}
   return GetColorsetByName("Default");/// This is guaranteed to be in the color registry
}



const WidgetColorset& WidgetBase::WidgetColors() const {
   SHAREDOBJECT<WidgetColorset> wc = GetWidgetColorset();/// This is safe because the colorset lives somewhere else
   return *(wc.get());
}



EagleColor WidgetBase::GetColor(WIDGETCOLOR wc) const {
   return WidgetColors()[wc];

   std::string cname = WidgetColorName(wc);

   if (HasAttribute(cname)) {
      return EagleColor(GetAttributeValue(cname));
   }
   SHAREDOBJECT<WidgetColorset> cset = GetWidgetColorset();
   if (cset) {
      return (*cset)[wc];
   }
   SHAREDOBJECT<WidgetColorset> cset2 = ColorRegistry::GlobalColorRegistry()->GetDefaultColorset();
   return (*cset2)[wc];
}



WidgetPainter WidgetBase::GetWidgetPainter() const {
   if (wpainter) {
      return wpainter;
   }
   if (wparent && wparent->GetWidgetPainter()) {
      return wparent->GetWidgetPainter();
   }
   if (wlayout && wlayout->GetWidgetPainter()) {
      return wlayout->GetWidgetPainter();
   }
   if (whandler && whandler->GetWidgetPainter()) {
      return whandler->GetWidgetPainter();
   }
   return default_widget_painter;
}



void WidgetBase::SetRedrawFlag() {
   SetWidgetFlags(Flags().AddFlags(NEEDS_REDRAW));
}



void WidgetBase::SetBgRedrawFlag() {
   SetWidgetFlags(Flags().AddFlags(NEEDS_BG_REDRAW));
}



void WidgetBase::ClearRedrawFlag() {
   SetWidgetFlags(Flags().RemoveFlags(NEEDS_BG_REDRAW | NEEDS_REDRAW));
}



void WidgetBase::SetLayoutOwner(LayoutBase* l) {
   wlayout = l;
}



void WidgetBase::SetWidgetHandler(WidgetHandler* wh) {
   whandler = wh;
}



void WidgetBase::SetParent(WidgetBase* p) {
   wparent = p;
}



int WidgetBase::AbsMinWidth() const {
   return 0;
}



int WidgetBase::AbsMinHeight() const {
   return 0;
}



Pos2I WidgetBase::AbsParentPos() const {
   WidgetBase* p = wparent;
   Pos2I pos(0,0);
   while (p) {
      Rectangle r = p->InnerArea();
      pos.MoveBy(r.X() , r.Y());
      pos.MoveBy(-p->ViewPos());/// Offset the position by the parent's view position
      p = p->wparent;
   }
   return pos;
}



int WidgetBase::AbsParentX() const {
   return AbsParentPos().X();
}



int WidgetBase::AbsParentY() const {
   return AbsParentPos().Y();
}



WidgetArea WidgetBase::AbsoluteArea() const {
   WidgetArea wa = GetWidgetArea();
   Pos2I p = AbsParentPos();
   wa.MoveBy(p);
   return wa;
}



WidgetBase* WidgetBase::Root() {
   WidgetBase* root = this;
   while (root->wparent) {root = root->wparent;}
   return root;
}



WidgetHandler* WidgetBase::RootHandler() {
   return dynamic_cast<WidgetHandler*>(Root());
}



bool WidgetBase::HasGui() {
   return GetGui();
}



WidgetHandler* WidgetBase::GetGui() {
   return dynamic_cast<WidgetHandler*>(this);
}



std::ostream& WidgetBase::DescribeTo(std::ostream& os , Indenter indent) const {
   /// TODO : FIXME : Out of date
   os << indent << "WIDGETBASE : [" << FullName() << "] {" << std::endl;
   ++indent;
   os << indent << "WAREA = [" << warea << "]" << std::endl;
   os << indent << "FLAGS = [" << wflags << "]" << std::endl;
   os << indent << "ATTVALMAP = [";
   wattributes.DescribeTo(os,indent);
   os << "]" << std::endl;
   --indent;
   os << indent << "}" << std::endl;
   return os;
}



/// Global functions



bool DrawPriorityIsLess(const WidgetBase* w1 , const WidgetBase* w2) {
   return (w1->ZValue() < w2->ZValue());
}


