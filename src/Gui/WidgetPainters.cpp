
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
#include "Eagle/Gui/Scripting/Parsing.hpp"


WidgetPainter default_widget_painter(HeapObject<WidgetPainterBase>(new WidgetPainterBasic()));



/// ---------------      WidgetPainterBase     ----------------------



WidgetPainterBase::WidgetPainterBase() : 
      window(0),
      widget(0),
      xoffset(0),
      yoffset(0)
{}      



void WidgetPainterBase::GetPainterReady(EagleGraphicsContext* win , const WidgetBase* w , int xpos , int ypos) {
   EAGLE_ASSERT(win);
   EAGLE_ASSERT(w);
   window = win;
   widget = w;
   xoffset = xpos;
   yoffset = ypos;
}




/// --------------      WidgetPainterBasic     ------------------------



void WidgetPainterBasic::PaintBackground() {
   const WidgetColorset& wc = widget->WidgetColors();
   WidgetArea wa = widget->GetWidgetArea();
   wa.MoveBy(Pos2I(xoffset , yoffset));
   
   /// Paint background
   window->DrawFilledRectangle(wa.OuterArea() , wc[BGCOL]);
   
   /// TODO : Implement
   VALUE v = widget->GetAttributeValue("BorderColor");
   if (v.length()) {
      EagleColor c = ParseColor(v);
      NPAREA border = wa.BorderNP();
      border.PaintOutsideSolid(window , c);
   }
}



void WidgetPainterBasic::PaintFocus() {
   const WidgetColorset& wc = widget->WidgetColors();
   WidgetArea wa = widget->GetWidgetArea();
   wa.MoveBy(Pos2I(xoffset , yoffset));
   EagleColor hl = wc[HLCOL];
   EagleColor col(hl.fR() , hl.fG() , hl.fB() , 0.5);
   window->DrawFilledRectangle(wa.OuterArea() , col);
}




















