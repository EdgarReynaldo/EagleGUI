
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
   
   virtual ~BasicIcon() {}
   
   void SetStretch(bool stretch);
   void SetImage(SHAREDIMAGE new_image);
   
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;
   
};





#endif // BasicIcon_HPP







