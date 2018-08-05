
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
 *    Copyright 2009-2016+ by Edgar Reynaldo
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



/// ------------------------------------     Layout     ------------------------------------------


int Layout::WidgetIndex(WidgetBase* widget) const {
   if (!widget) {return -1;}
   for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
      if (widget == wchildren[i]) {
         return (int)i;
      }
   }
   return -1;// use -1 to signify not in children
}



WidgetBase* Layout::GetWidget(int slot) {
   if (slot < 0 || slot >= (int)wchildren.size()) {
      return 0;
   }
   return wchildren[slot].get();
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
   wchildren.resize(nslots , SHAREDWIDGET());
}



void Layout::ReplaceWidget(SHAREDWIDGET widget , int slot) {
   
/** NOTE : This will not work right, and will prevent decorators from working properly.
   while (widget->GetDecoratorParent()) {
      widget = widget->GetDecoratorParent();
   }
*/   
   RemoveWidgetFromLayout(wchildren[slot].get());
   wchildren[slot] = widget;
   RepositionChild(slot);

   if (widget) {
      if (whandler) {
         whandler->TrackWidget(widget);
      }
      widget->SetLayoutOwner(this);
   }
}



void Layout::AdjustWidgetArea(const WidgetBase* widget , int* newx , int* newy , int* newwidth , int* newheight) const {
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
}




void Layout::RepositionAllChildren() {
   for (int slot = 0 ; slot < (int)wchildren.size() ; ++slot) {
      RepositionChild(slot);
   }
}



void Layout::RepositionChild(int slot) {
   WidgetBase* widget = GetWidget(slot);
   if (!widget) {
      EAGLE_DEBUG(
         EagleInfo() << "INFO : Layout::RepositionChild - Attempt to reposition NULL widget" << std::endl;
      );
      return;
   }
   widget->SetWidgetArea(RequestWidgetArea(slot , INT_MAX , INT_MAX , INT_MAX , INT_MAX) , false);
}



Layout::Layout(std::string objclass , std::string objname) :
      WidgetBase(objclass , objname),
      attributes(LAYOUT_ALLOWS_RESIZE_AND_REPOSITION),
      halign(HALIGN_LEFT),
      valign(VALIGN_TOP),
      wchildren(),
      whandler(0)
{
   SetDisplayPriority(LOW_DISPLAY_PRIORITY);
}



Layout::~Layout() {
   /// Okay to call detach and clear layout in our destructor, as no virtual methods are called :
   /// (ClearLayout calls RemoveWidget, PlaceWidget, ReplaceWidget , RemoveWidgetFromLayout)
   DetachFromGui();
   ClearWidgets();
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



Rectangle Layout::RequestWidgetArea(int slot , int newx , int newy , int newwidth , int newheight) const {
   
   WidgetBase* widget = GetWidget(slot);
   if (!widget) {
      return Rectangle(-1,-1,-1,-1);
   }
   
   AdjustWidgetArea(widget , &newx , &newy , &newwidth , &newheight);
   
   return Rectangle(newx , newy , newwidth , newheight);
}



Rectangle Layout::RequestWidgetArea(WidgetBase* widget , int newx , int newy , int newwidth , int newheight) const {
   return RequestWidgetArea(WidgetIndex(widget) , newx , newy , newwidth , newheight);
}



Rectangle Layout::RequestWidgetArea(int widget_slot , Rectangle newarea) const {
   return RequestWidgetArea(widget_slot , newarea.X() , newarea.Y() , newarea.W() , newarea.H());
}



Rectangle Layout::RequestWidgetArea(WidgetBase* widget , Rectangle newarea) const {
   return RequestWidgetArea(WidgetIndex(widget) , newarea.X() , newarea.Y() , newarea.W() , newarea.H());
}



Rectangle Layout::RequestWidgetArea(WidgetBase* widget) const {
   return RequestWidgetArea(WidgetIndex(widget) , INT_MAX , INT_MAX , INT_MAX , INT_MAX);
}



void Layout::Resize(unsigned int nsize) {
   ReserveSlots((int)nsize);
}



void Layout::PlaceWidget(WidgetBase* widget , int slot) {
   /// widget may be NULL to remove a widget from a slot
   EAGLE_ASSERT((slot >= 0) && (slot < (int)wchildren.size()));

   ReplaceWidget(widget , slot);
}



int Layout::AddWidget(WidgetBase* widget) {
   int slot = NextFreeSlot();
   if (slot == -1) {
      ReserveSlots((int)wchildren.size() + 1);
      slot = (int)wchildren.size() - 1;
   }
   ReplaceWidget(widget , slot);
   return slot;
}



void Layout::EmptySlot(int slot) {
   EAGLE_ASSERT(slot >= 0 && slot < (int)wchildren.size());
   PlaceWidget(0 , slot);
}



void Layout::RemoveWidget(WidgetBase* widget) {
   if (!widget) {return;}
   int index = WidgetIndex(widget);
   if (index == -1) {return;}
   PlaceWidget(0 , index);
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



void Layout::SetWChildren(std::vector<WidgetBase*> new_children) {
   ClearWidgets();
   Resize(new_children.size());
   for (int i = 0 ; i < (int)new_children.size() ; ++i) {
      PlaceWidget(new_children[i] , i);
   }
}



std::vector<WidgetBase*> Layout::WChildren() const {
   
   /// It's okay to preserve null children
   return wchildren;

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



std::vector<WidgetBase*> Layout::Descendants() const {
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



const Layout* Layout::RootLayout() const {
   if (layout) {
      return layout->RootLayout();
   }
   return this;
}



bool Layout::IsRootLayout() const {
   return layout == 0;
}



WidgetHandler* Layout::WHandler() const {
   return whandler;
}



int Layout::GetLayoutSize() const {
   return (int)wchildren.size();
}



std::ostream& Layout::DescribeTo(std::ostream& os , Indenter indent) const {
   os << indent << StringPrintF("Layout object %s (size %d):" , FullName() , (int)GetLayoutSize()) << std::endl;
   ++indent;
   os << indent << PrintLayoutAttributes(attributes) << std::endl;
   os << indent << PrintAlignment(halign , valign) << std::endl;
   os << indent << StringPrintF("WChildren (%d) : " , (int)wchildren.size()) << std::endl;
   ++indent;
   for (int i = 0 ; i < (int)wchildren.size() ; ++i) {
      WidgetBase* w = wchildren[i];
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



