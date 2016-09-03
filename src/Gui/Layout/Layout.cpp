
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
      default : throw EagleError(StringPrintF("Unknown layout attributes value (%d)\n",(int)attributes));break;
   }
   return ss.str();
}



/// ------------------------------------     Layout     ------------------------------------------


int Layout::WidgetIndex(WidgetBase* widget) {
   if (!widget) {return -1;}
   for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
      if (widget == wchildren[i]) {
         return (int)i;
      }
   }
   return -1;// use -1 to signify not in children
}



int Layout::NextFreeSlot() {
   for (int i = 0 ; i < (int)wchildren.size() ; ++i) {
      if (wchildren[i] == 0) {
         return i;
      }
   }
   return -1;
}



void Layout::ReserveSlots(int nslots) {
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
   wchildren.resize(nslots , 0);
}



void Layout::ReplaceWidget(WidgetBase* widget , int slot) {
   RemoveWidgetFromLayout(wchildren[slot]);
   wchildren[slot] = widget;
   RepositionChild(slot);

   if (whandler && widget) {
      whandler->TrackWidget(widget);
      widget->SetOwnerLayout(this);
   }
}



void Layout::AdjustWidgetArea(WidgetBase* widget , int* newx , int* newy , int* newwidth , int* newheight) {
   EAGLE_ASSERT(widget);
   EAGLE_ASSERT(newx);
   EAGLE_ASSERT(newy);
   EAGLE_ASSERT(newwidth);
   EAGLE_ASSERT(newheight);
   
   int wflags = widget->Flags();
   
   Rectangle w = widget->OuterArea();
   
   if (*newx == INT_MAX) {
      *newx = w.X();
   }
   if (*newy == INT_MAX) {
      *newy = w.Y();
   }
   if (*newwidth == INT_MAX) {
      *newwidth = w.W();
   }
   else {
      int mw = widget->AbsMinWidth();
      if (*newwidth < mw) {
         *newwidth = mw;
      }
   }
   if (*newheight == INT_MAX) {
      *newheight = w.H();
   }
   else {
      int mh = widget->AbsMinHeight();
      if (*newheight < mh) {
         *newheight = mh;
      }
   }
   
   if (!(wflags & MOVEABLE)) {
      *newx = w.X();
      *newy = w.Y();
   }
   if (!(wflags & RESIZEABLE)) {
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
}



Layout::Layout() :
      WidgetBase(StringPrintF("Layout object at %p" , this)),
      attributes(LAYOUT_ALLOWS_RESIZE_AND_REPOSITION),
      halign(HALIGN_LEFT),
      valign(VALIGN_TOP),
      wchildren(),
      whandler(0)
{
   SetDisplayPriority(LOW_DISPLAY_PRIORITY);
}



Layout::Layout(std::string name) :
      WidgetBase(name),
      attributes(LAYOUT_ALLOWS_RESIZE_AND_REPOSITION),
      halign(HALIGN_LEFT),
      valign(VALIGN_TOP),
      wchildren(),
      whandler(0)
{
   SetDisplayPriority(LOW_DISPLAY_PRIORITY);
}



Layout::~Layout() {
   // Can't call clear layout in our destructor, as virtual methods are called :
   // (ClearLayout calls RemoveWidget, PlaceWidget, ReplaceWidget , RemoveWidgetFromLayout)
///   ClearLayout();
}



int Layout::PrivateHandleInputEvent(EagleEvent e) {
   (void)e;
   return DIALOG_OKAY;
}



int Layout::PrivateUpdate(double dt) {
   (void)dt;
   return DIALOG_OKAY;
}



void Layout::PrivateDisplay(EagleGraphicsContext* win , int x , int y) {
   WidgetBase::PrivateDisplay(win,x,y);
}



void Layout::SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout) {
   WidgetBase::SetWidgetArea(xpos , ypos , width , height , notify_layout);
   RepositionAllChildren();
}




/// Changes position and outer area!!!
void Layout::SetMarginsExpandFromInner(int left , int right , int top , int bottom) {
   WidgetBase::SetMarginsExpandFromInner(left,right,top,bottom);
   RepositionAllChildren();
}



/// Make room in outer area for inner area first!!!
void Layout::SetMarginsContractFromOuter(int left , int right , int top , int bottom) {
   WidgetBase::SetMarginsContractFromOuter(left,right,top,bottom);
   RepositionAllChildren();
}



/**
void Layout::TakeOverLayoutFrom(Layout* l) {
   ClearLayout();
	if (l) {
		wchildren = l->wchildren;
		l->ClearLayout();
		for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
			WidgetBase* w = wchildren[i];
			if (w) {
            w->SetLayout(this);
            if (whandler) {
               whandler->TrackWidget(w);
            }
			}
		}
		RepositionAllChildren();
	}
}
//*/



Rectangle Layout::RequestWidgetArea(WidgetBase* widget , int newx , int newy , int newwidth , int newheight) {
   EAGLE_ASSERT(widget);
   EAGLE_ASSERT(WidgetIndex(widget) != -1);
   
   AdjustWidgetArea(widget , &newx , &newy , &newwidth , &newheight);
   
   Rectangle newrect(newx , newy , newwidth , newheight);
   
   return newrect;
}



Rectangle Layout::RequestWidgetArea(WidgetBase* widget , Rectangle newarea) {
   return RequestWidgetArea(widget , newarea.X() , newarea.Y() , newarea.W() , newarea.H());
}



void Layout::PlaceWidget(WidgetBase* widget , int slot) {
   /// widget may be NULL to remove a widget from a slot
   EAGLE_ASSERT((slot >= 0) && (slot < (int)wchildren.size()));

   ReplaceWidget(widget , slot);
}



void Layout::AddWidget(WidgetBase* widget) {
   int slot = NextFreeSlot();
   if (slot == -1) {
      ReserveSlots((int)wchildren.size() + 1);
      slot = (int)wchildren.size() - 1;
   }
   ReplaceWidget(widget , slot);
}



void Layout::EmptySlot(int slot) {
   EAGLE_ASSERT(slot >= 0 && slot < (int)wchildren.size());
   PlaceWidget(0 , slot);
}



void Layout::RemoveWidget(WidgetBase* widget) {
   EAGLE_ASSERT(widget);
   int index = WidgetIndex(widget);
   EAGLE_ASSERT(index != -1);
   if (index != -1) {
      PlaceWidget(0 , index);
   }
}



void Layout::ClearWidgets() {
   
   for (int i = 0 ; i < (int)wchildren.size() ; ++i) {
      WidgetBase* w = wchildren[i];
      RemoveWidget(w);/// Calls PlaceWidget(0,w) which calls ReplaceWidget which calls RemoveWidgetFromLayout
   }
   wchildren.clear();
}



void Layout::RemoveWidgetFromLayout(WidgetBase* widget) {
   if (!widget) {
      return;
   }

   if (whandler) {
      whandler->StopTrackingWidget(widget);
      widget->SetOwnerLayout(0);
   }
}



void Layout::DetachFromGui() {
   if (whandler) {
      whandler->StopTrackingWidget(this);/// This will remove us and all of our widgets from the widgethandler's tracker mechanism
      whandler = 0;
   }
}



void Layout::SetAlignment(HALIGNMENT h_align , VALIGNMENT v_align) {
	halign = h_align;
	valign = v_align;
	RepositionAllChildren();
}



void Layout::SetGuiHandler(WidgetHandler* handler) {
   whandler = handler;
}



std::vector<WidgetBase*> Layout::WChildren() {
   
   /// Some widgets may be NULL so we can't just copy the vector
   std::vector<WidgetBase*> children;
   children.reserve(wchildren.size());
   for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
      if (wchildren[i]) {
         children.push_back(wchildren[i]);
      }
   }
   return children;
}



std::vector<WidgetBase*> Layout::Descendants() {
	std::vector<WidgetBase*> descendants;
	for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
		WidgetBase* widget = wchildren[i];
		if (!widget) {continue;}
		descendants.push_back(widget);
		Layout* l = dynamic_cast<Layout*>(widget);
		if (l) {
			std::vector<WidgetBase*> grandchildren = l->Descendants();
			for (unsigned int j = 0 ; j < grandchildren.size() ; ++j) {
				descendants.push_back(grandchildren[j]);
			}
			
		}
	}
	return descendants;
}



Layout* Layout::RootLayout() {
   if (layout) {
      return layout->RootLayout();
   }
   return this;
}



bool Layout::IsRootLayout() {
   return layout == 0;
}



WidgetHandler* Layout::WHandler() {
   return whandler;
}



int Layout::GetLayoutSize() {
   return (int)wchildren.size();
}



std::ostream& Layout::DescribeTo(std::ostream& os , Indenter indent) const {
   os << indent << StringPrintF("Layout object at %p named %s :",this,GetName().c_str()) << std::endl;
   ++indent;
   os << indent << PrintLayoutAttributes(attributes) << std::endl;
   os << indent << PrintAlignment(halign , valign) << std::endl;
   WidgetBase::DescribeTo(os,indent);
   --indent;
   os << std::endl;
   return os;
}




