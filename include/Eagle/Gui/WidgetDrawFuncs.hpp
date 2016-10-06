
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
 *    Copyright 2009-2016+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */


#ifndef WidgetDrawFuncs_HPP
#define WidgetDrawFuncs_HPP

#include "Eagle/GraphicsContext.hpp"

#include "Eagle/Gui/WidgetArea.hpp"
#include "Eagle/Gui/WidgetColorset.hpp"


///#include "Eagle/Gui/Layout/SplitterLayouts.hpp"

#include <string>





/// Enums to control background and focus drawing types



enum BG_DRAW_TYPE {
   BG_DRAW_BACKGROUND_EMPTY = 0,
   BG_DRAW_BACKGROUND_SOLID = 1,
   BG_DRAW_BACKGROUND_IMAGE_CENTER = 2,
   BG_DRAW_BACKGROUND_IMAGE_STRETCH = 3,
   BG_DRAW_BACKGROUND_IMAGE_CENTER_CLIENT = 4,
   BG_DRAW_BACKGROUND_IMAGE_STRETCH_CLIENT = 5,
   BG_DRAW_BACKGROUND_NINEPATCH = 6,
   BG_DRAW_BACKGROUND_CONTRAST_BORDER = 7,
   BG_DRAW_BACKGROUND_DEPTH_BORDER = 8,
   BG_DRAW_BACKGROUND_SOLID_BORDER = 9,
   BG_DRAW_BACKGROUND_SOLID_SHADOW_BORDER = 10,
   BG_DRAW_BACKGROUND_SOLID_BORDER_IMAGE_CENTER = 11,
   BG_DRAW_BACKGROUND_SOLID_BORDER_IMAGE_STRETCH = 12,
   BG_DRAW_BACKGROUND_CUSTOM = 13
};

#define NUM_BACKGROUND_DRAW_TYPES 14

const char* BgDrawTypeString(BG_DRAW_TYPE draw_type);



enum FOCUS_DRAW_TYPE {
   FOCUS_DRAW_INVISIBLE               = 0,
   FOCUS_DRAW_HIGHLIGHT_OUTLINE       = 1,
   FOCUS_DRAW_HIGHLIGHT_THICK_OUTLINE = 2,
   FOCUS_DRAW_HIGHLIGHT_DOTTED        = 3,
   FOCUS_DRAW_HIGHLIGHT_THICK_DOTTED  = 4,
   FOCUS_DRAW_CUSTOM                  = 5
};


#define NUM_FOCUS_DRAW_TYPES 6

const char* FocusDrawTypeString(FOCUS_DRAW_TYPE draw_type);



/// Miscellaneous drawing functions

void DefaultSplitterDrawFunction(EagleGraphicsContext* window , Rectangle divider_rect , bool horizontal , EagleColor hlcol , EagleColor sdcol);
void DefaultSplitterDrawFunction(EagleGraphicsContext* window , Rectangle divider_rect , bool horizontal , const WidgetColorset& colors);



/// Background painter base



class WidgetBase;



class BackgroundPainterObject {
public :
   virtual ~BackgroundPainterObject() {}
   
   void PaintBackground(EagleGraphicsContext* window , const WidgetBase* widget , int xpos , int ypos);

   virtual void PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos)=0;

   virtual std::string GetPainterName() {return "BackgroundPainterObject";}
};



/// Background painters



class EmptyBackgroundPainter : public BackgroundPainterObject {
public :
   virtual void PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos);

   virtual std::string GetPainterName() {return "EmptyBackgroundPainter";}
};




class SolidBackgroundPainter : public BackgroundPainterObject {
public :
   virtual void PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos);

   virtual void PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , int xpos , int ypos , EagleColor c);

   virtual std::string GetPainterName() {return "SolidBackgroundPainter";}
};



class CenteredImageBackgroundPainter : public BackgroundPainterObject {
public :
   virtual void PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos);

   virtual std::string GetPainterName() {return "CenteredImageBackgroundPainter";}
};



class StretchedImageBackgroundPainter : public BackgroundPainterObject {
public :
   virtual void PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos);

   virtual std::string GetPainterName() {return "StretchedImageBackgroundPainter";}
};




class CenteredClientImageBackgroundPainter : public BackgroundPainterObject {
public :
   virtual void PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos);

   virtual std::string GetPainterName() {return "CenteredClientImageBackgroundPainter";}
};



class StretchedClientImageBackgroundPainter : public BackgroundPainterObject {
public :
   virtual void PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos);

   virtual std::string GetPainterName() {return "StretchedClientImageBackgroundPainter";}
};



class NinePatchImageBackgroundPainter : public BackgroundPainterObject {
public :
   virtual void PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos);

   virtual std::string GetPainterName() {return "NinePatchImageBackgroundPainter";}
};



class ContrastBorderBackgroundPainter : public BackgroundPainterObject {
public :
   virtual void PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos);
   virtual void PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , int xpos , int ypos , EagleColor hlcol , EagleColor sdcol);

   virtual std::string GetPainterName() {return "ContrastBorderBackgroundPainter";}
};




class DepthBorderBackgroundPainter : public BackgroundPainterObject {
public :
   virtual void PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos);
   virtual void PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , int xpos , int ypos , EagleColor fgcol , EagleColor sdcol);

   virtual std::string GetPainterName() {return "DepthBorderBackgroundPainter";}
};




class SolidBorderBackgroundPainter : public BackgroundPainterObject {
public :
   virtual void PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos);/// Uses FGCOL
   virtual void PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , int xpos , int ypos , EagleColor color);

   virtual std::string GetPainterName() {return "SolidBorderBackgroundPainter";}
};




class SolidShadowBorderBackgroundPainter : public BackgroundPainterObject {
public :
   virtual void PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos);

   virtual std::string GetPainterName() {return "SolidShadowBorderBackgroundPainter";}
};



class BorderedCenteredImageBackgroundPainter : public BackgroundPainterObject {
public :
   virtual void PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos);

   virtual std::string GetPainterName() {return "BorderedCenteredImageBackgroundPainter";}
};



class BorderedStretchedImageBackgroundPainter : public BackgroundPainterObject {
public :
   virtual void PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos);

   virtual std::string GetPainterName() {return "BorderedStretchedImageBackgroundPainter";}
};



class CustomBackgroundPainter : public BackgroundPainterObject {
public :
   virtual void PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos);

   virtual std::string GetPainterName() {return "CustomBackgroundPainter";}
};



/// Background painter



class BackgroundPainter {
   
private :
   static BackgroundPainterObject* const default_bg_painters[NUM_BACKGROUND_DRAW_TYPES];

   BackgroundPainterObject* painters[NUM_BACKGROUND_DRAW_TYPES];
   
public :
///   BG_DRAW_TYPE bg_draw_type;
   static EmptyBackgroundPainter                  empty_background_painter;
   static SolidBackgroundPainter                  solid_background_painter;
   static CenteredImageBackgroundPainter          centered_image_background_painter;
   static StretchedImageBackgroundPainter         stretched_image_background_painter;
   static CenteredClientImageBackgroundPainter    centered_client_image_background_painter;
   static StretchedClientImageBackgroundPainter   stretched_client_image_background_painter;
   static NinePatchImageBackgroundPainter         ninepatch_image_background_painter;
   static ContrastBorderBackgroundPainter         contrast_border_background_painter;
   static DepthBorderBackgroundPainter            depth_border_background_painter;
   static SolidBorderBackgroundPainter            solid_border_background_painter;
   static SolidShadowBorderBackgroundPainter      solid_shadow_border_background_painter;
   static BorderedCenteredImageBackgroundPainter  bordered_centered_image_background_painter;
   static BorderedStretchedImageBackgroundPainter bordered_stretched_image_background_painter;
   static CustomBackgroundPainter                 custom_background_painter;

   BackgroundPainter();
   

   void SetPainter(BG_DRAW_TYPE bg_draw_type , BackgroundPainterObject* bg_painter);
   
   BackgroundPainterObject* GetPainter(BG_DRAW_TYPE bg_draw_type);
   
   void PaintBackground(EagleGraphicsContext* window , const WidgetBase* widget , int xpos , int ypos , BG_DRAW_TYPE bg_draw_type);

   void PaintBackground(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos , BG_DRAW_TYPE bg_draw_type);
};



extern BackgroundPainter default_background_painter;



/// Focus painter base



class FocusPainterObject {
public :
   virtual ~FocusPainterObject() {}

   virtual void PaintFocus(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos);
   
   virtual std::string GetPainterName() {return "FocusPainterObject";}
};



/// Focus painters



class InvisibleFocusPainter : public FocusPainterObject {
public :
   virtual void PaintFocus(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos);
   
   virtual std::string GetPainterName() {return "InvisibleFocusPainter";}
};



class HighlightOutlineFocusPainter : public FocusPainterObject {
public :
   virtual void PaintFocus(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos);

   virtual std::string GetPainterName() {return "HighlightOutlineFocusPainter";}
};



class HighlightThickOutlineFocusPainter : public FocusPainterObject {
   float thickness;
public :
   HighlightThickOutlineFocusPainter() :
      thickness(2.0)
   {}
   
   virtual void PaintFocus(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos);
   
   virtual std::string GetPainterName() {return "HighlightThickOutlineFocusPainter";}

   void SetOutlineThickness(float outline_thickness) {
      thickness = (outline_thickness<0.0f)?0.0f:outline_thickness;
   }
};



class HighlightDottedOutlineFocusPainter : public FocusPainterObject {
public :
   virtual void PaintFocus(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos);

   virtual std::string GetPainterName() {return "HighlightDottedOutlineFocusPainter";}
};



class HighlightThickDottedOutlineFocusPainter : public FocusPainterObject {
   int thickness;
public :
   HighlightThickDottedOutlineFocusPainter() :
         thickness(2)
   {}
   
   virtual void PaintFocus(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos);

   virtual std::string GetPainterName() {return "HighlighThickDottedOutlineFocusPainter";}
};



class CustomFocusPainter : public FocusPainterObject {
public :
   virtual void PaintFocus(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos);
   
   virtual std::string GetPainterName() {return "CustomFocusPainter";}
};



/// ---------------------      FocusPainter      ----------------------------------



class FocusPainter {

protected :
   static FocusPainterObject* const default_focus_painters[NUM_FOCUS_DRAW_TYPES];

   FocusPainterObject* focus_painters[NUM_FOCUS_DRAW_TYPES];
   
public :

   static InvisibleFocusPainter                   invisible_focus_painter;
   static HighlightOutlineFocusPainter            highlight_outline_focus_painter;
   static HighlightThickOutlineFocusPainter       highlight_thick_outline_focus_painter;
   static HighlightDottedOutlineFocusPainter      highlight_dotted_outline_focus_painter;
   static HighlightThickDottedOutlineFocusPainter highlight_thick_dotted_outline_focus_painter;
   static CustomFocusPainter                      custom_focus_painter;

   FocusPainter();
   

   void SetFocusPainter(FocusPainterObject* painter , FOCUS_DRAW_TYPE draw_type);

   FocusPainterObject* GetPainter(FOCUS_DRAW_TYPE draw_type);
   
   void PaintFocus(EagleGraphicsContext* window , const WidgetBase* widget , int xpos , int ypos , FOCUS_DRAW_TYPE draw_type);

   void PaintFocus(EagleGraphicsContext* window , const WidgetArea& warea , const WidgetColorset& colors , int xpos , int ypos , FOCUS_DRAW_TYPE draw_type);
};



extern FocusPainter default_focus_painter;




/**



typedef void (*WIDGET_DRAW_FUNC)(EagleGraphicsContext* , const WidgetArea& , const WidgetColorset& , int , int);

/// Centers BG Image on WidgetArea
void WidgetBGImagePainterCenter(EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos);

/// Stretches BG Image to fit WidgetArea
void WidgetBGImagePainterStretch(EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos);

/// Centers BG Image on inner area
void WidgetBGImagePainterCenterClientArea(EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos);

/// Stretches BG Image to fit inner area
void WidgetBGImagePainterStretchClientArea(EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos);

//void WidgetDefaultPainter(EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos);

/// Paints the NinePatch defined by the WidgetArea
void WidgetNPPainter  (EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos);

/// Draws 2 color (HLCOL, SDCOL, HLCOL) striped high contrast border
void WidgetBorderPainterContrast(EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos);

/// Self explanatory - solid shadow covering the margins
void WidgetSolidBorderPainterShadow(EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos);

/// Draws a solid border around an image centered on the inner area
void WidgetBorderedImagePainterCenter(EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos);

/// Draws a solid border around an image stretched to fit the inner area
void WidgetBorderedImagePainterStretch(EagleGraphicsContext* win , const WidgetArea& a , const WidgetColorset& c , int xpos , int ypos);

std::string PrintWidgetDrawFunctionName(WIDGET_DRAW_FUNC draw_func);

//*/

#endif // WidgetDrawFuncs_HPP

