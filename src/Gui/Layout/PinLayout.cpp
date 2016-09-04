



#include "Eagle/Gui/Layout/PinLayout.hpp"




/// ---------------------------------     Pin     --------------------------------------------



Pin::Pin() :
      px(0),
      py(0),
      halign(HALIGN_LEFT),
      valign(VALIGN_TOP)
{}



void Pin::SetPinPosition(int xpos , int ypos) {
   SetPinPosition(xpos,ypos,HALIGN_LEFT , VALIGN_TOP);
}



void Pin::SetPinPosition(int xpos , int ypos , HALIGNMENT halignment , VALIGNMENT valignment) {
   px = xpos;
   py = ypos;
   halign = halignment;
   valign = valignment;
}
   


void Pin::SetPinAlignment(HALIGNMENT halignment , VALIGNMENT valignment) {
   halign = halignment;
   valign = valignment;
}



Rectangle Pin::GetPinPosition(int width , int height) {
   int x = px;
   int y = py;
   if (halign == HALIGN_CENTER) {
      x -= width/2;
   }
   else if (halign) {
      x -= width;
   }
   if (valign == VALIGN_CENTER) {
      y -= height/2;
   }
   else if (valign == VALIGN_BOTTOM) {
      y -= height;
   }
   return Rectangle(x , y , width , height);
}



/// ----------------------      PinLayout      ------------------------------------------



void PinLayout::ReserveSlots(int nslots) {
   Layout::ReserveSlots(nslots);
   pins.resize(wchildren.size());
}




