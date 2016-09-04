



#ifndef PinLayout_HPP
#define PinLayout_HPP



#include "Eagle/Gui/Layout/Layout.hpp"


class Pin {
   

public :
   Pin();
   
   int px;
   int py;
   HALIGNMENT halign;
   VALIGNMENT valign;
   
   void SetPinPosition(int xpos , int ypos);
   void SetPinPosition(int xpos , int ypos , HALIGNMENT halignment , VALIGNMENT valignment);

   void SetPinAlignment(HALIGNMENT halignment , VALIGNMENT valignment);
   
   Rectangle GetPinPosition(int width , int height);

};



class PinLayout : public Layout {
   
   std::vector<Pin> pins;

protected :
   virtual void ReserveSlots(int nslots);

/*
   virtual void RepositionAllChildren();
   virtual void RepositionChild(int slot);
void PinLayout::RepositionChild(int slot) {
   EAGLE_ASSERT((slot >= 0) && (slot < (int)wchildren.size()));
   
   Pin p = pins[slot];
   
}
*/
public :
   
/**
	/// WidgetBase

   virtual int PrivateHandleInputEvent(EagleEvent e);
   virtual int PrivateUpdate(double dt);
   virtual void PrivateDisplay(EagleGraphicsContext* win , int x , int y);

   virtual void SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout = true);

	/// Changes position and outer area!!!
	virtual void SetMarginsExpandFromInner(int left , int right , int top , int bottom);

	/// Make room in outer area for inner area first!!!
	virtual void SetMarginsContractFromOuter(int left , int right , int top , int bottom);

   virtual bool AcceptsFocus() {return false;}

   /// LayoutBase
   
   /// Pass INT_MAX for a parameter if you don't care about the position or size
   /// NOTE : These two functions do NOT change the widget's area, they only return the area that the layout would give it
   virtual Rectangle RequestWidgetArea(WidgetBase* widget , int newx , int newy , int newwidth , int newheight);

   /// Widget may be null for PlaceWidget
   /// Both replace the widget (addwidget replaces a null widget) and call RepositionChild
   virtual void PlaceWidget(WidgetBase* widget , int slot);
   virtual void AddWidget(WidgetBase* widget);/// Adds the widget to the next free slot or creates one if necessary
//*/   
};




#endif // PinLayout_HPP



