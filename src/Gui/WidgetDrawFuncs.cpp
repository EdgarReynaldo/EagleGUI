


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
 *    Copyright 2009-2016+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#include "Eagle/Gui/WidgetDrawFuncs.hpp"
#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/StringWork.hpp"


#include <sstream>
using std::stringstream;



/**


void WidgetHandler::DrawFocus(EagleGraphicsContext* win) {
   if (wfocus && OwnsWidget(wfocus)) {
      Rectangle r = wfocus->InnerArea();
      EagleColor color = wfocus->WCols()[HLCOL];
      EagleColor sdcolor = wfocus->WCols()[SDCOL];
      switch (focus_type) {
         case FOCUS_INVISIBLE :
            // do nothing
            break;
         case FOCUS_HIGHLIGHT_OUTLINE :
            r.Draw(win , color);
            break;
         case FOCUS_HIGHLIGHT_THICK_OUTLINE :
            r.Draw(win , color);
            r.Shrink(1);
            r.Draw(win , color);
            break;
         case FOCUS_HIGHLIGHT_DOTTED :
            r.DottedOutline(win , color , true);
            r.DottedOutline(win , sdcolor , false);
            break;
         case FOCUS_HIGHLIGHT_THICK_DOTTED :
            r.DottedOutline(win , color , true);
            r.DottedOutline(win , sdcolor , false);
            r.Shrink(1);
            r.DottedOutline(win , color , true);
            r.DottedOutline(win , sdcolor , false);
            break;
      }
   }
}


///*/


const char* BgDrawTypeString(BG_DRAW_TYPE draw_type) {
   static const char* bg_draw_type_strs[NUM_BACKGROUND_DRAW_TYPES] = {
      "BG_DRAW_BACKGROUND_EMPTY",
      "BG_DRAW_BACKGROUND_SOLID",
      "BG_DRAW_BACKGROUND_IMAGE_CENTER",
      "BG_DRAW_BACKGROUND_IMAGE_STRETCH",
      "BG_DRAW_BACKGROUND_IMAGE_CENTER_CLIENT",
      "BG_DRAW_BACKGROUND_IMAGE_STRETCH_CLIENT",
      "BG_DRAW_BACKGROUND_NINEPATCH",
      "BG_DRAW_BACKGROUND_CONTRAST_BORDER",
      "BG_DRAW_BACKGROUND_DEPTH_BORDER",
      "BG_DRAW_BACKGROUND_SOLID_BORDER",
      "BG_DRAW_BACKGROUND_SOLID_SHADOW_BORDER",
      "BG_DRAW_BACKGROUND_SOLID_BORDER_IMAGE_CENTER",
      "BG_DRAW_BACKGROUND_SOLID_BORDER_IMAGE_STRETCH",
      "BG_DRAW_BACKGROUND_CUSTOM"
   };
   EAGLE_ASSERT((draw_type >= 0) && (draw_type < NUM_BACKGROUND_DRAW_TYPES));
   return bg_draw_type_strs[draw_type];
}



const char* FocusDrawTypeString(FOCUS_DRAW_TYPE draw_type) {
   static const char* focus_draw_type_strs[NUM_FOCUS_DRAW_TYPES] = {
      "FOCUS_DRAW_INVISIBLE",
      "FOCUS_DRAW_HIGHLIGHT_OUTLINE",
      "FOCUS_DRAW_HIGHLIGHT_THICK_OUTLINE",
      "FOCUS_DRAW_HIGHLIGHT_DOTTED",
      "FOCUS_DRAW_HIGHLIGHT_THICK_DOTTED",
      "FOCUS_DRAW_CUSTOM"
   };
   EAGLE_ASSERT((draw_type >= 0) && (draw_type < NUM_FOCUS_DRAW_TYPES));
   return focus_draw_type_strs[draw_type];
}



void DefaultSplitterDrawFunction(EagleGraphicsContext* window , Rectangle divider_rect , bool horizontal , const WidgetColorset& colors) {
   DefaultSplitterDrawFunction(window , divider_rect , horizontal , colors[HLCOL] , colors[SDCOL]);
}



void DefaultSplitterDrawFunction(EagleGraphicsContext* window , Rectangle divider_rect , bool horizontal , EagleColor hlcol , EagleColor sdcol) {
   float x = divider_rect.X();
   float y = divider_rect.Y();
   float w = divider_rect.W();
   float h = divider_rect.H();
   window->DrawFilledRectangle(x,y,w,h,hlcol);
//   EagleLog() << StringPrintF("DefaultSplitterDrawFunction x,y,w,h = %d,%d,%d x %d\n",
//                              (int)x,(int)y,(int)w,(int)h);
   if (horizontal) {
      // horizontal bar
      window->DrawFilledRectangle(x,y,w,h/4.0,sdcol);
      window->DrawFilledRectangle(x,y+3.0*h/4.0,w,h/4.0,sdcol);
   }
   else {
      // vertical bar
      window->DrawFilledRectangle(x,y,w/4.0,h,sdcol);
      window->DrawFilledRectangle(x+3.0*w/4.0,y,w/4.0,h,sdcol);
   }
}



/// --------------------------     BackgroundPainterObject      --------------------------------



void BackgroundPainterObject::PaintBackground(EagleGraphicsContext* window , const WidgetBase* widget , int xpos , int ypos) {
   EAGLE_ASSERT(window);
   EAGLE_ASSERT(window->Valid());
   EAGLE_ASSERT(widget);
   PaintBackground(window , widget->Area() , widget->WCols() , xpos , ypos);
}



/// -------------------------      Background Painters      --------------------------------------



void EmptyBackgroundPainter::
PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos) {
   EAGLE_ASSERT(window);
   EAGLE_ASSERT(window->Valid());
   (void)window;
   (void)warea;
   (void)colors;
   (void)xpos;
   (void)ypos;
}



void SolidBackgroundPainter::
PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , int xpos , int ypos , EagleColor c) {
   EAGLE_ASSERT(window);
   EAGLE_ASSERT(window->Valid());
   Rectangle r = warea.OuterArea();
   r.MoveBy(xpos,ypos);
   window->DrawFilledRectangle(r , c);
///   window->DrawFilledRectangle(widget->OuterArea().MoveBy(xpos,ypos) , widget->WCols()[BGCOL]);
}



void SolidBackgroundPainter::
PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos) {
   PaintBackground(window , warea , xpos , ypos , colors[BGCOL]);
}



void CenteredImageBackgroundPainter::
PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos) {
   EAGLE_ASSERT(window);
   EAGLE_ASSERT(window->Valid());
   (void)colors;

   EagleImage* img = warea.GetCellImage(MARGIN_HCELL_CENTER , MARGIN_VCELL_CENTER);
   if (!img) {
      return;
   }
   Rectangle r = warea.OuterArea();
   r.MoveBy(xpos,ypos);

   window->Draw(img , r.X() + (r.W() - img->W())/2 , r.Y() + (r.H() - img->H())/2);
}



void StretchedImageBackgroundPainter::
PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos) {
   EAGLE_ASSERT(window);
   EAGLE_ASSERT(window->Valid());
   (void)colors;

   EagleImage* img = warea.GetCellImage(MARGIN_HCELL_CENTER , MARGIN_VCELL_CENTER);
   if (!img) {
      return;
   }
   Rectangle r = warea.OuterArea();
   r.MoveBy(xpos,ypos);
   window->DrawStretchedRegion(img , Rectangle(0,0,img->W(),img->H()) , r);
}



void CenteredClientImageBackgroundPainter::
PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos) {
   EAGLE_ASSERT(window);
   EAGLE_ASSERT(window->Valid());
   (void)colors;

   EagleImage* img = warea.GetCellImage(MARGIN_HCELL_CENTER , MARGIN_VCELL_CENTER);
   if (!img) {
      return;
   }
   Rectangle r = warea.InnerArea();
   r.MoveBy(xpos,ypos);
   Clipper clip(window->GetDrawingTarget() , r);/// Clip to InnerArea
   window->Draw(img , r.X() + (r.W() - img->W())/2 , r.Y() + (r.H() - img->H())/2);
}



void StretchedClientImageBackgroundPainter::
PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos) {
   EAGLE_ASSERT(window);
   EAGLE_ASSERT(window->Valid());
   (void)colors;

   EagleImage* img = warea.GetCellImage(MARGIN_HCELL_CENTER , MARGIN_VCELL_CENTER);
   if (!img) {
      return;
   }
   Rectangle r = warea.InnerArea();
   r.MoveBy(xpos,ypos);
   window->DrawStretchedRegion(img , Rectangle(0,0,img->W(),img->H()) , r);
}



void NinePatchImageBackgroundPainter::
PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos) {
   EAGLE_ASSERT(window);
   EAGLE_ASSERT(window->Valid());
   (void)colors;
   
   warea.PaintImages(window , xpos , ypos , DRAW_NORMAL);
}



void ContrastBorderBackgroundPainter::
PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos) {
   EAGLE_ASSERT(window);
   EAGLE_ASSERT(window->Valid());
   (void)colors;
   PaintBackground(window , warea , xpos , ypos , colors[HLCOL] , colors[SDCOL]);
}



/// TODO : This probably only produces correct results with 100% alpha FIXME
void ContrastBorderBackgroundPainter::
PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , int xpos , int ypos , EagleColor hlcol , EagleColor sdcol) {

   EAGLE_ASSERT(window);
   EAGLE_ASSERT(window->Valid());

   WidgetArea area = warea;
   area.MoveBy(xpos,ypos);
   
   DefaultSplitterDrawFunction(window , area.GetCellRectangle(MARGIN_HCELL_LEFT , MARGIN_VCELL_CENTER) , false , hlcol , sdcol);
   DefaultSplitterDrawFunction(window , area.GetCellRectangle(MARGIN_HCELL_RIGHT , MARGIN_VCELL_CENTER) , false , hlcol , sdcol);
   DefaultSplitterDrawFunction(window , area.GetCellRectangle(MARGIN_HCELL_CENTER , MARGIN_VCELL_TOP) , true , hlcol , sdcol);
   DefaultSplitterDrawFunction(window , area.GetCellRectangle(MARGIN_HCELL_CENTER , MARGIN_VCELL_BOTTOM) , true , hlcol , sdcol);
   
   Rectangle r;

   r = area.GetCellRectangle(MARGIN_HCELL_LEFT  , MARGIN_VCELL_TOP);
   window->DrawFilledRectangle(r.X() , r.Y() , r.W() , r.H() , sdcol);
   window->DrawFilledRectangle(r.X() + 0.25f*r.W() , r.Y() + 0.25f*r.H(), 0.75f*r.W() , 0.75f*r.H() , hlcol);
   window->DrawFilledRectangle(r.X() + 0.75f*r.W() , r.Y() + 0.75f*r.H(), 0.25f*r.W() , 0.25f*r.H() , sdcol);
   
   r = area.GetCellRectangle(MARGIN_HCELL_LEFT  , MARGIN_VCELL_BOTTOM);
   window->DrawFilledRectangle(r.X() , r.Y() , r.W() , r.H() , sdcol);
   window->DrawFilledRectangle(r.X() + 0.25f*r.W() , r.Y() + 0.0f, 0.75f*r.W() , 0.75f*r.H() , hlcol);
   window->DrawFilledRectangle(r.X() + 0.75f*r.W() , r.Y() + 0.0f, 0.25f*r.W() , 0.25f*r.H() , sdcol);

   r = area.GetCellRectangle(MARGIN_HCELL_RIGHT , MARGIN_VCELL_TOP);
   window->DrawFilledRectangle(r.X() , r.Y() , r.W() , r.H() , sdcol);
   window->DrawFilledRectangle(r.X() + 0.0f, r.Y() + 0.25f*r.H(), 0.75f*r.W() , 0.75f*r.H() , hlcol);
   window->DrawFilledRectangle(r.X() + 0.0f, r.Y() + 0.75f*r.H(), 0.25f*r.W() , 0.25f*r.H() , sdcol);

   r = area.GetCellRectangle(MARGIN_HCELL_RIGHT , MARGIN_VCELL_BOTTOM);
   window->DrawFilledRectangle(r.X() , r.Y() , r.W() , r.H() , sdcol);
   window->DrawFilledRectangle(r.X() + 0.0f, r.Y() + 0.0f, 0.75f*r.W() , 0.75f*r.H() , hlcol);
   window->DrawFilledRectangle(r.X() + 0.0f, r.Y() + 0.0f, 0.25f*r.W() , 0.25f*r.H() , sdcol);
   
}



void DepthBorderBackgroundPainter::
PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos) {
   ContrastBorderBackgroundPainter painter;
   painter.PaintBackground(window , warea , xpos , ypos , colors[SDCOL] , colors[FGCOL]);
}



void DepthBorderBackgroundPainter::
PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , int xpos , int ypos , EagleColor fgcol , EagleColor sdcol) {
   ContrastBorderBackgroundPainter painter;
   painter.PaintBackground(window , warea , xpos , ypos , sdcol , fgcol);
}



void SolidBorderBackgroundPainter::
PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos) {
   /// Uses FGCOL
   PaintBackground(window , warea , xpos , ypos , colors[FGCOL]);
}



void SolidBorderBackgroundPainter::
PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , int xpos , int ypos , EagleColor c) {

   EAGLE_ASSERT(window);
   EAGLE_ASSERT(window->Valid());
   
   WidgetArea area = warea;
   area.MoveBy(xpos,ypos);
   
   Rectangle r;
   
   /// Draw top 3 cells
   r = area.GetCellRectangle(MARGIN_HCELL_LEFT , MARGIN_VCELL_TOP);
   r.SetDimensions(area.W() , area.MTop());/// span top
   window->DrawFilledRectangle(r,c);

   /// Draw bottom 3 cells
   r = area.GetCellRectangle(MARGIN_HCELL_LEFT , MARGIN_VCELL_BOTTOM);
   r.SetDimensions(area.W() , area.MBot());/// span bottom
   window->DrawFilledRectangle(r,c);
   
   /// Draw left vcenter cell
   r = area.GetCellRectangle(MARGIN_HCELL_LEFT , MARGIN_VCELL_CENTER);
   window->DrawFilledRectangle(r,c);

   /// Draw right vcenter cell
   r = area.GetCellRectangle(MARGIN_HCELL_RIGHT , MARGIN_VCELL_CENTER);
   window->DrawFilledRectangle(r,c);

}



void SolidShadowBorderBackgroundPainter::
PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos) {
   SolidBorderBackgroundPainter painter;
   painter.PaintBackground(window , warea , xpos , ypos , colors[SDCOL]);
}



void BorderedCenteredImageBackgroundPainter::
PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos) {
   SolidBorderBackgroundPainter bgpainter;
   CenteredClientImageBackgroundPainter imgclient;
   bgpainter.PaintBackground(window , warea , colors , xpos , ypos);
   imgclient.PaintBackground(window , warea , colors , xpos , ypos);
}



void BorderedStretchedImageBackgroundPainter::
PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos) {
   SolidBorderBackgroundPainter bgpainter;
   StretchedClientImageBackgroundPainter imgclient;
   bgpainter.PaintBackground(window , warea , colors , xpos , ypos);
   imgclient.PaintBackground(window , warea , colors , xpos , ypos);
}



void CustomBackgroundPainter::
PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos) {
   EAGLE_ASSERT(window);
   EAGLE_ASSERT(window->Valid());
   (void)window;
   (void)warea;
   (void)colors;
   (void)xpos;
   (void)ypos;
}



/// --------------------------     BackgroundPainter      -----------------------------------------



EmptyBackgroundPainter                  BackgroundPainter::empty_background_painter;
SolidBackgroundPainter                  BackgroundPainter::solid_background_painter;
CenteredImageBackgroundPainter          BackgroundPainter::centered_image_background_painter;
StretchedImageBackgroundPainter         BackgroundPainter::stretched_image_background_painter;
CenteredClientImageBackgroundPainter    BackgroundPainter::centered_client_image_background_painter;
StretchedClientImageBackgroundPainter   BackgroundPainter::stretched_client_image_background_painter;
NinePatchImageBackgroundPainter         BackgroundPainter::ninepatch_image_background_painter;
ContrastBorderBackgroundPainter         BackgroundPainter::contrast_border_background_painter;
DepthBorderBackgroundPainter            BackgroundPainter::depth_border_background_painter;
SolidBorderBackgroundPainter            BackgroundPainter::solid_border_background_painter;
SolidShadowBorderBackgroundPainter      BackgroundPainter::solid_shadow_border_background_painter;
BorderedCenteredImageBackgroundPainter  BackgroundPainter::bordered_centered_image_background_painter;
BorderedStretchedImageBackgroundPainter BackgroundPainter::bordered_stretched_image_background_painter;
CustomBackgroundPainter                 BackgroundPainter::custom_background_painter;



BackgroundPainterObject* const BackgroundPainter::default_bg_painters[NUM_BACKGROUND_DRAW_TYPES] = {
   &empty_background_painter,
   &solid_background_painter,
   &centered_image_background_painter,
   &stretched_image_background_painter,
   &centered_client_image_background_painter,
   &stretched_client_image_background_painter,
   &ninepatch_image_background_painter,
   &contrast_border_background_painter,
   &depth_border_background_painter,
   &solid_border_background_painter,
   &solid_shadow_border_background_painter,
   &bordered_centered_image_background_painter,
   &bordered_stretched_image_background_painter,
   &custom_background_painter
};



BackgroundPainter::BackgroundPainter() :
      painters()
{
   for (int i = 0 ; i < NUM_BACKGROUND_DRAW_TYPES ; ++i) {
      painters[i] = default_bg_painters[i];
   }
}



void BackgroundPainter::SetPainter(BG_DRAW_TYPE bg_draw_type , BackgroundPainterObject* bg_painter) {
   EAGLE_ASSERT((bg_draw_type >= 0) && (bg_draw_type < NUM_BACKGROUND_DRAW_TYPES));
   if (bg_painter) {
      painters[bg_draw_type] = bg_painter;
   }
   else {
      painters[bg_draw_type] = default_bg_painters[bg_draw_type];
   }
}



BackgroundPainterObject* BackgroundPainter::GetPainter(BG_DRAW_TYPE bg_draw_type) {
   EAGLE_ASSERT((bg_draw_type >= 0) && (bg_draw_type < NUM_BACKGROUND_DRAW_TYPES));
   return painters[bg_draw_type];
}



void BackgroundPainter::PaintBackground(EagleGraphicsContext* window , const WidgetBase* widget , int xpos , int ypos , BG_DRAW_TYPE bg_draw_type) {
   PaintBackground(window , widget->Area() , widget->WCols() , xpos , ypos , bg_draw_type);
}



void BackgroundPainter::PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos , BG_DRAW_TYPE bg_draw_type) {
   EAGLE_ASSERT((bg_draw_type >= 0) && (bg_draw_type < NUM_BACKGROUND_DRAW_TYPES));
   painters[bg_draw_type]->PaintBackground(window , warea , colors , xpos , ypos);
}



BackgroundPainter default_background_painter;/// Make sure this comes after BackgroundPainter::default_bg_painters!!!




/// ---------------------------     FocusPainter objects     -------------------------------



void InvisibleFocusPainter::
PaintFocus(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos) {
   EAGLE_ASSERT(window);
   EAGLE_ASSERT(window->Valid());
   (void)window;
   (void)warea;
   (void)colors;
   (void)xpos;
   (void)ypos;
}



void HighlightOutlineFocusPainter::
PaintFocus(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos) {
   EAGLE_ASSERT(window);
   EAGLE_ASSERT(window->Valid());
   Rectangle r = warea.InnerArea();
   r.MoveBy(xpos,ypos);
   window->DrawFilledRectangle(r,colors[HLCOL]);
}



void HighlightThickOutlineFocusPainter::
PaintFocus(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos) {
   EAGLE_ASSERT(window);
   EAGLE_ASSERT(window->Valid());
   Rectangle r = warea.InnerArea();
   r.MoveBy(xpos,ypos);
   EagleColor hlcol = colors[HLCOL];
   window->DrawRectangle(r.X() + thickness/2.0f , r.Y() + thickness/2.0f , r.W() - thickness , r.H() - thickness , thickness , hlcol);
}



void HighlightDottedOutlineFocusPainter::
PaintFocus(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos) {
   EAGLE_ASSERT(window);
   EAGLE_ASSERT(window->Valid());
   Rectangle r = warea.InnerArea();
   r.MoveBy(xpos,ypos);
   EagleColor hlcol = colors[HLCOL];
   EagleColor sdcol = colors[SDCOL];
   r.DottedOutline(window , hlcol , true);
   r.DottedOutline(window , sdcol , false);
}



void HighlightThickDottedOutlineFocusPainter::
PaintFocus(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos) {
   EAGLE_ASSERT(window);
   EAGLE_ASSERT(window->Valid());
   Rectangle r = warea.InnerArea();
   r.MoveBy(xpos,ypos);
   EagleColor hlcol = colors[HLCOL];
   EagleColor sdcol = colors[SDCOL];
   for (int i = 0 ; i < thickness ; ++i) {
      bool on = (i%2==0);
      r.DottedOutline(window , hlcol , on);
      r.DottedOutline(window , sdcol , !on);
      r.Shrink(1);
   }
}



void CustomFocusPainter::
PaintFocus(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos) {
   EAGLE_ASSERT(window);
   EAGLE_ASSERT(window->Valid());
   (void)window;
   (void)warea;
   (void)colors;
   (void)xpos;
   (void)ypos;
}



/// -------------------------      FocusPainter      -----------------------------------



FocusPainterObject* const FocusPainter::default_focus_painters[NUM_FOCUS_DRAW_TYPES] = {
   &invisible_focus_painter,
   &highlight_outline_focus_painter,
   &highlight_thick_outline_focus_painter,
   &highlight_dotted_outline_focus_painter,
   &highlight_thick_dotted_outline_focus_painter,
   &custom_focus_painter
};



InvisibleFocusPainter                   FocusPainter::invisible_focus_painter;
HighlightOutlineFocusPainter            FocusPainter::highlight_outline_focus_painter;
HighlightThickOutlineFocusPainter       FocusPainter::highlight_thick_outline_focus_painter;
HighlightDottedOutlineFocusPainter      FocusPainter::highlight_dotted_outline_focus_painter;
HighlightThickDottedOutlineFocusPainter FocusPainter::highlight_thick_dotted_outline_focus_painter;
CustomFocusPainter                      FocusPainter::custom_focus_painter;



FocusPainter::FocusPainter() :
      focus_painters()
{
   for (int i = 0 ; i < NUM_FOCUS_DRAW_TYPES ; ++i) {
      focus_painters[i] = default_focus_painters[i];
   }
}



void FocusPainter::SetFocusPainter(FocusPainterObject* painter , FOCUS_DRAW_TYPE draw_type) {
   EAGLE_ASSERT((draw_type >= 0) && (draw_type < NUM_FOCUS_DRAW_TYPES));
   if (painter) {
      focus_painters[draw_type] = painter;
   }
   else {
      focus_painters[draw_type] = default_focus_painters[draw_type];
   }
}
   


FocusPainterObject* FocusPainter::GetPainter(FOCUS_DRAW_TYPE draw_type) {
   EAGLE_ASSERT((draw_type >= 0) && (draw_type < NUM_FOCUS_DRAW_TYPES));
   return focus_painters[draw_type];
}



void FocusPainter::PaintFocus(EagleGraphicsContext* window , const WidgetBase* widget , int xpos , int ypos , FOCUS_DRAW_TYPE draw_type) {
   PaintFocus(window , widget->Area() , widget->WCols() , xpos , ypos , draw_type);
}



void FocusPainter::PaintFocus(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos , FOCUS_DRAW_TYPE draw_type) {
   EAGLE_ASSERT((draw_type >= 0) && (draw_type < NUM_FOCUS_DRAW_TYPES));
   focus_painters[draw_type]->PaintFocus(window , warea , colors , xpos , ypos);
}
   


FocusPainter default_focus_painter;/// Make sure default_focus_painter is instantiated after default_focus_painters!!!




/**
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


/// Draws 2 color (HLCOL, SDCOL, HLCOL) striped high contrast border
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
void WidgetSolidBorderPainterShadow(EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos) {
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



std::string PrintWidgetDrawFunctionName(WIDGET_DRAW_FUNC draw_func) {
   stringstream ss;
   if (!draw_func) {ss << "NULL function";}
   else if (draw_func == WidgetBGImagePainterCenter)            {ss << "WidgetBGImagePainterCenter";}
   else if (draw_func == WidgetBGImagePainterStretch)           {ss << "WidgetBGImagePainterStretch";}
   else if (draw_func == WidgetBGImagePainterCenterClientArea)  {ss << "WidgetBGImagePainterCenterClientArea";}
   else if (draw_func == WidgetBGImagePainterStretchClientArea) {ss << "WidgetBGImagePainterStretchClientArea";}
   else if (draw_func == WidgetNPPainter)                       {ss << "WidgetNPPainter";}
   else if (draw_func == WidgetBorderPainterContrast)           {ss << "WidgetBorderPainterContrast";}
   else if (draw_func == WidgetBorderPainterShadow)             {ss << "WidgetBorderPainterShadow";}
   else if (draw_func == WidgetBorderedImagePainterCenter)      {ss << "WidgetBorderedImagePainterCenter";}
   else if (draw_func == WidgetBorderedImagePainterStretch)     {ss << "WidgetBorderedImagePainterStretch";}
   else {ss << StringPrintF("Unknown function (%p)",draw_func);}

   return ss.str();
}


//*/

