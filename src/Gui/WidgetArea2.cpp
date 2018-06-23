
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




/// BORDERAREA



BORDERAREA::BORDERAREA() :
   left(0),
   right(0),
   top(0),
   bottom(0)
{}



BORDERAREA::BORDERAREA(unsigned int l , unsigned int r , unsigned int t , unsigned int b) :
      left(l),
      right(r),
      top(t),
      bottom(b)
{}



void BORDERAREA::Set(unsigned int l , unsigned int r , unsigned int t , unsigned int b) {
   left = l;
   right = r;
   top = t;
   bottom = b;
}






/// WIDGETAREA



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



Rectangle CellArea(CELL_TYPE type , CELL_AREA area) const {
   EAGLE_ASSERT(area != CELL_AREA_OUTSIDE);
   return CellArea(type , (VCELL_AREA)((int)area/3) , (HCELL_AREA)((int)area%3));
}
Rectangle CellArea(CELL_TYPE type , VCELL_AREA vcell , HCELL_AREA hcell) const {
   
   int cellz = (int)type;

   if ((vcell == VCELL_CENTER) && (hcell == HCELL_CENTER)) {
      Rectangle (WIDGETAREA::*AREAFUNC[3])() = {
         BorderArea,PaddingArea,InnerArea
      };
      return AREAFUNC[cz];
   }
   
   int celly = (int)vcell;
   int cellx = (int)hcell;
   Pos2D cellpos = pos;
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
      cellpos.MoveBy(0 , cheights[cz][y]);
   }
   for (int x = 0 ; x < cellx ; ++x) {
      cellpos.MoveBy(cwidths[cz][x] , 0);
   }
   
   return Rectangle(cellpos.X() , cellpos.Y() , cwidths[cellx] , cheights[celly]);
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
   return PaddingHeight() + InnerAreaHeight()
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



