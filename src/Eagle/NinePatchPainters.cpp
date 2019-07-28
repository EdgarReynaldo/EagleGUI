



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
   np.PaintOutsideGradient(win , pcol , scol);
}

