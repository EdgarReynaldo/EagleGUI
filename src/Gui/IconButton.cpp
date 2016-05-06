


#include "Eagle/Gui/IconButton.hpp"




void IconButton::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   EagleImage* image = original_images[(int)btn_state];
   EAGLE_ASSERT(image && image->Valid());
   win->DrawStretchedRegion(image , 0 , 0 , image->W() , image->H() ,
                            InnerArea().X() + xpos , InnerArea().Y() + ypos , InnerArea().W() , InnerArea().H());
   ClearRedrawFlag();
}



void IconButton::SetImages(EagleImage* upimage , EagleImage* downimage , EagleImage* hoverupimage , EagleImage* hoverdownimage) {
   original_images[BUTTON_UP] = upimage;
   original_images[BUTTON_DOWN] = downimage;
   original_images[BUTTON_HOVER_UP] = hoverupimage;
   original_images[BUTTON_HOVER_DOWN] = hoverdownimage;
   SetBgRedrawFlag();
}



void IconButton::SetDrawDimensions(int width , int height , bool notify_layout) {
   WidgetBase::SetDrawDimensions(width,height,notify_layout);
///   RefreshImages();
}



void IconButton::SetArea(int xpos , int ypos , int width , int height , bool notify_layout) {
   WidgetBase::SetArea(xpos,ypos,width,height,notify_layout);
///   RefreshImages();
}



void IconButton::SetArea(const Rectangle& r , bool notify_layout) {
   WidgetBase::SetArea(r , notify_layout);
///   RefreshImages();
}
