



#ifndef BasicIcon_HPP
#define BasicIcon_HPP



#include "Eagle/Gui/WidgetBase.hpp"




class BasicIcon : public WidgetBase {
   
   
protected :   
   
   SHAREDIMAGE icon_image;
   
   bool stretch_icon;
   
   
   
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   
public :
   
   BasicIcon(std::string objclass = "BasicIcon" , std::string objname = "Nemo");
   
   void SetStretch(bool stretch);
   void SetImage(SHAREDIMAGE new_image);
   
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;
   
};





#endif // BasicIcon_HPP







