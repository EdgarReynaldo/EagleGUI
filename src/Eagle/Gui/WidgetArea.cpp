
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



#include "Eagle/Gui/WidgetArea.hpp"



void WIDGETAREA::SetBoxArea(BOX_TYPE box , unsigned int l , unsigned int r , unsigned int t , unsigned int b) {
   BOXAREA* boxes[3] = {&margin , &border , &padding};
   boxes[box]->Set(l,r,t,b);
}



void WIDGETAREA::SetBoxArea(BOX_TYPE box , BOXAREA b) {
   BOXAREA* boxes[3] = {&margin , &border , &padding};
   *(boxes[box]) = b;
}



WIDGETAREA::WIDGETAREA() :
      pos(0,0),
      margin(0,0,0,0),
      border(0,0,0,0),
      padding(0,0,0,0),
      inner_width(0),
      inner_height(0)
{}



WIDGETAREA::WIDGETAREA(const WIDGETAREA& wa) :
   pos(wa.pos),
   margin(wa.margin),
   border(wa.border),
   padding(wa.padding),
   inner_width(wa.inner_width),
   inner_height(wa.inner_height)
{
}



WIDGETAREA::WIDGETAREA(Rectangle outerarea , BOXAREA marginbox , BOXAREA borderbox , BOXAREA paddingbox) :
      pos(0,0),
      margin(0,0,0,0),
      border(0,0,0,0),
      padding(0,0,0,0),
      inner_width(0),
      inner_height(0)
{
   SetBoxesContract(outerarea , marginbox , borderbox , paddingbox);
}



WIDGETAREA::WIDGETAREA(BOXAREA marginbox , BOXAREA borderbox , BOXAREA paddingbox , Rectangle innerarea) :
      pos(0,0),
      margin(0,0,0,0),
      border(0,0,0,0),
      padding(0,0,0,0),
      inner_width(0),
      inner_height(0)
{
   SetBoxesExpand(marginbox , borderbox , paddingbox , innerarea);
}



/// Setters



WIDGETAREA& WIDGETAREA::MoveBy(Pos2I p) {
   pos.MoveBy(p);
   return *this;
}



WIDGETAREA WIDGETAREA::MovedBy(Pos2I p) const {
   WIDGETAREA ret = *this;
   ret.MoveBy(p);
   return ret;
}



WIDGETAREA& WIDGETAREA::operator=(const WIDGETAREA& wa) {
   SetWidgetArea(wa);
   return *this;
}



WIDGETAREA& WIDGETAREA::SetBoxesContract(Rectangle outerarea , BOXAREA marginbox , BOXAREA borderbox , BOXAREA paddingbox) {
   margin = marginbox;
   border = borderbox;
   padding = paddingbox;
   return SetOuterArea(outerarea);
}



WIDGETAREA& WIDGETAREA::SetBoxesContract(Rectangle outerarea , int marginsize , int bordersize , int paddingsize) {
   return SetBoxesContract(outerarea , BOXAREA(marginsize) , BOXAREA(bordersize) , BOXAREA(paddingsize));
}



WIDGETAREA& WIDGETAREA::SetBoxesContract(BOXAREA marginbox , BOXAREA borderbox , BOXAREA paddingbox) {
   return SetBoxesContract(OuterArea() , marginbox , borderbox , paddingbox);
}



WIDGETAREA& WIDGETAREA::SetBoxesContract(int marginsize , int bordersize , int paddingsize) {
   return SetBoxesContract(BOXAREA(marginsize) , BOXAREA(bordersize) , BOXAREA(paddingsize));
}



WIDGETAREA& WIDGETAREA::SetBoxesExpand(BOXAREA marginbox , BOXAREA borderbox , BOXAREA paddingbox , Rectangle innerarea) {
   margin = marginbox;
   border = borderbox;
   padding = paddingbox;
   return SetInnerArea(innerarea);
}



WIDGETAREA& WIDGETAREA::SetBoxesExpand(int marginsize , int bordersize , int paddingsize , Rectangle innerarea) {
   return SetBoxesExpand(BOXAREA(marginsize) , BOXAREA(bordersize) , BOXAREA(paddingsize) , innerarea);
}



WIDGETAREA& WIDGETAREA::SetBoxesExpand(BOXAREA marginbox , BOXAREA borderbox , BOXAREA paddingbox) {
   return SetBoxesExpand(marginbox , borderbox , paddingbox , InnerArea());
}



WIDGETAREA& WIDGETAREA::SetBoxesExpand(int marginsize , int bordersize , int paddingsize) {
   return SetBoxesExpand(BOXAREA(marginsize) , BOXAREA(bordersize) , BOXAREA(paddingsize));
}



WIDGETAREA& WIDGETAREA::SetBoxAreaContractFromOuter(BOX_TYPE box , unsigned int l , unsigned int r , unsigned int t , unsigned int b) {
   SetBoxArea(box , l , r , t , b);
   SetOuterArea(OuterArea());
   return *this;
}



WIDGETAREA& WIDGETAREA::SetBoxAreaContractFromOuter(BOX_TYPE box , BOXAREA b) {
   SetBoxArea(box , b);
   SetOuterArea(OuterArea());
   return *this;
}



WIDGETAREA& WIDGETAREA::SetBoxAreaExpandFromInner(BOX_TYPE box , unsigned int l , unsigned int r , unsigned int t , unsigned int b) {
   SetBoxArea(box , l , r , t , b);
   SetInnerArea(InnerArea());
   return *this;
}



WIDGETAREA& WIDGETAREA::SetBoxAreaExpandFromInner(BOX_TYPE box , BOXAREA b) {
   SetBoxArea(box , b);
   SetInnerArea(InnerArea());
   return *this;
}



WIDGETAREA& WIDGETAREA::SetOuterArea(Rectangle oa) {
   pos.SetPos(oa.X() , oa.Y());
   int iw = oa.Width() - (OuterAreaWidth() - inner_width);
   if (iw < 0) {iw = 0;}
   int ih = oa.Height() - (OuterAreaHeight() - inner_height);
   if (ih < 0) {ih = 0;}
   inner_width = iw;
   inner_height = ih;
   return *this;
}



WIDGETAREA& WIDGETAREA::SetInnerArea(Rectangle ia) {
   inner_width = ia.Width();
   inner_height = ia.Height();
   int px = ia.X() - (margin.left + border.left + padding.left);
   int py = ia.Y() - (margin.top + border.top + padding.top);
   pos.SetPos(px,py);
   return *this;
}



WIDGETAREA& WIDGETAREA::SetWidgetArea(const WIDGETAREA& wa) {
   pos = wa.pos;
   margin = wa.margin;
   border = wa.border;
   padding = wa.padding;
   inner_width = wa.inner_width;
   inner_height = wa.inner_height;
   return *this;
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



BOXAREA WIDGETAREA::MarginBox()  const {
   return margin;
}
BOXAREA WIDGETAREA::BorderBox()  const {
   return border;
}
BOXAREA WIDGETAREA::PaddingBox() const {
   return padding;
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
         &WIDGETAREA::BorderArea,
         &WIDGETAREA::PaddingArea,
         &WIDGETAREA::InnerArea
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
      &WIDGETAREA::OuterArea,
      &WIDGETAREA::BorderArea,
      &WIDGETAREA::PaddingArea,
      &WIDGETAREA::InnerArea
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



int WIDGETAREA::LeftIndent() const {
   return margin.left + border.left + padding.left;
}



int WIDGETAREA::RightIndent() const {
   return margin.right + border.right + padding.right;
}



int WIDGETAREA::TopIndent() const {
   return margin.top + border.top + padding.top;
}



int WIDGETAREA::BottomIndent() const {
   return margin.bottom + border.bottom + padding.bottom;
}



std::ostream& WIDGETAREA::DescribeTo(std::ostream& os , Indenter indent) const {
   os << indent << "WIDGETAREA(OuterArea = " << OuterArea() << " , margin = " << margin << " , border = " << border << " , padding = " << padding << " , InnerArea = " << InnerArea() << ")";
   return os;
}


std::ostream& operator<<(std::ostream& os , const WIDGETAREA& wa) {
   return wa.DescribeTo(os);
}



