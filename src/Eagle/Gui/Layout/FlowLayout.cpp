
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
#include "Eagle/Lib.hpp"
#include "Eagle/System.hpp"


int FlowLayout::GetMaxColWidth() {
   int max = 0;
   for (unsigned int i = 0 ; i < colwidths.size() ; ++i) {
      if (colwidths[i] > max) {max = colwidths[i];}
   }
   return max;
}



int FlowLayout::GetTotalRowHeight() {
   int h = 0;
   for (unsigned int i = 0 ; i < rowheights.size() ; ++i) {
      h += rowheights[i];
   }
   return h;
}



int FlowLayout::GetColumn(int index) {
   if (index < 0 || index >= (int)wchildren.size()) {
      return -1;
   }
   int col = index + 1;
   for (unsigned int i = 0 ; i < colcount.size() ; ++i) {
      if (colcount[i] < col) {
         col -= colcount[i];
      }
   }
   return col - 1;
}



int FlowLayout::GetRow(int index) {
   if (index < 0 || index >= (int)wchildren.size()) {
      return -1;
   }
   int row = 0;
   int col = index + 1;
   for (unsigned int i = 0 ; i < colcount.size() ; ++i) {
      if (colcount[i] < col) {
         col -= colcount[i];
         row++;
      }
   }
   return row;
}



int FlowLayout::GetWidgetIndex(int row , int col) {
   int index = 0;
   for (int i = 0 ; i < row ; ++i) {
      index += colcount[i];
   }
   index += col;
   if (index < 0 || index >= (int)wchildren.size()) {
      return -1;
   }
   return index;
}



void FlowLayout::ReserveSlots(int nslots) {
   rcsizes.resize(nslots , BADRECTANGLE);
   LayoutBase::ReserveSlots(nslots);
}



void FlowLayout::OnAreaChanged() {
   RecalcFlow();
   RepositionAllChildren();
}



void FlowLayout::RepositionAllChildren() {
   RecalcFlow();
   LayoutBase::RepositionAllChildren();
}



void FlowLayout::RepositionChild(int slot) {
   RecalcFlow();
   LayoutBase::RepositionChild(slot);
}



void FlowLayout::RecalcFlow() {
   std::vector<WidgetBase*> wc = wchildren;
   if (wchildren.empty()) {return;}
   rcsizes.clear();
   rcsizes.resize(wc.size() , BADRECTANGLE);
   waspects.clear();
   
   overflow = false;/// reset overflow warning
   rowcount = 0;
   colcount.clear();
   colwidths.clear();
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
   
   /// Collect statistics on widget children, pack all widgets into upper left placing as many as possible on the current row
   
   colcount.push_back(0);
//   colwidths.push_back(0);
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
   for (unsigned int i = 0 ; i < wc.size() ; ++i) {/// For all widgets
      nextrow = false;
      if (!wc[i]) {continue;}
      const int pw = wc[i]->PreferredWidth();
      const int ph = wc[i]->PreferredHeight();
      int mjr = (favored_direction == FLOW_FAVOR_HORIZONTAL)?pw:ph;
      int mnr = (favored_direction == FLOW_FAVOR_HORIZONTAL)?ph:pw;
      if (mjr && mnr) {
         waspects[i] = (double)mjr/mnr;
      }
      else {
         /// TODO At least one of the preferred sizes is 0. Do something with it. 
         if (!mjr) {mjr = (favored_direction == FLOW_FAVOR_HORIZONTAL)?defwidth:defheight;}
         if (!mnr) {mnr = (favored_direction == FLOW_FAVOR_HORIZONTAL)?defheight:defwidth;}
         waspects[i] = (double)mjr/mnr;
      }
      const int major = mjr;
      const int minor = mnr;
      /// Place widgets in rows based upon preferred width (for horizontal layouts)
      if ((major <= rcmajorrem) && (minor <= rcminorrem)) {
         /// This widget fits the major flow and the minor flow
         colcount[rowcount]++;
//         colwidths[rowcount] += major;
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
//         colwidths.push_back(major);
      }
      else if (minor >= rcminorrem) {
         /// Overflowed the layout height remaining, keep going anyway, don't wrap to next row
         overflow = true;
         colcount[rowcount]++;
//         colwidths[rowcount] += major;
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
//            colwidths.push_back(major);
         }
         else {
            /// Leave major1 and minor1 alone
            nextrow = true;
            if (rowheights[rowcount] < minor) {rowheights[rowcount] = minor;}
//            colwidths[rowcount] += major;
//            colwidths.push_back(0);
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
         rowheights.push_back(minor);
      }


      if (overflow) {
         EagleWarn() << "Flow layout overflowed." << std::endl;
      }
   }
   
   for (unsigned int i = 0 ; i < rowspace.size() ; ++i) {
      colwidths.push_back(colspacetotal - rowspace[i]);
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
      colspace(0),
      defwidth(40),
      defheight(30)
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
   if (WChildren().empty()) {return BADRECTANGLE;}
   (void)newx;
   (void)newy;
   (void)newwidth;
   (void)newheight;
   
   int row = GetRow(widget_slot);
   int col = GetColumn(widget_slot);
   
   
   /// rcsizes is laid out to match wchildren, and is based on a generic case in the nw corner flowing right or down
   /// all other cases can be mirrored to match
   /// First add spacing and alignment to rcsizes
   Rectangle r = rcsizes[widget_slot];

   Transform t = Eagle::EagleLibrary::System("Any")->GetSystemTransformer()->CreateTransform();

   Rectangle in = InnerArea();
   
   double colwidth = GetMaxColWidth();
   EAGLE_ASSERT(colwidth);
   double totalrowheight = GetTotalRowHeight();
   EAGLE_ASSERT(totalrowheight);
   
   std::vector<double> rheights(rowheights.size() , 0.0);
   std::vector<double> rspace(rowspace.size() , 0.0);
   
   /// Use double precision for scaling, and truncate to ints later
   for (unsigned int i = 0 ; i < rheights.size() ; ++i) {
      rheights[i] = rowheights[i];
   }
   for (unsigned int i = 0 ; i < rspace.size() ; ++i) {
      rspace[i] = rowspace[i];
   }

   int nspacecolumns = 0;
   int space_per_column = 0;
   int space_left = 0;
   
   if (size_rules == BOX_ALIGN_ONLY) {
      /// Handle major axis alignment
      if (favored_direction == FLOW_FAVOR_HORIZONTAL) {
         int ox = rowspace[row];
         if (halign == HALIGN_LEFT) {
            (void)0;/// Already aligned left
         }
         else if (halign == HALIGN_CENTER) {
            r.MoveBy(ox/2 , 0);
         }
         else if (halign == HALIGN_RIGHT) {
            r.MoveBy(ox , 0);
         }
      }
      else if (favored_direction == FLOW_FAVOR_VERTICAL) {
         int oy = rowspace[row];
         if (valign == VALIGN_TOP) {
            (void)0;/// Already aligned top
         }
         else if (valign == VALIGN_CENTER) {
            r.MoveBy(0 , oy/2);
         }
         else if (valign == VALIGN_BOTTOM) {
            r.MoveBy(0 , oy);
         }
      }
   }
   else if (size_rules == BOX_SPACE_BETWEEN) {
      /// Apply space between consecutive pairs of widgets
      nspacecolumns = colcount[row] - 1;
      space_per_column = ((int)rspace[row])/nspacecolumns;
      space_left = ((int)rspace[row])%nspacecolumns;
      if (col == 0) {
         (void)0;/// This column is already pushed out from the middle as far as it will go
      }
      else if (col > 0) {
         if (favored_direction == FLOW_FAVOR_HORIZONTAL) {
            r.MoveBy(col*space_per_column + (col - 1 < space_left)?1:0 , 0);
         }
         else if (favored_direction == FLOW_FAVOR_VERTICAL) {
            r.MoveBy(0 , col*space_per_column + (col - 1 < space_left)?1:0);
         }
      }
   }
   else if (size_rules == BOX_SPACE_EVEN) {
      /// Apply space evenly around each widget
      nspacecolumns = colcount[row];
      space_per_column = ((int)rspace[row])/nspacecolumns;
      int extra = space_per_column - 2*(space_per_column/2);
      space_per_column -= extra;
      space_left = ((int)rspace[row])%nspacecolumns;
      space_left += extra;
      if (favored_direction == FLOW_FAVOR_HORIZONTAL) {
         r.MoveBy(space_left/2 + (col - 1)*space_per_column , 0);
      }
      else if (favored_direction == FLOW_FAVOR_VERTICAL) {
         r.MoveBy(0 , space_left/2 + (col - 1)*space_per_column);
      }
   }
   else if ((size_rules == BOX_EXPAND) || (overflow && shrink_on_overflow)) {
      /// Handle underflow and overflow (just scale to fit)
      t.Scale((double)in.W() / colwidth , (double)in.H() / totalrowheight , 1.0);
   }
   
   /// Handle minor axis alignment
   if (favored_direction == FLOW_FAVOR_HORIZONTAL) {
      /// Apply vertical alignment to rows
      if (valign == VALIGN_TOP) {
         (void)0;/// Already aligned top
      }
      else if (valign == VALIGN_CENTER) {
         r.MoveBy(0 , (rowheights[row] - r.H())/2);
      }
      else if (valign == VALIGN_BOTTOM) {
         r.MoveBy(0 , (rowheights[row] - r.H()));
      }
   }
   else if (favored_direction == FLOW_FAVOR_VERTICAL) {
      /// Apply horizontal alignment to columns
      if (halign == HALIGN_LEFT) {
         (void)0;/// Already aligned left
      }
      else if (halign == HALIGN_CENTER) {
         r.MoveBy((rowheights[row] - r.W())/2 , 0);
      }
      else if (halign == HALIGN_RIGHT) {
         r.MoveBy((rowheights[row] - r.W()) , 0);
      }
   }
   
   double rx = r.X();
   double ry = r.Y();
   double rw = r.W();
   double rh = r.H();
   
   t.ApplyTransformation(&rx , &ry , 0);
   t.ApplyTransformation(&rw , &rh , 0);
   
   r = Rectangle(rx , ry , rw , rh);
   
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
   RepositionAllChildren();
   SetRedrawFlag();
}



int FlowLayout::AddWidget(WidgetBase* w) {
   int ret = LayoutBase::AddWidget(w);
   RepositionAllChildren();
   SetRedrawFlag();
   return ret;
}



void FlowLayout::InsertWidget(WidgetBase* w , int slot_before) {
   LayoutBase::InsertWidget(w , slot_before);
   RepositionAllChildren();
   SetRedrawFlag();
}



void FlowLayout::ShrinkOnOverflow(bool shrink) {
   if (shrink != shrink_on_overflow) {
      shrink_on_overflow = shrink;
      RepositionAllChildren();
      SetRedrawFlag();
   }
}



void FlowLayout::SetDefaultWidth(unsigned int w) {
   if (w < 1) {w = 1;}
   defwidth = w;
   RepositionAllChildren();
   SetRedrawFlag();
}



void FlowLayout::SetDefaultHeight(unsigned int h) {
   if (h < 1) {h = 1;}
   defheight = h;
   RepositionAllChildren();
   SetRedrawFlag();
}



void FlowLayout::SetDefaultSize(unsigned int w , unsigned int h) {
   if (w < 1) {w = 1;}
   if (h < 1) {h = 1;}
   defwidth = w;
   defheight = h;
}



void FlowLayout::SetAlignment(HALIGNMENT h_align , VALIGNMENT v_align) {
   LayoutBase::SetAlignment(h_align , v_align);
   RepositionAllChildren();
   SetRedrawFlag();
}



void FlowLayout::SetFlowAnchor(FLOW_ANCHOR_POINT p) {
   anchor_pt = p;
   RepositionAllChildren();
   SetRedrawFlag();
}



void FlowLayout::SetFlowDirection(FLOW_FAVORED_DIRECTION d) {
   favored_direction = d;
   RepositionAllChildren();
   SetRedrawFlag();
}








