



#ifndef TextDecorator_HPP
#define TextDecorator_HPP



#include "Eagle/Gui/Decorators/Decorator.hpp"
#include "Eagle/Gui/Layout/PinLayout.hpp"
#include "Eagle/Gui/Text/BasicText.hpp"



class TextDecorator : public WidgetDecorator {
   
protected :
   BasicText* basic_text_widget;
   
   BasicText default_text;
   
   Layout* text_widget_layout;
   
   PinLayout pin_layout;

   bool center_text;
   
   
/**
   void SetFlagState(bool state , void (WidgetBase::*StateSetter)(bool));

    void TextDecorator::SetFlagState(bool state , void (WidgetBase::*StateSetter)(bool)) {
        if (basic_text_widget) {
           (basic_text_widget->WidgetBase::*StateSetter)(state);
        }
        (text_widget_layout->WidgetBase::->*StateSetter)(state);
        (this->WidgetDecoratorBase::->*StateSetter)(state);
    }
//*/
   
public :
/**   
   TextDecorator();
   TextDecorator(std::string name);
   
   TextDecorator(WidgetBase* widget_to_decorate , BasicText* basic_text);
   TextDecorator(std::string name , WidgetBase* widget_to_decorate , BasicText* basic_text);
//*/
   
   TextDecorator(WidgetBase* widget_to_decorate = 0,
                 BasicText* basic_text = 0,
                 Layout* widget_layout = 0,
                 Layout* text_layout = 0,
                 std::string name = "");
                 
   
   
   /// TextDecorator member functions
   
   void UseTextLayout(Layout* text_layout);/// layout may be NULL to use the default PinLayout
   
   void UseTextWidget(BasicText* text_widget);/// text_widget may be NULL to use the default text (which is blank until you set it)
      
   void CenterText(bool center_the_text);
   void ReCenterText();

   WidgetBase* DecoratedWidget() {return decorated_widget;}
   
   BasicText* GetTextWidget() {return basic_text_widget;}
   
   
///   operator BasicText*() {return basic_text_widget;}
   
   
   
   
   
   /// Overridden WidgetBase functions
   
   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);

   virtual void SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout = true);

	virtual void SetMarginsExpandFromInner(int left , int right , int top , int bottom);

	virtual void SetMarginsContractFromOuter(int left , int right , int top , int bottom);



   virtual void SetEnabledState      (bool state);
   virtual void SetVisibilityState   (bool state);
   virtual void SetHoverState        (bool state);
   virtual void SetFocusState        (bool state);
   virtual void SetMoveableState     (bool state);
   virtual void SetResizeableState   (bool state);
   virtual void SetNeedsRedrawState  (bool state);
   virtual void SetNeedsBgRedrawState(bool state);
   virtual void SetAllowCloseState   (bool state);
   virtual void SetAllowOverlapState (bool state);
   
   
   /// Decorator member functions
   
///   virtual void UseLayoutForDecoratedWidget(Layout* layout);/// layout may be NULL to use the default DumbLayout

///   virtual void DecorateWidget(WidgetBase* widget);/// widget may be NULL to remove the decorated widget
   

};



   

#endif // TextDecorator_HPP
