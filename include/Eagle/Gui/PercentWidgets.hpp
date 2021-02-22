
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
 * @file PercentWidgets.hpp
 * @brief Holds the interfaces for PercentBar, and PercentDial
 * 
 * 
 * 
 */


#ifndef PercentWidgets_HPP
#define PercentWidgets_HPP


#include <string>

#include "Eagle/Gui/WidgetBase.hpp"




class PercentWidget : public WidgetBase {
protected :
   double percent;/// [0.0 , 1.0]


public:
//   PercentWidget(std::string classname = "PercentWidget" , std::string objname = "Nemo");
   PercentWidget(std::string classname , std::string objname) :
         WidgetBase(classname , objname),
         percent(0.0)
   {}
   
   
   void SetPercent(double pct , bool sendmessage = true);///< Set the percent of the slider (from 0.0 to 1.0)
   void GetPercent(bool inverted = false);
   
};



class PercentBar : public PercentWidget {
protected :
   
   bool horizontal;
   bool invert;/// False - left to right or top to bottom, True - right to left or bottom to top
   
   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual int PrivateUpdate(double dt);
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);

public :

   PercentBar() :
         PercentWidget("PercentBar" , "% bar"),
         horizontal(true),
         invert(false)
   {}

   void SetOrientation(bool is_horizontal);
   void SetInversion(bool do_invert);
   bool IsHorizontal();
   bool IsInverted();

};


class PercentDial : public PercentWidget {
   
   /// 0.5 is N, 0.0 and 1.0 are S
   
protected :
   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual int PrivateUpdate(double dt);
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);

public :
   PercentDial() :
         PercentWidget("PercentDial" , "% dial"),
         


};





#endif // PercentWidgets_HPP
