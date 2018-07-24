





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
   SHAREDIMAGE original_images[4];/// These are just for reference, but they are used when the button size changes
///   SHAREDIMAGE scaled_images[4];/// We own these, they are scaled to fit the inner area

   Rectangle click_rect;

      

   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);

   virtual void OnAreaChanged();

public :
   
   IconButton(std::string classname = "IconButton" , std::string objname = "Nemo");

   virtual ~IconButton() {}
   
   virtual void SetImages(SHAREDIMAGE upimage , SHAREDIMAGE downimage , SHAREDIMAGE hoverupimage , SHAREDIMAGE hoverdownimage);
};

#endif // IconButton_HPP
