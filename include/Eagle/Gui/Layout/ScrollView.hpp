
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
#include "Eagle/Gui/Layout/Layout.hpp"
#include "Eagle/Gui/Scrollable.hpp"



/**! @class ScrollView
 *   @brief Simple view on another widget
 */

class ScrollView : public LayoutBase , public IScrollable {
   
protected :
   WidgetBase* our_widget;
   
   /// WidgetBase
   virtual void OnAreaChanged();

   /// Members
   void RepositionWidget();///< Repositions the view widget
   
   /// IScrollable
   virtual void ScrollCallback();

public :
   
   ///< Constructor to create a view on a widget
   ScrollView(WidgetBase* w = 0 , std::string classname = "ScrollView" , std::string objname = "Nemo");
   virtual ~ScrollView();
   
   
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight);

   virtual void PlaceWidget(WidgetBase* w , int slot);
   virtual int AddWidget(WidgetBase* w);/// Adds the widget to the next free slot or creates one if necessary, returns slot used





   void SetOurWidget(WidgetBase* widget);///< Call to set the view widget

   WidgetBase* GetOurWidget() {return our_widget;}
};


#endif // ScrollView_HPP
