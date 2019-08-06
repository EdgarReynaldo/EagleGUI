
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
 * @file ScrollBar.hpp
 * @brief A simple scroll bar base class
 * 
 * A scroll bar class that contains a scroll area with a draggable handle and two scroll buttons with a variable
 * layout orientation. Default widgets are provided, but as always, you can derive new ones with your own
 * desired behavior and then make the ScrollBar class use them. Simply derive from @ref BasicScroller and @ref BasicScrollButton
 * and pass pointers to your widgets to the @sa ScrollBar::SetScrollWidgets function.
 * 
 */

#ifndef ScrollBar_HPP
#define ScrollBar_HPP



#include "Eagle/Gui/WidgetBase.hpp"

#include "Eagle/Gui/Scroller.hpp"
#include "Eagle/Gui/Button/ScrollButton.hpp"

#include "Eagle/SharedMemory.hpp"


/**! @class BasicScrollBar
 *   @brief A basic scroll bar class that you can use custom scrollers and scroll buttons with
 */

class BasicScrollBar : public WidgetBase {
   
public :
   /// DIAGNOSTIC purposes only
   float GetStartPercent() {return scroller->GetStartPercent();}
   float GetScrollPercent() {return scroller->GetScrollPercent();}
   
   
protected:
   
   BasicScroller basic_scroller;///< Default scroll area
   BasicScrollButton basic_scroll_button_up_or_left;///< Default scroll button on the top or left based on orientation
   BasicScrollButton basic_scroll_button_down_or_right;///< Default scroll button on the bottom or right based on orientation
   
   SHAREDOBJECT<BasicScroller> scroller;///< Shared pointer to the scroller in use
   SHAREDOBJECT<BasicScrollButton> up_or_left_button;///< Shared pointer to the up or left button in use
   SHAREDOBJECT<BasicScrollButton> down_or_right_button;///< Shared pointer to the down or right button in use
   
   bool is_horizontal;///< True if the scroller is layed out horizontally, ie. left to right
   
   
   
   void ResetHandleArea();
   void ResetScrollBarArea();
   
   
   virtual int PrivateHandleEvent(EagleEvent e);///< Overridden @ref WidgetBase::PrivateHandleEvent
   virtual int PrivateCheckInputs();///< Overridden @ref WidgetBase::PrivateCheckInputs
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);///< Overridden @ref WidgetBase::PrivateDisplay
   virtual int PrivateUpdate(double tsec);///< Overridden @ref WidgetBase::PrivateUpdate

   virtual void QueueUserMessage(const WidgetMsg& wmsg) override;///< Overridden @ref WidgetBase::QueueUserMessage

   virtual void OnAreaChanged();///< Overridden @ref WidgetBase::OnAreaChanged

public :
   ///< Basic scroll bar constructor takes a class name and object name
   BasicScrollBar(std::string objclass = "BasicScrollBar" , std::string name = "Nemo");
   
   ///< Set the widgets to use in the scroll bar. Pass 0 (NULL) to use the default scroller or default scroll button widget
   void SetScrollWidgets(SHAREDOBJECT<BasicScroller> pbasic_scroller , 
                         SHAREDOBJECT<BasicScrollButton> pbasic_up_or_left_button ,
                         SHAREDOBJECT<BasicScrollButton> pbasic_down_or_right_button);

   void SetScrollLength(int max_scroll);///< Set how long you want the scroll bar to be in whole numbers, this will affect the precision
   void SetScrollPercent(float new_percent);///< Set the exact scroll percentage for the view
   void SetScroll(int value);///< Set the integer value for the scroll, which is limited to scroll length as specified in SetScrollLength
   void ScrollBy(int amt);///< Scroll by amt units
   void ScrollBy(float percent);///< Scroll by x percent

   void SetScrollDirection(bool is_horizontal_scroller);///< Set default scroller orientation
   void SetupView(int total_length_of_view , int actual_length_in_view);///< Setup view length and amount shown

   int GetScrollValue() {return scroller->GetScrollValue();}///< Retrieve the current integer scroll value
   int GetScrollMax() {return scroller->GetScrollMax();}
   
   
//   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;///< Describes this widget to an ostream
//std::ostream& BasicScrollBar::DescribeTo(std::ostream& os , Indenter indent) const {
std::ostream& DescribeTo(std::ostream& os , Indenter indent) const {
   os << indent << "BasicScrollBar :" << std::endl;
   WidgetBase::DescribeTo(os,indent);
   os << indent << "BasicScrollButtonUpOrLeft :" << std::endl;
   basic_scroll_button_up_or_left.DescribeTo(os , indent);
   os << indent << "BasicScrollButtonDownOrRight :" << std::endl;
   basic_scroll_button_down_or_right.DescribeTo(os , indent);
   return os;
}
};



#endif // ScrollBar_HPP



