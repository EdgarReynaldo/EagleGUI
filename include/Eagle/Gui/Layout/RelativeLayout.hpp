
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

   virtual void RepositionAllChildren();
   virtual void RepositionChild(int slot);

public :
   
   RelativeLayout();
   RelativeLayout(std::string name);
   ~RelativeLayout();

   // LayoutBase
   void SetArea(const Rectangle& r , bool notify_layout = true);

   virtual Rectangle RequestWidgetArea(WidgetBase* widget , int newx , int newy , int newwidth , int newheight);
   
   /// TODO : For some reason these two functions are invisible - the overloaded PlaceWidget and AddWidget below
   ///        hide them so here we make them explicitly visible again :
   virtual bool PlaceWidget(WidgetBase* widget , int slot , bool delete_when_removed = false);
   virtual bool AddWidget(WidgetBase* widget , bool delete_when_removed = false);
   
   /// Member funcs
   Rectangle SetLayoutRectangle(int index , LayoutRectangle layout_rect);
   Rectangle SetLayoutRectangle(WidgetBase* widget , LayoutRectangle layout_rect);
   
   bool PlaceWidget(WidgetBase* widget , int slot , LayoutRectangle lrect , bool delete_when_removed = false);
   bool AddWidget(WidgetBase* widget , LayoutRectangle lrect , bool delete_when_removed = false);


   void Resize(unsigned int nsize);
};




#endif // RelativeLayout_HPP

