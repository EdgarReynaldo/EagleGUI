



#include "Eagle/Gui/Layout/PinLayout.hpp"


#include "Eagle/StringWork.hpp"




/// ---------------------------------     Pin     --------------------------------------------



Pin::Pin() :
      px(0),
      py(0),
      halign(HALIGN_LEFT),
      valign(VALIGN_TOP)
{}



void Pin::SetPinPosition(int xpos , int ypos) {
   SetPinPosition(xpos , ypos , halign , valign);
}



void Pin::SetPinAlignment(HALIGNMENT halignment , VALIGNMENT valignment) {
   SetPinPosition(px , py , halignment , valignment);
}



void Pin::SetPinPosition(int xpos , int ypos , HALIGNMENT halignment , VALIGNMENT valignment) {
   px = xpos;
   py = ypos;
   halign = halignment;
   valign = valignment;
}
   


Rectangle Pin::GetPinArea(int width , int height) {
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



Pos2d Pin::GetPosition() {
   return Pos2d(px,py);
}



/// ----------------------      PinLayout      ------------------------------------------



PinLayout::PinLayout(std::string name) :
      Layout(name),
      pins()
{
   if (name.compare("") == 0) {
      SetName(StringPrintF("PinLayout at %p" , this));
   }
}



void PinLayout::ReserveSlots(int nslots) {
   Layout::ReserveSlots(nslots);
   pins.resize(wchildren.size());
}



Rectangle PinLayout::RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) {
   WidgetBase* widget = GetWidget(widget_slot);
   if (!widget) {
      return Rectangle(-1,-1,-1,-1);
   }
   
   Rectangle current = widget->OuterArea();
   
   Pin& pin = pins[widget_slot];

   if (newx != INT_MAX) {
      pin.px = newx;
   }
   if (newy != INT_MAX) {
      pin.py = newy;
   }
   
   if (newwidth == INT_MAX) {
      newwidth = current.W();
   }
   if (newheight == INT_MAX) {
      newheight = current.H();
   }
   
   return pin.GetPinArea(newwidth , newheight);
}



void PinLayout::SetPinPosition(int pin_slot , int newx , int newy) {
   EAGLE_ASSERT((pin_slot >= 0) && (pin_slot < (int)pins.size()));
   Pin& p = pins[pin_slot];
   p.SetPinPosition(newx , newy);
   RepositionChild(pin_slot);
}



void PinLayout::SetPinAlignment(int pin_slot , HALIGNMENT halignment , VALIGNMENT valignment) {
   EAGLE_ASSERT((pin_slot >= 0) && (pin_slot < (int)pins.size()));
   Pin& p = pins[pin_slot];
   p.SetPinAlignment(halignment , valignment);
   RepositionChild(pin_slot);
}



void PinLayout::SetPinPosition(int pin_slot , int newx , int newy , HALIGNMENT halignment , VALIGNMENT valignment) {
   EAGLE_ASSERT((pin_slot >= 0) && (pin_slot < (int)pins.size()));
   Pin& p = pins[pin_slot];
   p.SetPinPosition(newx , newy , halignment , valignment);
   RepositionChild(pin_slot);
}



void PinLayout::SetAlignment(HALIGNMENT halignment , VALIGNMENT valignment) {
   for (int i = 0 ; i < (int)pins.size() ; ++i) {
      SetPinAlignment(i , halignment , valignment);
   }
}



void PinLayout::CyclePins(bool cycle_forward) {
   if (pins.size()) {
      if (cycle_forward) {
         Pin ptemp = pins.back();
         for (int i = (int)pins.size() - 2 ; i >= 0 ; --i) {
            pins[i + 1] = pins[i];
         }
         pins[0] = ptemp;
      }
      else {
         Pin ptemp = pins.front();
         for (int i = 0 ; i < (int)pins.size() - 1 ; ++i) {
            pins[i] = pins[i + 1];
         }
         pins.back() = ptemp;
      }
      RepositionAllChildren();
   }
}



Pin PinLayout::GetPin(int pin_slot) {
   pin_slot %= pins.size();
   if (pin_slot < 0) {pin_slot += pins.size();}
   return pins[pin_slot];
}








