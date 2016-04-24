

#include "Eagle/Gui/WidgetDrawFuncs.hpp"

#include "Eagle/Gui/Layout/SplitterLayouts.hpp"
/*
void WidgetDefaultPainter(EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos) {
   (void)win;
   (void)a;
   (void)c;
   (void)xpos;
   (void)ypos;
}
*/


void WidgetBGImagePainterCenter(EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos) {
   (void)c;
   EAGLE_ASSERT(win);
   Rectangle r = a.OuterArea();
   r.MoveBy(xpos,ypos);
   EagleImage* img = a.GetCellImage(MARGIN_HCELL_CENTER , MARGIN_VCELL_CENTER);
   if (!img) {
      return;
   }
   Clipper clip(win->GetDrawingTarget() , r);
   win->Draw(img , r.X() + (r.W() - img->W())/2 , r.Y() + (r.H() - img->H())/2);
}



void WidgetBGImagePainterStretch(EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos) {
   (void)c;
   EAGLE_ASSERT(win);
   Rectangle r = a.OuterArea();
   r.MoveBy(xpos,ypos);
   EagleImage* img = a.GetCellImage(MARGIN_HCELL_CENTER , MARGIN_VCELL_CENTER);
   if (!img) {
      return;
   }
   win->DrawStretchedRegion(img , Rectangle(0,0,img->W(),img->H()) , r);
}



void WidgetBGImagePainterCenterClientArea(EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos) {
   (void)c;
   EAGLE_ASSERT(win);
   Rectangle r = a.InnerArea();
   r.MoveBy(xpos,ypos);
   EagleImage* img = a.GetCellImage(MARGIN_HCELL_CENTER , MARGIN_VCELL_CENTER);
   if (!img) {
      return;
   }
   Clipper clip(win->GetDrawingTarget() , r);
   win->Draw(img , r.X() + (r.W() - img->W())/2 , r.Y() + (r.H() - img->H())/2);
}



void WidgetBGImagePainterStretchClientArea(EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos) {
   (void)c;
   EAGLE_ASSERT(win);
   Rectangle r = a.InnerArea();
   r.MoveBy(xpos,ypos);
   EagleImage* img = a.GetCellImage(MARGIN_HCELL_CENTER , MARGIN_VCELL_CENTER);
   if (!img) {
      return;
   }
   win->DrawStretchedRegion(img , Rectangle(0,0,img->W(),img->H()) , r);
}



void WidgetNPPainter(EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos) {
   (void)c;
   a.PaintImages(win , xpos , ypos);
}



void WidgetBorderPainterContrast(EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos) {
   EagleColor sdcol = c[SDCOL];
   EagleColor hlcol = c[HLCOL];
   DefaultSplitterDrawFunction(win , a.GetCellRectangle(MARGIN_HCELL_LEFT , MARGIN_VCELL_CENTER).MoveBy(Pos2d(xpos,ypos)) , SPLITTER_VERTICAL , c);
   DefaultSplitterDrawFunction(win , a.GetCellRectangle(MARGIN_HCELL_RIGHT , MARGIN_VCELL_CENTER).MoveBy(Pos2d(xpos,ypos)) , SPLITTER_VERTICAL , c);
   DefaultSplitterDrawFunction(win , a.GetCellRectangle(MARGIN_HCELL_CENTER , MARGIN_VCELL_TOP).MoveBy(Pos2d(xpos,ypos)) , SPLITTER_HORIZONTAL , c);
   DefaultSplitterDrawFunction(win , a.GetCellRectangle(MARGIN_HCELL_CENTER , MARGIN_VCELL_BOTTOM).MoveBy(Pos2d(xpos,ypos)) , SPLITTER_HORIZONTAL , c);
   
   Rectangle r;
   
   r = a.GetCellRectangle(MARGIN_HCELL_LEFT  , MARGIN_VCELL_TOP).MoveBy(Pos2d(xpos,ypos));
   win->DrawFilledRectangle(r.X() , r.Y() , r.W() , r.H() , sdcol);
   win->DrawFilledRectangle(r.X() + 0.25f*r.W() , r.Y() + 0.25f*r.H(), 0.75f*r.W() , 0.75f*r.H() , hlcol);
   win->DrawFilledRectangle(r.X() + 0.75f*r.W() , r.Y() + 0.75f*r.H(), 0.25f*r.W() , 0.25f*r.H() , sdcol);
   
   r = a.GetCellRectangle(MARGIN_HCELL_LEFT  , MARGIN_VCELL_BOTTOM).MoveBy(Pos2d(xpos,ypos));
   win->DrawFilledRectangle(r.X() , r.Y() , r.W() , r.H() , sdcol);
   win->DrawFilledRectangle(r.X() + 0.25f*r.W() , r.Y() + 0.0f, 0.75f*r.W() , 0.75f*r.H() , hlcol);
   win->DrawFilledRectangle(r.X() + 0.75f*r.W() , r.Y() + 0.0f, 0.25f*r.W() , 0.25f*r.H() , sdcol);

   r = a.GetCellRectangle(MARGIN_HCELL_RIGHT , MARGIN_VCELL_TOP).MoveBy(Pos2d(xpos,ypos));
   win->DrawFilledRectangle(r.X() , r.Y() , r.W() , r.H() , sdcol);
   win->DrawFilledRectangle(r.X() + 0.0f, r.Y() + 0.25f*r.H(), 0.75f*r.W() , 0.75f*r.H() , hlcol);
   win->DrawFilledRectangle(r.X() + 0.0f, r.Y() + 0.75f*r.H(), 0.25f*r.W() , 0.25f*r.H() , sdcol);

   r = a.GetCellRectangle(MARGIN_HCELL_RIGHT , MARGIN_VCELL_BOTTOM).MoveBy(Pos2d(xpos,ypos));
   win->DrawFilledRectangle(r.X() , r.Y() , r.W() , r.H() , sdcol);
   win->DrawFilledRectangle(r.X() + 0.0f, r.Y() + 0.0f, 0.75f*r.W() , 0.75f*r.H() , hlcol);
   win->DrawFilledRectangle(r.X() + 0.0f, r.Y() + 0.0f, 0.25f*r.W() , 0.25f*r.H() , sdcol);
   
}
void WidgetBorderPainterShadow(EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos) {
   EagleColor col = c[SDCOL];
//   EagleLog() << "WBP: EC = " << col << std::endl;
   a.Paint(win , MARGIN_HCELL_LEFT   , MARGIN_VCELL_TOP    , col , xpos , ypos);
   a.Paint(win , MARGIN_HCELL_CENTER , MARGIN_VCELL_TOP    , col , xpos , ypos);
   a.Paint(win , MARGIN_HCELL_RIGHT  , MARGIN_VCELL_TOP    , col , xpos , ypos);
   a.Paint(win , MARGIN_HCELL_LEFT   , MARGIN_VCELL_CENTER , col , xpos , ypos);
   a.Paint(win , MARGIN_HCELL_RIGHT  , MARGIN_VCELL_CENTER , col , xpos , ypos);
   a.Paint(win , MARGIN_HCELL_LEFT   , MARGIN_VCELL_BOTTOM , col , xpos , ypos);
   a.Paint(win , MARGIN_HCELL_CENTER , MARGIN_VCELL_BOTTOM , col , xpos , ypos);
   a.Paint(win , MARGIN_HCELL_RIGHT  , MARGIN_VCELL_BOTTOM , col , xpos , ypos);
}



void WidgetBorderedImagePainterCenter(EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos) {
   WidgetBorderPainterContrast(win , a , c , xpos , ypos);
   WidgetBGImagePainterCenterClientArea(win , a , c , xpos , ypos);
}



void WidgetBorderedImagePainterStretch(EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos) {
   WidgetBorderPainterContrast(win , a , c , xpos , ypos);
   WidgetBGImagePainterStretchClientArea(win , a , c , xpos , ypos);
}




