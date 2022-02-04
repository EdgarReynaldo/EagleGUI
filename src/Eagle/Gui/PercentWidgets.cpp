
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
 * @file PercentWidgets.cpp
 * @brief Implentation file for percent widget classes
 */

 
 
 
#include "Eagle/Gui/PercentWidgets.hpp"
#include "Eagle/GraphicsContext.hpp"
#include "Eagle/Gui/Slider.hpp"


void PercentWidget::SetPercent(double pct , bool sendmessage) {
   double old = percent;
   if (pct < 0.0) {pct = 0.0;}
   if (pct > 1.0) {pct = 1.0;}
   percent = pct;
   SetRedrawFlag();
   if (sendmessage && (old != percent)) {
      QueueUserMessage(WidgetMsg(this , TOPIC_SLIDER , SLIDER_VALUE_CHANGED));
   }
}



double PercentWidget::GetPercent() {
   return percent;
}




/// -------------------------     PercentBar      ---------------------------------





int PercentBar::PrivateHandleEvent(EagleEvent e) {
   int ret = DIALOG_OKAY;
   if (IsMouseEvent(e)) {
      if (e.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN && e.mouse.button == 1) {
         if (InnerArea().Contains(e.mouse.x , e.mouse.y)) {
            ret |= DIALOG_INPUT_USED;
            if (horizontal) {
               if (!invert) {
                  /// left to right
                  SetPercent((double)(e.mouse.x - InnerArea().X())/InnerArea().W());
               }
               else {
                  /// right to left
                  SetPercent((double)(InnerArea().BRX() - e.mouse.x)/InnerArea().W());
               }
            }
            else {/// vertical
               if (!invert) {
                  /// top to bottom
                  SetPercent((double)(e.mouse.y - InnerArea().Y())/InnerArea().H());
               }
               else {
                  /// bottom to top
                  SetPercent((double)(InnerArea().BRY() - e.mouse.y)/InnerArea().H());
               }
            }
            drag = true;
         }
      }
      else if (e.type == EAGLE_EVENT_MOUSE_AXES) {
         if (drag) {
            if (horizontal) {
               if (!invert) {
                  /// left to right
                  SetPercent((double)(e.mouse.x - InnerArea().X())/InnerArea().W());
               }
               else {
                  /// right to left
                  SetPercent((double)(InnerArea().BRX() - e.mouse.x)/InnerArea().W());
               }
            }
            else {/// vertical
               if (!invert) {
                  /// top to bottom
                  SetPercent((double)(e.mouse.y - InnerArea().Y())/InnerArea().H());
               }
               else {
                  /// bottom to top
                  SetPercent((double)(InnerArea().BRY() - e.mouse.y)/InnerArea().H());
               }
            }
         }
      }
      else if (e.type == EAGLE_EVENT_MOUSE_BUTTON_UP && drag && e.mouse.button == 1) {
         drag = false;
         ret |= DIALOG_INPUT_USED;
      }
   }
   return ret;
}



int PercentBar::PrivateCheckInputs() {
   return DIALOG_OKAY;
}



int PercentBar::PrivateUpdate(double dt) {
   (void)dt;
   return DIALOG_OKAY;
}



void PercentBar::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   Rectangle r = InnerArea();
   r.MoveBy(xpos , ypos);
   win->DrawFilledRectangle(r , GetColor(MGCOL));
   Rectangle pcth(r.X() , r.Y() , r.W()*(this->GetPercent()) , r.H());
   Rectangle pcthrl(r.X() + r.W()*(1.0 - this->GetPercent()) , r.Y() , r.W()*(this->GetPercent()) , r.H());
   Rectangle pctv(r.X() , r.Y() , r.W() , r.H()*(this->GetPercent()));
   Rectangle pctvdu(r.X() , r.Y() + r.H()*(1.0 - this->GetPercent()) , r.W() , r.H()*(this->GetPercent()));
   Rectangle* pct = 0;
   
   if (horizontal) {
      if (!invert) {
         pct = &pcth;
      }
      else {
         pct = &pcthrl;
      }
   }
   else {
      if (!invert) {
         pct = &pctv;
      }
      else {
         pct = &pctvdu;
      }
   }
   win->DrawFilledRectangle(*pct , GetColor(FGCOL));
}



void PercentBar::SetOrientation(bool is_horizontal) {
   horizontal = is_horizontal;
   SetRedrawFlag();
}



void PercentBar::SetInversion(bool do_invert) {
   invert = do_invert;
   SetRedrawFlag();
}



bool PercentBar::IsHorizontal() {
   return horizontal;
}



bool PercentBar::IsInverted() {
   return invert;
}
