
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
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file WidgetPainters.hpp
 * @brief Simple class to paint widgets
 * 
 */

#ifndef WidgetPainters_HPP
#define WidgetPainters_HPP


#include "Eagle/SharedMemory.hpp"
#include "Eagle/Image.hpp"
#include "Eagle/Gui/WidgetArea.hpp"



/// Forward declarations of classes we'll be using

class EagleGraphicsContext;
class WidgetBase;
class WidgetColorset;



/*! @enum BG_AREA_PAINT_TYPE
    @brief BG_AREA_PAINT_TYPE controls how the background of a widget is painted
*/

enum BG_AREA_PAINT_TYPE {
   BG_AREA_EMPTY         = 0,///< No background will be painted
   BG_AREA_CSSMODEL      = 1,///< Paints Margin, Border, and Padding
   BG_AREA_FILL          = 2,///< Fills border
   BG_AREA_ROUNDED       = 3,///< Round fills border
   BG_AREA_CONTRAST      = 4,///< Paints contrasted border
   BG_AREA_RCONTRAST     = 5,///< Paints contrasted border with reversed colors
   BG_AREA_CUSTOM        = 6,///< For your own custom paint routine
   NUM_BG_AREA_TYPES     = 7 ///< The maximum number of background area types
};



/*! @enum FOCUS_AREA_PAINT_TYPE
    @brief FOCUS_AREA_PAINT_TYPE controls how the focus is drawn for a widget
*/

enum FOCUS_AREA_PAINT_TYPE {
   FOCUS_AREA_INVISIBLE = 0,///< Do not draw any focus
   FOCUS_AREA_HIGHLIGHT = 1,///< Highlight the active widget white at half alpha
   FOCUS_AREA_OUTLINE   = 2,///< Draw a solid outline in the margin using HLCOL
   FOCUS_AREA_DOTTED    = 3,///< Draw a dotted outline in the margin using HLCOL
   FOCUS_AREA_CONTRAST  = 4,///< Draw a contrasted outline in the margin using HLCOL and SDCOL
   FOCUS_AREA_DDOTTED   = 5,///< Draw a contrasted double dotted outline in the margin using HLCOL and SDCOL
   FOCUS_AREA_CUSTOM    = 6,///< For custom focus drawing
   NUM_FOCUS_AREA_TYPES = 7
};



/*! @class WidgetPainterBase
 *  @brief The WidgetPainterBase class is an abstract base class with pure virtual methods to draw the background
 *         and focus of a widget
 */

class WidgetPainterBase {

protected :
   EagleGraphicsContext* window;
   const WidgetBase* widget;
   WidgetArea warea;
   SHAREDOBJECT<WidgetColorset> wcolors;
   
   EagleImage* bg;
   

   
   virtual void PaintCSSModel();///< Overload this to paint the entire CSS model and boxes
   virtual void PaintFill();    ///< Overload this to 
   virtual void PaintRounded();
   virtual void PaintContrast(EagleColor outer , EagleColor inner);
   virtual void PaintCustom();
   
   virtual void PaintFocusHighlight();
   virtual void PaintFocusOutline();
   virtual void PaintFocusDotted();
   virtual void PaintFocusContrast();
   virtual void PaintFocusDDotted();
   virtual void PaintFocusCustom();
   
public :
   
   BG_AREA_PAINT_TYPE bgtype;///< Controls how the background will be drawn
   FOCUS_AREA_PAINT_TYPE ftype;///< Controls how the focus will be drawn

   WidgetPainterBase();///< Empty constructor, always call GetPainterReady before use
   virtual ~WidgetPainterBase() {}///< Empty virtual destructor
   
   void PaintBackground();///< Paint the background, using the stored bgtype
   void PaintFocus();///< Paint the focus, using the stored ftype
   void PaintBackground(BG_AREA_PAINT_TYPE pt);///< Paint the background, with the given paint type pt
   void PaintFocus(FOCUS_AREA_PAINT_TYPE pt);///< Paint the focus, calls virtual function based on paint type pt
   
   void GetPainterReady(EagleGraphicsContext* win , const WidgetBase* w , int xpos , int ypos);///< Gets the painter ready for painting w
   
};

typedef SHAREDOBJECT<WidgetPainterBase> WidgetPainter;///< A WidgetPainter is a shared pointer to a WidgetPainterBase object



extern WidgetPainter default_widget_painter;///< This is the default widget painter, unless otherwise specified, all widgets use it











#endif // WidgetPainters_HPP




