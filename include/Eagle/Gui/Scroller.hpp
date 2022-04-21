
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
 * @file Scroller.hpp
 * @brief The interface for a simple Scroller base class
 * 
 * Includes a scroller only, this is not a scroll bar. @ref BasicScrollBar
 */

#ifndef Scroller_HPP
#define Scroller_HPP



#include "Eagle/Gui/WidgetBase.hpp"

#include "Eagle/Gui/Layout/LayoutRectangle.hpp"

#include <string>



///< The default topic returned by all events from BasicScroller objects
extern const unsigned int TOPIC_SCROLL_WIDGET;

///< The only message ID that a BasicScroller object currently sends
extern const int SCROLL_VALUE_CHANGED;

///< Registers the SCROLL_VALUE_CHANGED message ID for the TOPIC_SCROLL_WIDGET topic
REGISTER_WIDGET_MESSAGE(TOPIC_SCROLL_WIDGET , SCROLL_VALUE_CHANGED);



///< A lazy global that sets the scroller repeat delay
extern double SCROLLER_REPEAT_DELAY;

///< A lazy global that sets the number of scroll repeats per second, starting after the delay
extern double SCROLLER_NUM_REPEAT_PER_SEC;



/*! \class BasicScroller
 *  @brief Basic scroller widget
 *
 * Derive your scroller widgets from the Scroller base class, and it will work with @ref BasicScrollBar objects.
 */

class BasicScroller : public WidgetBase {

public :
   /// DIAGNOSTIC purposes only

   float GetStartPercent() {return start_percent;}
   float GetScrollPercent() {return scroll_percent;}
   
protected :
   
//   WidgetArea scroller_area;
   
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

   ///< Set the maximum scroll value, or the virtual length of the scroller
   void SetScrollLength(int max_scroll);

   ///< Set the percentage of scroll
   void SetScrollPercent(float new_percent);

   ///< Set the scroll value directly
   void SetScroll(int value);

   ///< Scroll the view by amt
   void ScrollBy(int amt);
   
   ///< Scroll by x percent
   void ScrollBy(float percent);

   ///< Pass true to make this a horizontal scroller, and false for vertical
   void SetScrollDirection(bool is_horizontal_scroller);

   ///< Setup the view length, and how much is shown
   void SetupView(int total_length_of_view , int actual_length_in_view);


   /// Getters
   
   ///< Get the current scroller value
   int GetScrollValue() {return scroll_val;}
   int GetScrollMax() {return total_length;}
};




#endif // Scroller_HPP



