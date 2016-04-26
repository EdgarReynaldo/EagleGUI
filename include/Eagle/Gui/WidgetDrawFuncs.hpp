
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
 *    Copyright 2009-2014+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */


#ifndef WidgetDrawFuncs_HPP
#define WidgetDrawFuncs_HPP

#include "Eagle/GraphicsContext.hpp"
#include "Eagle/Gui/WidgetArea.hpp"
#include "Eagle/Gui/WidgetColorset.hpp"

#include <string>


typedef void (*WIDGET_DRAW_FUNC)(EagleGraphicsContext* , const WidgetArea& , const WidgetColorset& , int , int);

void WidgetBGImagePainterCenter(EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos);
void WidgetBGImagePainterStretch(EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos);

void WidgetBGImagePainterCenterClientArea(EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos);
void WidgetBGImagePainterStretchClientArea(EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos);

//void WidgetDefaultPainter(EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos);
void WidgetNPPainter  (EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos);
void WidgetBorderPainterContrast(EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos);
void WidgetBorderPainterShadow(EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos);
void WidgetBorderedImagePainterCenter(EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos);
void WidgetBorderedImagePainterStretch(EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos);

std::string PrintWidgetDrawFunctionName(WIDGET_DRAW_FUNC draw_func);



#endif // WidgetDrawFuncs_HPP

