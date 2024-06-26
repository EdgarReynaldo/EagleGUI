
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
 *
 *
 *
 * @file ScrollButton.hpp
 * @brief Simple basic implementation for scroll buttons
 * 
 */


#ifndef ScrollButton_HPP
#define ScrollButton_HPP



#include "Eagle/Gui/Button/BasicButton.hpp"



class BasicScrollBar;



/**! @class BasicScrollButton
 *   @brief Simple scroll button class
 */

class BasicScrollButton : public WidgetBase {

protected :
   BasicButton our_basic_button;

   BasicButton* scroll_button;
   BasicScrollBar* our_scrollbar;
   
   int increment;
   
   bool scroll_up_or_left;
   bool scroll_horizontal;
   
   Triangle our_click_area;

   void Scroll();

   void Reset();
   void ResetTriangle();
   void SyncButtonArea();

   /// WidgetBase
   virtual int PrivateHandleEvent(EagleEvent e);
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);

   virtual void QueueUserMessage(const WidgetMsg& wmsg);
   
   /// Callbacks, overload if you need to
   virtual void OnAreaChanged();
   virtual void OnAttributeChanged(const ATTRIBUTE& a , const VALUE& v);
   virtual void OnFlagChanged(WIDGET_FLAGS f , bool on);
///   virtual void OnColorChanged();

public :
   
   BasicScrollButton(std::string objclass = "BasicScrollButton" , std::string objname = "Nemo");
   ~BasicScrollButton() {}

   void UseBasicButton();///< Use the default basic button for the scroll button
   void UseButton(BasicButton* button);///< Use a custom button for the scroll button
   void SetScrollBar(BasicScrollBar* scrollbar);/// For scrollbars to use
   void SetIncrement(int new_increment);///< Set the rate at which this button scrolls
   virtual void SetScrollDirection(bool up_or_left , bool horizontal);///< Set the direction of this button
};



#endif // ScrollButton_HPP




