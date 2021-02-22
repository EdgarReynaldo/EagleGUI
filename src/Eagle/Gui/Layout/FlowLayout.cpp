
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
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#include "Eagle/Gui/Layout/FlowLayout.hpp"



void FlowLayout::ReserveSlots(int nslots) {
   rcsizes.resize(nslots , BADRECTANGLE);
   LayoutBase::ReserveSlots(nslots);
}



void FlowLayout::OnAreaChanged() {
   RecalcFlow();
   RepositionAllChildren();
}



void FlowLayout::RecalcFlow() {
   std::vector<WidgetBase*> wc = wchildren;
   rcsizes.clear();
   rcsizes.resize(wc.size() , BADRECTANGLE);

   
   overflow = false;/// reset overflow warning
   
   
   /// There are really only two flows - horizontal and vertical. Every other position / combination of anchors and directions can be mirrored from the first two
   Rectangle r;

   int x1 = 0;InnerArea().X();
   int y1 = 0;InnerArea().Y();
   int x2 = 0;
   int y2 = 0;

   const int maxw = InnerArea().W();
   const int maxh = InnerArea().H();
   /// assume horizontal
   int rcmajorrem = maxw;
   int rcminorrem = maxh;
   /// accomodate vertical
   if (favored_direction == FLOW_FAVOR_VERTICAL) {
      rcmajorrem = maxh;
      rcminorrem = maxw;
   }
   
   unsigned int i = 0 , n = 0 , N = 0;
   int rcminorsize = 0;
   for (i = 0 ; i < wc.size() ; ) {/// For all widgets
      rcmajorrem = maxw;
      if (favored_direction == FLOW_FAVOR_VERTICAL) {
         rcmajorrem = maxh;
      }
      rcminorrem -= rcminorsize;
      
      for (n = i ; n < wc.size() ; ++n) {/// For as many widgets fit
         WidgetBase* w = wc[n];
         if (!w) {continue;}
         int major = w->PreferredWidth();
         int minor = w->PreferredHeight();
         if (favored_direction == FLOW_FAVOR_VERTICAL) {
            major = w->PreferredHeight();
            minor = w->PreferredWidth();
         }
         if (major > rcmajorrem || (n == (int)wc.size() - 1)) {
            /// Major flow position overflowed or reached end of widgets
            int NSTOP = n;
            if (n == (int)wc.size() - 1) {
               NSTOP = n + 1;
            }
            if (n == i) {
               /// The first one won't fit the major length . overflow
               overflow = true;
               i = wc.size();
               break;
            }
            for (N = i ; N < NSTOP ; ++N) {/// For each widget that fits
               w = wc[N];
               if (!w) {continue;}
               major = w->PreferredWidth();
               minor = w->PreferredHeight();
               if (favored_direction == FLOW_FAVOR_VERTICAL) {
                  major = w->PreferredHeight();
                  minor = w->PreferredWidth();
               }
               int width = (favored_direction == FLOW_FAVOR_HORIZONTAL)?major:rcminorsize;
               int height = (favored_direction == FLOW_FAVOR_HORIZONTAL)?rcminorsize:major;
               x2 = x1 + width - 1;
               y2 = y1 + height - 1;
               rcsizes[N].SetCorners(x1 , y1 , x2 , y2);
               if (favored_direction == FLOW_FAVOR_HORIZONTAL) {
                  x1 += width;
                  /// y1 doesn't change
               }
               else if (favored_direction == FLOW_FAVOR_VERTICAL) {
                  /// x1 doesn't change
                  y1 += height;
               }
            }
            if (favored_direction == FLOW_FAVOR_HORIZONTAL) {
               x1 = 0;
               y1 += rcminorsize;
            }
            else if (favored_direction == FLOW_FAVOR_VERTICAL) {
               x1 += rcminorsize;
               y1 = 0;
            }
            rcminorsize = 0;/// New row / column
            i = n;
            break;
         }
         else {/// major <= rcmajorrem
            rcmajorrem -= major;
            /// This one fits along the major axis
            if (minor > rcminorrem) {
               /// Minor flow position overflowed
               /// Can't try another row
               overflow = true;
               i = wc.size();
               break;
            }
            else {/// minor <= rcminorrem
               /// This one fits the minor axis too
               if (minor > rcminorsize) {rcminorsize = minor;}/// Get the max height of this row or width of this column
            }
         }
      }
   }
}



FlowLayout::FlowLayout() :
      LayoutBase("FlowLayout" , "FLOW"),
      anchor_pt(FLOW_ANCHOR_NW),
      favored_direction(FLOW_FAVOR_HORIZONTAL),
      rcsizes(),
      overflow(false),
      padding(0)
{
   attributes = LAYOUT_ALLOWS_RESIZE;
}



FlowLayout::~FlowLayout() {
   DetachFromGui();
   (void)0;
}



Rectangle FlowLayout::RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) {
   WidgetBase* w = GetWidget(widget_slot);
   if (!w) {return BADRECTANGLE;}
   (void)newx;
   (void)newy;
   (void)newwidth;
   (void)newheight;
   
   /// rcsizes is laid out to match wchildren, and is based on a generic case in the nw corner flowing right or down
   /// all other cases can be mirrored to match
   Rectangle r = rcsizes[widget_slot];
   Rectangle in = InnerArea();
   int x1 = in.X() + r.X();
   int y1 = in.Y() + r.Y();
   int x2 = in.X() + r.BRX();
   int y2 = in.Y() + r.BRY();
   switch (anchor_pt) {
      case FLOW_ANCHOR_NW :
         /// Generic case, do nothing
         r.SetCorners(x1,y1,x2,y2);
         break;
      case FLOW_ANCHOR_SW :
         /// Left to right along bottom - flip vertically
         r.SetCorners(x1 , in.BRY() - y1 , x2 , in.BRY() - y2);
         break;
      case FLOW_ANCHOR_NE :
         /// Right to left along top - flip horizontally
         r.SetCorners(in.BRX() - x1 , y1 , in.BRX() - x2 , y2);
         break;
      case FLOW_ANCHOR_SE :
         /// Right to left along bottom, double mirror
         r.SetCorners(in.BRX() - x1 , in.BRY() - y1 , in.BRX() - x2 , in.BRY() - y2);
         break;
   }
   
   return r;
}



void FlowLayout::PlaceWidget(WidgetBase* w , int slot) {
   LayoutBase::PlaceWidget(w , slot);
   RecalcFlow();
   RepositionAllChildren();
}



int FlowLayout::AddWidget(WidgetBase* w) {
   LayoutBase::AddWidget(w);
   RecalcFlow();
   RepositionAllChildren();
}


