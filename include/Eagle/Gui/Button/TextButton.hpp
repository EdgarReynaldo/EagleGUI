



#ifndef TextButton_HPP
#define TextButton_HPP


#include "Eagle/Gui/Button/BasicButton.hpp"




























/**
class TextButton : public BasicButton {

protected :
   BasicButton* our_button;
   BasicText* our_text;
   
   BasicText basic_text;

public :
   
   TextButton();
   TextButton(BasicButton* button_to_use , BasicText* text_to_use);

   TextButton() :
      WidgetBase(StringPrintF("TextButton at %p" , this)),
      our_button(dynamic_cast<BasicButton*>(this)),
      our_text(0)
   {}
   TextButton(BasicButton* button_to_use , BasicText* text_to_use) :
      WidgetBase(StringPrintF("TextButton at %p" , this)),
      our_button(button_to_use?button_to_use:dynamic_cast<BasicButton*>(this)),
      our_text(text_to_use?text_to_use:0)
   {}
   
protected :
   /// WidgetBase
      
   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);

public :
   
   /// WidgetBase
   virtual void QueueUserMessage(const WidgetMsg& wmsg);// For the top level WidgetHandlers to store messages to the user from 

   
	/// Setters
	virtual void SetParent(WidgetBase* parent);
	virtual void         SetOwnerLayout(Layout* l);
	
	virtual void SetBackgroundPainter(BackgroundPainter* painter);
	virtual void SetFocusPainter(FocusPainter* painter);
	virtual void SetBackgroundDrawType(BG_DRAW_TYPE draw_type);
	virtual void SetFocusDrawType(FOCUS_DRAW_TYPE draw_type);
   


   
   
   /// These only set the colorset, they don't determine which you are using, and public colors are
   /// set by default - call UseColorset if you wanna change the setting
   virtual void SetColorset(const WidgetColorset& colors , bool set_descendants_colors = false);
   virtual void SetPrivateColorset(const WidgetColorset& colors);

   virtual void UseColorset(bool use_public_colorset);// false means use private colorset
   virtual void UsePrivateColorset(bool use_priv_colorset);

   /// Virtual Setters, override them if you need to
   virtual void SetFlagStates(UINT FLAGS , bool state);/// (Which flags set to which state)
   
   /// Default behaviour for state setters          Behaviour regarding redraw state
   virtual void SetEnabledState      (bool state);// Sets bg redraw flag
   virtual void SetVisibilityState   (bool state);// Sets bg redraw flag
   virtual void SetHoverState        (bool state);// /// Does not set any redraw flag  - OLD:Sets redraw flag and (false) sets bg redraw flag
   virtual void SetFocusState        (bool state);// Sets redraw flag and (false) sets bg redraw flag
   virtual void SetMoveableState     (bool state);// Does not set redraw flag
   virtual void SetResizeableState   (bool state);// Does not set redraw flag
   virtual void SetNeedsRedrawState  (bool state);
   virtual void SetNeedsBgRedrawState(bool state);// (true) sets redraw flag
   virtual void SetAllowCloseState   (bool state);// Does not set redraw flag
   virtual void SetAllowOverlapState (bool state);// Does not set redraw flag

   virtual void SetRedrawFlag();// Shortcut to SetNeedsRedrawState(true)
   virtual void SetBgRedrawFlag();// Shortcut to SetNeedsBgRedrawState(true)
   virtual void ClearRedrawFlag();// Shortcut to SetNeedsRedrawState(false) and SetNeedsBgRedrawState(false)

   virtual void SetRedrawAllFlag();// To tell parent widget handlers to redraw all widgets

   virtual void ShowWidget();// Makes the widget enabled and visible
   virtual void HideWidget();// Makes the widget disabled and invisible
   virtual void ToggleWidgetVisibility();// Toggles the enabled and visible state of the widget

   virtual bool AcceptsFocus() {return true;}
   virtual bool IsMouseOver(int realmsx , int realmsy) const;
   
   /// For widgets to ask their parent widget handlers to give them the focus
///   virtual bool GiveWidgetFocus(WidgetBase* widget);
   virtual bool GiveWidgetFocus(WidgetBase* widget , bool notify_parent = true);

   
   virtual void         SetBgImage(EagleImage* img , MARGIN_HCELL hcell, MARGIN_VCELL vcell);
   virtual void         SetBgImages(EagleImage* imgs[3][3]);
   virtual void         SetImagesHaveAlpha(bool have_alpha);

   virtual void SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout = true);

	/// Changes position and outer area!!!
	virtual void SetMarginsExpandFromInner(int left , int right , int top , int bottom);

	/// Make room in outer area for inner area first!!!
	virtual void SetMarginsContractFromOuter(int left , int right , int top , int bottom);

   virtual void SetMinInnerWidth(int w);
   virtual void SetMinInnerHeight(int h);
   virtual void SetMinInnerDimensions(int w , int h);

   virtual void SetDisplayPriority(int priority);

   virtual int AbsMinWidth() const {return 1;}
   virtual int AbsMinHeight() const {return 1;}
   
   virtual bool HasGui() {return false;}/// TODO : What is this function for again??? Oh right, it's for if the widget is a GUI
   virtual WidgetHandler* GetGui() {return 0;}/// TODO : What is this function for again??? Oh Right, it's for if the widget is a GUI

   virtual WidgetHandler* NearestParentGui();
   virtual WidgetBase*    Root();
   virtual WidgetHandler* RootGui();

   /// Getters , some references for direct modification
   virtual int                   AbsParentX() const ;
   virtual int                   AbsParentY() const ;
   virtual Pos2d                 GetParentOffset() const ;
   
   virtual WidgetBase*           Parent()          const {return wparent;}
   virtual WidgetColorset&       WCols();/// SetRedrawFlag if you change the colors!
   virtual const WidgetColorset& WCols()           const {return WCols();}/// SetRedrawFlag if you change the colors!
   virtual WidgetArea            Area()            const {return area;}
   virtual Rectangle             OuterArea()       const {return area.OuterArea();}
   virtual Rectangle             InnerArea()       const {return area.InnerArea();}
   virtual int                   MinWidth()        const {return minw + area.MLeft() + area.MRight();}
   virtual int                   MinHeight()       const {return minh + area.MTop() + area.MBot();}
   virtual int                   MinInnerWidth()   const {return minw;}
   virtual int                   MinInnerHeight()  const {return minh;}
   virtual UINT                  Flags()           const {return flags;}
   virtual int                   DisplayPriority() const {return display_priority;}
   
///   virtual std::string GetWidgetClassName()=0;/// TODO : What is this for? ICR. See how many classes implement this.
   virtual std::string GetWidgetClassName() {return "WidgetBase object";}/// TODO : What is this for? ICR.

   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const;
   
   
   
   

};


//*/


















/**
#include "Eagle/Gui/Button/BasicButton.hpp"
#include "Eagle/Gui/Decorators/TextDecorator.hpp"




class TextButton : public TextDecorator {/// Yes, we are a decorator, not a button, but we can cast to a button
   
protected :
   
   BasicButton* button_in_use;

   BasicButton basic_button;
   
public :
   
   
   
   operator(BasicButton*)() {return button_in_use;}
};




class TextButton : public BasicButton {
   
protected :
   
   BasicButton* button_in_use;/// May be 'this', may be user pointer, must not be NULL
   
   TextDecorator text_decorator;/// We are a button, but most of our functionality comes from TextDecorator
   
public :
   
   TextButton();
   TextButton(std::string name);
   
   TextButton() :
         
   {}
   TextButton(std::string name) :
         
   {}

   /// WidgetBase class forwarding functions
   
   /// Overridden WidgetBase functions, overload and call the Decorator versions from your new decorator classes
   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);

	virtual void SetOwnerLayout(Layout* l);

   virtual void SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout = true);

	/// Changes position and outer area!!!
	virtual void SetMarginsExpandFromInner(int left , int right , int top , int bottom);

	/// Make room in outer area for inner area first!!!
	virtual void SetMarginsContractFromOuter(int left , int right , int top , int bottom);

/// Overridden WidgetBase functions, overload and call the Decorator versions from your new decorator classes
int PrivateHandleEvent(EagleEvent e) {
   return text_decorator.PrivateHandleEvent(e);
}
int PrivateCheckInputs() {
   return text_decorator.PrivateCheckInputs();
}
void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   text_decorator.PrivateDisplay(win,xpos,ypos);
}
int PrivateUpdate(double tsec) {
   return text_decorator.PrivateUpdate(tsec);
}
void SetOwnerLayout(Layout* l) {
   text_decorator.SetOwnerLayout(l);
}

void SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout) {
   text_decorator.SetWidgetArea(xpos,ypos,width,height,notify_layout);
}

/// Changes position and outer area!!!
void SetMarginsExpandFromInner(int left , int right , int top , int bottom);

/// Make room in outer area for inner area first!!!
void SetMarginsContractFromOuter(int left , int right , int top , int bottom);

	
	
   /// TextDecorator forwarding functions
   
   virtual void UseLayoutForText(Layout* text_layout);/// layout may be NULL to use the default DumbLayout

   virtual void UseTextWidget(BasicText* text_widget);/// text_widget may be NULL to remove the text

   virtual void UseLayoutForButton(Layout* layout);/// layout may be NULL to use the default DumbLayout

   virtual void UseButton(BasicButton* button);/// button may be NULL to use the default button ('this')

/// layout may be NULL to use the default DumbLayout
void TextButton::UseLayoutForText(Layout* text_layout) {
   text_decorator.UseLayoutForText(text_layout);
}

/// text_widget may be NULL to remove the text
void TextButton::UseTextWidget(BasicText* text_widget) {
   text_decorator.UseTextWidget(text_widget);
}

/// layout may be NULL to use the default DumbLayout
void TextButton::UseLayoutForButton(Layout* layout) {
   text_decorator.UseLayoutForDecoratedWidget(layout);
}

/// button may be NULL to use the default button ('this')
void TextButton::UseButton(BasicButton* button) {
   text_decorator.DecorateWidget(button);
}

   /// TextButton member functions
   BasicButton* ButtonInUse() {return button_in_use;}/// Use this to interface with the actual button in use
      
   
};


//*/



#endif // TextButton_HPP




