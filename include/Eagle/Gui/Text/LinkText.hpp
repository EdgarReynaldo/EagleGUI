




#ifndef LinkText_HPP
#define LinkText_HPP


#include "Eagle/Gui/Text/GlobalText.hpp"
#include "Eagle/Gui/Text/SelectText.hpp"
#include "Eagle/StringWork.hpp"


extern const int LINK_LAUNCHED;

REGISTER_WIDGET_MESSAGE(TOPIC_TEXT_WIDGET , LINK_LAUNCHED);



class LinkText : public SelectText {

protected :

   float link_height;
   

   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);

   virtual void OnFlagChanged(WIDGET_FLAGS f , bool on);

public :
   LinkText(std::string classname = "LinkText" , std::string name = "Nemo");
   
   virtual void SetFont(EagleFont* font);

   virtual void SetLineSpacing(int vspacing);

   void LaunchLink();
   
};



#endif // LinkText_HPP

