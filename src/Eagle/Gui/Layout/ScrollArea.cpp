
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
 * @file ScrollArea.cpp
 * @brief The implementation of the ScrollArea class
 */

#include "Eagle/Gui/Layout/ScrollArea.hpp"
#include "Eagle/GraphicsContext.hpp"


void ScrollArea::ReserveSlots(int nslots) {
   nslots = 3;///< Always 3 slots
   LayoutBase::ReserveSlots(nslots);
}



void ScrollArea::ResetScrollbars() {
   if (our_scroll_widget) {
      /// Determine which scroll bars are visible
      Rectangle our_area = InnerArea();
      Rectangle their_area = our_scroll_widget->OuterArea();
      if (their_area.H() > our_area.H()) {
         /// Need to show the vertical scrollbar
         vscrollbar->SetEnabledState(true);
         vscrollbar->SetVisibleState(true);
         /// The vscrollbar takes up horizontal room
         if (their_area.W() > (our_area.W() - scrollbarsize)) {
            /// Need to show the horizontal scrollbar too
            hscrollbar->SetEnabledState(true);
            hscrollbar->SetVisibleState(true);
         }
      }
      else if (their_area.W() > our_area.W()) {
         /// Need to show the horizontal scrollbar
         hscrollbar->SetEnabledState(true);
         hscrollbar->SetVisibleState(true);
         /// The hscrollbar takes up vertical room
         if (their_area.H() > our_area.H() - scrollbarsize) {
            vscrollbar->SetEnabledState(true);
            vscrollbar->SetVisibleState(true);
         }
      }
   }
   else {
      /// Hide and disable the scrollbars
      hscrollbar->SetVisibleState(false);
      vscrollbar->SetVisibleState(false);
      hscrollbar->SetEnabledState(false);
      vscrollbar->SetEnabledState(false);
   }
}



Rectangle ScrollArea::GetViewRectangle() {
   Rectangle inner = InnerArea();
   if (hscrollbar->FlagOn(VISIBLE)) {
      inner.SetDimensions(inner.W() , inner.H() - scrollbarsize);
      if (ontop) {
         inner.MoveBy(0 , scrollbarsize);
      }
   }
   if (vscrollbar->FlagOn(VISIBLE)) {
      inner.SetDimensions(inner.W() - scrollbarsize , inner.H());
      if (onleft) {
         inner.MoveBy(scrollbarsize , 0);
      }
   }
   return inner;
}



void ScrollArea::SetScrollBars(BasicScrollBar* horizontalscrollbar , BasicScrollBar* verticalscrollbar) {
   
   if (hscrollbar) {StopListeningTo(hscrollbar);}
   if (vscrollbar) {StopListeningTo(vscrollbar);}
   
   if (horizontalscrollbar == 0) {horizontalscrollbar = &basic_hscrollbar;}
   if (verticalscrollbar == 0) {verticalscrollbar = &basic_vscrollbar;}
   
   horizontalscrollbar->SetScrollDirection(true);
   verticalscrollbar->SetScrollDirection(false);
   
   hscrollbar = horizontalscrollbar;
   vscrollbar = verticalscrollbar;
   
   ListenTo(hscrollbar);
   ListenTo(vscrollbar);
   
   ResetScrollbars();
   
   LayoutBase::PlaceWidget(hscrollbar , 0);
   LayoutBase::PlaceWidget(vscrollbar , 1);
   
   RepositionChild(2);
}



void ScrollArea::SetScrollbarPosition(bool on_top , bool on_left) {
   ontop = on_top;
   onleft = on_left;
   RepositionAllChildren();
}



void ScrollArea::SetHScrollbarPosition(bool on_top) {
   ontop = on_top;
   RepositionAllChildren();
}



void ScrollArea::SetVSCrollbarPosition(bool on_left) {
   onleft = on_left;
   RepositionAllChildren();
}



void ScrollArea::RespondToEvent(EagleEvent e , EagleThread* thread) {
   (void)thread;
   if (e.type == EAGLE_EVENT_WIDGET) {
      if (e.widget.from == hscrollbar || e.widget.from == vscrollbar) {
         if (our_scroll_widget) {
            /// Do we need to offset this? A widget is centered, and then scroll applies
            /// Yes, it needs to be offset, because the scroll could be negative (to the left)
            int sx = hscrollbar->GetScrollX() - hscrollbar->GetScrollMax()/2;
            int sy = vscrollbar->GetScrollY() - vscrollbar->GetScrollMax()/2;
            our_scroll_widget->SetScrollOffset(sx,sy);///< This will set the redraw flag on our widget
         }
      }
   }
}



Rectangle ScrollArea::RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) {

   /// We control the position of all widgets
   (void)newx;
   (void)newy;

   /// Slot 2 is reserved for the widget to view, give it the leftover room
   if (widget_slot == 2) {
      /// The widget in view is always centered
      int wx = InnerArea().CX();
      int wy = InnerArea().CY();
      int ww = 0;
      int wh = 0;

      WidgetBase* w = GetWidget(2);

      if (!w) {
         return BADRECTANGLE;
      }
   
      /// Find out how much room is left for the scrollbars and where they are at
      ww = w->InnerArea().W();
      wh = w->InnerArea().H();
      if (widget_slot == 2) {
         if (newwidth != INT_MAX) {
            ww = newwidth;
         }
         if (newheight != INT_MAX) {
            wh = newheight;
         }
      }
      if (hscrollbar->FlagOn(VISIBLE)) {
         if (onleft) {
            wx += scrollbarsize;
         }
      }
      if (vscrollbar->FlagOn(VISIBLE)) {
         if (ontop) {
            wy += scrollbarsize;
         }
      }
      /// We always center the widget, it will handle its scroll position by itself
      wx -= (ww/2);
      wy -= (wh/2);
      return Rectangle(wx,wy,ww,wh);
   }
   
   /// Slots 0 and 1 are reserved for the scrollbars, and they can take up space used for the widget
   if (widget_slot == 0 || widget_slot == 1) {
      /// We handle the sizing and positioning of our scroll bars
      (void)newx;
      (void)newy;
      (void)newwidth;
      (void)newheight;
   }
   
   /// Slot 0 is the horizontal scroll bar
   if (widget_slot == 0) {
      int wx = InnerArea().X();
      int wy = InnerArea().Y();
      int ww = InnerArea().W();
      int wh = scrollbarsize;
      if (vscrollbar->FlagOn(VISIBLE)) {
         ww -= scrollbarsize;
      }
      if (!onleft) {
         wx = InnerArea().BRX() - scrollbarsize;
      }
      return Rectangle(wx,wy,ww,wh);
   }
   /// Slot 1 is the vertical scroll bar
   if (widget_slot == 1) {
      int wx = InnerArea().X();
      int wy = InnerArea().Y();
      int ww = scrollbarsize;
      int wh = InnerArea().H();
      if (hscrollbar->FlagOn(VISIBLE)) {
         wh -= scrollbarsize;
      }
      if (!ontop) {
         wy = InnerArea().BRY() - scrollbarsize;
      }
      return Rectangle(wx,wy,ww,wh);
   }
   return BADRECTANGLE;
}



void ScrollArea::Resize(unsigned int nsize) {
   nsize = 3;///< Always 3
   LayoutBase::Resize(nsize);
}



void ScrollArea::PlaceWidget(WidgetBase* w , int slot) {
   if (w == hscrollbar) {
      slot = 0;
   }
   else if (w == vscrollbar) {
      slot = 1;
   }
   else {
      slot = 2;
      our_scroll_widget = w;
   }
   LayoutBase::PlaceWidget(w , slot);

   /// Our scroll max depends on the size of the widget being viewed
   if (w && (slot == 2)) {
      Rectangle wrect = w->OuterArea();
      Rectangle view = GetViewRectangle();

      /// The total view is the larger of the viewed widget size and our view rectangle
      /// The view in length is the smaller of the viewed widget and our view rectangle
      
      bool hfit = wrect.W() <= view.W();
      bool vfit = wrect.H() <= view.H();
      
      int hmax = hfit?view.W():wrect.W();
      int hmin = hfit?wrect.W():view.W();

      int vmax = vfit?view.H():wrect.H();
      int vmin = vfit?wrect.H():view.H();
      
      hscrollbar->SetupView(hmax , hmin);
      vscrollbar->SetupView(vmax , vmin);
      hscrollbar->SetScrollPercent(0.5f);
      vscrollbar->SetScrollPercent(0.5f);
   }
}



int ScrollArea::AddWidget(WidgetBase* w) {
   PlaceWidget(w , 2);
   return 2;
}



void ScrollArea::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   Rectangle inner = InnerArea().MovedBy(Pos2I(xpos,ypos));
   win->DrawFilledRectangle(inner , GetColor(BGCOL));
}







