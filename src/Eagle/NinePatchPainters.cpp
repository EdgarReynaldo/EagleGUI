



#include "Eagle/GraphicsContext.hpp"
#include "Eagle/NinePatchPainters.hpp"



void PaintNPOutsideFill   (EagleGraphicsContext* win , NPAREA np , EagleColor pcol , EagleColor scol , void* unused) {
   (void)unused;
   (void)scol;
   np.PaintOutsideSolid(win , pcol);
}

void PaintNPOutsideRounded(EagleGraphicsContext* win , NPAREA np , EagleColor pcol , EagleColor scol , void* unused) {
   (void)unused;
   (void)scol;
   np.PaintOutsideRounded(win , pcol);
}

void PaintNPOutsideContrast(EagleGraphicsContext* win , NPAREA np , EagleColor pcol , EagleColor scol , void* unused) {
   (void)unused;
   (void)scol;
   np.PaintOutsideContrast(win , pcol , scol);
}

void PaintNPOutsideRContrast(EagleGraphicsContext* win , NPAREA np , EagleColor pcol , EagleColor scol , void* unused) {
   (void)unused;
   (void)scol;
   np.PaintOutsideContrast(win , scol , pcol);
}

void PaintNPOutsideGradient(EagleGraphicsContext* win , NPAREA np , EagleColor pcol , EagleColor scol , void* unused) {
   (void)unused;
   for (int i = 0 ; i < 9 ; ++i) {
      CELL_AREA c = (CELL_AREA)i;

      if (c == CELL_AREA_MIDDLEMIDDLE) {continue;}/// Skip the middle cell

      HCELL_AREA hc = (HCELL_AREA)(i%3);
      VCELL_AREA vc = (VCELL_AREA)(i/3);

      Rectangle r = np.GetNPCell(c);

      bool left = (hc == HCELL_LEFT);
      bool top = (vc == VCELL_TOP);

      /// The colors for the corners of our shaded quad
      EagleColor cornercols[2][2];/// Top left, top right, bottom left, bottom right
      
      switch (c) {
      case CELL_AREA_MIDDLELEFT :
      case CELL_AREA_MIDDLERIGHT:
      case CELL_AREA_TOPMIDDLE :
      case CELL_AREA_BOTTOMMIDDLE:
         {
            if (hc == HCELL_LEFT) {
               cornercols[0][0] = pcol;///< top left
               cornercols[1][0] = pcol;///< bottom left
               cornercols[0][1] = scol;///< top right
               cornercols[1][1] = scol;///< bottom right
            }
            else if (hc == HCELL_CENTER) {
               /// Top or bottom
               if (vc == VCELL_TOP) {
                  cornercols[0][0] = pcol;///< top left
                  cornercols[0][1] = pcol;///< top right
                  cornercols[1][0] = scol;///< bottom left
                  cornercols[1][1] = scol;///< bottom right
               }
               else if (vc == VCELL_BOTTOM) {
                  cornercols[0][0] = scol;///< top left
                  cornercols[0][1] = scol;///< top right
                  cornercols[1][0] = pcol;///< bottom left
                  cornercols[1][1] = pcol;///< bottom right
               }
            }
            else if (hc == HCELL_RIGHT) {/// right
               cornercols[0][0] = scol;///< top left
               cornercols[1][0] = scol;///< bottom left
               cornercols[0][1] = pcol;///< top right
               cornercols[1][1] = pcol;///< bottom right
            }
         }
         break;
      /// Paint the corners using a shaded quad, this will create beveled edges, not a rounded rectangle
      case CELL_AREA_TOPLEFT :
      case CELL_AREA_TOPRIGHT :
      case CELL_AREA_BOTTOMLEFT :
      case CELL_AREA_BOTTOMRIGHT :
         {
            if (left) {
               if (top) {/// top left
                  cornercols[0][0] = pcol;/// top left
                  cornercols[0][1] = pcol;///col;/// top right
                  cornercols[1][0] = pcol;/// bottom left
                  cornercols[1][1] = scol;/// bottom right
               }
               else {/// bottom left
                  cornercols[0][0] = pcol;/// top left
                  cornercols[0][1] = scol;/// top right
                  cornercols[1][0] = pcol;/// bottom left
                  cornercols[1][1] = pcol;/// bottom right
               }
            }
            else {
               if (top) {/// top right
                  cornercols[0][0] = pcol;/// top left
                  cornercols[0][1] = pcol;/// top right
                  cornercols[1][0] = scol;/// bottom left
                  cornercols[1][1] = pcol;/// bottom right
               }
               else {/// bottom right
                  cornercols[0][0] = scol;/// top left
                  cornercols[0][1] = pcol;/// top right
                  cornercols[1][0] = pcol;/// bottom left
                  cornercols[1][1] = pcol;/// bottom right
               }
            }
         }
         break;
      default :
         break;
      };
/*      
	virtual void DrawShadedQuad(float x1 , float y1 , EagleColor c1 ,
										 float x2 , float y2 , EagleColor c2 ,
										 float x3 , float y3 , EagleColor c3 ,
										 float x4 , float y4 , EagleColor c4)=0;
*/
      win->DrawShadedQuad(r.X()  , r.Y()  , cornercols[0][0] ,
                          r.X()  , r.BY() , cornercols[1][0] ,
                          r.RX() , r.BY() , cornercols[1][1] ,
                          r.RX() , r.Y()  , cornercols[0][1]  );
   }
}

