


#include "Eagle/Gui/Text.hpp"

#include "Eagle/Gui/Decorators/TextDecorator.hpp"

#include "Eagle/Gui/WidgetHandler.hpp"

#include "Eagle/StringWork.hpp"




/**
TextDecorator::TextDecorator() :
      WidgetDecorator(StringPrintF("TextDecorator at %p" , this)),
      basic_text_widget(0),
      default_text(),
      text_widget_layout(0),
      pin_layout(),
      center_text(true)
{
   UseTextLayout(&pin_layout);
   UseTextWidget(&default_text);
}



TextDecorator::TextDecorator(std::string name) :
      WidgetDecorator(name),
      basic_text_widget(0),
      default_text(),
      text_widget_layout(0),
      pin_layout(),
      center_text(true)
{
   UseTextLayout(&pin_layout);
   UseTextWidget(&default_text);
}



TextDecorator::TextDecorator(WidgetBase* widget_to_decorate , BasicText* basic_text) :
      WidgetDecorator(StringPrintF("TextDecorator at %p" , this)),
      basic_text_widget(0),
      default_text(),
      text_widget_layout(0),
      pin_layout(),
      center_text(true)
{
   DecorateWidget(widget_to_decorate);
   UseTextLayout(&pin_layout);
   UseTextWidget(basic_text);
}





TextDecorator::TextDecorator(std::string name , WidgetBase* widget_to_decorate , BasicText* basic_text,
                             Layout* widget_layout , Layout* text_layout) :
      WidgetDecorator(name),
      basic_text_widget(0),
      text_widget_layout(0),
      pin_layout(),
      center_text(true)
{
   DecorateWidget(widget_to_decorate);
   UseTextLayout(&pin_layout);
   UseTextWidget(basic_text);
}


//*/
TextDecorator::TextDecorator(WidgetBase* widget_to_decorate,
                             BasicText* basic_text,
                             Layout* widget_layout,
                             Layout* text_layout,
                             std::string name) :
      WidgetDecorator(widget_to_decorate , widget_layout , name),
      basic_text_widget(0),
      text_widget_layout(&pin_layout),
      pin_layout(),
      center_text(true)
{
   DecorateWidget(widget_to_decorate);
   UseLayout(widget_layout);
   UseTextWidget(basic_text);
   UseTextLayout(text_layout);
   if (name.compare("") == 0) {
      SetName(StringPrintF("TextDecorator at %p" , this));
   }
}



/// layout may be NULL to use the default DumbLayout
void TextDecorator::UseTextLayout(Layout* text_layout) {
   
   BasicText* old_text = basic_text_widget;
   
   UseTextWidget(0);
   
   text_widget_layout->SetParent(0);
   
   if (!text_layout) {
      text_layout = &pin_layout;
   }
   text_widget_layout = text_layout;

   text_widget_layout->SetParent(this);
   text_widget_layout->WidgetBase::SetWidgetArea(InnerArea() , false);
   UseTextWidget(old_text);
   
}



/// text_widget may be NULL to use the default text (which is blank until you set it)
void TextDecorator::UseTextWidget(BasicText* text_widget) {
   
   /// Remove old widget from layout
   if (basic_text_widget) {
      text_widget_layout->RemoveWidget(basic_text_widget);
      basic_text_widget->SetParent(0);
      basic_text_widget = 0;
   }
   
   if (!text_widget) {
      text_widget = &default_text;
   }
   basic_text_widget = text_widget;
   basic_text_widget->SetParent(this);
   text_widget_layout->Resize(1);
   text_widget_layout->PlaceWidget(basic_text_widget , 0);
   ReCenterText();
}



void TextDecorator::CenterText(bool center_the_text) {
   center_text = center_the_text;
   ReCenterText();
}



void TextDecorator::ReCenterText() {
   if (center_text) {
      pin_layout.SetPinPosition(0 , InnerArea().CX() , InnerArea().CY() , HALIGN_CENTER , VALIGN_CENTER);
   }
}



int TextDecorator::PrivateHandleEvent(EagleEvent e) {
   int retmsg = DIALOG_OKAY;
   
   retmsg |= basic_text_widget->HandleEvent(e);

   if (!(retmsg & DIALOG_INPUT_USED)) {
      retmsg |= text_widget_layout->HandleEvent(e);
   }
   if (!(retmsg & DIALOG_INPUT_USED)) {
      retmsg |= WidgetDecorator::HandleEvent(e);
   }
   return retmsg;
}



int TextDecorator::PrivateCheckInputs() {
   int retmsg = DIALOG_OKAY;
   return retmsg;
}



void TextDecorator::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   WidgetDecorator::Display(win,xpos,ypos);

   text_widget_layout->Display(win,xpos,ypos);

   basic_text_widget->Display(win,xpos,ypos);

   ClearRedrawFlag();
}



int TextDecorator::PrivateUpdate(double tsec) {
   int retmsg = DIALOG_OKAY;
   
   retmsg |= WidgetDecorator::Update(tsec);
   
   retmsg |= text_widget_layout->Update(tsec);
   
   retmsg |= basic_text_widget->Update(tsec);
   
   return retmsg;
}



void TextDecorator::SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout) {
   WidgetDecorator::SetWidgetArea(xpos,ypos,width,height,notify_layout);
   text_widget_layout->WidgetBase::SetWidgetArea(InnerArea() , false);
   ReCenterText();
}



void TextDecorator::SetMarginsExpandFromInner(int left , int right , int top , int bottom) {
   WidgetDecorator::SetMarginsExpandFromInner(left,right,top,bottom);
   text_widget_layout->WidgetBase::SetWidgetArea(InnerArea() , false);
   ReCenterText();
}



void TextDecorator::SetMarginsContractFromOuter(int left , int right , int top , int bottom) {
   WidgetDecorator::SetMarginsContractFromOuter(left,right,top,bottom);
   text_widget_layout->WidgetBase::SetWidgetArea(InnerArea() , false);
   ReCenterText();
}








/**
    void TextDecorator::SetEnabledState(bool state) {
        SetFlagState(state , SetEnabledState);
    }
//*/



void TextDecorator::SetEnabledState(bool state) {
   WidgetDecorator::SetEnabledState(state);

   text_widget_layout->SetEnabledState(state);

   basic_text_widget->SetEnabledState(state);
}



void TextDecorator::SetVisibilityState(bool state) {
   WidgetDecorator::SetVisibilityState(state);

   text_widget_layout->SetVisibilityState(state);

   basic_text_widget->SetVisibilityState(state);
}



void TextDecorator::SetHoverState(bool state) {

   if (!state) {
      basic_text_widget->SetHoverState(false);
      text_widget_layout->SetHoverState(false);
      WidgetDecorator::SetHoverState(false);
      return;
   }
   
   WidgetHandler* wh = NearestParentGui();
   if (wh) {
      int mx = wh->GetMouseX();
      int my = wh->GetMouseY();

      Rectangle r;

      r = basic_text_widget->OuterArea();
      if (r.Contains(mx,my)) {
         basic_text_widget->SetHoverState(true);
      }

      r = text_widget_layout->OuterArea();
      if (r.Contains(mx,my)) {
         text_widget_layout->SetHoverState(true);
      }
      
      r = OuterArea();
      if (r.Contains(mx,my)) {
         WidgetDecorator::SetHoverState(true);
      }
   }
   else {
      basic_text_widget->SetHoverState(true);
      text_widget_layout->SetHoverState(true);
      WidgetDecorator::SetHoverState(true);
   }
}



void TextDecorator::SetFocusState(bool state) {
   WidgetDecorator::SetFocusState(state);
   text_widget_layout->SetFocusState(state);
   basic_text_widget->SetFocusState(state);
}



void TextDecorator::SetMoveableState(bool state) {
   WidgetDecorator::SetMoveableState(state);
   text_widget_layout->SetMoveableState(state);
   basic_text_widget->SetMoveableState(state);
}



void TextDecorator::SetResizeableState(bool state) {
   WidgetDecorator::SetResizeableState(state);
   text_widget_layout->SetResizeableState(state);
   basic_text_widget->SetResizeableState(state);
}



void TextDecorator::SetNeedsRedrawState(bool state) {
   WidgetDecorator::SetNeedsRedrawState(state);
   text_widget_layout->SetNeedsRedrawState(state);
   basic_text_widget->SetNeedsRedrawState(state);
}



void TextDecorator::SetNeedsBgRedrawState(bool state) {
   WidgetDecorator::SetNeedsBgRedrawState(state);
   text_widget_layout->SetNeedsBgRedrawState(state);
   basic_text_widget->SetNeedsBgRedrawState(state);
}



void TextDecorator::SetAllowCloseState(bool state) {
   WidgetDecorator::SetAllowCloseState(state);
   text_widget_layout->SetAllowCloseState(state);
   basic_text_widget->SetAllowCloseState(state);
}



void TextDecorator::SetAllowOverlapState(bool state) {
   WidgetDecorator::SetAllowOverlapState(state);
   text_widget_layout->SetAllowOverlapState(state);
   basic_text_widget->SetAllowOverlapState(state);
}







