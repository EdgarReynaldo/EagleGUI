





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
/**
   EagleImage** upimage;
   EagleImage** downimage;
   EagleImage** hoverupimage;
   EagleImage** hoverdownimage;
//*/
   EagleImage* original_images[4];/// These are just for reference, but they are used when the button size changes
///   EagleImage* scaled_images[4];/// We own these, they are scaled to fit the inner area

   /// BUTTON_STATE btn_state
   Rectangle click_rect;
   

   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);


   IconButton(std::string classname , std::string objname);
public :
   
   IconButton(std::string object_name);
   ~IconButton() {}
   
   virtual void SetImages(EagleImage* upimage , EagleImage* downimage , EagleImage* hoverupimage , EagleImage* hoverdownimage);
   
   
   virtual void SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout = true);

   


};

#endif // IconButton_HPP
