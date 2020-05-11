
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
 *    Copyright 2009-2019+ by Edgar Reynaldo
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



NinePatch::NinePatch() : 
      imgs(),
      window(0)
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



EagleImage** NinePatch::operator[](int yindex) {
   return imgs[yindex];
}



EagleImage* const* NinePatch::operator[](int index) const{
   return imgs[index];
}



NinePatch::NinePatch(const NinePatch& np) {
   *this = np;
}



NinePatch& NinePatch::operator=(const NinePatch& np) {
   Free();
   window = np.window;
   for (int y = 0 ; y < 3 ; ++y) {
      for (int x = 0 ; x < 3 ; ++x) {
         imgs[y][x] = window->CloneImage(np.imgs[y][x] , StringPrintF("NP#%d" , y*3 + x));
      }
   }
   return *this;
}



NinePatch MakeNinePatch(EagleGraphicsContext* win , EagleImage* src_img , WidgetArea src_area) {
   
   EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());
   EAGLE_ASSERT(src_img);
   EAGLE_ASSERT(src_img->Valid());
   
   Rectangle src_outer = src_area.OuterArea();
   Rectangle src_inner = src_area.InnerArea();

   Rectangle img_rect = Rectangle(0 , 0 , src_img->W() , src_img->H());
   
   if (!img_rect.Contains(src_outer) || !img_rect.Contains(src_inner)) {
      throw EagleException("MakeNinePatch() - Source image area does not fully contain the nine patch area!");
   }
   
   /// Make sure inner area of source is smaller than outer area
   EAGLE_ASSERT((src_inner.W() > 0) && (src_inner.W() < src_outer.W()));
   EAGLE_ASSERT((src_inner.H() > 0) && (src_inner.H() < src_outer.H()));

   /// Make sure the left and right and top and bottom margins are positive
   EAGLE_ASSERT((src_area.LeftIndent() > 0) && (src_area.RightIndent() > 0));
   EAGLE_ASSERT((src_area.TopIndent() > 0) && (src_area.BottomIndent() > 0));
   
/// Don't uncomment following line : Users may want a different area, in case of sprite sheets
///   src_area.MoveBy(-(src_area.OuterArea().X()) , -(src_area.OuterArea().Y()));

///   WidgetArea img_area;
   
///   img_area.SetOuterArea(0 , 0 , src_img->W() , src_img->H());
   
   
   /// Store relative positions in nplayout
   LayoutRectangle nplayout(src_outer , src_inner);
   
   /// Set the matching relative area on the image
///   img_area.SetFractionalInnerArea(nplayout.fx , nplayout.fy , nplayout.fw , nplayout.fh);
   
   EagleLog() << "NinePatch source area : " << src_area << std::endl;
///   EagleLog() << "IMGAREA :" << std::endl;
///   EagleLog() << img_area << std::endl;
   
   win->SetCopyBlender();
   NinePatch np;
   for (int y = 0 ; y < 3 ; ++y) {
      for (int x = 0 ; x < 3 ; ++x) {
         np[y][x] = 0;
         VCELL_AREA vcell = (VCELL_AREA)y;
         HCELL_AREA hcell = (HCELL_AREA)x;
         NPAREA nparea = src_area.BorderNP();
         Rectangle src = nparea.GetNPCell(hcell , vcell);
         EagleLog() << StringPrintF("Drawing Image : [%d][%d]" , y , x) << std::endl;
         EagleLog() << "Src = [" << src << "]" << std::endl;
         EagleImage* img = win->CreateImage(src.W() , src.H() , VIDEO_IMAGE);
         EAGLE_ASSERT(img);
         EAGLE_ASSERT(img->Valid());
         win->PushDrawingTarget(img);
         win->DrawTintedRegion(src_img , src , 0.0 , 0.0);
         win->PopDrawingTarget();
         np[y][x] = img;
      }
   }
   win->RestoreLastBlendingState();

   return np;
}



void NinePatch::Draw(NPAREA dest_area , int xoffset , int yoffset) {
   if (window) {
      for (int y = 0 ; y < 3 ; ++y) {
         for (int x = 0 ; x < 3 ; ++x) {
            VCELL_AREA vcell = (VCELL_AREA)y;
            HCELL_AREA hcell = (HCELL_AREA)x;
            Rectangle dest = dest_area.GetNPCell(hcell , vcell);
            dest.MoveBy(xoffset , yoffset);
            window->DrawStretched(imgs[y][x] , dest , DRAW_NORMAL);
         }
      }
   }
}





