
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
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */




#ifndef ScrollBar_HPP
#define ScrollBar_HPP


#include "Eagle/Gui/WidgetBase.hpp"

#include "Eagle/Gui/Scroller.hpp"
#include "Eagle/Gui/Button/ScrollButton.hpp"

#include "Eagle/SharedMemory.hpp"



class BasicScrollBar : public WidgetBase {
   
public :
   /// DIAGNOSTIC purposes only
   float GetStartPercent() {return scroller->GetStartPercent();}
   float GetScrollPercent() {return scroller->GetScrollPercent();}
   
   
protected:
   
   BasicScroller basic_scroller;
   BasicScrollButton basic_scroll_button_up_or_left;
   BasicScrollButton basic_scroll_button_down_or_right;
   
   SHAREDOBJECT<BasicScroller> scroller;
   SHAREDOBJECT<BasicScrollButton> up_or_left_button;
   SHAREDOBJECT<BasicScrollButton> down_or_right_button;
   
   bool is_horizontal;
   
   
   
   void ResetHandleArea();
   void ResetScrollBarArea();
   
   
   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);

   virtual void QueueUserMessage(const WidgetMsg& wmsg);

   virtual void OnAreaChanged();

public :
   BasicScrollBar(std::string objclass = "BasicScrollBar" , std::string name = "Nemo");
   
   void SetScrollWidgets(SHAREDOBJECT<BasicScroller> pbasic_scroller , 
                         SHAREDOBJECT<BasicScrollButton> pbasic_up_or_left_button ,
                         SHAREDOBJECT<BasicScrollButton> pbasic_down_or_right_button);

   void SetScrollLength(int max_scroll);
   void SetScrollPercent(float new_percent);
   void SetScroll(int value);
   void ScrollBy(int amt);
   void ScrollBy(float percent);

   void SetScrollDirection(bool is_horizontal_scroller);
   void SetupView(int total_length_of_view , int actual_length_in_view);

   int GetScrollValue() {return scroller->GetScrollValue();}
   
};


#endif // ScrollBar_HPP

