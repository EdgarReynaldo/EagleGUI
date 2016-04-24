
#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/Color.hpp"

class TestWidget : public WidgetBase {
   
   EagleColor bgcolor;
   
public :
   
   TestWidget();

protected :
   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);
   
public :
   
   virtual void SetRedrawFlag();
   
   void SetColor(EagleColor c);
   void RandomizeColor();
   void FadeColor();
};
