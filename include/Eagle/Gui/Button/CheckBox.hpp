



#ifndef CheckBox_HPP
#define CheckBox_HPP


#include "Eagle/Gui/Button/IconButton.hpp"



typedef void(*CHECKBOXDRAWFUNC)(EagleGraphicsContext* , Rectangle , int , int , const WidgetColorset& , bool , bool);

void DefaultCBDrawFunc(EagleGraphicsContext* win , Rectangle area , int xpos , int ypos , const WidgetColorset& wc , bool up , bool hover);



class BasicCheckBox : public IconButton {

protected :
   
   CHECKBOXDRAWFUNC draw_func;
   
   EagleImage* cb_images[4];
   
   bool current;
   
   
   
   void RefreshImages(EagleGraphicsContext* win);
   void FreeImages();

   /// IconButton
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);

public :
   
   BasicCheckBox(std::string objname = "Nemo");
   ~BasicCheckBox();
   
   /// WidgetBase
   virtual void SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout = true);/// Pass INT_MAX if you don't care
   
   
   
   
   void SetDrawFunc(CHECKBOXDRAWFUNC cbdrawfunc);
   virtual void SetImages(EagleImage* upimage , EagleImage* downimage , EagleImage* hoverupimage , EagleImage* hoverdownimage);

   void ToggleChecked();
   void SetChecked(bool checked);
   
   bool Checked() {return !Up();}
};


#endif // CheckBox_HPP
