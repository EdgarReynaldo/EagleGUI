
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
 * @file Scroller.hpp
 * @brief The interface for a simple Scroller base class
 * 
 */



#ifndef Scroller_HPP
#define Scroller_HPP


#include "Eagle/Gui/WidgetBase.hpp"

#include <string>



extern const unsigned int TOPIC_SCROLL_WIDGET;///< The topic returned by all events from BasicScroller objects

extern const int SCROLL_VALUE_CHANGED;///< The only message BasicScroller objects currently send

REGISTER_WIDGET_MESSAGE(TOPIC_SCROLL_WIDGET , SCROLL_VALUE_CHANGED);///< Register our widget message



extern double SCROLLER_REPEAT_DELAY;///< A lazy global that sets the scroller repeat delay
extern double SCROLLER_NUM_REPEAT_PER_SEC;///< A lazy global that sets the number of scroll repeats per second,
                                          ///< starting after the delay



/*! \class BasicScroller
 *  @brief Basic scroller widget
 *
 * Derive your scroller widgets from the Scroller base class, and it will work with ScrollBar objects.
 * 
 */



class BasicScroller : public WidgetBase {

public :
   /// DIAGNOSTIC purposes only
   float GetStartPercent() {return start_percent;}
   float GetScrollPercent() {return scroll_percent;}
   
protected :
   
   WidgetArea scroller_area;
   
   LayoutRectangle scroll_handle_layout;
   Rectangle scroll_handle_area;

   int scroll_max;
   int scroll_val;
   float scroll_percent;

   int total_length;
   int length_in_view;
   
   bool horizontal_scroller;

   bool mouse_drag_handle;
   int start_mouse_x;
   int start_mouse_y;
   float start_percent;
   
   bool down;
   int repeat_scroll_increment;
   double repeat_delay;
   double repeat_rate;
   double repeat_elapsed;
   double repeat_previous;





   friend class BasicScrollBar;
   void ResetHandleArea();

   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);

   virtual void OnAreaChanged();


public :
   BasicScroller(std::string objclass = "BasicScroller" , std::string objname = "Nemo");
   
   // Setters

   void SetScrollLength(int max_scroll);///< Set the maximum scroll value, or the virtual length of the scroller
   void SetScrollPercent(float new_percent);///< Set the percentage of scroll
   void SetScroll(int value);///< Set the scroll value directly
   void ScrollBy(int amt);///< Scroll the view by amt
   void ScrollBy(float percent);///< Scroll by x percent

   void SetScrollDirection(bool is_horizontal_scroller);///< Pass true to make this a horizontal scroller, and false for vertical
   void SetupView(int total_length_of_view , int actual_length_in_view);///< Setup the view length, and how much is shown


   // Getters
   
   int GetScrollValue() {return scroll_val;}///< Get the current scroller value
   
   
   
};






#endif // Scroller_HPP


