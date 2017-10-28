



#ifndef TextButton_HPP
#define TextButton_HPP


#include "Eagle/Gui/Decorators/TextDecorator.hpp"

#include "Eagle/Gui/Button/BasicButton.hpp"


/**
   NOTES : What happens when a layout adds a TextButton to its layout?
               A: The 

*/


class TextButton : public BasicButton {
protected:
   BasicButton* real_button;

   TextDecorator text_decorator;


   public :

   TextButton(std::string name = "Nemo");

   
   /// TextButton functions
   
   void UseButton(BasicButton* button_to_use);
   void UseButtonLayout(Layout* button_layout);
   
   TextDecorator* GetTextDecorator() {return &text_decorator;}/// Use this to set the position of the whole object
   
   /// Functions forwarded from BasicButton base
   
   virtual void SetButtonType(BUTTON_ACTION_TYPE type);
   
   virtual void SetHoverState (bool state);

   virtual void SetSpringDuration(double duration);
   virtual void SetButtonUpState(bool button_up);
   virtual void ToggleButton();
   virtual void SetClickArea(AreaBase* new_click_area , bool delete_when_done);
   
   virtual void SetButtonState(bool hover , bool up);
   virtual void SetInputGroup(InputGroup ig);

   virtual bool JustActivated();
   
   virtual void EnableHoverMessage(bool enabled);

   virtual InputGroup InputKey();
   virtual BUTTON_STATE ButtonState();
   
   virtual bool Up();
   virtual bool Hover();
   
   virtual BUTTON_ACTION_TYPE ActionType();
   
   /// Functions forwarded from TextDecorator
   
   void UseTextLayout(Layout* text_layout);/// layout may be NULL to use the default PinLayout
   
   void CenterText(bool center_the_text);
   void RepositionText();

   void UseTextWidget(BasicText* text_widget);/// text_widget may be NULL to remove the text
   
   BasicText* GetTextWidget();
   
   
   /// Functions forwarded from BasicText
   
   void Realign(HALIGNMENT hal , VALIGNMENT val , int hpad = 0 , int vpad = 0);
   void SetText(std::string text);
   void SetFont(EagleFont* font);
   
   std::string GetText() {return GetTextWidget()->GetText();}
   
   /// Functions forwarded from WidgetBase
   

   virtual int PrivateHandleEvent(EagleEvent e);
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);

   /// Allow these functions to be called normally
   ///   virtual int HandleEvent(EagleEvent e);
   ///   virtual void Display(EagleGraphicsContext* win , int xpos , int ypos);
   ///   virtual int Update(double tsec);

   virtual void QueueUserMessage(const WidgetMsg& wmsg);

   
   /// These functions will only set the area of the contained button after consulting it's layout if necessary
   /// To set the area of the entire object, access the TextDecorator through GetTextDecorator
   
   virtual void SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout = true);

	virtual void SetMarginsExpandFromInner(int left , int right , int top , int bottom);

	virtual void SetMarginsContractFromOuter(int left , int right , int top , int bottom);

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
///   virtual void SetHoverState        (bool state);/// Does not set any redraw flag  - OLD:Sets redraw flag and (false) sets bg redraw flag
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
   

   virtual void SetMinInnerWidth(int w);
   virtual void SetMinInnerHeight(int h);
   virtual void SetMinInnerDimensions(int w , int h);

   virtual void SetDisplayPriority(int priority);

   virtual int AbsMinWidth()  const ;
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

   virtual WidgetBase* GetRealWidget() {return &text_decorator;}
   
   virtual std::string GetWidgetClassName();

   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;
      
   
   
   
};





#endif // TextButton_HPP




