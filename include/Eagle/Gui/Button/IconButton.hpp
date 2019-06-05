
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
 *
 *
 *
 * @file IconButton.hpp
 * @brief Generic class for icon buttons and ninepatches
 * 
 */




#ifndef IconButton_HPP
#define IconButton_HPP

#include "Eagle/Area.hpp"
#include "Eagle/Gui/Button/BasicButton.hpp"
#include "Eagle/Image.hpp"
#include "Eagle/StringWork.hpp"



/**! @class IconButton
 *   @brief A simple class to draw icon buttons in 4 different states
 */

class IconButton : public BasicButton {

protected :
   EagleImage* original_images[4];///< These are just for reference, but they are used when the button size changes
///   EagleImage* scaled_images[4];///< We own these, they are scaled to fit the inner area

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
};




#endif // IconButton_HPP



