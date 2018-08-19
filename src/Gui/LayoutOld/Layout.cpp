
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


#include <limits.h>// for INT_MAX

#include "Eagle/Gui/Layout/Layout2.hpp"
#include "Eagle/Gui2/WidgetHandler2.hpp"

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


int LAYOUTBASE::WidgetIndex(WidgetBase* widget) const {
   if (!widget) {return -1;}
   for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
      if (widget == wchildren[i].get()) {
         return (int)i;
      }
   }
   return -1;// use -1 to signify not in children
}



SHAREDWIDGET LAYOUTBASE::GetWidget(int slot)  const {
   if (slot < 0 || slot >= (int)wchildren.size()) {
      return 0;
   }
   return wchildren[slot];
}



int LAYOUTBASE::NextFreeSlot() {
   for (int i = 0 ; i < (int)wchildren.size() ; ++i) {
      if (wchildren[i].get() == 0) {
         return i;
      }
   }
   return -1;
}



void LAYOUTBASE::ReserveSlots(int nslots) {
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
   wchildren.resize(nslots , std::shared_ptr<WIDGETBASE>(0));
}



void LAYOUTBASE::ReplaceWidget(SHAREDWIDGET widget , int slot) {
   
/** NOTE : This will not work right, and will prevent decorators from working properly.
   while (widget->GetDecoratorParent()) {
      widget = widget->GetDecoratorParent();
   }
*/   
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



void LAYOUTBASE::AdjustWidgetArea(const WidgetBase* widget , int* newx , int* newy , int* newwidth , int* newheight) const {
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




void LAYOUTBASE::RepositionAllChildren() {
   for (int slot = 0 ; slot < (int)wchildren.size() ; ++slot) {
      RepositionChild(slot);
   }
}



void LAYOUTBASE::RepositionChild(int slot) {
   SHAREDWIDGET widget = GetWidget(slot);
   if (!widget) {
      EAGLE_DEBUG(
         EagleInfo() << "INFO : LAYOUTBASE::RepositionChild - Attempt to reposition NULL widget" << std::endl;
      );
      return;
   }
   widget->SetWidgetArea(RequestWidgetArea(slot , INT_MAX , INT_MAX , INT_MAX , INT_MAX) , false);
}



void LAYOUTBASE::OnAreaChanged() {
   RepositionAllChildren();
}



int LAYOUTBASE::PrivateHandleInputEvent(EagleEvent e) {
   (void)e;
   return DIALOG_OKAY;
}



int LAYOUTBASE::PrivateUpdate(double dt) {
   (void)dt;
   return DIALOG_OKAY;
}



void LAYOUTBASE::PrivateDisplay(EagleGraphicsContext* win , int x , int y) {
   WidgetBase::PrivateDisplay(win,x,y);
}



LAYOUTBASE::LAYOUTBASE(std::string objclass , std::string objname) :
      WidgetBase(objclass , objname),
      wchildren(),
      attributes(LAYOUT_ALLOWS_RESIZE_AND_REPOSITION),
      halign(HALIGN_LEFT),
      valign(VALIGN_TOP)
{
}



LAYOUTBASE::~LAYOUTBASE() {
   /// Okay to call detach and clear layout in our destructor, as no virtual methods are called :
   /// (ClearLayout calls RemoveWidget, PlaceWidget, ReplaceWidget , RemoveWidgetFromLayout)
   DetachFromGui();
   ClearWidgets();
}



Rectangle LAYOUTBASE::RequestWidgetArea(int slot , int newx , int newy , int newwidth , int newheight) const {
   
   SHAREDWIDGET w = GetWidget(slot);
   if (!widget) {
      return Rectangle(-1,-1,-1,-1);
   }
   
   AdjustWidgetArea(widget , &newx , &newy , &newwidth , &newheight);
   
   return Rectangle(newx , newy , newwidth , newheight);
}



Rectangle LAYOUTBASE::RequestWidgetArea(WidgetBase* widget , int newx , int newy , int newwidth , int newheight) const {
   return RequestWidgetArea(WidgetIndex(widget) , newx , newy , newwidth , newheight);
}



Rectangle LAYOUTBASE::RequestWidgetArea(int widget_slot , Rectangle newarea) const {
   return RequestWidgetArea(widget_slot , newarea.X() , newarea.Y() , newarea.W() , newarea.H());
}



Rectangle LAYOUTBASE::RequestWidgetArea(WidgetBase* widget , Rectangle newarea) const {
   return RequestWidgetArea(WidgetIndex(widget) , newarea.X() , newarea.Y() , newarea.W() , newarea.H());
}



Rectangle LAYOUTBASE::RequestWidgetArea(WidgetBase* widget) const {
   return RequestWidgetArea(WidgetIndex(widget) , INT_MAX , INT_MAX , INT_MAX , INT_MAX);
}



void LAYOUTBASE::Resize(unsigned int nsize) {
   ReserveSlots((int)nsize);
}



void LAYOUTBASE::PlaceWidget(SHAREDWIDGET w , int slot) {
   /// widget may be NULL to remove a widget from a slot
   EAGLE_ASSERT((slot >= 0) && (slot < (int)wchildren.size()));

   ReplaceWidget(widget , slot);
}



int LAYOUTBASE::AddWidget(SHAREDWIDGET w) {
   int slot = NextFreeSlot();
   if (slot == -1) {
      ReserveSlots((int)wchildren.size() + 1);
      slot = (int)wchildren.size() - 1;
   }
   ReplaceWidget(widget , slot);
   return slot;
}



void LAYOUTBASE::EmptySlot(int slot) {
   EAGLE_ASSERT(slot >= 0 && slot < (int)wchildren.size());
   PlaceWidget(0 , slot);
}



void LAYOUTBASE::RemoveWidget(SHAREDWIDGET w) {
   if (!widget) {return;}
   int index = WidgetIndex(widget);
   if (index == -1) {return;}
   PlaceWidget(0 , index);
}



void LAYOUTBASE::ClearWidgets() {
   
   for (int i = 0 ; i < (int)wchildren.size() ; ++i) {
      WidgetBase* w = wchildren[i];
      RemoveWidget(w);/// Calls PlaceWidget(0,w) which calls ReplaceWidget which calls RemoveWidgetFromLayout
   }
   wchildren.clear();
}



void LAYOUTBASE::RemoveWidgetFromLayout(SHAREDWIDGET w) {
   if (!widget) {
      return;
   }

   if (whandler) {
      whandler->StopTrackingWidget(widget);
      widget->SetOwnerLayout(0);
   }
}



void LAYOUTBASE::DetachFromGui() {
   if (whandler) {
      whandler->StopTrackingWidget(this);/// This will remove us and all of our widgets from the widgethandler's tracker mechanism
      whandler = 0;
   }
}



void LAYOUTBASE::SetAlignment(HALIGNMENT h_align , VALIGNMENT v_align) {
	halign = h_align;
	valign = v_align;
	RepositionAllChildren();
}



void LAYOUTBASE::SetWChildren(std::vector<WidgetBase*> new_children) {
   ClearWidgets();
   Resize(new_children.size());
   for (int i = 0 ; i < (int)new_children.size() ; ++i) {
      PlaceWidget(new_children[i] , i);
   }
}



std::vector<WidgetBase*> LAYOUTBASE::WChildren() const {
   
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



std::vector<WidgetBase*> LAYOUTBASE::Descendants() const {
	std::vector<WidgetBase*> descendants;
	for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
		SHAREDWIDGET w = wchildren[i];
		if (!widget) {continue;}
		descendants.push_back(widget);
		LAYOUTBASE* l = dynamic_cast<LAYOUTBASE*>(widget);
		if (l) {
			std::vector<WidgetBase*> grandchildren = l->Descendants();
			for (unsigned int j = 0 ; j < grandchildren.size() ; ++j) {
				descendants.push_back(grandchildren[j]);
			}
			
		}
	}
	return descendants;
}



LAYOUTBASE* LAYOUTBASE::RootLayout() {
   if (layout) {
      return layout->RootLayout();
   }
   return this;
}



const LAYOUTBASE* LAYOUTBASE::RootLayout() const {
   if (layout) {
      return layout->RootLayout();
   }
   return this;
}



bool LAYOUTBASE::IsRootLayout() const {
   return layout == 0;
}



WidgetHandler* LAYOUTBASE::WHandler() const {
   return whandler;
}



int LAYOUTBASE::GetLayoutSize() const {
   return (int)wchildren.size();
}



std::ostream& LAYOUTBASE::DescribeTo(std::ostream& os , Indenter indent) const {
   os << indent << StringPrintF("LAYOUTBASE object %s (size %d):" , FullName() , (int)GetLayoutSize()) << std::endl;
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




