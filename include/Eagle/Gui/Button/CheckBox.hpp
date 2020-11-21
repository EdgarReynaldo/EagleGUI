
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
 *
 *
 * 
 * @file CheckBox.hpp
 * @brief This file contains a generic check box class, with a premade icon drawing function
 * 
 */



#ifndef CheckBox_HPP
#define CheckBox_HPP


#include "Eagle/Gui/Button/IconButton.hpp"


/*! @brief CHECKBOXDRAWFUNC is a type for drawing the checkbox, you can use this type to override the default drawing function */
typedef void(*CHECKBOXDRAWFUNC)(EagleGraphicsContext* , Rectangle , int , int , const WidgetColorset& , bool , bool);

/*! @brief The default check box drawing function. Use this to reset the drawing function set in the check box */
void DefaultCBDrawFunc(EagleGraphicsContext* win , Rectangle area , int xpos , int ypos , const WidgetColorset& wc , bool up , bool hover);


/*! @class BasicCheckBox
 *  @brief A simple icon button with a premade check box drawing function
 *
 *
 */

class BasicCheckBox : public IconButton {

protected :
   
   CHECKBOXDRAWFUNC draw_func;
   
   EagleImage* cb_images[4];
   
   bool current;
   
   
   
   void RefreshImages(EagleGraphicsContext* win);
   void FreeImages();

   /// IconButton
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);

   virtual void OnAreaChanged();

public :
   
   BasicCheckBox(std::string objname = "Nemo");
   ~BasicCheckBox();
   
   void SetDrawFunc(CHECKBOXDRAWFUNC cbdrawfunc);///< Set your own custom drawing function for the checkbox
   virtual void SetImages(EagleImage* upimage , EagleImage* downimage , EagleImage* hoverupimage , EagleImage* hoverdownimage);///< Set your images

   void ToggleChecked();///< Toggles the checkmark
   void SetChecked(bool checked);///< Sets whether or not it is checked (down)
   
   bool Checked() {return !Up();}///< Returns the checked state or whether it is down
};




#endif // CheckBox_HPP




