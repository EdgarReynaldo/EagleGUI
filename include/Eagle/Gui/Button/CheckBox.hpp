
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

   virtual void OnAreaChanged();

public :
   
   BasicCheckBox(std::string objname = "Nemo");
   ~BasicCheckBox();
   
   void SetDrawFunc(CHECKBOXDRAWFUNC cbdrawfunc);
   virtual void SetImages(EagleImage* upimage , EagleImage* downimage , EagleImage* hoverupimage , EagleImage* hoverdownimage);

   void ToggleChecked();
   void SetChecked(bool checked);
   
   bool Checked() {return !Up();}
};


#endif // CheckBox_HPP

