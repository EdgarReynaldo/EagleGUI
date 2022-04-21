



#include "Eagle/GraphicsContext.hpp"
#include "Eagle/NinePatchPainters.hpp"



void PaintNPOutsideFill   (EagleGraphicsContext* win , NPAREA np , EagleColor pcol , EagleColor scol , void* unused) {
   (void)unused;
   (void)scol;
   PaintOutsideSolid(win , np , pcol);
}

void PaintNPOutsideRounded(EagleGraphicsContext* win , NPAREA np , EagleColor pcol , EagleColor scol , void* unused) {
   (void)unused;
   (void)scol;
   PaintOutsideRounded(win , np , pcol);
}

void PaintNPOutsideContrast(EagleGraphicsContext* win , NPAREA np , EagleColor pcol , EagleColor scol , void* unused) {
   (void)unused;
   (void)scol;
   PaintOutsideContrast(win , np , pcol , scol);
}

void PaintNPOutsideRContrast(EagleGraphicsContext* win , NPAREA np , EagleColor pcol , EagleColor scol , void* unused) {
   (void)unused;
   (void)scol;
   PaintOutsideContrast(win , np , scol , pcol);
}

void PaintNPOutsideGradient(EagleGraphicsContext* win , NPAREA np , EagleColor pcol , EagleColor scol , void* unused) {
   (void)unused;
   PaintOutsideGradient(win , np , pcol , scol);
}

