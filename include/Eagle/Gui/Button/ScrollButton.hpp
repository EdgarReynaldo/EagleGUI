

#ifndef ScrollButton_HPP
#define ScrollButton_HPP


#include "Eagle/Gui/Button/BasicButton.hpp"
#include "Eagle/StringWork.hpp"



class BasicScrollBar;




class BasicScrollButton : public WidgetBase {

protected :
   BasicButton our_basic_button;

   BasicButton* scroll_button;
   BasicScrollBar* our_scrollbar;
   
   int increment;
   
   bool scroll_up_or_left;
   bool scroll_horizontal;
   
   Triangle our_click_area;

   void Scroll();

   void Reset();
   void ResetTriangle();
   void SyncButtonArea();

public :
   
   BasicScrollButton(std::string objclass = "BasicScrollButton" , std::string objname = "Nemo");
   ~BasicScrollButton() {}

   void UseBasicButton();
   void UseButton(BasicButton* button);/// Shallow copy - don't destroy this button while in use
   void SetScrollBar(BasicScrollBar* scrollbar);/// For scrollbars to use
   void SetIncrement(int new_increment);
   virtual void SetScrollDirection(bool up_or_left , bool horizontal);

   /// WidgetBase
   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);

   virtual void QueueUserMessage(const WidgetMsg& wmsg);
   
   virtual void SetColorset(const WidgetColorset& colors , bool set_descendants_colors = false);
   virtual void SetPrivateColorset(const WidgetColorset& colors);
   virtual void UseColorset(bool use_public_colorset);
   virtual void UsePrivateColorset(bool use_priv_colorset);

   virtual void SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout = true);
   virtual void SetMarginsContractFromOuter(int left , int right , int top , int bottom);



   
   
   
   
   
};



#endif // ScrollButton_HPP




