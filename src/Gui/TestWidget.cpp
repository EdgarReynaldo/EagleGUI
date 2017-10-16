

#include "Eagle/Gui/TestWidget.hpp"
#include "Eagle/InputHandler.hpp"

#include <cstdlib>


#include "Eagle/StringWork.hpp"


TestWidget::TestWidget() :
      WidgetBase("TestWidget" , "Test"),
      bgcolor() 
{
   RandomizeColor();
}



int TestWidget::PrivateHandleEvent(EagleEvent e) {
   (void)e;
   return DIALOG_OKAY;
}



int TestWidget::PrivateCheckInputs() {
   if (input_mouse_press(LMB)) {
//      OutputLog() << StringPrintF("Mouse x,y = %i,%i and InnerArea = " , mouse_x , mouse_y) << InnerArea() << std::endl;
   }
   if (input_mouse_press(LMB) && area.InnerArea().Contains(mouse_x , mouse_y)) {
      RandomizeColor();
   }
   return DIALOG_OKAY;
}






void TestWidget::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {

   Rectangle r = area.InnerArea();
   r.MoveBy(xpos,ypos);
   win->DrawFilledRectangle(r , bgcolor);

   ClearRedrawFlag();
}



int TestWidget::PrivateUpdate(double tsec) {
   (void)tsec;
//   FadeColor();
   return DIALOG_OKAY;
}



void TestWidget::SetRedrawFlag() {
   SetBgRedrawFlag();
}



void TestWidget::SetColor(EagleColor c) {
   bgcolor = c;
   SetRedrawFlag();
}



void TestWidget::RandomizeColor() {
   bgcolor = EagleColor(rand()%255 , rand()%255 , rand()%255);
   SetRedrawFlag();
}



void TestWidget::FadeColor() {
   bgcolor.SetColor(bgcolor.r - 1 , bgcolor.g - 1 , bgcolor.b - 1);
   SetRedrawFlag();
}



