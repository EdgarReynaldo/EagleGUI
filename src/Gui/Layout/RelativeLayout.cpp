
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



#include "Eagle/Gui/Layout/RelativeLayout.hpp"
#include "Eagle/StringWork.hpp"



void RelativeLayout::ReserveSlots(int nslots) {
   Layout::ReserveSlots(nslots);
   layout_rectangles.resize(wchildren.size() , LayoutRectangle(0,0,1,1));
}



void RelativeLayout::RepositionAllChildren() {
   for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
      RepositionChild(i);
   }
}



void RelativeLayout::RepositionChild(int slot) {
   EAGLE_ASSERT(slot >= 0 && slot < (int)wchildren.size());
   LayoutRectangle layout_rect = layout_rectangles[slot];
   WidgetBase* widget = wchildren[slot];
   if (widget) {
      widget->SetWidgetArea(LayoutArea(area.InnerArea() , layout_rect) , false);
   }
}




RelativeLayout::RelativeLayout() :
      Layout(StringPrintF("RelativeLayout object at %p" , this))
{}



RelativeLayout::RelativeLayout(std::string name) :
      Layout(name)
{}



RelativeLayout::~RelativeLayout() {
   /// In case we go out of scope before our WidgetHandler
   DetachFromGui();
}



Rectangle RelativeLayout::RequestWidgetArea(WidgetBase* widget , int newx , int newy , int newwidth , int newheight) {
   EAGLE_ASSERT(widget);
   EAGLE_ASSERT(WidgetIndex(widget) != -1);
   
   AdjustWidgetArea(widget , &newx , &newy , &newwidth , &newheight);
   
   Rectangle newrect(newx , newy , newwidth , newheight);
   
///   SetWidgetPos(widget , newx , newy , newwidth , newheight);/// NOTE : Causes endless loops
   
   // need to reset layout rectangle for this widget
///   layout_rectangles[WidgetIndex(widget)] = LayoutRectangle(InnerArea() , newrect);

   return newrect;
}



void RelativeLayout::PlaceWidget(WidgetBase* widget , int slot) {
   if (widget) {
      LayoutRectangle lrect(InnerArea() , widget->OuterArea());
      PlaceWidget(widget , slot , lrect);
      return;
   }
   Layout::PlaceWidget(0 , slot);
}



void RelativeLayout::AddWidget(WidgetBase* widget) {
   if (widget) {
      LayoutRectangle lrect(InnerArea() , widget->OuterArea());
      AddWidget(widget , lrect);
      return;
   }
   Layout::AddWidget(0);
}



Rectangle RelativeLayout::SetLayoutRectangle(int index , LayoutRectangle layout_rect) {
   EAGLE_ASSERT(index >= 0 && index < GetLayoutSize());
   layout_rectangles[index] = layout_rect;
   RepositionChild(index);
   if (wchildren[index]) {
      return wchildren[index]->OuterArea();
   }
   return Rectangle();
}



Rectangle RelativeLayout::SetLayoutRectangle(WidgetBase* widget , LayoutRectangle layout_rect) {
   EAGLE_ASSERT(widget);
   int index = WidgetIndex(widget);
   EAGLE_ASSERT(index != -1);
   
   return SetLayoutRectangle(index , layout_rect);
}



void RelativeLayout::PlaceWidget(WidgetBase* widget , int slot , LayoutRectangle lrect) {
   Layout::PlaceWidget(widget , slot);
   if (widget) {
      SetLayoutRectangle(widget , lrect);
   }
}



void RelativeLayout::AddWidget(WidgetBase* widget , LayoutRectangle lrect) {
   Layout::AddWidget(widget);
   if (widget) {
      SetLayoutRectangle(widget , lrect);
   }
}



void RelativeLayout::Resize(unsigned int nsize) {
   ReserveSlots((int)nsize);
}






