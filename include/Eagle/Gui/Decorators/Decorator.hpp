


#ifndef Decorator_HPP
#define Decorator_HPP

class WidgetBase;




#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/Gui/Layout/Layout.hpp"


#include <string>


class Decorator : public WidgetBase {
   
protected :
   WidgetBase* dwidget;
   
   void SetWidget(WidgetBase* widget) {
      EAGLE_ASSERT(widget);
      dwidget = widget;
      dwidget->SetParent(this);
   }
   
   
public :
   Decorator(WidgetBase* widget_to_decorate , std::string name) :
         WidgetBase(name),
         dwidget(widget_to_decorate)
   {
      SetWidget(widget_to_decorate);
   }
//

   /// Overridden WidgetBase functions
   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);

   virtual void SetWidgetArea(int xpos , int ypos , int width , int height);
/*
int Decorator::PrivateHandleEvent(EagleEvent e) {
   return dwidget->PrivateHandleEvent(e);
}
int Decorator::PrivateCheckInputs() {
   return dwidget->PrivateCheckInputs();
}
void Decorator::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   dwidget->PrivateDisplay(win , xpos , ypos);
}
int Decorator::PrivateUpdate(double tsec) {
   dwidget->PrivateUpdate(tsec);
}

void Decorator::SetDrawPos(int xpos , int ypos) {
   WidgetBase::SetDrawPos(xpos,ypos);
   dwidget->SetDrawPos(xpos,ypos);
}
void Decorator::SetDrawDimensions(int width , int height) {
   
}
void Decorator::SetArea(int xpos , int ypos , int width , int height) {
   
}
void Decorator::SetArea(const Rectangle& r) {
   
}
*/
   
   WidgetBase* Widget() {return dwidget;}
};




#endif // Decorator_HPP

