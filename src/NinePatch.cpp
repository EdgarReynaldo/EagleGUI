
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
 *    Copyright 2009-2014+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */


#include "Eagle/NinePatch.hpp"
#include "Eagle/GraphicsContext.hpp"
#include "Eagle/Image.hpp"



NinePatch::NinePatch() : imgs() {
   for (int y = 0 ; y < 3 ; ++y) {
      for (int x = 0 ; x < 3 ; ++x) {
         imgs[x][y] = 0;
      }
   }
}



EagleImage** NinePatch::operator[](int xindex) {
   return imgs[xindex];
}



EagleImage* const* NinePatch::operator[](int xindex) const{
   return imgs[xindex];
}



NinePatch::NinePatch(const NinePatch& np) {
   *this = np;
}



NinePatch& NinePatch::operator=(const NinePatch& np) {
   for (int y = 0 ; y < 3 ; ++y) {
      for (int x = 0 ; x < 3 ; ++x) {
         imgs[x][y] = np[x][y];
      }
   }
   return *this;
}



NinePatch MakeNinePatch(EagleGraphicsContext* win , EagleImage* src_img , WidgetArea nparea) {
   
   EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());
   EAGLE_ASSERT(src_img);
   EAGLE_ASSERT(src_img->Valid());
   
   EAGLE_ASSERT(nparea.InnerArea().W() < nparea.OuterArea().W() && nparea.InnerArea().W() > 0);
   EAGLE_ASSERT(nparea.InnerArea().H() < nparea.OuterArea().H() && nparea.InnerArea().H() > 0);
   
   Rectangle img_rect(0 , 0 , src_img->W() , src_img->H());
   
   EAGLE_ASSERT(img_rect.Contains(nparea.OuterArea()));
   
   NinePatch np;
   for (int y = 0 ; y < 3 ; ++y) {
      for (int x = 0 ; x < 3 ; ++x) {
         np[x][y] = 0;
         MARGIN_HCELL hcell = (MARGIN_HCELL)x;
         MARGIN_VCELL vcell = (MARGIN_VCELL)y;
         Rectangle r = nparea.GetCellRectangle(hcell , vcell);
         EagleImage* img = win->CreateImage(r.W() , r.H() , VIDEO_IMAGE);
         EAGLE_ASSERT(img);
         EAGLE_ASSERT(img->Valid());
         win->PushDrawingTarget(img);
         win->DrawRegion(src_img , r , 0 , 0);
         win->PopDrawingTarget();
         np[x][y] = img;
      }
   }
   return np;
}





