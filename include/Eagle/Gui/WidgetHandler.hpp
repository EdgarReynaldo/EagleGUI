
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
 *    Copyright 2009-2013+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#ifndef EagleGuiWidgetHandler_HPP
#define EagleGuiWidgetHandler_HPP


#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/Gui/Layout/Layout.hpp"
#include "Eagle/Gui/Layout/DumbLayout.hpp"
#include "Eagle/Gui/Camera.hpp"

#include "Eagle/Error.hpp"
#include "Eagle/GraphicsContext.hpp"
#include "Eagle/InputHandler.hpp"


#include <vector>
#include <list>
#include <map>



/// Used by WidgetHandler::TakeNextMessage to signify an empty message queue
extern const WidgetMsg NoMessages;


extern const unsigned int TOPIC_GUI;



enum GUI_MSGS {
   GUI_RESIZED = 0
};




enum FOCUS_DRAW_TYPE {
   FOCUS_INVISIBLE               = 0,
   FOCUS_HIGHLIGHT_OUTLINE       = 1,
   FOCUS_HIGHLIGHT_THICK_OUTLINE = 2,
   FOCUS_HIGHLIGHT_DOTTED        = 3,
   FOCUS_HIGHLIGHT_THICK_DOTTED  = 4
// TODO : FOCUS_HIGHLIGHT_CUSTOM
};



class WidgetHandler : public WidgetBase {
	
protected :

   typedef std::vector<WidgetBase*> WIDGETLIST;
   typedef WIDGETLIST::iterator WLIT;

   typedef std::list<WidgetMsg> MESSAGEQUEUE;
   typedef MESSAGEQUEUE::iterator MQIT;

   typedef std::map<WidgetBase* , bool> MEMTRACKER;
   typedef MEMTRACKER::iterator MTIT;

protected :

   EagleGraphicsContext* gwindow;;

   EagleImage* buffer;
   EagleImage* background;
   EagleImage* user_bg_ptr;
   
   bool stretch_bg;
   bool use_bg_pic;
   EagleColor bg_col;

   Camera          cam;
   bool            shrink_buffer_on_resize;
//   int             realw;
//   int             realh;

   DumbLayout      dumb_layout;
   /// ~dumblayout calls StopTrackingWidget which calls Widgethandler functions during its destructor...
   Layout*         root_layout;

   WIDGETLIST      wlist;
   WIDGETLIST      inputlist;// in order from top to bottom, not necessarily the reverse of drawlist's order
   WIDGETLIST      drawlist;// in order from back to front

   WIDGETLIST      removelist;
//   MEMTRACKER      memtrack;

   int             focus_index;
   bool            focus_start;
   WidgetBase*     wfocus;
   
   WidgetBase*     whover;
   
   MESSAGEQUEUE    mque;
   bool            clear_background;

   InputGroup      focus_cycle_forward;
   InputGroup      focus_cycle_backward;

   FOCUS_DRAW_TYPE focus_type;

   std::list<Rectangle> dbg_list;//dirty background rectangle list
   
   
private :

   bool OwnsWidget(WidgetBase* widget);
   UINT WidgetIndex(WidgetBase* widget);
   WLIT InputListIterator(WidgetBase* widget);

   /// TODO : Possibly rewrite check redraw to use two map<WidgetBase* , list<WidgetBase*> >'s
   /// TODO : for widgets in front of and behind each widget
   void CheckRedraw(UINT widget_index);
   void CheckRedraw();

   void DrawFocus(EagleGraphicsContext* win);

   void SortDrawListByPriority();// sorts from least drawing priority to highest drawing priority
   
   void AddToRemoveList(WidgetBase* w);
   void RemoveOldWidgets();
   
   void RedrawBackgroundBuffer();


public :

   /// These 4 functions are only public because they have to be for things to work right
   /// TODO : Find a way to make these protected instead of public
   bool HasWidget(WidgetBase* widget);// call this if you want, but you should already know the answer, includes sub GUIs.
   void        FocusStart();// starts the focus search over at the beginning(also the end)
   WidgetBase* NextFocus();// don't call directly, alters internal state
   WidgetBase* PreviousFocus();// don't call directly, alters internal state
   
   /// The focus cycling functions will only function if this is the top level widget handler
   void CycleFocusForward();
   void CycleFocusBackward();


   /// Constructors
   WidgetHandler();
   
   ~WidgetHandler();

	/// Sets up drawing background and buffer to draw to.
	void SetDrawWindow(EagleGraphicsContext* window);
	
	bool SetupBufferDimensions(int w , int h) throw (EagleError);// set up draw window first please, and set up buffers before drawing please!
	void SetBufferShrinkOnResize(bool buffer_shrink_on_resize);// defaults to false
	void UseBackgroundImage(EagleImage* bg , bool stretch);// pass null to turn off the background image
	void UseBackgroundColor(EagleColor col);

   void FreeImageBuffers();// Frees the BitmapHandler buffer and background so global WidgetHandler destruction doesn't crash.

   bool      HasMessages();
   WidgetMsg TakeNextMessage();
   void      ClearMessages();// Clears the message queue of any messages remaining



   /// New API, for layout class to use privately
protected :
   friend class Layout;
   
   void TrackWidget(WidgetBase* widget);
   void StopTrackingWidget(WidgetBase* widget);

public :
   
   /// Functions for working with the base layout
   Layout* GetRootLayout() {return root_layout;}
   void SetRootLayout(Layout* l);
   
   void AddWidget(WidgetBase* widget , bool delete_when_removed);
   WidgetHandler& operator<<(WidgetBase* widget);
   WidgetHandler& operator<<(WidgetBase& widget);
   void RemoveWidget(WidgetBase* widget);
   void ClearLayout();// Removes all widgets from the dialog and destroys the ones marked for deletion. Global WH's should probably call this.

   /// Widgetbase functions
protected :
   virtual int  PrivateHandleEvent(EagleEvent e);// Only handle events not handled in CheckInputs here
   virtual int  PrivateCheckInputs();// Called automatically by HandleEvent and? by system during gui update.
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int  PrivateUpdate(double tsec);


public :
   virtual void QueueUserMessage(const WidgetMsg& wmsg);// For the top level WidgetHandlers to store messages to the user from 
                                                        // the widgets it is handling. Messages are passed up the chain using
                                                        // the parent pointer.


   virtual void SetColorset(const WidgetColorset& colors , bool set_descendants_colors = false);
   
   virtual void SetBackgroundColor(const EagleColor color);

   void SyncLayoutPos();
   void SyncCamera();
   virtual void SetDrawPos(int xpos , int ypos);
   virtual void SetDrawDimensions(int width , int height);
   virtual void SetArea(int xpos , int ypos , int width , int height);

	/// Changes position and outer area!!!
	virtual void SetMarginsExpandFromInner(int left , int right , int top , int bottom);


   virtual void SetRedrawFlag();/// TODO : SetBgRedrawFlag() when necessary
   void SetFullRedraw() {SetRedrawFlag();clear_background = true;}


   virtual void SetFocusState(bool state);

   virtual bool GiveWidgetFocus(WidgetBase* widget , bool notify_parent = true);// notify_parent for internal use only

   virtual void SetRedrawAllFlag();

   virtual bool HasGui();
   virtual WidgetHandler* GetGui();


   void SetFocusDrawType(FOCUS_DRAW_TYPE type , bool for_all_guis = false);

   void MakeAreaDirty(Rectangle dirty_area);


   /// Camera control
   void AccMoveViewTlxTo(int xpos , int ypos , double time = 0.0);// decelerated move to position, instant move if time = 0.0
   void AccMoveViewCenterTo(int xpos , int ypos , double time = 0.0);// decelerated move to position, instant move if time = 0.0
   void AccMoveViewBy(int dx , int dy , double time = 0.0);// decelerated move by position delta
   
   void MoveViewTlxTo(int xpos , int ypos , double time = 0.0);// set velocity move to position, instant move if time = 0.0
   void MoveViewCenterTo(int xpos , int ypos , double time = 0.0);// set velocity move to position, instant move if time = 0.0
   void MoveViewBy(int dx , int dy , double time = 0.0);// set velocity move by position delta

   void AllowMiddleMouseButtonDrag(bool allow);// false by default, drag moves the view

   /// Getters
   WidgetBase*   CurrentFocus();
   WidgetBase*   CurrentHover();

   EagleImage* BackgroundBitmap() {return background;}// For drawing the background only, pair with MakeAreaDirty().
   EagleImage* BufferBitmap() {return buffer;}// For saving purposes only. Don't draw to this, or destroy it.


   bool AreaFree(Rectangle r , WidgetBase* widget);
   bool InView(WidgetBase* w);
   WidgetBase* GetWidgetAt(int x , int y);
   
   int GetMouseX();// returns mouse x relative to gui's inner area
   int GetMouseY();// returns mouse y relative to gui's inner area
   
   
   
   const WidgetMsg ViewMovedMessage() {return WidgetMsg(&cam , TOPIC_CAMERA , CAMERA_VIEW_MOVED);}
   const WidgetMsg ViewDestReachedMessage() {return WidgetMsg(&cam , TOPIC_CAMERA , CAMERA_VIEW_DEST_REACHED);}


   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;
///   virtual void PrintTo(std::ostream& os , Indenter& indent) const;
///   friend std::ostream& operator<<(std::ostream& os , const WidgetHandler& wh);

};






#endif // EagleGuiWidgetHandler_HPP

