





#ifndef IconButton_HPP
#define IconButton_HPP


#include "Eagle/Gui/Button.hpp"
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

class IconButton : public Button {

private :
/**
   EagleImage** upimage;
   EagleImage** downimage;
   EagleImage** hoverupimage;
   EagleImage** hoverdownimage;
//*/
   EagleImage* original_images[4];/// These are just for reference, but they are used when the button size changes
///   EagleImage* scaled_images[4];/// We own these, they are scaled to fit the inner area

   /// BUTTON_STATE btn_state

   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);


public :
   
   IconButton() :
         Button(StringPrintF("IconButton object at %p" , this)),
         original_images()
   {
      for (int i = 0 ; i < 4 ; ++i) {
         original_images[i] = 0;
      }
   }
   
   void SetImages(EagleImage* upimage , EagleImage* downimage , EagleImage* hoverupimage , EagleImage* hoverdownimage);
   
   
   
   virtual void SetDrawDimensions(int width , int height , bool notify_layout = true);
   virtual void SetArea(int xpos , int ypos , int width , int height , bool notify_layout = true);
   virtual void SetArea(const Rectangle& r , bool notify_layout = true);

   


};

#endif // IconButton_HPP
