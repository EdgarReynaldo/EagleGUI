
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file PinLayout.hpp
 * @brief A simple pin layout that keeps relative positions
 */

#ifndef PinLayout_HPP
#define PinLayout_HPP



#include "Eagle/Gui/Layout/Layout.hpp"



/**! @class Pin
 *   @brief A simple pin class to store a position and alignment with that position
 */

class Pin {
   
public :
   Pin();
   
   int px;
   int py;
   HALIGNMENT halign;
   VALIGNMENT valign;
   
   void SetPinPosition(int xpos , int ypos);///< Move the pin, preserving alignment

   void SetPinAlignment(HALIGNMENT halignment , VALIGNMENT valignment);///< Realign the pin

   void SetPinPosition(int xpos , int ypos , HALIGNMENT halignment , VALIGNMENT valignment);///< Move and realign the pin


   Rectangle GetPinArea(int width , int height);///< Get the area a widget would occupy given a specific width and height
   Pos2I GetPosition();///< Get the pin position
   
   friend std::ostream& operator<<(std::ostream& os , const Pin& pin);///< Describe this pin to a stream
};



/**! @class PinLayout
 *   @brief A simple layout to store absolute positions and alignment
 */

class PinLayout : public LayoutBase {
   
protected :

   std::vector<Pin> pins;

   virtual void ReserveSlots(int nslots);

public :

   PinLayout(std::string objname = "Nemo");

   /// LayoutBase
   
   ///< Pass INT_MAX for a parameter if you don't care about the position or size
   ///< If you don't specify a position, it will use the current pins position and widget size
   ///< NOTE : This function does NOT change the widget's area, it only returns the area that the layout would give it
   virtual Rectangle RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight);/// Will move pins accordingly

   /// Member functions
   
   ///< Set the pin's (and widget's if one is associated) position
   void SetPinPosition(int pin_slot , int newx , int newy);
   
   ///< Set the pin's (and widget's if one is associated) position
   void SetPinAlignment(int pin_slot , HALIGNMENT halignment , VALIGNMENT valignment);
   
   ///< Set the pin's (and widget's if one is associated) position
   void SetPinPosition(int pin_slot , int newx , int newy , HALIGNMENT halignment , VALIGNMENT valignment);
   
   ///< Set the global alignment for all pins
   virtual void SetAlignment(HALIGNMENT halignment , VALIGNMENT valignment);
   
   ///<
   void CyclePins(bool cycle_forward);///< An easy way to cycle the pin slots
   
   ///< All values within [0,Layout::GetLayoutSize) are valid, pins can cycle
   ///< @sa LayoutBase::GetLayoutSize
   Pin GetPin(int pin_slot);
   

   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const;///< Describe this layout to a stream
};




#endif // PinLayout_HPP



