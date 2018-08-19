



#ifndef FillLayout_HPP
#define FillLayout_HPP



#include "Eagle/Gui/Layout/Layout.hpp"



class FillLayout : public Layout {
   
   virtual void ReserveSlots(int nslots);

public :
   
   FillLayout(std::string objname = "Nemo");
   
   
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) const;
   
   virtual void PlaceWidget(SHAREDWIDGET widget , int slot);/// Only valid slot is zero
   virtual int AddWidget(SHAREDWIDGET widget);/// Will only add a widget to an empty layout



   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const;

};


#endif // FillLayout_HPP
