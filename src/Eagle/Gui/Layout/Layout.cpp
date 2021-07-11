
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


#include <limits.h>// for INT_MAX

#include "Eagle/Gui/Layout/Layout.hpp"
#include "Eagle/Gui/WidgetHandler.hpp"

#include "Eagle/StringWork.hpp"

#include <sstream>
using std::stringstream;



/// ---------------------------------     Global functions     ----------------------------------


std::string PrintLayoutAttributes(LAYOUT_ATTRIBUTES attributes) {
   stringstream ss;
   ss << "Layout attributes : ";
   switch (attributes) {
      case LAYOUT_ALLOWS_NOTHING :
         ss << "LAYOUT_ALLOWS_NOTHING";
         break;
      case LAYOUT_ALLOWS_REPOSITION :
         ss << "LAYOUT_ALLOWS_REPOSITION";
         break;
      case LAYOUT_ALLOWS_RESIZE :
         ss << "LAYOUT_ALLOWS_RESIZE";
         break;
      case LAYOUT_ALLOWS_RESIZE_AND_REPOSITION :
         ss << "LAYOUT_ALLOWS_RESIZE_AND_REPOSITION";
         break;
      default : throw EagleException(StringPrintF("Unknown layout attributes value (%d)\n",(int)attributes));break;
   }
   return ss.str();
}



/// ------------------------------------     LayoutBase     ------------------------------------------


int LayoutBase::WidgetIndex(const WidgetBase* widget) const {
   if (!widget) {return -1;}
   for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
      if (widget == wchildren[i]) {
         return (int)i;
      }
   }
   return -1;// use -1 to signify not in children
}



WidgetBase* LayoutBase::GetWidget(int slot) {
   if (slot < 0 || slot >= (int)wchildren.size()) {
      return 0;
   }
   return wchildren[slot];
}



const WidgetBase* LayoutBase::GetWidget(int slot) const {
   if (slot < 0 || slot >= (int)wchildren.size()) {
      return 0;
   }
   return wchildren[slot];
}



int LayoutBase::NextFreeSlot() {
   for (int i = 0 ; i < (int)wchildren.size() ; ++i) {
      if (wchildren[i] == 0) {
         return i;
      }
   }
   return -1;
}



void LayoutBase::ReserveSlots(int nslots) {
   if (nslots == (int)wchildren.size()) {return;}
   
   if (nslots < 1) {
      nslots = 0;// don't crash resize below
      ClearWidgets();
   }
   if (nslots < (int)wchildren.size()) {
      for (unsigned int i = nslots ; i < wchildren.size() ; ++i) {
         EmptySlot(i);
      }
   }
   wchildren.resize(nslots , (WidgetBase*)0);
}



void LayoutBase::ReplaceWidget(WidgetBase* widget , int slot) {
   
   RemoveWidgetFromLayout(wchildren[slot]);
   wchildren[slot] = widget;
   RepositionChild(slot);

   if (widget) {
      if (whandler) {
         whandler->TrackWidget(widget);
      }
      widget->SetLayoutOwner(this);
   }
}



void LayoutBase::AdjustWidgetArea(const WidgetBase* widget , int* newx , int* newy , int* newwidth , int* newheight) const {
   EAGLE_ASSERT(widget);
   EAGLE_ASSERT(newx);
   EAGLE_ASSERT(newy);
   EAGLE_ASSERT(newwidth);
   EAGLE_ASSERT(newheight);
   
   unsigned int flags = (unsigned int)widget->Flags();
   
   Rectangle w = widget->OuterArea();
   
   if (*newx == INT_MAX) {
      *newx = w.X();
   }
   if (*newy == INT_MAX) {
      *newy = w.Y();
   }
   if (*newwidth == INT_MAX) {
      *newwidth = prefw;
///      *newwidth = w.W();
   }
   else {
      int mw = widget->AbsMinWidth();
      if (*newwidth < abs(mw)) {
         *newwidth = mw;
      }
   }
   if (*newheight == INT_MAX) {
      *newheight = prefh;
///      *newheight = w.H();
   }
   else {
      int mh = widget->AbsMinHeight();
      if (*newheight < abs(mh)) {
         *newheight = mh;
      }
   }
   
   if (!(flags & MOVEABLE)) {
      *newx = w.X();
      *newy = w.Y();
   }
   if (!(flags & RESIZEABLE)) {
      *newwidth = w.W();
      *newheight = w.H();
   }
   
   if (!(attributes & LAYOUT_ALLOWS_REPOSITION)) {
      *newx = w.X();
      *newy = w.Y();
   }
   if (!(attributes & LAYOUT_ALLOWS_RESIZE)) {
      *newwidth = w.W();
      *newheight = w.H();
   }
   
   /// Handle rectangles with negative width or height
   Rectangle r;
   r.SetCorners(*newx , *newy , *newx + *newwidth - 1 , *newy + *newheight - 1);
   *newx = r.X();
   *newy = r.Y();
   *newwidth = (r.W()>0)?r.W():1;
   *newheight = (r.H()>0)?r.H():1;
   
}




void LayoutBase::RepositionAllChildren() {
   for (int slot = 0 ; slot < (int)wchildren.size() ; ++slot) {
      LayoutBase::RepositionChild(slot);
   }
}



void LayoutBase::RepositionChild(int slot) {
   WidgetBase* widget = GetWidget(slot);
   if (!widget) {
      return;
   }
   widget->SetWidgetArea(RequestWidgetArea(slot , INT_MAX , INT_MAX , INT_MAX , INT_MAX) , false);
}



void LayoutBase::RemoveWidgetFromLayout(WidgetBase* widget) {
   if (!widget) {
      return;
   }

   if (whandler) {
      whandler->StopTrackingWidget(widget);
   }
   widget->SetLayoutOwner(0);
}



void LayoutBase::OnFlagChanged(WIDGET_FLAGS f , bool on) {
   if (f & VISIBLE) {
      for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
         WidgetBase* w = wchildren[i];
         if (w) {
            w->SetVisibleState(on);
         }
      }
   }
   if (f & ENABLED) {
      for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
         WidgetBase* w = wchildren[i];
         if (w) {
            w->SetEnabledState(on);
         }
      }
   }
}



void LayoutBase::OnAreaChanged() {
   RepositionAllChildren();
}



int LayoutBase::PrivateHandleInputEvent(EagleEvent e) {
   (void)e;
   return DIALOG_OKAY;
}



int LayoutBase::PrivateUpdate(double dt) {
   (void)dt;
   return DIALOG_OKAY;
}



void LayoutBase::PrivateDisplay(EagleGraphicsContext* win , int x , int y) {
   WidgetBase::PrivateDisplay(win,x,y);
}



LayoutBase::LayoutBase(std::string objclass , std::string objname) :
      WidgetBase(objclass , objname),
      wchildren(),
      attributes(LAYOUT_ALLOWS_RESIZE_AND_REPOSITION),
      halign(HALIGN_LEFT),
      valign(VALIGN_TOP)
{
   zdepth = ZORDER_PRIORITY_LOW;
}



LayoutBase::~LayoutBase() {
   /// Okay to call detach and clear layout in our destructor, as no virtual methods are called :
   /// (ClearLayout calls RemoveWidget, PlaceWidget, ReplaceWidget , RemoveWidgetFromLayout)
   DetachFromGui();
   ClearWidgets();
}



Rectangle LayoutBase::RequestWidgetArea(int slot , int newx , int newy , int newwidth , int newheight) {
   
   const WidgetBase* widget = GetWidget(slot);
   if (!widget) {
      return BADRECTANGLE;
   }
   
   AdjustWidgetArea(widget , &newx , &newy , &newwidth , &newheight);
   
   return Rectangle(newx , newy , newwidth , newheight);
}



Rectangle LayoutBase::RequestWidgetArea(const WidgetBase* widget , int newx , int newy , int newwidth , int newheight) {
   return RequestWidgetArea(WidgetIndex(widget) , newx , newy , newwidth , newheight);
}



Rectangle LayoutBase::RequestWidgetArea(int widget_slot , Rectangle newarea) {
   return RequestWidgetArea(widget_slot , newarea.X() , newarea.Y() , newarea.W() , newarea.H());
}



Rectangle LayoutBase::RequestWidgetArea(const WidgetBase* widget , Rectangle newarea) {
   return RequestWidgetArea(WidgetIndex(widget) , newarea.X() , newarea.Y() , newarea.W() , newarea.H());
}



Rectangle LayoutBase::RequestWidgetArea(const WidgetBase* widget) const {
#warning TODO : Think of a better hack than casting away const
   return const_cast<LayoutBase*>(this)->RequestWidgetArea(WidgetIndex(widget) , INT_MAX , INT_MAX , INT_MAX , INT_MAX);
}



void LayoutBase::Resize(unsigned int nsize) {
   ReserveSlots((int)nsize);
}



void LayoutBase::PlaceWidget(WidgetBase* widget , int slot) {
   /// widget may be NULL to remove a widget from a slot
   EAGLE_ASSERT((slot >= 0) && (slot < (int)wchildren.size()));

   ReplaceWidget(widget , slot);
}



int LayoutBase::AddWidget(WidgetBase* widget) {
   int slot = NextFreeSlot();
   if (slot == -1) {
      ReserveSlots((int)wchildren.size() + 1);
      slot = (int)wchildren.size() - 1;
   }
   ReplaceWidget(widget , slot);
   return slot;
}



void LayoutBase::InsertWidget(WidgetBase* w , int insertion_slot) {
   EAGLE_ASSERT(insertion_slot >= 0 && insertion_slot <= (int)wchildren.size());
   if (insertion_slot == (int)wchildren.size()) {
      AddWidget(w);
      return;
   }
   Resize(wchildren.size() + 1);
   for (int i = (int)wchildren.size() -1 ; i > insertion_slot ; --i) {
      wchildren[i] = wchildren[i - 1];
   }
   wchildren[insertion_slot] = 0;
   ReplaceWidget(w , insertion_slot);
   for (unsigned int i = insertion_slot + 1 ; i < wchildren.size() ; ++i) {
      RepositionChild(i);
   }
}



void LayoutBase::EmptySlot(int slot) {
   EAGLE_ASSERT(slot >= 0 && slot < (int)wchildren.size());
   PlaceWidget(0 , slot);
}



void LayoutBase::RemoveWidget(WidgetBase* widget) {
   if (!widget) {return;}
   int index = WidgetIndex(widget);
   if (index == -1) {return;}
   PlaceWidget(0 , index);
}



void LayoutBase::EraseWidgetSlot(int slot) {
   EAGLE_ASSERT(slot >= 0 && slot < (int)wchildren.size());
   EAGLE_ASSERT(wchildren.size() >= 1);
   EmptySlot(slot);
   for (int i = slot ; i < (int)wchildren.size() - 1 ; ++i) {
      wchildren[i] = wchildren[i+1];
      RepositionChild(i);
   }
}



void LayoutBase::EraseWidgetSlot(WidgetBase* widget) {
   int index = WidgetIndex(widget);
   EAGLE_ASSERT(index != -1);
   EraseWidgetSlot(index);
}



void LayoutBase::ClearWidgets() {
   
   for (int i = 0 ; i < (int)wchildren.size() ; ++i) {
      WidgetBase* w = wchildren[i];
      RemoveWidget(w);/// Calls PlaceWidget(0,w) which calls ReplaceWidget which calls RemoveWidgetFromLayout
   }
   wchildren.clear();
}



void LayoutBase::DetachFromGui() {
   if (whandler) {
      whandler->StopTrackingWidget(this);/// This will remove us and all of our widgets from the widgethandler's tracker mechanism
      whandler = 0;
   }
}



void LayoutBase::SetAlignment(HALIGNMENT h_align , VALIGNMENT v_align) {
	halign = h_align;
	valign = v_align;
	RepositionAllChildren();
}



void LayoutBase::SetWChildren(std::vector<WidgetBase*> new_children) {
   ClearWidgets();
   ReserveSlots(new_children.size());
   for (int i = 0 ; i < (int)new_children.size() ; ++i) {
      PlaceWidget(new_children[i] , i);
   }
}



std::vector<WidgetBase*> LayoutBase::ChildrenVector() const {
   return wchildren;
}



std::vector<WidgetBase*> LayoutBase::WChildren() const {
   std::vector<WidgetBase*> real;
   for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
      if (wchildren[i]) {
         real.push_back(wchildren[i]);
      }
   }
   return real;
}



std::vector<WidgetBase*> LayoutBase::Descendants() const {
	std::vector<WidgetBase*> descendants;
	for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
		WidgetBase* widget = wchildren[i];
		if (!widget) {continue;}
		descendants.push_back(widget);
		LayoutBase* l = dynamic_cast<LayoutBase*>(widget);
		if (l) {
			std::vector<WidgetBase*> grandchildren = l->Descendants();
			for (unsigned int j = 0 ; j < grandchildren.size() ; ++j) {
				descendants.push_back(grandchildren[j]);
			}
			
		}
	}
	return descendants;
}


LayoutBase* LayoutBase::RootLayout() {
   if (wlayout) {
      return wlayout->RootLayout();
   }
   return this;
}



const LayoutBase* LayoutBase::RootLayout() const {
   if (wlayout) {
      return wlayout->RootLayout();
   }
   return this;
}



bool LayoutBase::IsRootLayout() const {
   return wlayout == 0;
}



int LayoutBase::GetLayoutSize() const {
   return (int)wchildren.size();
}



Rectangle LayoutBase::GetClipRectangle() {
   LayoutBase* playout = GetLayout();
   if (playout) {
      return Overlap(InnerArea() , playout->GetClipRectangle());
   }
   return InnerArea();
}



std::ostream& LayoutBase::DescribeTo(std::ostream& os , Indenter indent) const {
   os << indent << StringPrintF("LayoutBase object %s (size %d):" , FullName() , (int)GetLayoutSize()) << std::endl;
   ++indent;
   os << indent << PrintLayoutAttributes(attributes) << std::endl;
   os << indent << PrintAlignment(halign , valign) << std::endl;
   os << indent << StringPrintF("WChildren (%d) : " , (int)wchildren.size()) << std::endl;
   ++indent;
   for (int i = 0 ; i < (int)wchildren.size() ; ++i) {
      const WidgetBase* w = wchildren[i];
      if (w) {
         os << indent << StringPrintF("#%d (%s) " , i , w->FullName());
         os << "Requested Area = " << RequestWidgetArea(w) << std::endl;
      }
   }
   --indent;
   WidgetBase::DescribeTo(os,indent);
   --indent;
   os << std::endl;
   return os;
}




