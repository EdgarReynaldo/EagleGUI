
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



RelativeLayout::RelativeLayout() :
      Layout(StringPrintF("RelativeLayout object at %p" , this)),
      layout_rectangles()
{}



RelativeLayout::RelativeLayout(std::string name) :
      Layout(name),
      layout_rectangles()
{}



RelativeLayout::~RelativeLayout() {
   /// In case we go out of scope before our WidgetHandler
   DetachFromGui();
}



Rectangle RelativeLayout::RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) {
   
   (void)newx;
   (void)newy;
   (void)newwidth;
   (void)newheight;
   
   WidgetBase* widget = GetWidget(widget_slot);
   
   if (!widget) {
      return Rectangle(-1,-1,-1,-1);
   }
   
   LayoutRectangle layout_rect = layout_rectangles[widget_slot];
   
   return LayoutArea(area.InnerArea() , layout_rect);
}



void RelativeLayout::PlaceWidget(WidgetBase* widget , int slot) {
   Layout::PlaceWidget(0 , slot);
   if (widget) {
      LayoutRectangle lrect(InnerArea() , widget->OuterArea());
      PlaceWidget(widget , slot , lrect);
   }
}



int RelativeLayout::AddWidget(WidgetBase* widget) {
   int new_slot = Layout::AddWidget(0);
   if (widget) {
      LayoutRectangle lrect(InnerArea() , widget->OuterArea());
      PlaceWidget(widget , new_slot , lrect);
   }
   return new_slot;
}



void RelativeLayout::PlaceWidget(WidgetBase* widget , int slot , LayoutRectangle lrect) {
   EAGLE_ASSERT(slot >= 0 && slot < GetLayoutSize());
   layout_rectangles[slot] = lrect;

   Layout::PlaceWidget(widget , slot);
}



int RelativeLayout::AddWidget(WidgetBase* widget , LayoutRectangle lrect) {
   int new_slot = Layout::AddWidget(0);
   if (widget) {
      EAGLE_ASSERT(new_slot >= 0 && new_slot < GetLayoutSize());
      layout_rectangles[new_slot] = lrect;
      PlaceWidget(widget , new_slot , lrect);
   }
   return new_slot;
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










