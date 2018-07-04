
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

 
 
#include "Eagle/Gui2/WidgetPainters.hpp"

#include "Eagle/GraphicsContext.hpp"
#include "Eagle/Gui2/WidgetBase2.hpp"



/// ---------------      WidgetPainterBase     ----------------------






/// --------------      WidgetPainterBasic     ------------------------




void PaintWidgetBackground(EagleGraphicsContext* win , const WIDGETBASE* wb) {
   
}



void PaintWidgetFocus(EagleGraphicsContext* win , const WIDGETBASE* wb) {
   
}



void WidgetPainterBasic::SetBgPaintType(BG_AREA_PAINT_TYPE bgtype) {
   bgptype = bgtype;
}



void WidgetPainterBasic::SetFocusPaintType(FOCUS_AREA_PAINT_TYPE ftype)
   fptype = ftype;
}








/// -----------------------     WidgetPainter      -------------------------------



void WidgetPainter::PaintWidgetBackground(EagleGraphicsContext* win , const WIDGETBASE* wb) {
   wpainter.get()->PaintWidgetBackground(win , wb);
}



void WidgetPainter::PaintWidgetFocus(EagleGraphicsContext* win , const WIDGETBASE* wb);
   wpainter.get()->PaintWidgetFocus(win , wb);
}
