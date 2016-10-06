


/**
#include "Eagle/Gui/Decorators/TextDecorator.hpp"











TextDecorator::TextDecorator() :
      Decorator(StringPrintF("TextDecorator at %p" , this)),
      text_widget(0),
      text_widget_layout(0),
      dumb_text_layout(StringPrintF("TextDecorator's dumb_text_layout at %p" , &dumb_text_layout));
{
   UseTextLayout(&dumb_text_layout);
}



TextDecorator::TextDecorator(std::string name) :
      Decorator(name),
      text_widget(0),
      text_widget_layout(0),
      dumb_text_layout(StringPrintF("%s's dumb_text_layout at %p" , name.c_str() , &dumb_text_layout));
{
   UseTextLayout(&dumb_text_layout);
}



int TextDecorator::PrivateHandleEvent(EagleEvent e) {
   int retmsg = DIALOG_OKAY;
   
   retmsg |= text_widget->PrivateHandleEvent(e);
   if (!(retmsg & DIALOG_INPUT_USED)) {
      retmsg |= text_widget_layout->PrivateHandleEvent(e);
   }
   if (!(retmsg & DIALOG_INPUT_USED)) {
      retmsg |= Decorator::PrivateHandleEvent(e);
   }
   return retmsg;
}



int TextDecorator::PrivateCheckInputs() {
   int retmsg = DIALOG_OKAY;
   
   retmsg |= text_widget->PrivateCheckInputs();
   if (!(retmsg & DIALOG_INPUT_USED)) {
      retmsg |= text_widget_layout->PrivateCheckInputs();
   }
   if (!(retmsg & DIALOG_INPUT_USED)) {
      retmsg |= Decorator::PrivateCheckInputs();
   }
   return retmsg;
}



void TextDecorator::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   Decorator::PrivateDisplay(win,xpos,ypos);
   text_widget_layout->PrivateDisplay(win,xpos,ypos);
   if (text_widget) {
      text_widget->PrivateDisplay(win,xpos,ypos);
   }
   ClearRedrawFlag();
}



int TextDecorator::PrivateUpdate(double tsec) {
   int retmsg = DIALOG_OKAY;
   
   retmsg |= Decorator::Update(tsec);
   
   retmsg |= text_widget_layout->Update(tsec);
   
   if (text_widget) {
      retmsg |= text_widget->Update(tsec);
   }
   
   return retmsg;
}



void TextDecorator::SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout = true) {
   Decorator::SetWidgetArea(xpos,ypos,width,height,notify_layout);
   text_widget_layout->SetWidgetArea(InnerArea() , false);
}



/// Changes position and outer area!!!
void TextDecorator::SetMarginsExpandFromInner(int left , int right , int top , int bottom) {
   Decorator::SetMarginsExpandFromInner(left,right,top,bottom);
   text_widget_layout->SetWidgetArea(InnerArea() , false);
}



// Make room in outer area for inner area first!!!
void TextDecorator::SetMarginsContractFromOuter(int left , int right , int top , int bottom) {
   Decorator::SetMarginsContractFromOuter(left,right,top,bottom);
   text_widget_layout->SetWidgetArea(InnerArea() , false);
}


   
/// layout may be NULL to use the default DumbLayout
void TextDecorator::UseLayoutForText(Layout* text_layout) {
   
   /// Remove widget from old layout
   if (text_widget_layout) {
      text_widget_layout->PlaceWidget(0 , 0);
   }
   
   if (!text_layout) {
      text_layout = &dumb_text_layout;
   }
   
   text_widget_layout = text_layout;
   
   text_widget_layout->SetParent(this);/// So QueueUserMessage works as expected
   
   text_widget_layout->SetWidgetArea(InnerArea() , false);
   
   text_widget_layout->Resize(1);
   
   text_widget_layout->PlaceWidget(text_widget , 0);/// text_widget may be NULL
   
}



/// text_widget may be NULL to remove the text
void TextDecorator::UseTextWidget(BasicText* text_decorator_widget) {
   
   /// Remove old widget from layout
   text_widget_layout->PlaceWidget(0 , 0);
   
   text_widget = text_decorator_widget;
   
   text_widget->SetParent(this);
   
   text_widget_layout->PlaceWidget(text_widget , 0);
   
}


//*/



