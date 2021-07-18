
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
   waspects.clear();
   
   overflow = false;/// reset overflow warning
   rowcount = 0;
   colcount.clear();
   rowheights.clear();
   rowspace.clear();
   
   /// There are really only two flows - horizontal and vertical. Every other position / combination of anchors and directions can be mirrored from the first two
   Rectangle r;

   const int maxw = InnerArea().W();
   const int maxh = InnerArea().H();
   const int colspacetotal = (favored_direction == FLOW_FAVOR_HORIZONTAL)?maxw:maxh;
   const int rowspacetotal = (favored_direction == FLOW_FAVOR_HORIZONTAL)?maxh:maxw;
   /// assume major flow is horizontal
   int rcmajorrem = maxw;
   int rcminorrem = maxh;
   /// accomodate vertical flow
   if (favored_direction == FLOW_FAVOR_VERTICAL) {
      rcmajorrem = maxh;
      rcminorrem = maxw;
   }
   
   unsigned int i = 0 , n = 0 , N = 0;
   int rcminorsize = 0;
   WidgetBase* first_widget = 0;
   WidgetBase* last_widget = 0;
   for (i = 0 ; i < wc.size() ; ++i) {
      if (wc[i]) {
         first_widget = wc[i];
         break;
      }
   }
   for (i = wc.size() ; i > 0 ; --i) {
      if (wc[(int)i - 1]) {
         last_widget = wc[(int)i - 1];
         break;
      }
   }
   if (!first_widget) {
      return;
   }
   
   /// Collect statistics on widget children, pack all widgets into upper left placing as many as possible on the current row
   
///   std::vector<WidgetBase*> wc = wchildren;
   colcount.push_back(0);
   rowheights.push_back(0);
   rowspace.push_back(rowspacetotal);
   waspects.resize(wc.size() , -1.0);
   int x1 = 0;
   int y1 = 0;
   int x2 = 0;
   int y2 = 0;
   int* const major1 = (favored_direction == FLOW_FAVOR_HORIZONTAL)?&x1:&y1;
   int* const minor1 = (favored_direction == FLOW_FAVOR_HORIZONTAL)?&y1:&x1;
   int* const major2 = (favored_direction == FLOW_FAVOR_HORIZONTAL)?&x2:&y2;
   int* const minor2 = (favored_direction == FLOW_FAVOR_HORIZONTAL)?&y2:&x2;
   bool nextrow = false;
   for (i = 0 ; i < wc.size() ; ++i) {/// For all widgets
      nextrow = false;
      if (!wc[i]) {continue;}
      const int pw = wc[i]->PreferredWidth();
      const int ph = wc[i]->PreferredHeight();
      const int major = (favored_direction == FLOW_FAVOR_HORIZONTAL)?pw:ph;
      const int minor = (favored_direction == FLOW_FAVOR_HORIZONTAL)?ph:pw;
      if (major && minor) {
         waspects[i] = (double)major/minor;
      }
      else {
         /// TODO At least one of the preferred sizes is 0. Do something with it. 
      }
      /// Place widgets in rows based upon preferred width (for horizontal layouts)
      if ((major <= rcmajorrem) && (minor <= rcminorrem)) {
         /// This widget fits the major flow and the minor flow
         colcount[rowcount]++;
         
         /// Calculate max height for each row
         if (rowheights[rowcount] < minor) {rowheights[rowcount] = minor;}
         
         rcmajorrem -= major;
         rowspace[rowcount] -= major;
      }
      else if (major > rcmajorrem) {
         /// Overflowed the row, wrap to next
         rcmajorrem = colspacetotal - major;
         rcminorrem -= rowheights[rowcount];
         colspace -= rowheights[rowcount];
         *major1 = 0;
         *minor1 += rowheights[rowcount];
         ++rowcount;
         rowheights.push_back(minor);
         colcount.push_back(1);
      }
      else if (minor >= rcminorrem) {
         /// Overflowed the layout height remaining, keep going anyway, don't wrap to next row
         overflow = true;
         colcount[rowcount]++;
         if (rowheights[rowcount] < minor) {rowheights[rowcount] = minor;}
         rcmajorrem -= major;
         rowspace[rowcount] -= major;/// Could be negative now, since it overflowed to the right (for horizontal layouts)
      }
      else {/// Minor and major overflow
         /// Overflowed the layout completely, keep going anyway
         overflow = true;
         if (rowheights[rowcount] < minor) {rowheights[rowcount] = minor;}
         /// If widget will fit on the next row, place it there, otherwise tack it on the end and start a new row
         if (major < colspacetotal) {
            *major1 = 0;
            *minor1 += rowheights[rowcount];
            rcminorrem -= rowheights[rowcount];/// This could be negative now
            rowheights.push_back(minor);
         }
         else {
            /// Leave major1 and minor1 alone
            nextrow = true;
            rowheights.push_back(0);
         }
         rcmajorrem = colspacetotal;
         colspace -= rowheights[rowcount];/// This could be negative now too
         ++rowcount;
         colcount.push_back(1);
      }

      *major2 = *major1 + major;/// Pack as tightly as possible, spacing comes later
      *minor2 = *minor1 + minor;
      
      rcsizes[i].SetCorners(x1 , y1 , x2 , y2);
      
      *major1 += major;
      if (nextrow) {
         rcmajorrem = rowspacetotal;
         *major1 = 0;
         int h = 0;
         if (rowheights.size() > 0) {
            h = rowheights.back();
         }
         *minor1 += h;
      }


      if (overflow) {
         EagleWarn() << "Flow layout overflowed." << std::endl;
      }
   }
   int pw = 0;
   int ph = 0;
   int ox = 0;
   for (unsigned int y = 0 ; y < rowheights.size() ; ++y) {
      int w = 0;
      for (int x = 0 ; x < colcount[y] ; ++x) {
         w += rcsizes[x + ox].W();
      }
      if (pw < w) {pw = w;}
      ox += colcount[y];
      ph += rowheights[y];
   }
   if (pw != prefw || ph != prefh) {
      SetPreferredSize(prefw , prefh);/// This may trigger another call to CalcFlow, so only do it if our preferred size hasn't changed
   }
}


FlowLayout::FlowLayout(std::string classname , std::string objname) :
      LayoutBase(classname , objname),
      size_rules(BOX_SPACE_EVEN),
      anchor_pt(FLOW_ANCHOR_NW),
      favored_direction(FLOW_FAVOR_HORIZONTAL),
      overflow(false),
      shrink_on_overflow(true),
      rcsizes(),
      waspects(),
      rowcount(0),
      colcount(),
      rowheights(),
      colwidths(),
      rowspace(),
      colspace(0)
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
   /// First add spacing and alignment to rcsizes
   Rectangle r = rcsizes[widget_slot];



   
   
//   Transform t = Eagle::EagleLibrary::System("Any")->GetSystemTransformer()->CreateTransform();/// Identity

   Rectangle in = InnerArea();
   
   if (overflow && shrink_on_overflow) {
         
   }
   
   
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
   int ret = LayoutBase::AddWidget(w);
   RecalcFlow();
   RepositionAllChildren();
   return ret;
}



void FlowLayout::InsertWidget(WidgetBase* w , int slot_before) {
   LayoutBase::InsertWidget(w , slot_before);
   RecalcFlow();
   RepositionAllChildren();
}



void FlowLayout::ShrinkOnOverflow(bool shrink) {
   if (shrink != shrink_on_overflow) {
      shrink_on_overflow = shrink;
      RecalcFlow();
   }
}


