
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
 *    Copyright 2009-2015+ by Edgar Reynaldo
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

#include "Eagle/Gui/WidgetArea.hpp"
#include "Eagle/Gui/WidgetColorset.hpp"
#include "Eagle/Gui/WidgetDrawFuncs.hpp"
#include "Eagle/Gui/WidgetMessage.hpp"


extern const unsigned int TOPIC_DIALOG;

/// DIALOG_MSGS = Bitfield values for messages returned to a dialog from the Update() and CheckInput() functions.

enum DIALOG_MSGS {
   DIALOG_OKAY       = 0x00,/// Also used to signify that there are no messages for the WidgetHandler::TakeNextMessage function.
   DIALOG_CLOSE      = 0x01,
   DIALOG_REDRAW_ALL = 0x02,/// For when the background needs to be cleared / redrawn
   DIALOG_INPUT_USED = 0x04,/// Play nice, a WidgetHandler will stop checking other widgets input after this is received.
   DIALOG_TAKE_FOCUS = 0x08,
   DIALOG_DISABLED   = 0x10,/// Returned when you try to update or check the inputs of a disabled dialog.
   DIALOG_REMOVE_ME  = 0x20
};

enum WIDGET_FLAGS {
   FLAGS_NONE      = 0x000,
   ENABLED         = 0x001,   // Currently active or not
   VISIBLE         = 0x002,   // Whether anything is drawn
   HOVER           = 0x004,   // If it has mouse hover - don't steal the focus for hover!
   HASFOCUS        = 0x008,   // If it has the focus
   MOVEABLE        = 0x010,   // Whether it can be moved
   RESIZEABLE      = 0x020,   // Whether it can be resized
   NEEDS_REDRAW    = 0x040,   // Whether it should be redrawn before display?
   NEEDS_BG_REDRAW = 0x080,   // Whether it needs its background redrawn before being redrawn
   ALLOW_CLOSE     = 0x100,   // If it is allowed to close the dialog based on input
   ALLOW_OVERLAP   = 0x200   // TODO : DEPRECATED - REMOVE - If it is allowed to overlap other widgets
};

static const int NUM_WIDGET_FLAGS = 11;

typedef unsigned int UINT;

std::string PrintFlags(UINT flags);

extern const UINT BASIC_FLAGS;
extern const UINT DEFAULT_FLAGS;
extern const UINT DEFAULT_OVERLAP_FLAGS;


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



class WidgetBase : public EagleObject , public EagleEventSource {

protected :
   
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
   
   WIDGET_DRAW_FUNC bg_draw_func;
   
   

   virtual void PrivateRaiseEvent(WidgetMsg msg) {(void)msg;}

   void RaiseEvent(WidgetMsg msg);
   
   EagleEvent MakeEagleEvent(WidgetMsg msg);

public :
   
   WidgetBase();
   WidgetBase(std::string name);
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
   virtual int PrivateCheckInputs();// Called automatically by HandleEvent
                                    // ?and by system during gui update.
                                    // Redefine to check state machine, will be called once per system event if not
                                    // handled directly by PrivateHandleEvent already as signaled by using
                                    // the DIALOG_INPUT_USED flag
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);

   virtual int PrivateUpdate(double tsec);

public :

   /// TODO : Do we want to call CheckInputs every time a timer event is fired? Not sure.
   /// TODO : Right now it is called every time we get an event, which doesn't seem quite right.
   int HandleEvent(EagleEvent e);

   
   void Display(EagleGraphicsContext* win , int xpos , int ypos);// Drawing target will already be set for you
                                                                 // But you can push and pop other drawing targets as well

   int Update(double tsec);



   virtual void QueueUserMessage(const WidgetMsg& wmsg);// For the top level WidgetHandlers to store messages to the user from 
                                                        // the widgets it is handling. Messages are passed up the chain using
                                                        // the parent pointer.
                                                        /// Warning - if the widget has no parent, these messages just get lost,
                                                        /// and only in the case that the top level parent widget is a 
                                                        /// WidgetHandler do they get spooled for the user.
   
   virtual void QueueUserMessage(WidgetBase* widget_address , UINT widget_topic , int messages);// Shortcut to virtual Q.U.M.

	/// Setters
	virtual void SetParent(WidgetBase* parent);
	void         SetOwnerLayout(Layout* l);
   
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

//   virtual void SetFont(FONT* textfont) {}

   void         SetBgDrawFunc(WIDGET_DRAW_FUNC draw_func);
   void         SetBgImage(EagleImage* img , MARGIN_HCELL hcell, MARGIN_VCELL vcell);
   void         SetBgImages(EagleImage* imgs[3][3]);
   void         SetImagesHaveAlpha(bool have_alpha);
   
   /// These 4 functions all pass the requested parameters to their layout owner for adjustment if necessary
   void SetWidgetPos(int xpos , int ypos , bool notify_layout = true);
   void SetWidgetCorners(int x1 , int y1 , int x2 , int y2 , bool notify_layout = true);
   void SetWidgetDimensions(int width , int height , bool notify_layout = true);
   void SetWidgetArea(Rectangle outer , bool notify_layout = true);
   
   virtual void SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout = true);

	/// Changes position and outer area!!!
	virtual void SetMarginsExpandFromInner(int left , int right , int top , int bottom);

	/// Make room in outer area for inner area first!!!
	virtual void SetMarginsContractFromOuter(int left , int right , int top , int bottom);

   void SetMinInnerWidth(int w);
   void SetMinInnerHeight(int h);
   void SetMinInnerDimensions(int w , int h);

   void SetDisplayPriority(int priority);

   virtual int AbsMinWidth() {return 1;}
   virtual int AbsMinHeight() {return 1;}
   
   virtual bool HasGui() {return false;}/// TODO : What is this function for again??? Oh right, it's for if the widget is a GUI
   virtual WidgetHandler* GetGui() {return 0;}/// TODO : What is this function for again??? Oh Right, it's for if the widget is a GUI
   
   WidgetHandler* NearestParentGui();
   WidgetBase*    Root();
   WidgetHandler* RootGui();

   /// Getters , some references for direct modification
   int                   AbsParentX() const ;
   int                   AbsParentY() const ;
   Pos2d                 GetParentOffset() const ;
   
   WidgetBase*           Parent()          const {return wparent;}
   WidgetColorset&       WCols();// SetRedrawFlag if you change the colors!
   virtual WidgetArea    Area()      const {return area;}
   virtual Rectangle     OuterArea()       const {return area.OuterArea();}
   virtual Rectangle     InnerArea()       const {return area.InnerArea();}
   int                   MinWidth()        const {return minw + area.MLeft() + area.MRight();}
   int                   MinHeight()       const {return minh + area.MTop() + area.MBot();}
   int                   MinInnerWidth()   const {return minw;}
   int                   MinInnerHeight()  const {return minh;}
   UINT                  Flags()           const {return flags;}
   int                   DisplayPriority() const {return display_priority;}
   
   virtual std::string GetWidgetClassName() {return "WidgetBase object";}

   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const;
};




bool DrawPriorityIsLess(WidgetBase* lhs , WidgetBase* rhs);




#endif // EagleGuiWidgetBase_HPP

