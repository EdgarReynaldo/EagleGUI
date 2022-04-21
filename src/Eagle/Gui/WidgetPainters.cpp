
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
   PaintOutsideSolid(window , np , widget->GetColor(MARGCOL));
   np = warea.BorderNP();
   PaintOutsideSolid(window , np , widget->GetColor(BORDCOL));
   np = warea.PaddingNP();
   PaintOutsideSolid(window , np , widget->GetColor(PADCOL));
}



void WidgetPainterBase::PaintFill() {
   NPAREA border = warea.BorderNP();
   PaintOutsideSolid(window , border , widget->GetColor(BORDCOL));
}



void WidgetPainterBase::PaintRounded() {
   NPAREA border = warea.BorderNP();
   PaintOutsideRounded(window , border , widget->GetColor(BORDCOL));
}



void WidgetPainterBase::PaintContrast(EagleColor outer , EagleColor inner) {
   NPAREA border = warea.BorderNP();
   PaintOutsideContrast(window , border , outer , inner);
}



void WidgetPainterBase::PaintCustom() {
   (void)0;
}



void WidgetPainterBase::PaintFocusHighlight() {
   EagleColor hl = widget->GetColor(HLCOL);
   EagleColor col(hl.fR() , hl.fG() , hl.fB() , 0.5);
   window->DrawFilledRectangle(warea.OuterArea() , col);
}



void WidgetPainterBase::PaintFocusOutline() {
   BOXAREA b = warea.MarginBox();
   BOXAREA outlinebox(b.left/2 , b.right/2 , b.top/2 , b.bottom/2);
   NPAREA np(warea.OuterArea() , outlinebox);
   PaintOutsideSolid(window , np , widget->GetColor(HLCOL));
   
}



void WidgetPainterBase::PaintFocusDotted() {
   /// TODO : Implement a shader for this
   throw EagleException("WidgetPainterBase::PaintFocusDotted not implemented yet!\n");
}



void WidgetPainterBase::PaintFocusContrast() {
   PaintOutsideContrast(window , warea.OuterNP() , (*wcolors)[HLCOL] , (*wcolors)[SDCOL]);
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
      bg(0),
      bgtype(BG_AREA_CSSMODEL),
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
   PaintBackground(bgtype);
}



void WidgetPainterBase::PaintFocus() {
   PaintFocus(ftype);
}



void WidgetPainterBase::PaintBackground(BG_AREA_PAINT_TYPE pt) {
   switch (pt) {
   case BG_AREA_EMPTY :
      (void)0;
      break;
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
      PaintContrast(widget->GetColor(HLCOL) , widget->GetColor(SDCOL));
      break;
   case BG_AREA_RCONTRAST :
      PaintContrast(widget->GetColor(SDCOL) , widget->GetColor(HLCOL));
      break;
   case BG_AREA_CUSTOM :
      PaintCustom();
      break;
   default : break;
   };
   
}
void WidgetPainterBase::PaintFocus(FOCUS_AREA_PAINT_TYPE pt) {
   switch (pt) {
   case FOCUS_AREA_INVISIBLE :
      (void)0;
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
   default : break;
   };
}

















