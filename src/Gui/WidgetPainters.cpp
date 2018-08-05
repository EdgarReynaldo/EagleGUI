
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
 *    EAGLE
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2018+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */

 
 
#include "Eagle/Gui/WidgetPainters.hpp"

#include "Eagle/GraphicsContext.hpp"
#include "Eagle/Gui/WidgetBase.hpp"



WidgetPainter default_widget_painter(HeapObject<WidgetPainterBase>(new WidgetPainterBasic()));



/// ---------------      WidgetPainterBase     ----------------------






/// --------------      WidgetPainterBasic     ------------------------





void WidgetPainterBasic::PaintSolid(EagleGraphicsContext* win , const NPAREA& np , EagleColor c) {
   win->DrawFilledRectangle(np.GetRow(VCELL_TOP) , c);
   win->DrawFilledRectangle(np.GetRow(VCELL_BOTTOM) , c);
   win->DrawFilledRectangle(np.GetNPCell(HCELL_LEFT , VCELL_CENTER) , c);
   win->DrawFilledRectangle(np.GetNPCell(HCELL_RIGHT , VCELL_CENTER) , c);
}



void WidgetPainterBasic::PaintRounded(EagleGraphicsContext* win , const NPAREA& np , EagleColor c) {
   win->DrawFilledRectangle(np.GetNPCell(HCELL_LEFT   , VCELL_CENTER) , c);
   win->DrawFilledRectangle(np.GetNPCell(HCELL_RIGHT  , VCELL_CENTER) , c);
   win->DrawFilledRectangle(np.GetNPCell(HCELL_CENTER , VCELL_TOP   ) , c);
   win->DrawFilledRectangle(np.GetNPCell(HCELL_CENTER , VCELL_BOTTOM) , c);
   win->DrawFilledQuarterEllipse(np.GetNPCell(HCELL_LEFT  , VCELL_TOP)    , QUADRANT_NW , c);
   win->DrawFilledQuarterEllipse(np.GetNPCell(HCELL_RIGHT , VCELL_TOP)    , QUADRANT_NE , c); 
   win->DrawFilledQuarterEllipse(np.GetNPCell(HCELL_RIGHT , VCELL_BOTTOM) , QUADRANT_SE , c);
   win->DrawFilledQuarterEllipse(np.GetNPCell(HCELL_LEFT  , VCELL_BOTTOM) , QUADRANT_SW , c);
}



void WidgetPainterBasic::PaintArea(EagleGraphicsContext* win , const WidgetBase* wb , const NPAREA& np , AREA_FILL_TYPE filltype) {
   if (filltype == AREA_EMPTY) {return;}
   EagleColor bgcol = wb->GetColor(BGCOL);
   EagleColor sdcol = wb->GetColor(SDCOL);
   EagleColor hlcol = wb->GetColor(HLCOL);
   
///      Rectangle topleft = np.GetNPCell(HCELL_LEFT , VCELL_TOP);
///      Rectangle botright = np.GetNPCell(HCELL_RIGHT , VCELL_BOTTOM);
   
   EagleColor temp = hlcol;
   switch (filltype) {
   case AREA_SOLID :
      PaintSolid(win , np , bgcol);
      break;
   case AREA_ROUNDED :
      PaintRounded(win , np , bgcol);
      break;
   case AREA_RCONTRAST :
      hlcol = bgcol;
      bgcol = temp;
   case AREA_CONTRAST :
      {
#warning TODO : FIXME : IMPLEMENT
         /**
         NPAREA np1(np.Area() , BOXAREA(np.left/2 , np.right/2 , np.top/2 , np.bottom/2));
         NPAREA np2(np1.GetCellBox(HCELL_CENTER , VCELL_CENTER) , 
                    BOXAREA(np.left - np.left/2 , np.right - np.right/2 , np.top - np.top/2 , np.bottom - np.bottom/2));
         PaintSolid(win , np1 , hlcol);
         PaintSolid(win , np2 , sdcol);
         //*/
      }
      break;
   case AREA_IMAGE_CENTER :
      {
         VALUE bgimage = "";
         if (!wb->InheritsAttribute("BackgroundImage") && wb->HasAttribute("BackgroundImage")) {
            bgimage = wb->GetAttributeValue("BackgroundImage");
         }
         if (bgimage.length()) {
            EagleImage* img = 0;
#warning TODO : FIXME : BROKEN RESOURCE
///            EagleImage* img = GetImageResource(bgimage);
            Rectangle a = np.GetNPCell(HCELL_CENTER , VCELL_CENTER);
            EagleImage* target = win->GetDrawingTarget();
            target->PushClippingRectangle(a);
            win->Draw(img , a.X() + (a.Width() - img->W())/2 , a.Y() + (a.Height() - img->H())/2);
            target->PopClippingRectangle();
         }
      }
      break;
   case AREA_IMAGE_STRETCH :
      {
         VALUE bgimage = "";
         if (!wb->InheritsAttribute("BackgroundImage") && wb->HasAttribute("BackgroundImage")) {
            bgimage = wb->GetAttributeValue("BackgroundImage");
         }
         if (bgimage.length()) {
            EagleImage* img = 0;
#warning TODO : FIXME : BROKEN RESOURCE
///            EagleImage* img = GetImageResource(bgimage);
            Rectangle a = np.GetNPCell(HCELL_CENTER , VCELL_CENTER);
            EagleImage* target = win->GetDrawingTarget();
            target->PushClippingRectangle(a);
            win->DrawStretchedRegion(img , Rectangle(0,0,img->W(),img->H()) , a , DRAW_NORMAL);
            target->PopClippingRectangle();
         }
      }
      break;
   default :
      break;
   };
}



void WidgetPainterBasic::PaintWidgetBackground(EagleGraphicsContext* win , const WidgetBase* wb) {
   (void)win;
   (void)wb;
/**   
   WIDGETAREA warea = wb->GetWidgetArea();
   if (warea.HasBorder() && wb->AttributeIsSet("BorderFillType")) {
      VALUE tval = wb->GetAttributeValue("BorderFillType");
      
   }
*/
}



void WidgetPainterBasic::PaintWidgetFocus(EagleGraphicsContext* win , const WidgetBase* wb) {
   (void)win;
   (void)wb;
}



