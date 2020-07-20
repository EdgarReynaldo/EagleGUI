
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
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file GuiButton.hpp
 * @brief This file contains a generic, jack of all trades shaped gui button class
 * 
 */



#ifndef EagleGuiButton_HPP
#define EagleGuiButton_HPP


#include "Eagle/InputHandler.hpp"
#include "Eagle/Gui/Button/BasicButton.hpp"

#include <string>



/** Basic variety of simple spring or toggle buttons - rectangular, circular, rounded rectangular, and elliptical.
      Support for displaying text on the button included, will probably not look right on the circular buttons due to size.
*/

class EagleFont;



class GuiButton : public BasicButton {

protected :

   
   BUTTON_SHAPE btn_shape;///< GuiButton shape determines hit detection properties as well
   
   EagleFont* text_font;///< Font to use 
   std::string text;///< Text to display


   int rad_a;///< Primary radius for circles and the horizontal radius for ellipse type buttons,
             ///< as well as the corner radius for rounded rectangular buttons
   int rad_b;///< Secondary vertical radius, only for ellipses

   float hradpercent;///< How rounded the horizontal radius is, for ROUND_BTNS
   float vradpercent;///< How rounded the vertical radius is, for ROUND_BTNs
   

   void ResetRadii();
   void ResetClickArea();

   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);

   virtual void OnAreaChanged();

public :
   /// GuiButton area is determined from the button shape, and the radii are set accordingly
   GuiButton(std::string classname = "GuiButton" , std::string name = "Nemo");

   virtual ~GuiButton() {}

/**   virtual bool DoIReallyHaveHover(int mx , int my);
bool GuiButton::DoIReallyHaveHover(int mx , int my) {
   
}*/

   virtual void SetRedrawFlag();///< Shortcut to SetBgRedrawFlag, since we always need our background re-drawn

   /// Setters
   void SetButtonType(BUTTON_SHAPE shape , BUTTON_ACTION_TYPE action_type , BUTTON_CLASS button_class);///< Set the button properties
   
   virtual void SetLabel(std::string label_text);///< Set the label
   
   void SetRoundingPercent(float hpercent , float vpercent);///< Set the rounding percent for ROUNDED_BTN type buttons

   virtual void SetFont(EagleFont* textfont);///< Set the font to use, you must set a font before trying to draw a non-empty string

   /// Getters
   std::string        Text();///< Get the text for the label
   EagleFont*         Font();///< Get the font in use
   BUTTON_SHAPE       ButtonShape();///< The shape type of the button
   BUTTON_CLASS       ButtonClass();///< The class type of the button

   int                RadiusA();///< Horizontal radius, for ROUNDED_BTNs
   int                RadiusB();///< Vertical radius, for ROUNDED_BTNs

   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;///< Describe this object to the stream
};



void DrawGuiButtonShape(EagleGraphicsContext* win , GuiButton* btn , int x , int y);///< Global function to draw a gui button's shape

void DrawGuiButtonText(EagleGraphicsContext* win , GuiButton* btn , int x , int y);///< Global function to draw a gui button's text




#endif // EagleGuiGuiButton_HPP



