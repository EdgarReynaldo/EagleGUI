
/*
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
 *    EAGLE
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2018+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#include "Eagle/Gui2/WidgetArea2.hpp"

/// BOXAREA



BOXAREA::BOXAREA() :
   left(0),
   right(0),
   top(0),
   bottom(0)
{}



BOXAREA::BOXAREA(unsigned int l , unsigned int r , unsigned int t , unsigned int b) :
      left(l),
      right(r),
      top(t),
      bottom(b)
{}



void BOXAREA::Set(unsigned int l , unsigned int r , unsigned int t , unsigned int b) {
   left = l;
   right = r;
   top = t;
   bottom = b;
}



/// NPAREA


NPAREA::NPAREA(Rectangle area , BOXAREA box) :
   pos(area.X() , area.Y()),
   left(box.left),
   width(area.Width() - box.Width()),
   right(box.right),
   top(box.top),
   height(area.Height() - box.Height()),
   bottom(box.bottom)
{}
   


Rectangle NPAREA::GetNPCell(HCELL_AREA hcell , VCELL_AREA vcell) const {
   Pos2I p2 = pos;
   const int cellwidths[4]  = {0 , left , width , right};
   const int cellheights[4] = {0 , top , height , bottom};
   for (unsigned int j = 0 ; j < vcell ; ++j) {
      p2.MoveBy(0 , cellheights[j]);
   }
   for (unsigned int i = 0 ; i < hcell ; ++i) {
      p2.MoveBy(cellwidths[i] , 0);
   }
   return Rectangle(p2.X() , p2.Y() , cellwidths[hcell + 1] , cellheights[vcell + 1]);
}



Rectangle NPAREA::GetRow(VCELL_AREA vcell) const {
   Rectangle r = GetNPCell(HCELL_LEFT , vcell);
   int heights[3] = {top , height , bottom};
   return Rectangle(r.X() , r.Y() , Width() , heights[vcell]);
}



Rectangle NPAREA::GetColumn(HCELL_AREA hcell) const {
   Rectangle r = GetNPCell(hcell , VCELL_TOP);
   int widths[3] = {left , width , bottom};
   return Rectangle(r.X() , r.Y() , Height() , widths[hcell]);
}



/// WIDGETAREA



void WIDGETAREA::SetBoxArea(BOX_TYPE box , unsigned int l , unsigned int r , unsigned int t , unsigned int b) {
   BOXAREA* boxes[3] = {&margin , &border , &padding};
   boxes[box]->Set(l,r,t,b);
}



void WIDGETAREA::SetBoxArea(BOX_TYPE box , BOXAREA b) {
   BOXAREA* boxes[3] = {&margin , &border , &padding};
   *(boxes[box]) = b;
}



/// Setters



WIDGETAREA& WIDGETAREA::operator=(const WIDGETAREA& wa) {
   SetWidgetArea(wa);
   return *this;
}



void WIDGETAREA::SetBoxAreaContractFromOuter(BOX_TYPE box , unsigned int l , unsigned int r , unsigned int t , unsigned int b) {
   SetBoxArea(box , l , r , t , b);
   SetOuterArea(OuterArea());
}



void WIDGETAREA::SetBoxAreaContractFromOuter(BOX_TYPE box , BOXAREA b) {
   SetBoxArea(box , b);
   SetOuterArea(OuterArea());
}



void WIDGETAREA::SetBoxAreaExpandFromInner(BOX_TYPE box , unsigned int l , unsigned int r , unsigned int t , unsigned int b) {
   SetBoxArea(box , l , r , t , b);
   SetInnerArea(InnerArea());
}



void WIDGETAREA::SetBoxAreaExpandFromInner(BOX_TYPE box , BOXAREA b) {
   SetBoxArea(box , b);
   SetInnerArea(InnerArea());
}



void WIDGETAREA::SetOuterArea(Rectangle oa) {
   pos.SetPos(oa.X() , oa.Y());
   int iw = oa.Width() - (OuterAreaWidth() - inner_width);
   if (iw < 0) {iw = 0;}
   int ih = oa.Height() - (OuterAreaHeight() - inner_height);
   if (ih < 0) {ih = 0;}
   inner_width = iw;
   inner_height = ih;
}



void WIDGETAREA::SetInnerArea(Rectangle ia) {
   inner_width = ia.Width();
   inner_height = ia.Height();
   int px = ia.X() - (margin.left + border.left + padding.left);
   int py = ia.Y() - (margin.top + border.top + padding.top);
   pos.SetPos(px,py);
}



void WIDGETAREA::SetWidgetArea(const WIDGETAREA& wa) {
   pos = wa.pos;
   margin = wa.margin;
   border = wa.border;
   padding = wa.padding;
   inner_width = wa.inner_width;
   inner_height = wa.inner_height;
}



/// Getters



NPAREA WIDGETAREA::OuterNP() const {
   return NPAREA(OuterArea() , GetAreaBox(BOX_TYPE_MARGIN));
}



NPAREA WIDGETAREA::BorderNP() const {
   return NPAREA(BorderArea() , GetAreaBox(BOX_TYPE_BORDER));
}



NPAREA WIDGETAREA::PaddingNP() const {
   return NPAREA(PaddingArea() , GetAreaBox(BOX_TYPE_PADDING));
}



Rectangle WIDGETAREA::OuterArea() const {
   return Rectangle(pos.X() , pos.Y() , OuterAreaWidth() , OuterAreaHeight());
}



Rectangle WIDGETAREA::BorderArea() const {
   return Rectangle(pos.X() + margin.left , pos.Y() + margin.top , BorderAreaWidth() , BorderAreaHeight());
}



Rectangle WIDGETAREA::PaddingArea() const {
   return Rectangle(pos.X() + margin.left + border.left , pos.Y() + margin.top + border.top , PaddingAreaWidth() , PaddingAreaHeight());
}



Rectangle WIDGETAREA::InnerArea() const {
   return Rectangle(pos.X() + margin.left + border.left + padding.left , pos.Y() + margin.top + border.top + padding.top , InnerAreaWidth() , InnerAreaHeight());
}



Rectangle WIDGETAREA::CellBox(BOX_TYPE box , CELL_AREA area) const {
   EAGLE_ASSERT(area != CELL_AREA_OUTSIDE);
   return CellBox(box , (VCELL_AREA)((int)area/3) , (HCELL_AREA)((int)area%3));
}



Rectangle WIDGETAREA::CellBox(BOX_TYPE box , VCELL_AREA vcell , HCELL_AREA hcell) const {
   
   int cellz = (int)box;

   typedef Rectangle (WIDGETAREA::*AREAFUNC)() const;
   if ((vcell == VCELL_CENTER) && (hcell == HCELL_CENTER)) {
      AREAFUNC areafunc[3] = {
         BorderArea,PaddingArea,InnerArea
      };
      AREAFUNC afunc = areafunc[cellz];
      
      return (this->*afunc)();
   }
   
   int celly = (int)vcell;
   int cellx = (int)hcell;
   Pos2I cellpos = pos;
   const int cwidths[3][3] = {
      {
         margin.left , 
         BorderAreaWidth(),
         margin.right
      },
      {
         border.left,
         PaddingAreaWidth(),
         border.right
      },
      {
         padding.left,
         InnerAreaWidth(),
         padding.right
      }
   };
   const int cheights[3][3] = {
      {
         margin.top , 
         BorderAreaHeight(),
         margin.bottom
      },
      {
         border.top,
         PaddingAreaHeight(),
         border.bottom
      },
      {
         padding.top,
         InnerAreaHeight(),
         padding.bottom
      }
   };
   const int cxoffsets[3] = {
      0 , margin.left , border.left
   };
   const int cyoffsets[3] = {
      0 , margin.top , border.top
   };
   for (int z = 0 ; z < cellz + 1 ; ++z) {
      cellpos.MoveBy(cxoffsets[z] , cyoffsets[z]);
   }
   for (int y = 0 ; y < celly ; ++y) {
      cellpos.MoveBy(0 , cheights[cellz][y]);
   }
   for (int x = 0 ; x < cellx ; ++x) {
      cellpos.MoveBy(cwidths[cellz][x] , 0);
   }
   
   return Rectangle(cellpos.X() , cellpos.Y() , cwidths[cellz][cellx] , cheights[cellz][celly]);
}



int WIDGETAREA::OuterAreaWidth() const {
   return MarginWidth() + BorderAreaWidth();
}



int WIDGETAREA::OuterAreaHeight() const {
   return MarginHeight() + BorderAreaHeight();
}



int WIDGETAREA::BorderAreaWidth() const {
   return BorderWidth() + PaddingAreaWidth();
}



int WIDGETAREA::BorderAreaHeight() const {
   return BorderHeight() + PaddingAreaHeight();
}



int WIDGETAREA::PaddingAreaWidth() const {
   return PaddingWidth() + InnerAreaWidth();
}



int WIDGETAREA::PaddingAreaHeight() const {
   return PaddingHeight() + InnerAreaHeight();
}



int WIDGETAREA::InnerAreaWidth() const {
   return inner_width;
}



int WIDGETAREA::InnerAreaHeight() const {
   return inner_height;
}



int WIDGETAREA::MarginWidth() const {
   return margin.Width();
}



int WIDGETAREA::MarginHeight() const {
   return margin.Height();
}



int WIDGETAREA::BorderWidth() const {
   return border.Width();
}



int WIDGETAREA::BorderHeight() const {
   return border.Height();
}



int WIDGETAREA::PaddingWidth() const {
   return padding.Width();
}



int WIDGETAREA::PaddingHeight() const {
   return padding.Height();
}



Rectangle WIDGETAREA::GetAreaRectangle(WAREA_TYPE atype) const {
   static Rectangle (WIDGETAREA::*AREAFUNC[4])() const = {
      &OuterArea,
      &BorderArea,
      &PaddingArea,
      &InnerArea
   };
   return (this->*AREAFUNC[atype])();
}



BOXAREA WIDGETAREA::GetAreaBox(BOX_TYPE btype) const {
   const BOXAREA * pbarea[3] = {
      &margin,
      &border,
      &padding
   };
   return *(pbarea[btype]);
}



