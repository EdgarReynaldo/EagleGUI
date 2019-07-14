
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
 * @file RadioLayout.hpp
 * @brief The interface for radio button layouts
 */



#ifndef RadioLayout_HPP
#define RadioLayout_HPP



#include "Eagle/Gui/Layout/Layout.hpp"
#include "Eagle/Gui/Layout/DumbLayout.hpp"


/**! @class RadioLayout
 *   @brief A simple interface for a layout that turns a group of buttons into a radio button
 *
 *   RadioLayout is a decorator class for Layouts, you can use any layout you like with it, and it will handle
 *   forwarding layout functions to the real layout, as well as deselecting all non selected buttons when one is toggled.
 */

class RadioLayout : public LayoutBase {
   
   DumbLayout dumb_layout;
   LayoutBase* real_layout;
   
   
public :
   
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight);

   virtual void PlaceWidget(WidgetBase* w , int slot);
   virtual int AddWidget(WidgetBase* w);

};



Rectangle RadioLayout::RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) {
   return real_layout->RequestWidgetArea(widget_slot , newx , newy , newwidth , newheight);
}



void RadioLayout::PlaceWidget(WidgetBase* w , int slot) {
   real_layout->PlaceWidget(w,slot);
}



int RadioLayout::AddWidget(WidgetBase* w) {
   return real_layout->AddWidget(w);
}



#endif // RadioLayout_HPP



