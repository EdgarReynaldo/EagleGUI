
/**
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
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */




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


   Rectangle GetPinArea(int width , int height);
   Pos2I GetPosition();
   
   friend std::ostream& operator<<(std::ostream& os , const Pin& pin);
   
};



class PinLayout : public LayoutBase {
   
protected :

   std::vector<Pin> pins;

   virtual void ReserveSlots(int nslots);

public :

   PinLayout(std::string objname = "Nemo");

   /// LayoutBase
   
   /// Pass INT_MAX for a parameter if you don't care about the position or size
   /// If you don't specify a position, it will use the current pins position
   /// NOTE : This function does NOT change the widget's area, it only returns the area that the layout would give it
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight);/// Will move pins accordingly

   /// Member functions
   void SetPinPosition(int pin_slot , int newx , int newy);
   
   void SetPinAlignment(int pin_slot , HALIGNMENT halignment , VALIGNMENT valignment);
   
   void SetPinPosition(int pin_slot , int newx , int newy , HALIGNMENT halignment , VALIGNMENT valignment);
   
   virtual void SetAlignment(HALIGNMENT halignment , VALIGNMENT valignment);
   
   void CyclePins(bool cycle_forward);
   
   Pin GetPin(int pin_slot);/// All values are valid, pins can cycle
   

   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const;
};




#endif // PinLayout_HPP



