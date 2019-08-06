
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
 * @file ScrollView.hpp
 * @brief A simple widget to create a view port on a larger widget
 */

#ifndef ScrollView_HPP
#define ScrollView_HPP


#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/Gui/Scrollable.hpp"



/**! @class ScrollView
 *   @brief Simple view on another widget
 */

class ScrollView : public WidgetBase , public IScrollable {
   
protected :
   WidgetBase* our_widget;
   


   /// WidgetBase

   virtual int PrivateHandleEvent(EagleEvent ee);///< Virtual function that controls how widgets react to events
   virtual int PrivateUpdate(double dt);///< Virtual function that controls what happens when time passes for a widget
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);///< Virtual function that controls how a widget is drawn
   
   virtual void OnAreaChanged();///< Override to react to changes in this widget's area

   /// Members
   
   void RepositionWidget();///< Repositions the view widget
   
   /// IScrollable
   
   virtual void ScrollCallback();

public :
   
   ///< Constructor to create a view on a widget
   ScrollView(WidgetBase* w = 0 , std::string classname = "ScrollView" , std::string objname = "Nemo");
   
   void SetOurWidget(WidgetBase* widget);///< Call to set the view widget

   unsigned int ViewWidth();///< The width of the widget we're viewing
   unsigned int ViewHeight();///< The height of the widget we're viewing
};


#endif // ScrollView_HPP
