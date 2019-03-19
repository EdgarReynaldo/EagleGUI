
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



#ifndef IconButton_HPP
#define IconButton_HPP

#include "Eagle/Area.hpp"
#include "Eagle/Gui/Button/BasicButton.hpp"
#include "Eagle/Image.hpp"
#include "Eagle/StringWork.hpp"

/**
enum BUTTON_STATE {
   BUTTON_UP = 0,
   BUTTON_DOWN = 1,
   BUTTON_HOVER_UP = 2,
   BUTTON_HOVER_DOWN = 3
};
//*/

class IconButton : public BasicButton {

protected :
   EagleImage* original_images[4];/// These are just for reference, but they are used when the button size changes
///   EagleImage* scaled_images[4];/// We own these, they are scaled to fit the inner area

   Rectangle click_rect;

      

   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);

   virtual void OnAreaChanged();

public :
   
   IconButton(std::string classname = "IconButton" , std::string objname = "Nemo");

   virtual ~IconButton() {}
   
   virtual void SetImages(EagleImage* upimage , EagleImage* downimage , EagleImage* hoverupimage , EagleImage* hoverdownimage);
};

#endif // IconButton_HPP
