
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






std::string PrintFlowFavoredDirection(FLOW_FAVORED_DIRECTION d) {
   static const char* dirs[2] = {
      "FLOW_FAVOR_HORIZONTAL",
      "FLOW_FAVOR_VERTICAL"
   };
   return dirs[d];
}



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
   rcsizes.clear();
   rcsizes.resize(wc.size() , BADRECTANGLE);/// Default to bad rectangle
   waspects.clear();/// For later maybe
   waspects.resize(wc.size() , -1.0);/// Default to -1, invalid
   

   overflow = false;/// reset overflow warning
   rowcount = 0;/// simple row count
   colcount.clear();/// For indexing
   rowheights.clear();/// Track how tall each row is
   rowspace.clear();/// Track how much space is used in each 'row'
   
   if (WChildren().empty()) {return;}

   /// There are really only two flows - horizontal and vertical. Every other position / combination of anchors and directions can be mirrored from the first two
   Rectangle r;

   const int maxw = InnerArea().W();
   const int maxh = InnerArea().H();
   const int colspacetotal = (favored_direction == FLOW_FAVOR_HORIZONTAL)?maxw:maxh;///How much space is available for columns (major axis)
   const int rowspacetotal = (favored_direction == FLOW_FAVOR_HORIZONTAL)?maxh:maxw;/// How much space is available for rows (minor axis)
   int rcmajorrem = colspacetotal;/// The main axis is thought of as a set of columns
   int rcminorrem = rowspacetotal;/// The minor axis is thought of as a set of rows
   
   /// Collect statistics on widget children, pack all widgets into upper left placing as many as possible on the current row
   
   colcount.push_back(0);
   rowheights.push_back(0);
   rowspace.push_back(colspacetotal);

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
         rowspace[rowcount] = rcmajorrem;
      }
      else if (major > rcmajorrem) {
         /// Overflowed the row
         /// First on row, don't wrap until next widget
         if (colcount[rowcount] == 0) {
            colcount[rowcount] = 1;
            colcount.push_back(0);
            rowspace[rowcount] -= major;
            rowspace.push_back(colspacetotal);
            rcmajorrem = colspacetotal;
            rcminorrem -= minor;
            rowheights[rowcount] = minor;
            *major1 = 0;
            (void)(*minor1);/// leave y alone
            ++rowcount;
            nextrow = true;
         }
         /// Wrap to next row
         else {
            rcmajorrem = colspacetotal - major;
            rowspace.push_back(rcmajorrem);
            rcminorrem -= rowheights[rowcount];
            colspace = rcminorrem;
            *major1 = 0;
            *minor1 += rowheights[rowcount];
            ++rowcount;
            rowheights.push_back(minor);
            colcount.push_back(1);
         }
      }
      else if (minor >= rcminorrem) {
         /// Overflowed the layout height remaining, keep going anyway, don't wrap to next row
         overflow = true;
         colcount[rowcount]++;
         if (rowheights[rowcount] < minor) {rowheights[rowcount] = minor;}
         rcmajorrem -= major;
         rowspace[rowcount] -= rcmajorrem;
      }
      else {/// Minor and major overflow
         /// Overflowed the layout completely, keep going anyway
         overflow = true;
         /// If widget will fit on the next row, place it there, otherwise tack it on the end and start a new row
         if (major < colspacetotal) {
            *major1 = 0;
            *minor1 += rowheights[rowcount];
            rcmajorrem = colspacetotal - major;
            rcminorrem -= rowheights[rowcount];/// This could be negative now
            colcount.push_back(1);
            rowheights.push_back(minor);
            rowspace.push_back(rcmajorrem);
         }
         else {
            /// Leave major1 and minor1 alone
            nextrow = true;
            if (rowheights[rowcount] < minor) {rowheights[rowcount] = minor;}
            rcmajorrem -= major;
            colcount[rowcount]++;
            colcount.push_back(0);
            rowspace[rowcount] = rcmajorrem;
            rowspace.push_back(colspacetotal);
         }
      }

      *major2 = *major1 + major;/// Pack as tightly as possible, spacing comes later
      *minor2 = *minor1 + minor;
      
      rcsizes[i].SetCorners(x1 , y1 , x2 , y2);
      
      *major1 += major;
      /// Flow to the next row for the next widget
      if (nextrow) {
         rcmajorrem = colspacetotal;
         *major1 = 0;
         int h = 0;
         if (rowheights.size() > 0) {
            h = rowheights.back();
         }
         *minor1 += h;
         rowheights.push_back(0);
      }


      if (overflow) {
         EagleWarn() << "Flow layout overflowed." << std::endl;
      }
   }
   
   colwidths.clear();/// Calculate at end from rowspace
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
   AdjustSpacing();
   MirrorFlow();
}


void FlowLayout::AdjustSpacing() {

   HALIGNMENT hal = halign;
   VALIGNMENT val = valign;
   /// Anchoring on the opposite side reverses alignment
   if (anchor == FBOX_ANCHOR_NE || anchor == FBOX_ANCHOR_SE) {
      hal = (halign == HALIGN_LEFT)?HALIGN_RIGHT:(halign == HALIGN_RIGHT)?HALIGN_LEFT:HALIGN_CENTER;
   }
   if (anchor == FBOX_ANCHOR_SW || anchor == FBOX_ANCHOR_SE) {
      val = (valign == VALIGN_TOP)?VALIGN_BOTTOM:(valign == VALIGN_BOTTOM)?VALIGN_TOP:VALIGN_CENTER;
   }

   for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
      WidgetBase* w = wchildren[i];
      if (!w) {continue;}
      Rectangle r2 = rcsizes[i];
      Rectangle* r = &rcsizes[i];
      
      
      int row = GetRow(i);
      int col = GetColumn(i);
      
      if (spacing == BOX_ALIGN_ONLY) {
         /// Handle major axis alignment
         if (favored_direction == FLOW_FAVOR_HORIZONTAL) {
            int ox = rowspace[row];
            if (hal == HALIGN_LEFT) {
               (void)0;/// Already aligned left
            }
            else if (hal == HALIGN_CENTER) {
               r->MoveBy(ox/2 , 0);
            }
            else if (hal == HALIGN_RIGHT) {
               r->MoveBy(ox , 0);
            }
         }
         else if (favored_direction == FLOW_FAVOR_VERTICAL) {
            int oy = rowspace[row];
            if (val == VALIGN_TOP) {
               (void)0;/// Already aligned top
            }
            else if (val == VALIGN_CENTER) {
               r->MoveBy(0 , oy/2);
            }
            else if (val == VALIGN_BOTTOM) {
               r->MoveBy(0 , oy);
            }
         }
      }
      else if (spacing == BOX_SPACE_BETWEEN) {
         int ncols = colcount[row] - 1;
         if (ncols > 0) {
            int spc = rowspace[row]/ncols;
            int spcleft = rowspace[row]%ncols;
            if (col == 0) {
               (void)0;
            }
            else if (col > 0) {
               if (favored_direction == FLOW_FAVOR_HORIZONTAL) {
                  r->MoveBy(col*spc + col*(col - 1 < spcleft)?1:0 , 0);
               }
               else if (favored_direction == FLOW_FAVOR_VERTICAL) {
                  r->MoveBy(0 , col*spc + col*(col - 1 < spcleft)?1:0);
               }
            }
         }
      }
      else if (spacing == BOX_SPACE_EVEN) {
         int ncols = colcount[row];
         ncols *= 2;
         int spc = rowspace[row]/ncols;
         int leftover = rowspace[row]%ncols;
         
         if (favored_direction == FLOW_FAVOR_HORIZONTAL) {
            if (ncols > 1) {
               r->MoveBy(spc + spc*(2*(col-1)) + (leftover > 2*(col - 1))?2:0 , 0);
            }
            else if (ncols == 1) {
               r->MoveBy((InnerArea().W() - w->PreferredWidth())/2 , 0);
            }
         }
         else if (favored_direction == FLOW_FAVOR_VERTICAL) {
            if (ncols > 1) {
               r->MoveBy(0 , spc + spc*2*(col-1) + (leftover > 2*(col-1))?2:0);
            }
            else if (ncols == 1) {
               r->MoveBy(0 , (InnerArea().H() - w->PreferredHeight())/2);
            }
         }
         else if ((spacing == BOX_EXPAND) || (overflow && shrink_on_overflow)) {
            Transform t = Eagle::EagleLibrary::System("Any")->GetSystemTransformer()->CreateTransform();
            t.Scale((double)InnerArea().W() / GetMaxColWidth() , (double)InnerArea().H() / GetTotalRowHeight() , 1.0);
            double x = r->X();
            double y = r->Y();
            double w = r->W();
            double h = r->H();
            t.ApplyTransformation(&x , &y , 0);
            t.ApplyTransformation(&w , &h , 0);
            r->SetArea((int)x , (int)y , (int)w , (int)h);
         }

         /// Handle minor axis alignment
         if (favored_direction == FLOW_FAVOR_HORIZONTAL) {
            /// Apply vertical alignment to rows
            if (val == VALIGN_TOP) {
               (void)0;/// Already aligned top
            }
            else if (val == VALIGN_CENTER) {
               r->MoveBy(0 , (rowheights[row] - r2.H())/2);
            }
            else if (val == VALIGN_BOTTOM) {
               r->MoveBy(0 , (rowheights[row] - r2.H()));
            }
         }
         else if (favored_direction == FLOW_FAVOR_VERTICAL) {
            /// Apply horizontal alignment to columns
            if (hal == HALIGN_LEFT) {
               (void)0;/// Already aligned left
            }
            else if (hal == HALIGN_CENTER) {
               r->MoveBy((rowheights[row] - r2.W())/2 , 0);
            }
            else if (hal == HALIGN_RIGHT) {
               r->MoveBy((rowheights[row] - r2.W()) , 0);
            }
         }
      }
   }
}



void FlowLayout::MirrorFlow() {
   
   for (unsigned int i = 0 ; i < wchildren.size() ; ++i) {
      WidgetBase* w = wchildren[i];
      if (!w) {continue;}
      Rectangle* r = &rcsizes[i];
      Rectangle r2 = rcsizes[i];
      int x1 = r2.X();
      int x2 = r2.BRX();
      int y1 = r2.Y();
      int y2 = r2.BRY();
      switch (anchor) {
      case FBOX_ANCHOR_NW :
         /// Generic case, do nothing
         r->SetCorners(x1,y1,x2,y2);
         break;
      case FBOX_ANCHOR_SW :
         /// Left to right along bottom - flip vertically
         r->SetCorners(x1 , InnerArea().H() - r2.Y() , x2 , InnerArea().H() - r2.BRY());
         break;
      case FBOX_ANCHOR_NE :
         /// Right to left along top - flip horizontally
         r->SetCorners(InnerArea().W() - r2.X() , y1 , InnerArea().W() - r2.BRX() , y2);
         break;
      case FBOX_ANCHOR_SE :
         /// Right to left along bottom, double mirror
         r->SetCorners(InnerArea().W() - r2.X() , InnerArea().H() - r2.Y() , InnerArea().W() - r2.BRX() , InnerArea().H() - r2.BRY());
         break;
      default :
         /// Default to NW
         r->SetCorners(x1,y1,x2,y2);
      }
   }
}




FlowLayout::FlowLayout(std::string classname , std::string objname) :
      BoxLayout(classname , objname),
      favored_direction(FLOW_FAVOR_HORIZONTAL),
      shrink_on_overflow(true),
      waspects(),
      rowcount(0),
      colcount(),
      rowheights(),
      colwidths(),
      rowspace(),
      colspace(0),
      defwidth(60),
      defheight(45)
{
   attributes = LAYOUT_ALLOWS_RESIZE;
   anchor = FBOX_ANCHOR_NW;
}



FlowLayout::~FlowLayout() {
   DetachFromGui();
   (void)0;
}



Rectangle FlowLayout::RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) {
   (void)newx;
   (void)newy;
   
   WidgetBase* w = GetWidget(widget_slot);
   if (!w) {return BADRECTANGLE;}
      
   if (newwidth != INT_MAX && newheight != INT_MAX) {
      if ((w->PreferredWidth() != newwidth) || (w->PreferredHeight() != newheight)) {
         w->SetPreferredSize(newwidth , newheight);
         RecalcFlow();
      }
   }
      
   Rectangle r = rcsizes[widget_slot];// make a copy, don't change rectangle with inner position
   r.MoveBy(InnerArea().X() , InnerArea().Y());
   return r;
}



Rectangle FlowLayout::RequestWidgetAreaOld(int widget_slot , int newx , int newy , int newwidth , int newheight) {
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
//**   

   HALIGNMENT hal = halign;
   VALIGNMENT val = valign;
   /// Anchoring on the opposite side reverses alignment
   if (anchor == FBOX_ANCHOR_NE || anchor == FBOX_ANCHOR_SE) {
      hal = (halign == HALIGN_LEFT)?HALIGN_RIGHT:(halign == HALIGN_RIGHT)?HALIGN_LEFT:HALIGN_CENTER;
   }
   if (anchor == FBOX_ANCHOR_SW || anchor == FBOX_ANCHOR_SE) {
      val = (valign == VALIGN_TOP)?VALIGN_BOTTOM:(valign == VALIGN_BOTTOM)?VALIGN_TOP:VALIGN_CENTER;
   }
   if (spacing == BOX_ALIGN_ONLY) {
      /// Handle major axis alignment
      if (favored_direction == FLOW_FAVOR_HORIZONTAL) {
         int ox = rowspace[row];
         if (hal == HALIGN_LEFT) {
            (void)0;/// Already aligned left
         }
         else if (hal == HALIGN_CENTER) {
            r.MoveBy(ox/2 , 0);
         }
         else if (hal == HALIGN_RIGHT) {
            r.MoveBy(ox , 0);
         }
      }
      else if (favored_direction == FLOW_FAVOR_VERTICAL) {
         int oy = rowspace[row];
         if (val == VALIGN_TOP) {
            (void)0;/// Already aligned top
         }
         else if (val == VALIGN_CENTER) {
            r.MoveBy(0 , oy/2);
         }
         else if (val == VALIGN_BOTTOM) {
            r.MoveBy(0 , oy);
         }
      }
   }
   else if (spacing == BOX_SPACE_BETWEEN) {
      /// Apply space between consecutive pairs of widgets
      nspacecolumns = colcount[row] - 1;
      if (nspacecolumns) {
         space_per_column = ((int)rspace[row])/nspacecolumns;
         space_left = ((int)rspace[row])%nspacecolumns;
         if (col == 0) {
            (void)0;/// This column is already pushed out from the middle as far as it will go
         }
         else if (col > 0) {
            if (favored_direction == FLOW_FAVOR_HORIZONTAL) {
               r.MoveBy(col*space_per_column + col*(col - 1 < space_left)?1:0 , 0);
            }
            else if (favored_direction == FLOW_FAVOR_VERTICAL) {
               r.MoveBy(0 , col*space_per_column + col*(col - 1 < space_left)?1:0);
            }
         }
      }
   }
   else if (spacing == BOX_SPACE_EVEN) {
      /// Apply space evenly around each widget
      nspacecolumns = colcount[row];
      space_per_column = ((int)rspace[row])/nspacecolumns;
      int extra = space_per_column - 2*(space_per_column/2);
      space_per_column -= extra;
      space_left = ((int)rspace[row])%nspacecolumns;
      space_left += extra;
      if (favored_direction == FLOW_FAVOR_HORIZONTAL) {
         r.MoveBy(space_per_column/2 + (col - 1)*space_per_column + col*(col < space_left)?1:0 , 0);
      }
      else if (favored_direction == FLOW_FAVOR_VERTICAL) {
         r.MoveBy(0 , space_per_column/2 + (col - 1)*space_per_column + col*(col < space_left)?1:0);
      }
   }
   else if ((spacing == BOX_EXPAND) || (overflow && shrink_on_overflow)) {
      /// Handle underflow and overflow (just scale to fit)
      t.Scale((double)in.W() / colwidth , (double)in.H() / totalrowheight , 1.0);
   }
   
   /// Handle minor axis alignment
   if (favored_direction == FLOW_FAVOR_HORIZONTAL) {
      /// Apply vertical alignment to rows
      if (val == VALIGN_TOP) {
         (void)0;/// Already aligned top
      }
      else if (val == VALIGN_CENTER) {
         r.MoveBy(0 , (rowheights[row] - r.H())/2);
      }
      else if (val == VALIGN_BOTTOM) {
         r.MoveBy(0 , (rowheights[row] - r.H()));
      }
   }
   else if (favored_direction == FLOW_FAVOR_VERTICAL) {
      /// Apply horizontal alignment to columns
      if (hal == HALIGN_LEFT) {
         (void)0;/// Already aligned left
      }
      else if (hal == HALIGN_CENTER) {
         r.MoveBy((rowheights[row] - r.W())/2 , 0);
      }
      else if (hal == HALIGN_RIGHT) {
         r.MoveBy((rowheights[row] - r.W()) , 0);
      }
   }
//*/
   double rx = r.X();
   double ry = r.Y();
   double rw = r.W();
   double rh = r.H();
   
//   t.ApplyTransformation(&rx , &ry , 0);
//   t.ApplyTransformation(&rw , &rh , 0);
   
   r = Rectangle(rx , ry , rw , rh);
   
   int x1 = in.X() + r.X();
   int y1 = in.Y() + r.Y();
   int x2 = in.X() + r.BRX();
   int y2 = in.Y() + r.BRY();
   switch (anchor) {
      case FBOX_ANCHOR_NW :
         /// Generic case, do nothing
         r.SetCorners(x1,y1,x2,y2);
         break;
      case FBOX_ANCHOR_SW :
         /// Left to right along bottom - flip vertically
         r.SetCorners(x1 , in.BRY() - r.Y() , x2 , in.BRY() - r.BRY());
         break;
      case FBOX_ANCHOR_NE :
         /// Right to left along top - flip horizontally
         r.SetCorners(in.BRX() - r.X() , y1 , in.BRX() - r.BRX() , y2);
         break;
      case FBOX_ANCHOR_SE :
         /// Right to left along bottom, double mirror
         r.SetCorners(in.BRX() - r.X() , in.BRY() - r.Y() , in.BRX() - r.BRX() , in.BRY() - r.BRY());
         break;
      default :
         /// Default to NW
         r.SetCorners(x1,y1,x2,y2);
   }
   
   return r;
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
   RepositionAllChildren();
   SetRedrawFlag();
}



void FlowLayout::SetAnchorPosition(BOX_ANCHOR_POINT p) {
   switch (p) {
   case FBOX_ANCHOR_NW:
   case FBOX_ANCHOR_NE:
   case FBOX_ANCHOR_SE:
   case FBOX_ANCHOR_SW:
      anchor = p;
      break;
   default :
      anchor = FBOX_ANCHOR_NW;
   }
   RecalcFlow();
   RepositionAllChildren();
   SetRedrawFlag();
}



void FlowLayout::SetFlowDirection(FLOW_FAVORED_DIRECTION d) {
   favored_direction = d;
   RecalcFlow();
   RepositionAllChildren();
   SetRedrawFlag();
}



int FlowLayout::WidthLeft() {
   return InnerArea().W() - GetMaxColWidth();
}
int FlowLayout::HeightLeft() {
   return InnerArea().H() - GetTotalRowHeight();
}



bool FlowLayout::WidgetWouldOverflowLayout(WidgetBase* w) {
   if (!w) {return false;}
   
   int pw = w->PreferredWidth();
   int ph = w->PreferredHeight();
   return (pw > WidthLeft() || ph > HeightLeft());
}



std::ostream& FlowLayout::DescribeTo(std::ostream& os , Indenter indent) const {
   os << indent << "BOX_SPACE_RULES = " << PrintBoxSpaceRule(spacing) << std::endl;
   os << indent << "BOX_ANCHOR_POINT = " << PrintBoxAnchorPoint(anchor) << std::endl;
   os << indent << "FLOW_FAVORED_DIRECTION = " << PrintFlowFavoredDirection(favored_direction) << std::endl;
   os << indent << "overflow = " << (overflow?"true":"false") << " , shrink = " << (shrink_on_overflow?"true":"false") << std::endl;
   for (unsigned int row = 0 ; row < colcount.size() ; ++row) {
      os << indent << "Colcount for row #" << row << " is " << colcount[row] << " and height is " << rowheights[row] << ". Space left is " << rowspace[row] << std::endl;
   }
   os << indent << "Colspace left = " << colspace << std::endl;
   return os;
}


