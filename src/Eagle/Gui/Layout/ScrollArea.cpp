
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
 * @file ScrollArea.cpp
 * @brief The implementation of the ScrollArea class
 */

#include "Eagle/Gui/Layout/ScrollArea.hpp"
#include "Eagle/GraphicsContext.hpp"
#include "Eagle/Gui/WidgetHandler.hpp"

#include "Eagle/StringWork.hpp"



void ScrollArea::ReserveSlots(int nslots) {
   nslots = 3;///< Always 3 slots, one for each scrollbar and one for the scroll view
   LayoutBase::ReserveSlots(nslots);
}



void ScrollArea::ResetScrollbars() {
   if (our_scroll_widget) {
      Rectangle our_area = GetViewRectangle(false , false);
      Rectangle their_area = our_scroll_widget->OuterArea();


      hfit = their_area.W() <= our_area.W();
      vfit = their_area.H() <= our_area.H();

      hmax = hfit?our_area.W():their_area.W();
      hmin = hfit?their_area.W():our_area.W();

      vmax = vfit?our_area.H():their_area.H();
      vmin = vfit?their_area.H():our_area.H();

      if (!vfit) {
         hfit = their_area.W() <= (our_area.W() - scrollbarsize);   
      }
      else if (!hfit) {
         vfit = their_area.H() <= (our_area.H() - scrollbarsize);
      }
      if (!vfit) {
         hmax = hfit?(our_area.W() - scrollbarsize):their_area.W();
         hmin = hfit?their_area.W():(our_area.W() - scrollbarsize);
      }
      if (!hfit) {
         vmax = vfit?(our_area.H() - scrollbarsize):their_area.H();
         vmin = vfit?their_area.H():(our_area.H() - scrollbarsize);
      }
      
      hscrollbar->SetupView(hmax , hmin);
      vscrollbar->SetupView(vmax , vmin);
      
      if (Flags().FlagOn(VISIBLE)) {
         /// Hide and disable the scrollbars
         hscrollbar->SetVisibleState(!hfit);
         vscrollbar->SetVisibleState(!vfit);
      }
      if (Flags().FlagOn(ENABLED)) {
         /// Hide and disable the scrollbars
         hscrollbar->SetVisibleState(!hfit);
         vscrollbar->SetVisibleState(!vfit);
      }
      
      hscrollbar->SetScrollPercent(0.0f);
      vscrollbar->SetScrollPercent(0.0f);
   
      RepositionAllChildren();
   }
}



Rectangle ScrollArea::GetViewRectangle(bool hbar , bool vbar) {
   Rectangle inner = InnerArea();
   if (hbar) {
      inner.SetDimensions(inner.W() , inner.H() - scrollbarsize);
      if (ontop) {
         inner.MoveBy(0 , scrollbarsize);
      }
   }
   if (vbar) {
      inner.SetDimensions(inner.W() - scrollbarsize , inner.H());
      if (onleft) {
         inner.MoveBy(scrollbarsize , 0);
      }
   }
   return inner;
}



Rectangle ScrollArea::GetViewRectangle() {
//   return GetViewRectangle(hscrollbar->Flags().FlagOn(VISIBLE) , vscrollbar->Flags().FlagOn(VISIBLE));
   return GetViewRectangle(!hfit , !vfit);
}



void ScrollArea::SetScrollBars(BasicScrollBar* horizontalscrollbar , BasicScrollBar* verticalscrollbar) {
   
   if (hscrollbar) {
      hscrollbar->SetParent(0);
      StopListeningTo(hscrollbar);
   }
   if (vscrollbar) {
      vscrollbar->SetParent(0);
      StopListeningTo(vscrollbar);
   }
   
   if (horizontalscrollbar == 0) {horizontalscrollbar = &basic_hscrollbar;}
   if (verticalscrollbar == 0) {verticalscrollbar = &basic_vscrollbar;}
   
   horizontalscrollbar->SetScrollDirection(true);
   verticalscrollbar->SetScrollDirection(false);
   
   hscrollbar = horizontalscrollbar;
   vscrollbar = verticalscrollbar;
   
   ListenTo(hscrollbar);
   ListenTo(vscrollbar);
   
   hscrollbar->SetParent(this);
   vscrollbar->SetParent(this);
   
   LayoutBase::PlaceWidget(hscrollbar , 0);
   LayoutBase::PlaceWidget(vscrollbar , 1);

   ResetScrollbars();
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
            int sx = hscrollbar->GetScrollValue();
            int sy = vscrollbar->GetScrollValue();
            our_scroll_view.SetScrollOffset(sx,sy);///< This will set the redraw flag on our widget
         }
      }
   }
}



int ScrollArea::PrivateHandleEvent(EagleEvent ee) {
   int ret = DIALOG_OKAY;
   if (ee.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN) {
      if (ee.mouse.button == 3) {
         Rectangle viewport = GetViewRectangle();
         if (viewport.Contains(ee.mouse.x , ee.mouse.y)) {
            drag = true;
            anchorx = ee.mouse.x;
            anchory = ee.mouse.y;
            anchorxscroll = hscrollbar->GetScrollValue();
            anchoryscroll = vscrollbar->GetScrollValue();
            ret |= DIALOG_INPUT_USED;
         }
      }
   }
   if (ee.type == EAGLE_EVENT_MOUSE_AXES) {
      if (drag) {
         int dx = ee.mouse.x - anchorx;
         int dy = ee.mouse.y - anchory;
         hscrollbar->SetScroll(anchorxscroll - dx);
         vscrollbar->SetScroll(anchoryscroll - dy);
         ret |= DIALOG_INPUT_USED;
      }
      else if (ee.mouse.dz || ee.mouse.dw) {
         hscrollbar->ScrollBy(ee.mouse.dw);
         vscrollbar->ScrollBy(ee.mouse.dz);
      }
   }
   if (ee.type == EAGLE_EVENT_MOUSE_BUTTON_UP) {
      if (ee.mouse.button == 3) {drag = false;}
   }
   return ret;
}



void ScrollArea::OnAreaChanged() {
   LayoutBase::OnAreaChanged();
   ResetScrollbars();
}



void ScrollArea::OnFlagChanged(WIDGET_FLAGS f , bool on) {
   /// If we have the focus, bring the scroll bars to the top
   LayoutBase::OnFlagChanged(f , on);
   if ((f & VISIBLE || f & ENABLED) && on) {
      ResetScrollbars();
   }
}



ScrollArea::ScrollArea(std::string name) :
      LayoutBase("ScrollArea" , name),
      EagleEventListener(),
      basic_hscrollbar("BasicScrollBar" , "basic_hscrollbar"),
      basic_vscrollbar("BasicScrollBar" , "basic_vscrollbar"),
      hscrollbar(0),
      vscrollbar(0),
      scrollbarsize(10),
      onleft(false),
      ontop(false),
      hfit(false),
      vfit(false),
      hmax(-1),
      hmin(-1),
      vmax(-1),
      vmin(-1),
      our_scroll_view(),
      our_scroll_widget(0),
      drag(false),
      anchorx(0),
      anchory(0),
      anchorxscroll(0),
      anchoryscroll(0)
{
   Resize(3);
   SetScrollBars((BasicScrollBar*)0 , (BasicScrollBar*)0);
   LayoutBase::PlaceWidget(&our_scroll_view , 2);
   ListenTo(&our_scroll_view);
   ResetScrollbars();
}



ScrollArea::~ScrollArea() {
   StopBroadcasting();
   StopListening();
   DetachFromGui();
   ClearWidgets();
}



void ScrollArea::SetScrollbarValues(int xscroll , int yscroll) {
   our_scroll_view.SetScrollOffset(xscroll , yscroll);
}



void ScrollArea::SetViewWidget(WidgetBase* widget_to_view) {
   our_scroll_widget = widget_to_view;/// shallow reference, do not destroy, handled by another layout
   our_scroll_view.SetOurWidget(our_scroll_widget);
   our_scroll_view.SetWidgetFlags(Flags());
}



Rectangle ScrollArea::RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) {

   /// We control the position of all widgets
   (void)newx;
   (void)newy;

   /// Slot 2 is reserved for the widget to view, give it the leftover room
   if (widget_slot == 2) {
      return GetViewRectangle();
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

      if (!hfit) {
         ww -= scrollbarsize;
         if (onleft) {
            wx += scrollbarsize;
         }
      }
      if (!ontop) {
         wy = InnerArea().BRY() - scrollbarsize;
      }
      return Rectangle(wx,wy,ww,wh);
   }
   /// Slot 1 is the vertical scroll bar
   if (widget_slot == 1) {
      int wx = InnerArea().X();
      int wy = InnerArea().Y();
      int ww = scrollbarsize;
      int wh = InnerArea().H();
      if (!vfit) {
         wh -= scrollbarsize;
         if (ontop) {
            wy += scrollbarsize;
         }
      }
      if (!onleft) {
         wx = InnerArea().BRX() - scrollbarsize;
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
   (void)slot;
   if (w == &basic_hscrollbar) {
      slot = 0;
      LayoutBase::PlaceWidget(w , slot);
   }
   else if (w == &basic_vscrollbar) {
      slot = 1;
      LayoutBase::PlaceWidget(w , slot);
   }
   else {
      slot = 2;
      our_scroll_widget = w;
      our_scroll_view.SetOurWidget(our_scroll_widget);
   }

   /// Our scroll max depends on the size of the widget being viewed
   if (w && (slot == 2)) {
      ResetScrollbars();
      /// Yes, it needs to be offset, because the scroll could be negative (to the left)
      int sx = hscrollbar->GetScrollValue();
      int sy = vscrollbar->GetScrollValue();
      our_scroll_view.SetScrollOffset(sx,sy);///< This will set the redraw flag on our widget
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







