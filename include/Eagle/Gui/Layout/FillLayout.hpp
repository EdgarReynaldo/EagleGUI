



#ifndef FillLayout_HPP
#define FillLayout_HPP



#include "Eagle/Gui/Layout/Layout.hpp"



class FillLayout : public Layout {
   
   virtual void ReserveSlots(int nslots);

public :
   
   FillLayout();
   
   
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight);
   
   virtual void PlaceWidget(WidgetBase* widget , int slot);/// Only valid slot is zero
   virtual void AddWidget(WidgetBase* widget);/// Will only add a widget to an empty layout



   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const;

};


#endif // FillLayout_HPP
