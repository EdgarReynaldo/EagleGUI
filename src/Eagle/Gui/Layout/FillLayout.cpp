



#include "Eagle/Gui/Layout/FillLayout.hpp"

#include "Eagle/StringWork.hpp"
#include "Eagle/Exception.hpp"



void FillLayout::ReserveSlots(int nslots) {
   (void)nslots;
   LayoutBase::ReserveSlots(1);
}



FillLayout::FillLayout(std::string objname) :
      LayoutBase("FillLayout" , objname)
{}



Rectangle FillLayout::RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) const {
   (void)widget_slot;
   (void)newx;
   (void)newy;
   (void)newwidth;
   (void)newheight;
   return InnerArea();
}



void FillLayout::PlaceWidget(SHAREDWIDGET widget , int slot) {
   if (slot != 0) {
      throw EagleException(StringPrintF("FillLayout::PlaceWidget : slot #%d is not valid. Only slot #0 is valid.\n" , slot));
   }
   else {
      LayoutBase::PlaceWidget(widget , 0);
   }
}



int FillLayout::AddWidget(SHAREDWIDGET widget) {
   if (NextFreeSlot() == -1) {
      throw EagleException("FillLayout::AddWidget : FillLayout already full!\n");
   }
   else {
      LayoutBase::PlaceWidget(widget , 0);
      return 0;
   }
   return -1;/// Invalid slot
}



std::ostream& FillLayout::DescribeTo(std::ostream& os , Indenter indent) const {
   return LayoutBase::DescribeTo(os , indent);
}






