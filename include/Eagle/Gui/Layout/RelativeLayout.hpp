
/*
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
 *    EAGLE
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2014+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */

#ifndef RelativeLayout_HPP
#define RelativeLayout_HPP



#include "Eagle/Gui/Layout/Layout.hpp"




class RelativeLayout : public Layout {

protected :

   std::vector<LayoutRectangle> layout_rectangles;
   
   virtual void ReserveSlots(int nslots);

public :
   
   RelativeLayout(std::string name = "Nemo");
   ~RelativeLayout();

   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) const;
   
   /// TODO : For some reason these two functions are invisible - the overloaded PlaceWidget and AddWidget below
   ///        hide them so here we make them explicitly visible again :
   virtual void PlaceWidget(WidgetBase* widget , int slot);
   virtual int AddWidget(WidgetBase* widget);
   
   /// Member funcs
   void PlaceWidget(WidgetBase* widget , int slot , LayoutRectangle lrect);
   int AddWidget(WidgetBase* widget , LayoutRectangle lrect);

   Rectangle SetLayoutRectangle(int index , LayoutRectangle layout_rect);
   Rectangle SetLayoutRectangle(WidgetBase* widget , LayoutRectangle layout_rect);

};




#endif // RelativeLayout_HPP

