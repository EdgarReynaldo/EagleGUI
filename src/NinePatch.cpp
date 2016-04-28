
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
#include "Eagle/Gui/Layout/LayoutRectangle.hpp"
#include "Eagle/System.hpp"

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
   
   nparea.MoveBy(-(nparea.OuterArea().X()) , -(nparea.OuterArea().Y()));
   WidgetArea imgarea;
   
   imgarea.SetOuterArea(0 , 0 , src_img->W() , src_img->H());
   
   EAGLE_DEBUG(
      if (!imgarea.OuterArea().Contains(nparea.OuterArea())) {
///         EagleLog() << "MakeNinePatch() - Image area does not fully contain the nine patch area!" << std::endl;
      }
   );
   
   /// Store relative positions in nplayout
   LayoutRectangle nplayout(nparea.OuterArea() , nparea.InnerArea());
   
   /// Set the matching relative area on the image
   imgarea.SetFractionalInnerArea(nplayout.fx , nplayout.fy , nplayout.fw , nplayout.fh);
   
   EagleLog() << "NPAREA :" << std::endl;
   EagleLog() << nparea << std::endl;
   EagleLog() << "IMGREA :" << std::endl;
   EagleLog() << imgarea << std::endl;
   
   win->SetCopyBlender();
   NinePatch np;
   for (int y = 0 ; y < 3 ; ++y) {
      for (int x = 0 ; x < 3 ; ++x) {
         np[y][x] = 0;
         MARGIN_HCELL hcell = (MARGIN_HCELL)x;
         MARGIN_VCELL vcell = (MARGIN_VCELL)y;
         Rectangle src = imgarea.GetCellRectangle(hcell , vcell);
         Rectangle cell = nparea.GetCellRectangle(hcell , vcell);
         Rectangle dest(0,0,cell.W(),cell.H());

         EagleLog() << "Drawing Image :" << std::endl;
         EagleLog() << "Src = [" << src << "]" << std::endl;
         EagleLog() << "Dest = [" << dest << "]" << std::endl;
         EagleImage* img = win->CreateImage(dest.W() , dest.H() , VIDEO_IMAGE);
         EAGLE_ASSERT(img);
         EAGLE_ASSERT(img->Valid());
         win->PushDrawingTarget(img);
         win->DrawStretchedRegion(src_img , src , dest , DRAW_NORMAL);
         win->PopDrawingTarget();
         np[y][x] = img;
      }
   }
   win->RestoreLastBlendingState();

   return np;
}





