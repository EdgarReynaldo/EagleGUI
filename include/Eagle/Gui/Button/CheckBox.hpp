



#ifndef CheckBox_HPP
#define CheckBox_HPP


#include "Eagle/Gui/Button/IconButton.hpp"



typedef void(*CHECKBOXDRAWFUNC)(EagleGraphicsContext* , Rectangle , int , int , const WidgetColorset& , bool , bool);

void DefaultCBDrawFunc(EagleGraphicsContext* win , Rectangle area , int xpos , int ypos , const WidgetColorset& wc , bool up , bool hover);



class BasicCheckBox : public IconButton {

protected :
   
   CHECKBOXDRAWFUNC draw_func;
   
   SHAREDIMAGE cb_images[4];
   
   bool current;
   
   
   
   void RefreshImages(EagleGraphicsContext* win);
   void FreeImages();

   /// IconButton
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);

   virtual void OnAreaChanged();

public :
   
   BasicCheckBox(std::string objname = "Nemo");
   ~BasicCheckBox();
   
   void SetDrawFunc(CHECKBOXDRAWFUNC cbdrawfunc);
   virtual void SetImages(SHAREDIMAGE upimage , SHAREDIMAGE downimage , SHAREDIMAGE hoverupimage , SHAREDIMAGE hoverdownimage);

   void ToggleChecked();
   void SetChecked(bool checked);
   
   bool Checked() {return !Up();}
};


#endif // CheckBox_HPP

