
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


#include "Eagle/NinePatch.hpp"
#include "Eagle/GraphicsContext.hpp"
#include "Eagle/Image.hpp"
#include "Eagle/Gui/Layout/LayoutRectangle.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/System.hpp"



/// NPAREA


NPAREA::NPAREA() :
      pos(0,0),
      left(0),
      width(0),
      right(0),
      top(0),
      height(0),
      bottom(0)
{}



NPAREA::NPAREA(Rectangle area , BOXAREA box) :
   pos(area.X() , area.Y()),
   left(box.left),
   width(area.Width() - box.Width()),
   right(box.right),
   top(box.top),
   height(area.Height() - box.Height()),
   bottom(box.bottom)
{}
   

CELL_AREA NPAREA::GetCellArea(int xpos , int ypos) const {
   if (((xpos >= pos.X()) && (xpos <= pos.X() + left + width + right)) &&
       ((ypos >= pos.Y()) && (ypos <= pos.Y() + top + height + bottom))) {
      HCELL_AREA hcell = HCELL_LEFT;
      if (xpos >= pos.X() + left) {
         hcell = HCELL_CENTER;
      }
      if (xpos >= pos.X() + left + width) {
         hcell = HCELL_RIGHT;
      }
      VCELL_AREA vcell = VCELL_TOP;
      if (ypos >= pos.Y() + top) {
         vcell = VCELL_CENTER;
      }
      if (ypos >= pos.Y() + top + height) {
         vcell = VCELL_BOTTOM;
      }
      return GetCell(hcell,vcell);
   }
   return CELL_AREA_OUTSIDE;
}



Rectangle NPAREA::GetNPCell(HCELL_AREA hcell , VCELL_AREA vcell) const {
   Pos2I p2 = pos;
   const int cellwidths[4]  = {0 , left , width , right};
   const int cellheights[4] = {0 , top , height , bottom};
   for (unsigned int j = 0 ; j <= vcell ; ++j) {
      p2.MoveBy(0 , cellheights[j]);
   }
   for (unsigned int i = 0 ; i <= hcell ; ++i) {
      p2.MoveBy(cellwidths[i] , 0);
   }
   return Rectangle(p2.X() , p2.Y() , cellwidths[hcell + 1] , cellheights[vcell + 1]);
}



Rectangle NPAREA::GetNPCell(CELL_AREA cell) const {
   if (cell == CELL_AREA_OUTSIDE) {
      return BADRECTANGLE;
   }
   return GetNPCell((HCELL_AREA)(cell % 3) , (VCELL_AREA)(cell / 3));
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



std::ostream& NPAREA::DescribeTo(std::ostream& os , Indenter indent) const {
   os << indent << "NPAREA(Pos = " << pos.X() << "," << pos.Y() << " , " << "(l,w,r = " << left << "," << width << "," << right << ") (t,h,b = " << top << "," << height << "," << bottom << "))" << std::endl;
   return os;
}



std::ostream& operator<<(std::ostream& os , const NPAREA& np) {
   return np.DescribeTo(os);
}



/// Nine Patch drawing




void PaintOutsideSolid(EagleGraphicsContext* win , NPAREA np , EagleColor c) {
   for (int i = 0 ; i < 9 ; ++i) {
      if (i == 4) {continue;}
      Rectangle cell = np.GetNPCell((HCELL_AREA)(i%3) , (VCELL_AREA)(i/3));
      if (cell.Area() > 0) {
         win->DrawFilledRectangle(cell , c);
      }
   }
}



void PaintOutsideRounded(EagleGraphicsContext* win , NPAREA np , EagleColor c) {
   Rectangle cells[9];
   for (int i = 0 ; i < 9 ; ++i) {
      if (i == 4) {continue;}
      HCELL_AREA hcell = (HCELL_AREA)(i%3);
      VCELL_AREA vcell = (VCELL_AREA)(i/3);
      cells[i] = np.GetNPCell(hcell , vcell);
      if (cells[i].Area() > 0) {
         if ((hcell == HCELL_CENTER) || (vcell == VCELL_CENTER)) {
            win->DrawFilledRectangle(cells[i] , c);
         }
         else {
            /// Drawing a corner
            if (hcell == HCELL_LEFT) {
               if (vcell == VCELL_TOP) {
                  win->DrawFilledQuarterEllipse(cells[i] , QUADRANT_NW , c);
               }
               else {
                  win->DrawFilledQuarterEllipse(cells[i] , QUADRANT_SW , c);
               }
            }
            else {
               if (vcell == VCELL_TOP) {
                  win->DrawFilledQuarterEllipse(cells[i] , QUADRANT_NE , c);
               }
               else {
                  win->DrawFilledQuarterEllipse(cells[i] , QUADRANT_SE , c);
               }
            }
         }
      }
   }
}



void PaintOutsideContrast(EagleGraphicsContext* win , NPAREA np , EagleColor outer , EagleColor inner) {
   NPAREA outernp(np.Area() , BOXAREA(np.left/2 , np.right/2 , np.top/2 , np.bottom/2));
   NPAREA innernp(outernp.GetNPCell(HCELL_CENTER , VCELL_CENTER) , 
                  BOXAREA(np.left - np.left/2 , np.right - np.right/2 , np.top - np.top/2 , np.bottom - np.bottom/2));
   innernp.PaintOutsideSolid(win , inner);
   outernp.PaintOutsideSolid(win , outer);
}


void PaintOutsideGradient(EagleGraphicsContext* win , NPAREA np , EagleColor outer , EagleColor inner) {
   for (int i = 0 ; i < 9 ; ++i) {
      CELL_AREA c = (CELL_AREA)i;

      if (c == CELL_AREA_MIDDLEMIDDLE) {continue;}/// Skip the middle cell

      HCELL_AREA hc = (HCELL_AREA)(i%3);
      VCELL_AREA vc = (VCELL_AREA)(i/3);

      Rectangle r = np.GetNPCell(hc,vc);

      bool isleft = (hc == HCELL_LEFT);
      bool istop = (vc == VCELL_TOP);

      /// The colors for the corners of our shaded quad
      EagleColor cornercols[2][2];/// Top left, top right, bottom left, bottom right
      
      switch (c) {
      case CELL_AREA_MIDDLELEFT :
      case CELL_AREA_MIDDLERIGHT:
      case CELL_AREA_TOPMIDDLE :
      case CELL_AREA_BOTTOMMIDDLE:
         {
            if (hc == HCELL_LEFT) {
               cornercols[0][0] = outer;///< top left
               cornercols[1][0] = outer;///< bottom left
               cornercols[0][1] = inner;///< top right
               cornercols[1][1] = inner;///< bottom right
            }
            else if (hc == HCELL_CENTER) {
               /// Top or bottom
               if (vc == VCELL_TOP) {
                  cornercols[0][0] = outer;///< top left
                  cornercols[0][1] = outer;///< top right
                  cornercols[1][0] = inner;///< bottom left
                  cornercols[1][1] = inner;///< bottom right
               }
               else if (vc == VCELL_BOTTOM) {
                  cornercols[0][0] = inner;///< top left
                  cornercols[0][1] = inner;///< top right
                  cornercols[1][0] = outer;///< bottom left
                  cornercols[1][1] = outer;///< bottom right
               }
            }
            else if (hc == HCELL_RIGHT) {/// right
               cornercols[0][0] = inner;///< top left
               cornercols[1][0] = inner;///< bottom left
               cornercols[0][1] = outer;///< top right
               cornercols[1][1] = outer;///< bottom right
            }
         }
         break;
      /// Paint the corners using a shaded quad, this will create beveled edges, not a rounded rectangle
      case CELL_AREA_TOPLEFT :
      case CELL_AREA_TOPRIGHT :
      case CELL_AREA_BOTTOMLEFT :
      case CELL_AREA_BOTTOMRIGHT :
         {
            if (isleft) {
               if (istop) {/// top left
                  cornercols[0][0] = outer;/// top left
                  cornercols[0][1] = outer;///col;/// top right
                  cornercols[1][0] = outer;/// bottom left
                  cornercols[1][1] = inner;/// bottom right
               }
               else {/// bottom left
                  cornercols[0][0] = outer;/// top left
                  cornercols[0][1] = inner;/// top right
                  cornercols[1][0] = outer;/// bottom left
                  cornercols[1][1] = outer;/// bottom right
               }
            }
            else {
               if (istop) {/// top right
                  cornercols[0][0] = outer;/// top left
                  cornercols[0][1] = outer;/// top right
                  cornercols[1][0] = inner;/// bottom left
                  cornercols[1][1] = outer;/// bottom right
               }
               else {/// bottom right
                  cornercols[0][0] = inner;/// top left
                  cornercols[0][1] = outer;/// top right
                  cornercols[1][0] = outer;/// bottom left
                  cornercols[1][1] = outer;/// bottom right
               }
            }
         }
         break;
      default :
         break;
      };

      win->DrawShadedQuad(
            r.X()  , r.Y()  , cornercols[0][0] ,
            r.X()  , r.BY() , cornercols[1][0] ,
            r.RX() , r.BY() , cornercols[1][1] ,
            r.RX() , r.Y()  , cornercols[0][1]  
      );
   }
}






/// NinePatch



bool Create(EagleGraphicsContext* win , EagleImage* src , NPAREA area) :
{
   Free();

   EAGLE_ASSERT(window);
   EAGLE_ASSERT(src && src->Valid());
   EAGLE_ASSERT(area.InnerArea().Area() > 0);
   EAGLE_ASSERT(area.OuterArea().Area() >= area.InnerArea().Area());

   window = win;
   srcimage = src;,
   srcarea = area;

   for (unsigned int i = 0 ; i < 9 ; ++i) {
      HCELL_AREA hcell = (HCELL_AREA)(i%3);
      VCELL_AREA vcell = (VCELL_AREA)(i/3);
      Rectangle r = srcarea.GetNPCell(hcell , vcell);
      imgs[i/3][i%3] = win->CreateSubImage(src , r.X() , r.Y() , r.W() , r.H());
      EAGLE_ASSERT(imgs[i/3][i%3] && imgs[i/3][i%3]->Valid());
   }
   return imgs[0][0]->Valid();
}



NinePatch::NinePatch() : 
      window(0),
      imgs(),
      srcimage(0),
      srcarea()
{
   for (int y = 0 ; y < 3 ; ++y) {
      for (int x = 0 ; x < 3 ; ++x) {
         imgs[y][x] = 0;
      }
   }
}



NinePatch::~NinePatch() {
   Free();
}



void NinePatch::Free() {
   if (window) {
      for (unsigned int i = 0 ; i < 9 ; ++i) {
         if (imgs[i/3][i%3]) {
            window->FreeImage(imgs[i/3][i%3]);
         }
         imgs[i/3][i%3] = 0;
      }
   }
}



EagleImage* const NinePatch::operator[](CELL_AREA carea) const {
   return imgs[carea/3][carea%3];
}



NinePatch::NinePatch(const NinePatch& np) {
   *this = np;
}



NinePatch& NinePatch::operator=(const NinePatch& np) {
   Free();
   Create(np.window , np.srcimage , np.srcarea);
   return *this;
}



void DrawNinePatch(const NinePatch& np , NPAREA dest_area , int ox , int oy ) {
   if (np->window) {
      for (int y = 0 ; y < 3 ; ++y) {
         for (int x = 0 ; x < 3 ; ++x) {
            VCELL_AREA vcell = (VCELL_AREA)y;
            HCELL_AREA hcell = (HCELL_AREA)x;
            Rectangle dest = dest_area.GetNPCell(hcell , vcell);
            dest.MoveBy(ox , oy);
            window->DrawStretched(np->imgs[y][x] , dest , DRAW_NORMAL);
         }
      }
   }
}





