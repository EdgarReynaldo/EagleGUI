


#include "Eagle/GraphicsContext.hpp"
#include "Eagle/Gui/WidgetDecorators.hpp"
#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/NinePatchPainters.hpp"



void DecoratorBase::DecorateNext(void* object , EagleGraphicsContext* win , int xpos , int ypos) {
   if (next.get()) {
      next->Decorate(object , win , xpos , ypos);
   }
}



DecoratorBase::DecoratorBase(SHAREDDECORATOR d) :
      next(d)
{}



WidgetDecorator::WidgetDecorator(SHAREDDECORATOR d) :
      DecoratorBase(d)
{}



void WidgetDecorator::Decorate(void* object , EagleGraphicsContext* win , int xpos , int ypos) {
   DecorateWidget((WidgetBase*)object , win , xpos , ypos);
   DecorateNext(object , win , xpos , ypos);
}



DropShadowDecorator::DropShadowDecorator(SHAREDDECORATOR d) :
      WidgetDecorator(d)
{}



void DropShadowDecorator::DecorateWidget(WidgetBase* widget , EagleGraphicsContext* win , int xpos , int ypos) {
   if (!widget) {return;}
   WidgetArea wa = widget->GetWidgetArea();
   wa.MoveBy(Pos2I(xpos , ypos));
   NPAREA npmargin = wa.MarginNP();
   NPAREA npborder = wa.BorderNP();
   const WidgetColorset& wc = *(widget->GetWidgetColorset().get());
   EagleColor hl = wc[HLCOL];
   EagleColor sd = wc[SDCOL];
   
   win->DrawFilledRectangle(npmargin.GetNPCell(CELL_AREA_BOTTOMMIDDLE) , sd);
   win->DrawFilledRectangle(npmargin.GetNPCell(CELL_AREA_BOTTOMRIGHT) , sd);
   win->DrawFilledRectangle(npmargin.GetNPCell(CELL_AREA_MIDDLERIGHT) , sd);
   PaintNPOutsideRounded(win , npborder , hl , sd , 0);
}

