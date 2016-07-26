




#ifndef LinkText_HPP
#define LinkText_HPP


#include "Eagle/Gui/Text/GlobalText.hpp"
#include "Eagle/Gui/Text/SelectText.hpp"



extern const int LINK_LAUNCHED;

REGISTER_WIDGET_MESSAGE(TOPIC_TEXT_WIDGET , LINK_LAUNCHED);



class LinkText : public SelectText {

   float vline_padding;
   float lineheight;
   
protected :
   virtual void RefreshTextPosition(int lineheight);

   virtual void ResetLinePadding();
   
public :
///   LinkText();
   LinkText() :
      SelectText(),
      vline_padding(0),
      lineheight(0)
   {
      
   }
   
   /// overload hoverstate

   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);

   void SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout = true);

   virtual void SetHoverState(bool state);

   virtual void SetupText(HALIGNMENT hal , VALIGNMENT val , int hpad , int vpad , int vspacing ,
                          std::string textstr , EagleFont* font);
   
   virtual void SetText(std::string textstr , EagleFont* font);
   
   virtual void SetFont(EagleFont* font);


   void LaunchLink();
   
};



#endif // LinkText_HPP

