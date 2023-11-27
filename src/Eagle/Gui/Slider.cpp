
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
 * @file Slider.cpp
 * @brief The implementations for the Slider and Slider2D classes
 */

#include "Eagle/Gui/Slider.hpp"


const unsigned int TOPIC_SLIDER = NextFreeTopicId();


int Slider::PrivateHandleEvent(EagleEvent ee) {
   int ret = DIALOG_OKAY;
   
//   EagleLog() << "Event " << EagleEventName(ee.type) << " received.\n";
   
   
   if (ee.type == EAGLE_EVENT_MOUSE_AXES) {
      if (!drag) {
         mxstart = ee.mouse.x;
         mystart = ee.mouse.y;
      }
      else {
         if (horizontal) {
            SetPercent((double)(slider_start + (invert?-1:1)*(ee.mouse.x - mxstart))/slider_max);
         }
         else {
            SetPercent((double)(slider_start + (invert?-1:1)*(ee.mouse.y - mystart))/slider_max);
         }
         return DIALOG_INPUT_USED;
      }
   }
   if (ee.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN || ee.type == EAGLE_EVENT_MOUSE_BUTTON_UP) {
      if (ee.mouse.button == 1) {
         ret = handle->HandleEvent(ee);
      }
   }
   if (ee.type == EAGLE_EVENT_WIDGET) {
      if (ee.widget.from == handle) {
         if (ee.widget.msgs == BUTTON_CLICKED) {
            drag = true;
            slider_start = slider_pos;
         }
         else if (ee.widget.msgs == BUTTON_RELEASED) {
            drag = false;
         }
         return DIALOG_OKAY;
      }
   }
   return ret;
}



int Slider::PrivateCheckInputs() {
   return DIALOG_OKAY;
}



int Slider::PrivateUpdate(double dt) {
   return handle->Update(dt);
}



void Slider::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   Rectangle inner = warea.InnerArea();
   inner.DrawGuiRectDown(win , GetColor(MGCOL) , GetColor(SDCOL));
   handle->Display(win , xpos , ypos);
}



void Slider::OnAreaChanged() {
   if (horizontal) {
      slider_max = InnerArea().W() - handle->OuterArea().W();
   }
   else {
      slider_max = InnerArea().H() - handle->OuterArea().H();
   }
   if (slider_max < 1) {slider_max = 1;}
   SetPercent(slider_percent);
}



void Slider::ResizeButton() {
   Rectangle inner = warea.InnerArea();
   if (horizontal) {
      slider_max = InnerArea().W() - handle->InnerArea().W();
      if (invert) {
         handle->SetWidgetArea(WidgetArea(Rectangle(inner.X() + slider_max - slider_pos , inner.Y() , inner.W()/10 , inner.H()) , false));
      }
      else {
         handle->SetWidgetArea(WidgetArea(Rectangle(inner.X() + slider_pos , inner.Y() , inner.W()/10 , inner.H()) , false));
      }
   }
   else {
      slider_max = InnerArea().H() - handle->InnerArea().H();
      if (invert) {
         handle->SetWidgetArea(WidgetArea(Rectangle(inner.X() , inner.Y() + slider_max - slider_pos , inner.W() , inner.H()/10)) , false);
      }
      else {
         handle->SetWidgetArea(WidgetArea(Rectangle(inner.X() , inner.Y() + slider_pos , inner.W() , inner.H()/10)) , false);
      }
   }
}



void Slider::RespondToEvent(EagleEvent e , EagleThread* thread) {
   (void)thread;
   PrivateHandleEvent(e);
}



Slider::Slider(std::string classname , std::string objname , bool vertical , bool inverted) :
      WidgetBase(classname , objname),
      slider_pos(0),
      slider_max(1),
      slider_percent(0.0),
      drag(false),
      slider_start(0),
      mxstart(-1),
      mystart(-1),
      invert(inverted),
      horizontal(!vertical),
      basic_handle_button(),
      handle(0)
{
   basic_handle_button.SetWidgetArea(WidgetArea(Rectangle(0 , 0 , InnerArea().W() , InnerArea().H()/10)) , false);
   SetButton(&basic_handle_button);
}



void Slider::SetButton(BasicButton* btn) {
   drag = false;
   if (handle) {
      StopListeningTo(handle);
      handle->SetParent(NULL);
   }
   handle = btn?btn:&basic_handle_button;
   handle->SetActionType(SPRING_BTN);
   ListenTo(handle);
   handle->SetParent(this);
   SetPercent(slider_percent);
}



void Slider::SetPercent(double pct , bool sendmessage) {
   double old = slider_percent;
   if (pct < 0.0) {pct = 0.0;}
   if (pct > 1.0) {pct = 1.0;}
   slider_percent = pct;
   slider_pos = GetSliderValue(pct);
   ResizeButton();
   SetRedrawFlag();
   if (sendmessage && (old != slider_percent)) {
      QueueUserMessage(WidgetMsg(this , TOPIC_SLIDER , SLIDER_VALUE_CHANGED));
   }
}



int Slider::GetSliderValue(double pct) {
   return pct*slider_max;
}



int Slider::GetSliderMax() {
   return slider_max;
}



double Slider::GetPercent() {
   return slider_percent;
}



double Slider::GetInverted() {
   return 1.0 - slider_percent;
}



void Slider::SetInversion(bool invert_me) {
   invert = invert_me;
   SetRedrawFlag();
}



void Slider::SetOrientation(bool horizontal_slider) {
   horizontal = horizontal_slider;
   SetPercent(slider_percent);
}



/// ----------------------------      Slider2D     ------------------------------------



int Slider2D::PrivateHandleEvent(EagleEvent ee) {
   int ret = DIALOG_OKAY;
   
//   EagleLog() << "Event " << EagleEventName(ee.type) << " received.\n";
   
   
   if (ee.type == EAGLE_EVENT_MOUSE_AXES) {
      if (!drag) {
         mxstart = ee.mouse.x;
         mystart = ee.mouse.y;
      }
      else {
         SetPercent((double)(slider_xstart + (invertx?-1:1)*(ee.mouse.x - mxstart))/slider_xmax,
                    (double)(slider_ystart + (inverty?-1:1)*(ee.mouse.y - mystart))/slider_ymax);
         return DIALOG_INPUT_USED;
      }
   }
   if (ee.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN || ee.type == EAGLE_EVENT_MOUSE_BUTTON_UP) {
      if (ee.mouse.button == 1) {
         ret = handle->HandleEvent(ee);
      }
   }
   if (ee.type == EAGLE_EVENT_WIDGET) {
      if (ee.widget.from == handle) {
         if (ee.widget.msgs == BUTTON_CLICKED) {
            drag = true;
            slider_xstart = slider_xpos;
            slider_ystart = slider_ypos;
         }
         else if (ee.widget.msgs == BUTTON_RELEASED) {
            drag = false;
         }
         return DIALOG_OKAY;
      }
   }
   return ret;
}



int Slider2D::PrivateCheckInputs() {
   return DIALOG_OKAY;
}



int Slider2D::PrivateUpdate(double dt) {
   return handle->Update(dt);
}



void Slider2D::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   Rectangle inner = warea.InnerArea();
   inner.DrawGuiRectDown(win , GetColor(MGCOL) , GetColor(SDCOL));
   handle->Display(win , xpos , ypos);
}



void Slider2D::OnAreaChanged() {
   slider_xmax = InnerArea().W() - handle->OuterArea().W();
   slider_ymax = InnerArea().H() - handle->OuterArea().H();
   if (slider_xmax < 1) {slider_xmax = 1;}
   if (slider_ymax < 1) {slider_ymax = 1;}
   SetPercent(slider_xpercent , slider_ypercent);
}



void Slider2D::ResizeButton() {
   Rectangle inner = warea.InnerArea();

   slider_xmax = inner.W() - handle->InnerArea().W();
   slider_ymax = inner.H() - handle->InnerArea().H();
         
   handle->SetWidgetArea(WidgetArea(Rectangle(inner.X() + (invertx?(slider_xmax - slider_xpos):slider_xpos),
                                              inner.Y() + (inverty?(slider_ymax - slider_ypos):slider_ypos),
                                              inner.W()/10,
                                              inner.H()/10)) , false);
}



void Slider2D::RespondToEvent(EagleEvent e , EagleThread* thread) {
   (void)thread;
   PrivateHandleEvent(e);
}



Slider2D::Slider2D(std::string classname , std::string objname , bool invert_x , bool invert_y) :
      WidgetBase(classname , objname),
      slider_xpos(0),
      slider_xmax(1),
      slider_xpercent(0.0),
      slider_ypos(0),
      slider_ymax(1),
      slider_ypercent(0.0),
      drag(false),
      slider_xstart(-1),
      slider_ystart(-1),
      mxstart(-1),
      mystart(-1),
      invertx(invert_x),
      inverty(invert_y),
      basic_handle_button(),
      handle(0)
{
   basic_handle_button.SetWidgetArea(WidgetArea(Rectangle(0 , 0 , InnerArea().W() , InnerArea().H()/10)) , false);
   SetButton(&basic_handle_button);
}



void Slider2D::SetButton(BasicButton* btn) {
   drag = false;
   if (handle) {
      StopListeningTo(handle);
      handle->SetParent(NULL);
   }
   handle = btn?btn:&basic_handle_button;
   handle->SetActionType(SPRING_BTN);
   ListenTo(handle);
   handle->SetParent(this);
   SetPercent(slider_xpercent , slider_ypercent);
}



void Slider2D::SetPercent(double xpct , double ypct , bool sendmessage) {
   double oldx = slider_xpercent;
   double oldy = slider_ypercent;
   if (xpct < 0.0) {xpct = 0.0;}
   if (xpct > 1.0) {xpct = 1.0;}
   if (ypct < 0.0) {ypct = 0.0;}
   if (ypct > 1.0) {ypct = 1.0;}

   slider_xpercent = xpct;
   slider_ypercent = ypct;
   slider_xpos = GetSliderXValue(xpct);
   slider_ypos = GetSliderYValue(ypct);
   ResizeButton();
   SetRedrawFlag();
   if (sendmessage && (oldx != slider_xpercent || oldy != slider_ypercent)) {
      QueueUserMessage(WidgetMsg(this , TOPIC_SLIDER , SLIDER_VALUE_CHANGED));
   }
}



int Slider2D::GetSliderXValue(double pct) {
   return pct*slider_xmax;
}



int Slider2D::GetSliderYValue(double pct) {
   return pct*slider_ymax;
}



int Slider2D::GetSliderXMax() {
   return slider_xmax;
}



int Slider2D::GetSliderYMax() {
   return slider_ymax;
}



double Slider2D::GetXPercent() {
   return slider_xpercent;
}



double Slider2D::GetYPercent() {
   return slider_ypercent;
}



double Slider2D::GetXInverted() {
   return 1.0 - slider_xpercent;
}



double Slider2D::GetYInverted() {
   return 1.0 - slider_ypercent;
}



double Slider2D::GetXActual() {
   return (invertx?GetXInverted():GetXPercent());
}



double Slider2D::GetYActual() {
   return (inverty?GetYInverted():GetYPercent());
}



void Slider2D::SetInversion(bool invert_x , bool invert_y) {
   invertx = invert_x;
   inverty = invert_y;
   SetRedrawFlag();
}




