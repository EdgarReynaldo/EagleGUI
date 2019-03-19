
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




#include "Eagle/Gui/BasicIcon.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/GraphicsContext.hpp"




BasicIcon::BasicIcon(std::string objclass , std::string objname) :
      WidgetBase(objclass , objname),
      stretch_icon()
{
   
}




void BasicIcon::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   if (!icon_image) {return;}
   
   Rectangle src(0 , 0 , icon_image->W() , icon_image->H());
   if (stretch_icon) {
      Rectangle dest = InnerArea();
      dest.MoveBy(xpos,ypos);
      win->DrawStretchedRegion(icon_image , src , dest);
   }
   else {
      int x = xpos + (InnerArea().W() - icon_image->W())/2;
      int y = ypos + (InnerArea().H() - icon_image->H())/2;
      win->Draw(icon_image , InnerArea().X() + x , InnerArea().Y() + y);
   }
}



void BasicIcon::SetStretch(bool stretch) {
   stretch_icon = stretch;
   SetBgRedrawFlag();
}



void BasicIcon::SetImage(SHAREDIMAGE new_image) {
   icon_image = new_image;
   SetBgRedrawFlag();
}



std::ostream& BasicIcon::DescribeTo(std::ostream& os , Indenter indent) const {
   os << indent << FullName() << " :" << std::endl;
   os << indent << StringPrintF("icon_image is %p and is %s , stretch_icon is %s",
                                (const EagleImage*)icon_image , (icon_image && icon_image->Valid())?"Valid":"Invalid" , stretch_icon?"true":"false") << std::endl;
   WidgetBase::DescribeTo(os , indent);
   return os;
}




