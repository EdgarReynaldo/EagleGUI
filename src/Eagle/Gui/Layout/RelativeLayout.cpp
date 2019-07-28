
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



#include "Eagle/Gui/Layout/RelativeLayout.hpp"
#include "Eagle/StringWork.hpp"



void RelativeLayout::ReserveSlots(int nslots) {
   LayoutBase::ReserveSlots(nslots);
   layout_rectangles.resize(wchildren.size() , LayoutRectangle(0,0,1,1));
}



RelativeLayout::RelativeLayout(std::string classname , std::string objname) :
      LayoutBase(classname , objname),
      layout_rectangles()
{}



RelativeLayout::~RelativeLayout() {
   /// In case we go out of scope before our WidgetHandler
   DetachFromGui();
}



Rectangle RelativeLayout::RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) {
   
   const WidgetBase* widget = GetWidget(widget_slot);
   Rectangle r = widget->OuterArea();
   
   if (!widget) {
      return BADRECTANGLE;
   }
   
   if (newx == INT_MAX && newy == INT_MAX && newwidth == INT_MAX && newheight == INT_MAX) {
      /// Just return the stored layout position
      return LayoutArea(InnerArea() , layout_rectangles[widget_slot]);
   }
   
   if (newx == INT_MAX) {newx = r.X();}
   if (newy == INT_MAX) {newy = r.Y();}
   if (newwidth == INT_MAX) {newwidth = r.W();}
   if (newheight == INT_MAX) {newheight = r.H();}
   
   AdjustWidgetArea(widget , &newx , &newy , &newwidth , &newheight);
   
   Rectangle cl = InnerArea();
   if (newx < cl.X()) {newx = cl.X();}/// Fit left
   if (newy < cl.Y()) {newy = cl.Y();}/// Fit top
   if (newx > cl.BRX()) {newx = cl.BRX();}/// Fit right
   if (newy > cl.BRY()) {newy = cl.BRY();}/// Fit bottom
   if (newx + newwidth > cl.BRX()) {newwidth = cl.BRX() - newx + 1;}/// Adjust width
   if (newy + newheight > cl.BRY()) {newheight = cl.BRY() - newy + 1;}/// Adjust height
   
   Rectangle clipped(newx,newy,newwidth,newheight);
   
   EAGLE_ASSERT(clipped.Area());/// Probably don't want to set an area with zero height or width

   LayoutRectangle lr(warea.InnerArea() , clipped);
   layout_rectangles[widget_slot] = lr;
   return LayoutArea(InnerArea() , lr);
}



void RelativeLayout::PlaceWidget(WidgetBase* widget , int slot) {
   PlaceWidget(widget , slot , layout_rectangles[slot]);/// Use the stored layout rectangle
}



int RelativeLayout::AddWidget(WidgetBase* widget) {
   LayoutRectangle lr;
   if (widget) {
      lr = LayoutRectangle(InnerArea() , widget->OuterArea());
   }
   return AddWidget(widget , lr);
}



void RelativeLayout::PlaceWidget(WidgetBase* widget , int slot , LayoutRectangle lrect) {
   EAGLE_ASSERT(slot >= 0 && slot < GetLayoutSize());
   layout_rectangles[slot] = lrect;

   LayoutBase::PlaceWidget(widget , slot);
}



int RelativeLayout::AddWidget(WidgetBase* widget , LayoutRectangle lrect) {
   int new_slot = LayoutBase::AddWidget(0);
   EAGLE_ASSERT(new_slot >= 0 && new_slot < GetLayoutSize());
   layout_rectangles[new_slot] = lrect;
   PlaceWidget(widget , new_slot , lrect);
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



std::ostream& RelativeLayout::DescribeTo(std::ostream& os , Indenter indent) const {
   os << indent << "[" << FullName() << "]" << std::endl;
   for (int i = 0 ; i < GetLayoutSize() ; ++i) {
      os << indent << "LR#" << i << " : " << layout_rectangles[i] << std::endl;
   }
   return LayoutBase::DescribeTo(os,indent);
}







