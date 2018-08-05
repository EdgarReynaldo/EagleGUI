
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


#include "Eagle/SharedMemory.hpp"


/// types of bg paint
/// FILL, ROUNDEDFILL , IMAGE, CUSTOM

class EagleGraphicsContext;
class WidgetBase;


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

protected :
   EagleGraphicsContext* window;
   const WidgetBase* widget;
   int xoffset;
   int yoffset;


public :
   
   WidgetPainterBase();
   virtual ~WidgetPainterBase() {}
   
   virtual void PaintBackground()=0;
   virtual void PaintFocus()=0;
   
   void GetPainterReady(EagleGraphicsContext* win , const WidgetBase* w , int xpos , int ypos);
};



class WidgetPainterBasic : public WidgetPainterBase {
   
public :
   
   virtual void PaintBackground();
   virtual void PaintFocus();
};

typedef SHAREDOBJECT<WidgetPainterBase> WidgetPainter;

extern WidgetPainter default_widget_painter;





#endif // WidgetPainters_HPP



