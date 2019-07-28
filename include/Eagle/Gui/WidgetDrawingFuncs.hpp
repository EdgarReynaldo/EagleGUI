
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file WidgetDrawingFuncs.hpp
 * @brief Not currently used
 * 
 * Some typedefs for painting functions
 */

#error UNUSED

#ifndef WidgetDrawingFuncs_HPP
#define WidgetDrawingFuncs_HPP



#include "Eagle/Gui/WidgetArea.hpp"



typedef void (*RECTPAINTFUNC)(EagleGraphicsContext* win , const Rectangle& r , const WidgetColorset& wc , int xpos , int ypos);

typedef void (*NPPAINTFUNC)(EagleGraphicsContext* win , const NPAREA& np , const WidgetColorset& wc , int xpos , int ypos);


typedef void (*WIDGETPAINTFUNC)(EagleGraphicsContext* win , const WidgetBase* widget , int xpos , int ypos); 




#endif // WidgetDrawingFuncs_HPP



