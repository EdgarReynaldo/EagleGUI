
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



int FlowLayout::GetMaxColWidth() const {
   int max = 0;
   for (unsigned int i = 0 ; i < colwidths.size() ; ++i) {
      if (colwidths[i] > max) {max = colwidths[i];}
   }
   return max;
}



int FlowLayout::GetTotalRowHeight() const {
   int h = 0;
   for (unsigned int i = 0 ; i < rowheights.size() ; ++i) {
      h += rowheights[i];
   }
   return h;
}



int FlowLayout::GetColumn(int index) const {/// 0 based
   if (index < 0 || index >= (int)wchildren.size()) {
      return -1;
   }
   int col = 0;
   int row = 0;
   WidgetBase* w = wchildren[index];
   for (unsigned int i = 0 ; i < WChildren().size() ; ++i) {
      WidgetBase* compare = WChildren()[i];
      if (w == compare) {
         return col;
      }
      else {
         col++;
      }
      if (col >= colcount[row]) {
         col = 0;
         row++;
      }
   }
   return -1;
}



int FlowLayout::GetRow(int index) const {/// 0 based
   if (index < 0 || index >= (int)wchildren.size()) {
      return -1;
   }
   int col = 0;
   int row = 0;
   WidgetBase* w = wchildren[index];
   for (unsigned int i = 0 ; i < WChildren().size() ; ++i) {
      WidgetBase* compare = WChildren()[i];
      if (w == compare) {
         return row;
      }
      else {
         col++;
      }
      if (col >= colcount[row]) {
         col = 0;
         row++;
      }
   }
   return -1;
}



int FlowLayout::GetWidgetIndex(int row , int col) const {/// 0,0 based
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
   rowspace.clear();/// Track how much space is unused in each 'row'
   colwidths.clear();
   
   if (WChildren().empty()) {return;}

   /// There are really only two flows - horizontal and vertical. Every other position / combination of anchors and directions can be mirrored from the first two

   const int majormax = (favored_direction == FLOW_FAVOR_HORIZONTAL)?InnerArea().W():InnerArea().H();
   const int minormax = (favored_direction == FLOW_FAVOR_HORIZONTAL)?InnerArea().H():InnerArea().W();
   int majorrem = majormax;
   int minorrem = minormax;

   colspace = minormax;/// How much minor axis space there is
   colcount.push_back(0);/// Count of columns on this row
   rowspace.push_back(majormax);/// Space remaining on the major axis on this 'row'
   rowheights.push_back(0);/// How tall this row is
   colwidths.push_back(0);
   
   /// Coordinates of the new rectangle
   int x1 = 0;
   int y1 = 0;
   int x2 = 0;
   int y2 = 0;

   /// Setup some pointers to make it easier to pick the correct axes
   int* const major1 = (favored_direction == FLOW_FAVOR_HORIZONTAL)?&x1:&y1;
   int* const minor1 = (favored_direction == FLOW_FAVOR_HORIZONTAL)?&y1:&x1;
   int* const major2 = (favored_direction == FLOW_FAVOR_HORIZONTAL)?&x2:&y2;
   int* const minor2 = (favored_direction == FLOW_FAVOR_HORIZONTAL)?&y2:&x2;

   bool nextrow = false;

///   WidgetBase* last = WChildren().back();
   
   for (unsigned int i = 0 ; i < wc.size() ; ++i) {/// For all widgets
      nextrow = false;
      if (!wc[i]) {continue;}
      WidgetBase* w = wc[i];
      int mjr = (favored_direction == FLOW_FAVOR_HORIZONTAL)?w->PreferredWidth():w->PreferredHeight();
      int mnr = (favored_direction == FLOW_FAVOR_HORIZONTAL)?w->PreferredHeight():w->PreferredWidth();
      if (mjr && mnr) {
         waspects[i] = (double)mjr/mnr;
      }
      else {
         /// TODO At least one of the preferred sizes is 0. Do something with it. 
         if (mjr < 1) {
            mjr = (favored_direction == FLOW_FAVOR_HORIZONTAL)?defwidth:defheight;
         }
         if (mnr < 1) {
            mnr = (favored_direction == FLOW_FAVOR_HORIZONTAL)?defheight:defwidth;
         }
         waspects[i] = (double)mjr/mnr;
      }
      const int major = mjr;
      const int minor = mnr;

      /// Place widgets in rows based upon preferred width and remaining width available

      Rectangle* r = &rcsizes[i];/// reference

      /// Determine whether the widget goes on this row or starts the next
      nextrow = false;
      if (major <= majorrem) {
         /// Place on this row, there is enough space
      }
      else {
         /// Flow to next row if not first in row, there is not enough major axis space
         if (colcount[rowcount] > 0) {
            nextrow = true;
         }
         else {
            overflow = true;/// Overflowed major axis
         }
      }

      /// Pre processing
      if (!nextrow) {
         /// Place on this row
         if (rowheights[rowcount] < minor) {
            rowheights[rowcount] = minor;
         }
      }
      else {
         /// Place on next row
         *major1 = 0;
         *minor1 += rowheights[rowcount];
         colcount.push_back(0);
      }
      
      /// Add width and height to x2 and y2
      *major2 = *major1 + major;
      *minor2 = *minor1 + minor;
      
      /// Set our rectangle
      r->SetCorners(x1 , y1 , x2 , y2);
      
      if (!nextrow) {
         /// Advance to next column
         *major1 += major;/// Advance along row
         (void)(*minor1);/// Stay on same minor position
         majorrem -= major;
         rowspace[rowcount] = majorrem;
         colwidths[rowcount] += major;
         colcount[rowcount]++;
      }
      else {
         /// Advance to next row
         minorrem -= rowheights[rowcount];
         colspace = minorrem;
         ++rowcount;
         colcount[rowcount] = 1;
         majorrem = majormax - major;
         rowspace.push_back(majorrem);
         rowheights.push_back(minor);
         colwidths.push_back(major);
         *major1 += major;
      }
   }
   
   if (GetMaxColWidth() > InnerArea().W()) {
      overflow = true;
   }
   if (GetTotalRowHeight() > InnerArea().H()) {
      overflow = true;
   }
   
   /// Reset preferred size to fit widgets if not already correct
   int pw = (favored_direction == FLOW_FAVOR_HORIZONTAL)?GetMaxColWidth():GetTotalRowHeight();
   int ph = (favored_direction == FLOW_FAVOR_HORIZONTAL)?GetTotalRowHeight():GetMaxColWidth();
   if (PreferredWidth() == 0) {
      pw = 0;
   }
   if (PreferredHeight() == 0) {
      ph = 0;
   }
   if (pw != PreferredWidth() || ph != PreferredHeight()) {
      SetPreferredSize(pw , ph);
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
      
      /// Handle major spacing
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
         const int ncols = colcount[row] - 1;
         if (ncols > 0) {
            const int spc = rowspace[row]/ncols;
            const int spcleft = rowspace[row]%ncols;
            const int offset1 = col*spc;
            const int offset2 = (col < spcleft)?col:spcleft;
            const int offset = offset1 + offset2;
            if (favored_direction == FLOW_FAVOR_HORIZONTAL) {
               r->MoveBy(offset , 0);
            }
            else if (favored_direction == FLOW_FAVOR_VERTICAL) {
               r->MoveBy(0 , offset);
            }
         }
      }
      else if (spacing == BOX_SPACE_EVEN) {
         const int ncols = colcount[row];
         const int spc = rowspace[row]/ncols;
         const int leftover = rowspace[row]%ncols;
         const int offset1 = spc/2;
         const int offset2 = spc*col;
         const int offset3 = ((2*col) < leftover)?(2*col):leftover;
         const int offset = offset1 + offset2 + offset3;
         if (favored_direction == FLOW_FAVOR_HORIZONTAL) {
            if (ncols > 1) {
               r->MoveBy(offset , 0);
            }
            else if (ncols == 1) {/// center a single column
               r->MoveBy((InnerArea().W() - r2.W())/2 , 0);
            }
         }
         else if (favored_direction == FLOW_FAVOR_VERTICAL) {
            if (ncols > 1) {
               r->MoveBy(0 , offset);
            }
            else if (ncols == 1) {/// center a single column
               r->MoveBy(0 , (InnerArea().H() - r2.H())/2);
            }
         }
      }
      else if (spacing == BOX_EXPAND) {
         Transform t = Eagle::EagleLibrary::System("Any")->GetSystemTransformer()->CreateTransform();
         const double sx = (double)InnerArea().W() / (double)((favored_direction == FLOW_FAVOR_HORIZONTAL)?GetMaxColWidth():GetTotalRowHeight());
         const double sy = (double)InnerArea().H() / (double)((favored_direction == FLOW_FAVOR_HORIZONTAL)?GetTotalRowHeight():GetMaxColWidth());
         t.Scale(sx , sy , 1.0);
         double x = r->X();
         double y = r->Y();
         double wd = r->W();
         double h = r->H();
         t.ApplyTransformation(&x , &y , 0);
         t.ApplyTransformation(&wd , &h , 0);
         r->SetArea((int)x , (int)y , (int)wd , (int)h);
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
            r->MoveBy((colwidths[row] - r2.W())/2 , 0);
         }
         else if (hal == HALIGN_RIGHT) {
            r->MoveBy((colwidths[row] - r2.W()) , 0);
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
      minorspacing(BOX_ALIGN_ONLY),
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
   if (!rowcount) {
      return InnerArea().W();
   }
   return rowspace.back();
}



int FlowLayout::HeightLeft() {
   if (!rowcount) {
      return InnerArea().H();/// Full space available
   }
   return (colspace + rowheights.back());/// Return space left on this row plus space left in layout
}



bool FlowLayout::WidgetWouldOverflowLayout(WidgetBase* w) {
   if (!w) {return false;}
   
   int pw = w->PreferredWidth();
   int ph = w->PreferredHeight();
   if (pw < 1) {
      pw = defwidth;
   }
   if (ph < 1) {
      ph = defheight;
   }
   if (pw < WidthLeft()) {
      return true;
   }
   else if (!rowcount) {
      return (ph <= InnerArea().H());
   }
   else if (rowcount) {
      return (ph < (colspace + rowheights.back()));
   }
   return (ph < colspace);
}



std::ostream& FlowLayout::DescribeTo(std::ostream& os , Indenter indent) const {
   
   os << indent << "BOX_SPACE_RULES = " << PrintBoxSpaceRule(spacing) << std::endl;
   os << indent << "BOX_ANCHOR_POINT = " << PrintBoxAnchorPoint(anchor) << std::endl;
   
   os << indent << "FLOW_FAVORED_DIRECTION = " << PrintFlowFavoredDirection(favored_direction) << std::endl;
   os << indent << "overflow = " << (overflow?"true":"false") << std::endl;
   for (unsigned int row = 0 ; row < colcount.size() ; ++row) {
      os << indent << "Colcount for row #" << row << " is " << colcount[row] << " and height is " << rowheights[row] << ". Space left is " << rowspace[row] << std::endl;
   }
   os << indent << "Colspace left = " << colspace << " max col width = " << GetMaxColWidth() << " total row height = " << GetTotalRowHeight() << std::endl;
   BoxLayout::DescribeTo(os , indent);
   return os;
}


