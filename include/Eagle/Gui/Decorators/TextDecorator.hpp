



#ifndef TextDecorator_HPP
#define TextDecorator_HPP



































/**

#include "Eagle/Gui/Decorators/Decorator.hpp"




class TextDecorator : public Decorator {
   
protected :
   BasicText* text_widget;
   Layout* text_widget_layout;
   
   DumbLayout dumb_text_layout;
   
///   WidgetBase* dwidget;
///   Layout* dwidget_layout;/// For controlling the decorated widget's position on our Decorator widget
///   DumbLayout dumb_layout;
   
   
public :
   
   TextDecorator();
   TextDecorator(std::string name);


   /// Overridden WidgetBase functions, overload and call the Decorator versions from your new decorator classes
   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);

   virtual void SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout = true);

	/// Changes position and outer area!!!
	virtual void SetMarginsExpandFromInner(int left , int right , int top , int bottom);

	/// Make room in outer area for inner area first!!!
	virtual void SetMarginsContractFromOuter(int left , int right , int top , int bottom);


   
   
   /// Decorator member functions
   
///   virtual void UseLayoutForDecoratedWidget(Layout* layout);/// layout may be NULL to use the default DumbLayout

///   virtual void DecorateWidget(WidgetBase* widget);/// widget may be NULL to remove the decorated widget
   
   /// TextDecorator member functions
   
   virtual void UseLayoutForText(Layout* text_layout);/// layout may be NULL to use the default DumbLayout
   
   virtual void UseTextWidget(BasicText* text_widget);/// text_widget may be NULL to remove the text

};
//*/

#endif // TextDecorator_HPP

