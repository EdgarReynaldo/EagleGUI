
#error DECORATOR NOT IN USE

#ifndef Decorator_HPP
#define Decorator_HPP


#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/Gui/Layout/FillLayout.hpp"



class WidgetDecorator : public WidgetBase {
   
protected :

   WidgetBase* decorated_widget;
   
   Layout* layout;
   
   FillLayout default_fill_layout;
   
   

   virtual void PrivateRaiseEvent(WidgetMsg msg);

public :
      
   WidgetDecorator(WidgetBase* widget_to_decorate = 0,
                   Layout* widget_layout = 0,
                   std::string objclass = "WidgetDecorator",
                   std::string objname = "Nemo");

   virtual ~WidgetDecorator();

   
   void DecorateWidget(WidgetBase* widget_to_decorate);
   
   void UseLayout(Layout* new_layout);

   /// Forwarding functions inherited from WidgetBase

   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);

   /// Allow these functions to be called normally
///   virtual int HandleEvent(EagleEvent e);

///   virtual void Display(EagleGraphicsContext* win , int xpos , int ypos);

///   virtual int Update(double tsec);

   virtual void QueueUserMessage(const WidgetMsg& wmsg);

	/// Setters
	virtual void SetParent(WidgetBase* parent);
	virtual void SetOwnerLayout(Layout* l);
	
	virtual void SetBackgroundPainter(BackgroundPainter* painter);
	virtual void SetFocusPainter(FocusPainter* painter);
	virtual void SetBackgroundDrawType(BG_DRAW_TYPE draw_type);
	virtual void SetFocusDrawType(FOCUS_DRAW_TYPE draw_type);
   
   /// These only set the colorset, they don't determine which you are using, and public colors are
   /// set by default - call UseColorset if you wanna change the setting
   virtual void SetColorset(const WidgetColorset& colors , bool set_descendants_colors = false);
   virtual void SetPrivateColorset(const WidgetColorset& colors);

   virtual void UseColorset(bool use_public_colorset);/// false means use private colorset
   virtual void UsePrivateColorset(bool use_priv_colorset);

   virtual void SetFlagStates(UINT FLAGS , bool state);/// (Which flags set to which state)
   
   /// Default behaviour for state setters          Behaviour regarding redraw state
   virtual void SetEnabledState      (bool state);/// Sets bg redraw flag
   virtual void SetVisibilityState   (bool state);/// Sets bg redraw flag
   virtual void SetHoverState        (bool state);/// Does not set any redraw flag  - OLD:Sets redraw flag and (false) sets bg redraw flag
   virtual void SetFocusState        (bool state);/// Sets redraw flag and (false) sets bg redraw flag
   virtual void SetMoveableState     (bool state);/// Does not set redraw flag
   virtual void SetResizeableState   (bool state);/// Does not set redraw flag
   virtual void SetNeedsRedrawState  (bool state);
   virtual void SetNeedsBgRedrawState(bool state);/// (true) sets redraw flag
   virtual void SetAllowCloseState   (bool state);/// Does not set redraw flag
   virtual void SetAllowOverlapState (bool state);/// Does not set redraw flag

/** We don't need to override these functions, they function properly on their own

   virtual void SetRedrawFlag();/// Shortcut to SetNeedsRedrawState(true)
   virtual void SetBgRedrawFlag();/// Shortcut to SetNeedsBgRedrawState(true)
   virtual void ClearRedrawFlag();/// Shortcut to SetNeedsRedrawState(false) and SetNeedsBgRedrawState(false)

   virtual void SetRedrawAllFlag();/// To tell parent widget handlers to redraw all widgets

   virtual void ShowWidget();/// Makes the widget enabled and visible
   virtual void HideWidget();/// Makes the widget disabled and invisible
   virtual void ToggleWidgetVisibility();/// Toggles the enabled and visible state of the widget

//*/
   
   virtual bool AcceptsFocus();
   virtual bool IsMouseOver(int realmsx , int realmsy) const;
   
   virtual bool GiveWidgetFocus(WidgetBase* widget , bool notify_parent = true);

   virtual void SetBgImage(EagleImage* img , MARGIN_HCELL hcell, MARGIN_VCELL vcell);
   virtual void SetBgImages(EagleImage* imgs[3][3]);
   virtual void SetImagesHaveAlpha(bool have_alpha);
   
   virtual void SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout = true);

	virtual void SetMarginsExpandFromInner(int left , int right , int top , int bottom);

	virtual void SetMarginsContractFromOuter(int left , int right , int top , int bottom);

   virtual void SetMinInnerWidth(int w);
   virtual void SetMinInnerHeight(int h);
   virtual void SetMinInnerDimensions(int w , int h);

   virtual void SetDisplayPriority(int priority);

   virtual int AbsMinWidth() const ;
   virtual int AbsMinHeight() const ;
   
   virtual bool HasGui();
   virtual WidgetHandler* GetGui();
   
   virtual WidgetHandler* NearestParentGui();
   virtual WidgetBase*    Root();
   virtual WidgetHandler* RootGui();

   /// Getters , some references for direct modification
   virtual int                   AbsParentX() const ;
   virtual int                   AbsParentY() const ;
   virtual Pos2I                 GetParentOffset() const ;
   
   virtual WidgetBase*           Parent()          const ;
   virtual WidgetColorset&       WCols();
   virtual const WidgetColorset& WCols()           const ;
   virtual WidgetArea            Area()            const ;
   virtual Rectangle             OuterArea()       const ;
   virtual Rectangle             InnerArea()       const ;
   virtual int                   MinWidth()        const ;
   virtual int                   MinHeight()       const ;
   virtual int                   MinInnerWidth()   const ;
   virtual int                   MinInnerHeight()  const ;
   virtual UINT                  Flags()           const ;
   virtual int                   DisplayPriority() const ;
   
   virtual BackgroundPainter*    GetBackgroundPainter()  const ;
   virtual BG_DRAW_TYPE          GetBackgroundDrawType() const ;
   virtual FocusPainter*         GetFocusPainter()       const ;
   virtual FOCUS_DRAW_TYPE       GetFocusDrawType()      const ;

   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;
   
};

















/**
class Decorator : public WidgetBase {
   
protected :
   
   WidgetBase* decorated_widget;
   WidgetBase* decorator_widget;
   
   DumbLayout dumb_layout_for_decorated;
   DumbLayout dumb_layout_for_decorator;
   
   Layout* decorated_layout;
   Layout* decorator_layout;
   



public :
   



   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);
   
   virtual void QueueUserMessage(const WidgetMsg& wmsg);// For the top level WidgetHandlers to store messages to the user from 
   
   virtual void SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout = true);

	/// Changes position and outer area!!!
	virtual void SetMarginsExpandFromInner(int left , int right , int top , int bottom);

	/// Make room in outer area for inner area first!!!
	virtual void SetMarginsContractFromOuter(int left , int right , int top , int bottom);
   
   
};


int Decorator::PrivateHandleEvent(EagleEvent e) {
   int retmsg = DIALOG_OKAY;
   
}
int Decorator::PrivateCheckInputs() {
   
}
void Decorator::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   
}
int Decorator::PrivateUpdate(double tsec) {
   
}

void Decorator::SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout) {
   
}

void Decorator::SetMarginsExpandFromInner(int left , int right , int top , int bottom) {
   
}

void Decorator::SetMarginsContractFromOuter(int left , int right , int top , int bottom) {
   
}




//*/























/**

class WidgetBase;




#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/Gui/Layout/DumbLayout.hpp"


#include <string>


class Decorator : public WidgetBase {
   
protected :

   WidgetBase* decorator_widget;
   WidgetBase* decorated_widget;

   Layout* layout_for_decorator;
   Layout* layout_for_decorated;

   DumbLayout dumb_layout;

   
public :
   Decorator();
   Decorator(std::string name);
//

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
   
   virtual void UseLayoutForDecoratedWidget(Layout* layout);/// layout may be NULL to use the default DumbLayout

   virtual void DecorateWidget(WidgetBase* widget);/// widget may be NULL to remove the decorated widget


   WidgetBase* DecoratedWidget() {return dwidget;}
};



//*/

#endif // Decorator_HPP

