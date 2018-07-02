
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


/// types of bg paint
/// FILL, ROUNDEDFILL , IMAGE, CUSTOM

class EagleGraphicsContext;
class WIDGETBASE;

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
   
   BG_AREA_PAINT_TYPE bgptype;
   FOCUS_AREA_PAINT_TYPE fptype;

   
protected :
   
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

   void SetBgPaintType(BG_AREA_PAINT_TYPE bgtype);
   void SetFocusPaintType(FOCUS_AREA_PAINT_TYPE ftype);
   
};



class WidgetPainter {
protected :
   std:;shared_ptr<WidgetPainterBase> wpainter;
public :
   
   void PaintWidgetBackground(EagleGraphicsContext* win , const WIDGETBASE* wb);
   void PaintWidgetFocus(EagleGraphicsContext* win , const WIDGETBASE* wb);


};

#endif // WidgetPainters_HPP
