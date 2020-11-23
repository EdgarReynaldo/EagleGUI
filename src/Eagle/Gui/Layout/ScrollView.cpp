
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file ScrollView.cpp
 * @brief The implementation for the scroll view widget
 */

#include "Eagle/Gui/Layout/ScrollView.hpp"
#include "Eagle/GraphicsContext.hpp"



void ScrollView::OnAreaChanged() {
   RepositionChild(0);
}



void ScrollView::ScrollCallback() {
   RepositionChild(0);
}



ScrollView::ScrollView(WidgetBase* w , std::string classname , std::string objname) :
      LayoutBase(classname , objname),
      IScrollable(),
      our_widget(0)
{
   Resize(1);
   SetOurWidget(w);
}



ScrollView::~ScrollView() {
   StopBroadcasting();
   DetachFromGui();
}



Rectangle ScrollView::RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) {
   if (widget_slot != 0) {return BADRECTANGLE;}
   WidgetBase* w = GetWidget(0);
   if (!w) {return BADRECTANGLE;}
   if (newx == INT_MAX) {
      newx = GetScrollX();
   }
   if (newy == INT_MAX) {
      newy = GetScrollY();
   }
   scrollx = newx;
   scrolly = newy;
   if (newwidth == INT_MAX) {
      newwidth = w->OuterArea().W();
   }
   if (newheight == INT_MAX) {
      newheight = w->OuterArea().H();
   }
   return Rectangle(InnerArea().X() - newx , InnerArea().Y() - newy , newwidth , newheight);
}



void ScrollView::PlaceWidget(WidgetBase* w , int slot) {
   if (slot != 0) {return;}
   LayoutBase::PlaceWidget(w,slot);
}



int ScrollView::AddWidget(WidgetBase* w) {
   PlaceWidget(w , 0);
   return 0;
}



void ScrollView::SetOurWidget(WidgetBase* widget) {
   PlaceWidget(widget , 0);
}


