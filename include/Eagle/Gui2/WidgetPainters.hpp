
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

 
 
#ifndef WidgetPainters_HPP
#define WidgetPainters_HPP


#include "Eagle/Gui2/WidgetArea2.hpp"




/// types of bg paint
/// FILL, ROUNDEDFILL , IMAGE, CUSTOM

class EagleGraphicsContext;
class WIDGETBASE;


enum AREA_FILL_TYPE {
   AREA_EMPTY          = 0,
   AREA_SOLID          = 1,
   AREA_ROUNDED        = 2,
   AREA_CONTRAST       = 3,
   AREA_RCONTRAST      = 4,
   AREA_IMAGE_CENTER   = 5,
   AREA_IMAGE_STRETCH  = 6,
   NUM_AREA_FILL_TYPES = 7
};


enum BG_AREA_PAINT_TYPE {
   BG_AREA_EMPTY         = 0,
   BG_AREA_FILL          = 1,
   BG_AREA_ROUNDED_FILL  = 2,
   BG_AREA_IMAGE_CENTER  = 3,
   BG_AREA_IMAGE_STRETCH = 4,
   BG_AREA_CUSTOM        = 5,
   NUM_BG_AREA_TYPES     = 6
};
enum FOCUS_AREA_PAINT_TYPE {
   FOCUS_AREA_INVISIBLE = 0,
   FOCUS_AREA_OUTLINE   = 1,
   FOCUS_AREA_DOTTED    = 2,
   FOCUS_AREA_CONTRAST  = 3,
   FOCUS_AREA_CUSTOM    = 4,
   NUM_FOCUS_AREA_TYPES = 5
};


/// Abstract base class
class WidgetPainterBase {
   
public :
   WidgetPainterBase() {}
   virtual ~WidgetPainterBase() {}
   
   virtual void PaintWidgetBackground(EagleGraphicsContext* win , const WIDGETBASE* wb)=0;
   virtual void PaintWidgetFocus(EagleGraphicsContext* win , const WIDGETBASE* wb)=0;
};



class WidgetPainterBasic : public WidgetPainterBase {
   
   
protected :
/*   
enum AREA_FILL_TYPE {
   AREA_EMPTY          = 0,
   AREA_SOLID          = 1,
   AREA_ROUNDED        = 2,
   AREA_CONTRAST       = 3,
   AREA_RCONTRAST      = 4,
   AREA_IMAGE_CENTER   = 5,
   AREA_IMAGE_STRETCH  = 6,
   NUM_AREA_FILL_TYPES = 7
};
*/   
   
   void PaintSolid(EagleGraphicsContext* win , const NPAREA& np , EagleColor c);
   void PaintSolid(EagleGraphicsContext* win , const NPAREA& np , EagleColor c) {
      win->DrawFilledRectangle(np.GetRow(VCELL_TOP) , c);
      win->DrawFilledRectangle(np.GetRow(VCELL_BOTTOM) , c);
      win->DrawFilledRectangle(np.GetNPCell(HCELL_LEFT , VCELL_CENTER) , c);
      win->DrawFilledRectangle(np.GetNPCell(HCELL_RIGHT , VCELL_CENTER) , c);
   }
   void PaintRounded(EagleGraphicsContext* win , const NPAREA& np , EagleColor c);
   void PaintRounded(EagleGraphicsContext* win , const NPAREA& np , EagleColor c) {
      win->DrawFilledRectangle(np.GetNPCell(HCELL_LEFT , VCELL_CENTER) , bgcol);
      win->DrawFilledRectangle(np.GetNPCell(HCELL_RIGHT , VCELL_CENTER) , bgcol);
      win->DrawFilledRectangle(np.GetNPCell(HCELL_CENTER , VCELL_TOP) , bgcol);
      win->DrawFilledRectangle(np.GetNPCell(HCELL_CENTER , VCELL_BOTTOM) , bgcol);
      win->DrawFilledQuarterEllipse(np.GetNPCell(HCELL_LEFT  , VCELL_TOP)    , QUADRANT_NW , bgcol);
      win->DrawFilledQuarterEllipse(np.GetNPCell(HCELL_RIGHT , VCELL_TOP)    , QUADRANT_NE , bgcol); 
      win->DrawFilledQuarterEllipse(np.GetNPCell(HCELL_RIGHT , VCELL_BOTTOM) , QUADRANT_SE , bgcol);
      win->DrawFilledQuarterEllipse(np.GetNPCell(HCELL_LEFT  , VCELL_BOTTOM) , QUADRANT_SW , bgcol);
   }
   void PaintArea(EagleGraphicsContext* win , const WIDGETBASE* wb , const NPAREA& np , AREA_FILL_TYPE filltype);
   void PaintArea(EagleGraphicsContext* win , const WIDGETBASE* wb , const NPAREA& np , AREA_FILL_TYPE filltype) {
      if (filltype == AREA_EMPTY) {return;}
      EagleColor bgcol = wb->GetColor("BGCOL");
      EagleColor sdcol = wb->GetColor("BGCOL");
      EagleColor hlcol = wb->GetColor("BGCOL");
      
///      Rectangle topleft = np.GetNPCell(HCELL_LEFT , VCELL_TOP);
///      Rectangle botright = np.GetNPCell(HCELL_RIGHT , VCELL_BOTTOM);
      
      switch (filltype) {
      case AREA_SOLID :
         PaintSolid(win , np , bgcol);
         break;
      case AREA_ROUNDED :
         PaintRounded(win , np , bgcol);
         break;
      case AREA_RCONTRAST :
         EagleColor temp = hlcol;
         hlcol = bgcol;
         bgcol = temp;
      case AREA_CONTRAST :
         {
            NPAREA np1(np.Area() , BOXAREA(np.left/2 , np.right/2 , np.top/2 , np.bottom/2));
            NPAREA np2(np1.GetCellBox(HCELL_CENTER , VCELL_CENTER) , 
                       BOXAREA(np.left - np.left/2 , np.right - np.right/2 , np.top - np.top/2 , np.bottom - np.bottom/2))
            PaintSolid(win , np1 , hlcol);
            PaintSolid(win , np2 , sdcol);
         }
         break;
      case AREA_IMAGE_CENTER :
         {
            VALUE bgimage = "";
            if (!wb->InheritsAttribute("BackgroundImage") && wb->HasAttribute("BackgroundImage")) {
               bgimage = wb->GetAttributeValue("BackgroundImage");
            }
            if (bgimage.length()) {
               EagleImage* img = GetImageResource(bgimage);
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
               EagleImage* img = GetImageResource(bgimage);
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
   
   
   
   
   
   
   
   virtual void PaintMargin(const NPAREA& npmargin);
   virtual void PaintBorder(const NPAREA& npborder);
   virtual void PaintPadding(const NPAREA& nppadding);
   virtual void PaintInnerArea(const Rectangle& inner);
   
void PaintMargin(const NPAREA& npmargin) {
   
}
void PaintBorder(const NPAREA& npborder) {
   
}
void PaintPadding(const NPAREA& nppadding) {
   
}
void PaintInnerArea(const Rectangle& inner) {
   
}

public :
   
   
   
   virtual void PaintWidgetBackground(EagleGraphicsContext* win , const WIDGETBASE* wb);
   virtual void PaintWidgetFocus(EagleGraphicsContext* win , const WIDGETBASE* wb);

};



class WidgetPainter {
protected :
   std:;shared_ptr<WidgetPainterBase> wpainter;
public :
   
   void PaintWidgetBackground(EagleGraphicsContext* win , const WIDGETBASE* wb);
   void PaintWidgetFocus(EagleGraphicsContext* win , const WIDGETBASE* wb);


};

#endif // WidgetPainters_HPP
