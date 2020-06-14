
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
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file ScrollView.cpp
 * @brief The implementation for the scroll view widget
 */

#include "Eagle/Gui/ScrollView.hpp"
#include "Eagle/GraphicsContext.hpp"



void ScrollView::OnAreaChanged() {
   RepositionWidget();
}



void ScrollView::OnFlagChanged(WIDGET_FLAGS f , bool on) {
   if (!our_widget) {return;}
   our_widget->SetWidgetFlags(Flags().SetNewFlag(f , on));
}



void ScrollView::RepositionWidget() {
   if (!our_widget) {return;}
   
   Rectangle inner = InnerArea();

   Rectangle our_widget_area = our_widget->GetWidgetArea().OuterArea();
   int vw = our_widget_area.W();
   int vh = our_widget_area.H();
   int x = inner.X() - GetScrollX();
   int y = inner.Y() - GetScrollY();
   
   Rectangle wrect(x,y,vw,vh);
   EaglePrefix("SCROLLVIEW child pos = ") << wrect << std::endl;
   our_widget->SetWidgetArea(wrect , false);
   SetBgRedrawFlag();
}



void ScrollView::ScrollCallback() {
   RepositionWidget();
}



int ScrollView::PrivateHandleEvent(EagleEvent ee) {
   if (!our_widget) {
      return DIALOG_OKAY;
   }
   
   int ret = our_widget->HandleEvent(ee);
   return ret;
}



int ScrollView::PrivateUpdate(double dt) {
   return our_widget?our_widget->Update(dt):DIALOG_OKAY;
}



void ScrollView::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   if (our_widget) {
      EagleImage* img = win->GetDrawingTarget();
      Clipper clip(img , InnerArea());
      our_widget->Display(win , xpos , ypos);
   }
}



ScrollView::ScrollView(WidgetBase* w , std::string classname , std::string objname) :
      WidgetBase(classname , objname),
      IScrollable(),
      our_widget(0)
{
   SetOurWidget(w);
}



void ScrollView::SetOurWidget(WidgetBase* widget) {
   if (our_widget) {
      our_widget->SetParent(0);
      our_widget = 0;
   }
   if (widget) {
      widget->SetParent(this);
      our_widget = widget;
      RepositionWidget();
   }
}


