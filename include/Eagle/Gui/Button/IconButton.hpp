
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
 *    Copyright 2009-2023+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 *
 *
 *
 * @file IconButton.hpp
 * @brief Generic class for icon buttons and hover buttons
 * 
 */




#ifndef IconButton_HPP
#define IconButton_HPP

#include "Eagle/Area.hpp"
#include "Eagle/Gui/Button/BasicButton.hpp"
#include "Eagle/Image.hpp"
#include "Eagle/StringWork.hpp"



class IconButtonBase : public BasicButton {
public :
   IconButtonBase(std::string classname = "IconButtonBase" , std::string objname = "Nemo");
   virtual void DisplayIcon(EagleGraphicsContext* win , BUTTON_STATE state , int xpos , int ypos , EagleColor tint = EagleColor(255,255,255))=0;
};


/**! @class IconButton
 *   @brief A simple class to draw icon buttons in 4 different states
 */

class IconButton : public IconButtonBase {

protected :
   EagleImage* original_images[4];///< These are just for reference, but they are used when the button size changes

   Rectangle click_rect;

      

   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);

   virtual void OnAreaChanged();

public :
   
   IconButton(std::string classname = "IconButton" , std::string objname = "Nemo");

   virtual ~IconButton() {}
   
   virtual void SetImages(EagleImage* upimage ,
                          EagleImage* downimage , 
                          EagleImage* hoverupimage , 
                          EagleImage* hoverdownimage);///< References these image states for the button

   virtual void DisplayIcon(EagleGraphicsContext* win , BUTTON_STATE state , int xpos , int ypos , EagleColor tint = EagleColor(255,255,255));
};



/**!  @class HoverIconButton
 *    @brief A simple class to tint the image based on the hover and up states
 */


class HoverIcon : public IconButton {
public :
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
};



#endif // IconButton_HPP




