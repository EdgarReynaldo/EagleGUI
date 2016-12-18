




#include "Eagle/Gui/BasicIcon.hpp"
#include "Eagle/StringWork.hpp"



BasicIcon::BasicIcon(std::string name) :
      WidgetBase(name),
      stretch_icon(false)
{
   if (name.compare("") == 0) {
      SetName(StringPrintF("BasicIcon at %p" , this));
   }
}




void BasicIcon::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   if (!icon_image) {return;}
   
   Rectangle src(0 , 0 , icon_image->W() , icon_image->H());
   if (stretch_icon) {
      Rectangle dest = InnerArea();
      dest.MoveBy(xpos,ypos);
      win->DrawStretchedRegion(icon_image , src , dest , DRAW_NORMAL);
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



void BasicIcon::SetImage(EagleImage* new_image) {
   icon_image = new_image;
   SetBgRedrawFlag();
}



std::ostream& BasicIcon::DescribeTo(std::ostream& os , Indenter indent) const {
   os << indent << StringPrintF("BasicIcon (%s) at %p" , GetName().c_str() , this) << std::endl;
   os << indent << StringPrintF("icon_image is %p and is %s , stretch_icon is %s",
                                icon_image , (icon_image && icon_image->Valid())?"Valid":"Invalid" , stretch_icon?"true":"false") << std::endl;
   WidgetBase::DescribeTo(os , indent);
   return os;
}




