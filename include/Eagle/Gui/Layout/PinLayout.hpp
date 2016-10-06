



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

   void SetPinAlignment(HALIGNMENT halignment , VALIGNMENT valignment);

   void SetPinPosition(int xpos , int ypos , HALIGNMENT halignment , VALIGNMENT valignment);


   Rectangle GetPinPosition(int width , int height);
};



class PinLayout : public Layout {
   
   std::vector<Pin> pins;

protected :
   virtual void ReserveSlots(int nslots);

public :

   /// LayoutBase
   
   /// Pass INT_MAX for a parameter if you don't care about the position or size
   /// If you don't specify a position, it will use the current pins position
   /// NOTE : This function does NOT change the widget's area, it only returns the area that the layout would give it
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight);

   /// Member functions
   void SetPinPosition(int pin_slot , int newx , int newy);
   void SetPinAlignment(int pin_slot , HALIGNMENT halignment , VALIGNMENT valignment);
   void SetPinPosition(int pin_slot , int newx , int newy , HALIGNMENT halignment , VALIGNMENT valignment);
   
   virtual void SetAlignment(HALIGNMENT halignment , VALIGNMENT valignment);
   
   void CyclePins(bool cycle_forward);
};




#endif // PinLayout_HPP



