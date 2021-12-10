
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
 * @file RelativeLayout.hpp
 * @brief The interface for the @class RelativeLayout class that stores relative positions and areas
 */

#ifndef RelativeLayout_HPP
#define RelativeLayout_HPP



#include "Eagle/Gui/Layout/Layout.hpp"



/**! @class RelativeLayout
 *   @brief A simple layout class to store widget position and area relative to a parent rectangle
 *   @sa LayoutRectangle
 */

class RelativeLayout : public LayoutBase {

protected :

   std::vector<LayoutRectangle> layout_rectangles;
   
   virtual void ReserveSlots(int nslots);

public :
   
   RelativeLayout(std::string classname = "RelativeLayout" , std::string name = "Nemo");
   ~RelativeLayout();

   ///< @sa LayoutBase::RequestWidgetArea
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) ;
   
   /// TODO : For some reason these two functions are invisible - the overloaded PlaceWidget and AddWidget below
   ///        hide them so here we make them explicitly visible again :

   virtual void PlaceWidget(WidgetBase* widget , int slot);///< @sa LayoutBase::PlaceWidget
   virtual int AddWidget(WidgetBase* widget);///< @sa LayoutBase::AddWidget
   
   /// Member funcs

   ///< Place a widget with the given @ref LayoutRectangle
   void PlaceWidget(WidgetBase* widget , int slot , LayoutRectangle lrect);
   
   ///< Add a widget with the given @ref LayoutRectangle
   int AddWidget(WidgetBase* widget , LayoutRectangle lrect);

   ///< Set the @ref LayoutRectangle for the given layout index
   Rectangle SetLayoutRectangle(int index , LayoutRectangle layout_rect);

   ///< Set the @ref LayoutRectangle for the given widget
   Rectangle SetLayoutRectangle(WidgetBase* widget , LayoutRectangle layout_rect);

   ///< Describe this layout to a stream
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const;
};



#endif // RelativeLayout_HPP



