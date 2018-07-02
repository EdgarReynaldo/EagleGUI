
/*
 *
 *     _______       ___       ____      __       _______
 *    /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *    \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *     \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *      \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *       \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *        \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    EAGLE
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2016+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#ifndef EagleGuiWidgetBase_HPP
#define EagleGuiWidgetBase_HPP


#include "Eagle/Area.hpp"
#include "Eagle/Events.hpp"
#include "Eagle/Object.hpp"
#include "Eagle/GraphicsContext.hpp"
#include "Eagle/Image.hpp"
#include "Eagle/Gui/Scripting/GuiScript.hpp"
#include "Eagle/Gui/WidgetArea.hpp"
#include "Eagle/Gui/WidgetColorset.hpp"
#include "Eagle/Gui/WidgetDrawFuncs.hpp"
#include "Eagle/Gui/WidgetMessage.hpp"
#include "Eagle/Gui/WidgetFlags.hpp"



/// Widgets with higher display priority will display on top of widgets with lower display priority
/// When widgets have equal priority, the widget that last had the focus will display on top
enum WIDGET_DISPLAY_PRIORITY {
   LOWEST_DISPLAY_PRIORITY = 0,
   LOW_DISPLAY_PRIORITY = 64,
   MID_DISPLAY_PRIORITY = 128,
   HIGH_DISPLAY_PRIORITY = 192,
   HIGHEST_DISPLAY_PRIORITY = 255
};



class Layout;
class WidgetHandler;
class WidgetDecorator;



/// TODO : If the WidgetBase API changes, make sure to update the WidgetDecorator API as well



class WidgetBase : public EagleObject , public EagleEventSource {

protected :
   
   static bool clip_widgets;
   static bool display_widget_area;
   
   WidgetBase* wparent;
   Layout* layout;
   
   WidgetColorset wcols;
   WidgetColorset privwcols;
   bool use_private_colorset;

   WidgetArea area;
   int minw;
   int minh;
   
   UINT flags;
   
   int display_priority;
   
   BackgroundPainter* background_painter;
   BG_DRAW_TYPE background_draw_type;
   
   FocusPainter* focus_painter;
   FOCUS_DRAW_TYPE focus_draw_type;

   WidgetDecorator* decorator_parent;
   


   virtual void PrivateRaiseEvent(WidgetMsg msg) {(void)msg;}

   void RaiseEvent(WidgetMsg msg);
   
public :
   
///   WidgetBase();
   WidgetBase(std::string objclass , std::string objname);
   virtual ~WidgetBase();

/*   
   virtual void EagleEventSource::ReadEvents()=0;
   virtual void EagleEventListener::ProcessEvent(EagleEvent e)=0;
*/
	/// EventSource
   void ReadEvents() {}
   
   /// EventListener
//   void RespondToEvent(EagleEvent e);


/** Widgetbase virtual functions. Overload these to give your widgets the proper behaviour
protected :
   virtual int PrivateHandleEvent(EagleEvent e);
   virtual int PrivateCheckInputs();
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);
//*/

protected :
   /// Handle events here
   virtual int PrivateHandleEvent(EagleEvent e);// Only handle events not handled in CheckInputs here

   /// Only handle events not handled in PrivateHandleEvent here in CheckInputs, like state checks.
   virtual int PrivateCheckInputs();/// Called automatically by HandleEvent
                                    /// ?and by system during gui update.
                                    /// Redefine to check state machine, will be called once per system event if not
                                    /// handled directly by PrivateHandleEvent already as signaled by using
                                    /// the DIALOG_INPUT_USED flag
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);

   virtual int PrivateUpdate(double tsec);

public :

   /// Debugging
   static void ClipWidgets(bool clip);/// If you change this setting, tell your GUI to perform a full redraw (WidgetHandler::SetFullRedraw)
   static void DisplayWidgetArea(bool display);/// If you change this setting, tell your GUI to perform a full redraw
   
   /// HandleEvent, Display,, and Update are only virtual for certain classes like Decorator to function properly
   /// Don't overload them normally, overload the Private versions instead

   /// TODO : Do we want to call CheckInputs every time a timer event is fired? Not sure.
   /// TODO : Right now it is called every time we get an event, which doesn't seem quite right.
   /// TODO : FIX : CheckInputs are only called on non-timer events
   virtual int HandleEvent(EagleEvent e);

   
   virtual void Display(EagleGraphicsContext* win , int xpos , int ypos);// Drawing target will already be set for you
                                                                 // But you can push and pop other drawing targets as well

   virtual int Update(double tsec);



   virtual void QueueUserMessage(const WidgetMsg& wmsg);// For the top level WidgetHandlers to store messages to the user from 
                                                        // the widgets it is handling. Messages are passed up the chain using
                                                        // the parent pointer.
                                                        /// Warning - if the widget has no parent, these messages just get lost,
                                                        /// and only in the case that the top level parent widget is a 
                                                        /// WidgetHandler do they get spooled for the user.
                                                        /// Don't call QueueUserMessage directly, rather call RaiseEvent,
                                                        /// which calls QueueUserMessage indirectly, that way listeners 
                                                        /// and callbacks will work
   
   void QueueUserMessage(WidgetBase* widget_address , UINT widget_topic , int messages);// Shortcut to virtual Q.U.M.

	/// Setters
	virtual void SetParent(WidgetBase* parent);
	virtual void SetDecoratorParent(WidgetDecorator* decorator);
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
   virtual void SetHoverState        (bool state);// /* Does not set any redraw flag */ - OLD:Sets redraw flag and (false) sets bg redraw flag
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
   
   /// These 4 functions all pass the requested parameters to their layout owner for adjustment if necessary
   /// All 4 call SetWidgetArea(x,y,w,h,notify) indirectly
   void SetWidgetPos(int xpos , int ypos , bool notify_layout = true);
   void SetWidgetCorners(int x1 , int y1 , int x2 , int y2 , bool notify_layout = true);
   void SetWidgetDimensions(int width , int height , bool notify_layout = true);
   void SetWidgetArea(Rectangle outer , bool notify_layout = true);
   
   virtual void SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout = true);/// Pass INT_MAX if you don't care

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
   virtual Pos2I                 GetParentOffset() const ;
   
   virtual WidgetBase*           Parent()          const {return wparent;}
   virtual WidgetColorset&       WCols();/// SetRedrawFlag if you change the colors!
   virtual const WidgetColorset& WCols() const ;
   virtual WidgetArea            Area()            const {return area;}
   virtual Rectangle             OuterArea()       const {return area.OuterArea();}
   virtual Rectangle             InnerArea()       const {return area.InnerArea();}
   virtual int                   MinWidth()        const {return minw + area.MLeft() + area.MRight();}
   virtual int                   MinHeight()       const {return minh + area.MTop() + area.MBot();}
   virtual int                   MinInnerWidth()   const {return minw;}
   virtual int                   MinInnerHeight()  const {return minh;}
   virtual UINT                  Flags()           const {return flags;}
   virtual int                   DisplayPriority() const {return display_priority;}
   
   virtual BackgroundPainter*    GetBackgroundPainter()  const {return background_painter;}
   virtual BG_DRAW_TYPE          GetBackgroundDrawType() const {return background_draw_type;}
   virtual FocusPainter*         GetFocusPainter()       const {return focus_painter;}
   virtual FOCUS_DRAW_TYPE       GetFocusDrawType()      const {return focus_draw_type;}
   
   virtual WidgetDecorator*      GetDecoratorParent() const {return decorator_parent;}
   WidgetBase*                   GetDecoratorRoot();
///   const WidgetBase*             GetDecoratorRoot() const ;
   
   
   virtual void SetAttributes(AttributeValueMap avmap);
   
   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const;
};



bool DrawPriorityIsLess(WidgetBase* lhs , WidgetBase* rhs);

void PrintWidget(WidgetBase* widget);


#endif // EagleGuiWidgetBase_HPP

