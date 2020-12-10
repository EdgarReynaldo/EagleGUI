
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
 * @file Slider.hpp
 * @brief Simple slider class
 */

#ifndef Slider_HPP
#define Slider_HPP



#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/Gui/Layout/LayoutRectangle.hpp"
#include "Eagle/Gui/Button.hpp"



extern const unsigned int TOPIC_SLIDER;
   
enum SLIDER_MSGS {
   SLIDER_VALUE_CHANGED = 1
};


/**! @class Slider
 *   @brief Simple 1D slider class, can use custom button, can be configured to display left to right,
 *   right to left (inverted) , up to down , down to up (inverted)
 */

class Slider : public WidgetBase , public EagleEventListener {

protected :
   int slider_pos;
   int slider_max;
   double slider_percent;
   bool drag;
   int slider_start;
   int mxstart;
   int mystart;
   bool invert;
   bool horizontal;

   BasicButton basic_handle_button;
   BasicButton* handle;



   virtual int PrivateHandleEvent(EagleEvent ee);
   virtual int PrivateCheckInputs();
   virtual int PrivateUpdate(double dt);
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   
   virtual void OnAreaChanged();

   void ResizeButton();

   virtual void RespondToEvent(EagleEvent e , EagleThread* thread = MAIN_THREAD);

   virtual void  SetRedrawFlag() {SetBgRedrawFlag();}
public :
   
   Slider(std::string classname = "Slider" , std::string objname = "Nemo" , bool vertical = true , bool inverted = false);

   void SetButton(BasicButton* btn);///< Use a custom button, pass NULL to revert to default button

   void SetPercent(double pct , bool sendmessage = true);///< Set the percent of the slider (from 0.0 to 1.0)

   int GetSliderValue(double pct);///< Return pct*slider_max
   int GetSliderMax();

   double GetPercent();///< [0.0,1.0]
   double GetInverted();///< [0.0,1.0]

   void SetInversion(bool invert_me);///< Pass true to invert (use right to left or use bottom to top)
   void SetOrientation(bool horizontal_slider);///< Pass true to use a horizontal scrollbar, false for vertical
};




/**! @class Slider2D
 *   @brief Simple 2D slider class, can use custom button, can be configured to display left to right,
 *   right to left (invertx) , up to down , down to up (inverty)
 */

class Slider2D : public WidgetBase , public EagleEventListener {

protected :
   int slider_xpos;
   int slider_xmax;
   double slider_xpercent;

   int slider_ypos;
   int slider_ymax;
   double slider_ypercent;

   bool drag;
   int slider_xstart;
   int slider_ystart;
   int mxstart;
   int mystart;
   bool invertx;
   bool inverty;

   BasicButton basic_handle_button;
   BasicButton* handle;



   virtual int PrivateHandleEvent(EagleEvent ee);
   virtual int PrivateCheckInputs();
   virtual int PrivateUpdate(double dt);
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   
   virtual void OnAreaChanged();

   void ResizeButton();

   virtual void RespondToEvent(EagleEvent e , EagleThread* thread = MAIN_THREAD);


public :
   
   Slider2D(std::string classname = "Slider" , std::string objname = "Nemo" , bool invert_x = false , bool invert_y = true);

   void SetButton(BasicButton* btn);///< Use a custom button, pass NULL to revert to default button

   void SetPercent(double pctx , double pcty , bool sendmessage = true);///< Set the percent of the slider (from 0.0 to 1.0) on each axis

   int GetSliderXValue(double pct);///< Return pct*slider_xmax
   int GetSliderYValue(double pct);///< Return pct*slider_ymax

   int GetSliderXMax();
   int GetSliderYMax();

   double GetXPercent();///< [0.0,1.0]
   double GetYPercent();///< [0.0,1.0]
   double GetXInverted();///< [0.0,1.0]
   double GetYInverted();///< [0.0,1.0]
   double GetXActual();///< [0.0,1.0]
   double GetYActual();///< [0.0,1.0]

   void SetInversion(bool invert_x , bool invert_y);///< Pass true to invert x and/or y (use right to left or use bottom to top)
};



#endif // Slider_HPP



