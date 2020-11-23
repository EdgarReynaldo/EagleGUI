
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
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */

 
 
#include "Eagle/Gui/WidgetPainters.hpp"

#include "Eagle/GraphicsContext.hpp"
#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/Gui/Scripting/Parsing.hpp"


WidgetPainter default_widget_painter(HeapObject<WidgetPainterBase>(new WidgetPainterBase()));



/// ---------------      WidgetPainterBase     ----------------------



void WidgetPainterBase::PaintCSSModel() {
   
   NPAREA np;
   np = warea.OuterNP();
   np.PaintOutsideSolid(window , (*wcolors)[MARGCOL]);
   np = warea.BorderNP();
   np.PaintOutsideSolid(window , (*wcolors)[BORDCOL]);
   np = warea.PaddingNP();
   np.PaintOutsideSolid(window , (*wcolors)[PADCOL]);
}



void WidgetPainterBase::PaintFill() {
   NPAREA border = warea.BorderNP();
   border.PaintOutsideSolid(window , (*wcolors)[BORDCOL]);
}



void WidgetPainterBase::PaintRounded() {
   NPAREA border = warea.BorderNP();
   border.PaintOutsideRounded(window , (*wcolors)[BORDCOL]);
}



void WidgetPainterBase::PaintContrast(EagleColor outer , EagleColor inner) {
   NPAREA border = warea.BorderNP();
   border.PaintOutsideContrast(window , outer , inner);
}



void WidgetPainterBase::PaintCustom() {
   (void)0;
}



void WidgetPainterBase::PaintFocusHighlight() {
   EagleColor hl = (*wcolors)[HLCOL];
   EagleColor col(hl.fR() , hl.fG() , hl.fB() , 0.5);
   window->DrawFilledRectangle(warea.OuterArea() , col);
}



void WidgetPainterBase::PaintFocusOutline() {
   BOXAREA b = warea.MarginBox();
   BOXAREA outlinebox(b.left/2 , b.right/2 , b.top/2 , b.bottom/2);
   NPAREA np(warea.OuterArea() , outlinebox);
   np.PaintOutsideSolid(window , (*wcolors)[HLCOL]);
   
}



void WidgetPainterBase::PaintFocusDotted() {
   /// TODO : Implement a shader for this
   throw EagleException("WidgetPainterBase::PaintFocusDotted not implemented yet!\n");
}



void WidgetPainterBase::PaintFocusContrast() {
   warea.OuterNP().PaintOutsideContrast(window , (*wcolors)[HLCOL] , (*wcolors)[SDCOL]);
}



void WidgetPainterBase::PaintFocusDDotted() {
   /// TODO : Implement a shader for this
   throw EagleException("WidgetPainterBase::PaintFocusDDotted not implemented yet!\n");
}



void WidgetPainterBase::PaintFocusCustom() {
   (void)0;
}



WidgetPainterBase::WidgetPainterBase() : 
      window(0),
      widget(0),
      warea(),
      wcolors(),
      bgtype(BG_AREA_EMPTY),
      ftype(FOCUS_AREA_OUTLINE)
{}      



void WidgetPainterBase::GetPainterReady(EagleGraphicsContext* win , const WidgetBase* w , int xpos , int ypos) {
   EAGLE_ASSERT(win);
   EAGLE_ASSERT(w);
   window = win;
   widget = w;
   warea = w->GetWidgetArea();
   warea.MoveBy(Pos2I(xpos , ypos));
   wcolors = w->GetWidgetColorset();
}



void WidgetPainterBase::PaintBackground() {
   if (bgtype == BG_AREA_EMPTY || bgtype == NUM_BG_AREA_TYPES) {return;}
   
   switch(bgtype) {
   case BG_AREA_CSSMODEL :
      PaintCSSModel();
      break;
   case BG_AREA_FILL :
      PaintFill();
      break;
   case BG_AREA_ROUNDED :
      PaintRounded();
      break;
   case BG_AREA_CONTRAST :
      PaintContrast((*wcolors)[HLCOL] , (*wcolors)[SDCOL]);
      break;
   case BG_AREA_RCONTRAST :
      PaintContrast((*wcolors)[SDCOL] , (*wcolors)[HLCOL]);
      break;
   default :
      break;
   }
}



/*! \brief FOCUS_AREA_PAINT_TYPE controls how the focus is drawn for a widget 

enum FOCUS_AREA_PAINT_TYPE {
   FOCUS_AREA_INVISIBLE = 0,
   FOCUS_AREA_HIGHLIGHT = 1,
   FOCUS_AREA_OUTLINE   = 2,
   FOCUS_AREA_DOTTED    = 3,
   FOCUS_AREA_CONTRAST  = 4,
   FOCUS_AREA_DDOTTED   = 5,
   FOCUS_AREA_CUSTOM    = 6,
   NUM_FOCUS_AREA_TYPES = 7
};

*/



void WidgetPainterBase::PaintFocus() {
   switch (ftype) {
   case FOCUS_AREA_INVISIBLE :
      return;
      break;
   case FOCUS_AREA_HIGHLIGHT :
      PaintFocusHighlight();
      break;
   case FOCUS_AREA_OUTLINE :
      PaintFocusOutline();
      break;
   case FOCUS_AREA_DOTTED :
      PaintFocusDotted();
      break;
   case FOCUS_AREA_CONTRAST :
      PaintFocusContrast();
      break;
   case FOCUS_AREA_DDOTTED :
      PaintFocusDDotted();
      break;
   case FOCUS_AREA_CUSTOM :
      PaintFocusCustom();
      break;
   default :
      break;
   }
}




















