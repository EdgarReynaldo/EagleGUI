
/**
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
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2019+ by Edgar Reynaldo
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

#include "Eagle/Exception.hpp"
#include "Eagle/Image.hpp"
#include "Eagle/GraphicsContext.hpp"
#include "Eagle/InputHandler.hpp"

#include <vector>
#include <list>
#include <map>
#include <set>


/// Used by WidgetHandler::TakeNextMessage to signify an empty message queue
extern const WidgetMsg NoMessages;

extern const unsigned int TOPIC_GUI;

enum GUI_MSGS {
   GUI_RESIZED = 0
};

REGISTER_WIDGET_MESSAGE(TOPIC_GUI , GUI_RESIZED);



class WidgetHandler : public WidgetBase {
	
protected :

   typedef std::vector<WidgetBase*> WIDGETLIST;
   typedef WIDGETLIST::iterator WLIT;

   typedef std::list<WidgetMsg> MESSAGEQUEUE;
   typedef MESSAGEQUEUE::iterator MQIT;



   EagleGraphicsContext* gwindow;

   EagleImage* buffer;
   EagleImage* background;
   EagleImage* user_bg_ptr;
   
   bool stretch_bg;
   bool use_bg_pic;
   EagleColor bg_col;

   Camera          cam;
   bool            shrink_buffer_on_resize;

   DumbLayout      dumb_layout;
   LayoutBase*     root_layout;

   WIDGETLIST      wlist;
   WIDGETLIST      inputlist;/// in order from top to bottom, not necessarily the reverse of drawlist's order
   WIDGETLIST      drawlist;/// in order from back to front

   WIDGETLIST      removelist;

   int             focus_index;
   bool            focus_start;
   WidgetBase*     wfocus;
   
   WidgetBase*     whover;
   
   MESSAGEQUEUE    mque;
   bool            clear_background;
   bool            always_clear;

   InputGroup      focus_cycle_forward;
   InputGroup      focus_cycle_backward;

   std::list<Rectangle> dbg_list;///dirty background rectangle list
       
protected :

   /// Widgetbase functions
   virtual int  PrivateHandleEvent(EagleEvent e);// Only handle events not handled in CheckInputs here
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateUpdate(double tsec);

   virtual void OnAreaChanged();
   virtual void OnColorChanged();

   /// New API, for layout class to use privately
   friend class LayoutBase;
   
   void TrackWidget(WidgetBase* widget);
   void StopTrackingWidget(WidgetBase* widget);



private :
   typedef unsigned int UINT;
   
   bool OwnsWidget(WidgetBase* widget);
   UINT WidgetIndex(WidgetBase* widget);
   WLIT InputListIterator(WidgetBase* widget);
   WLIT DrawListIterator(WidgetBase* widget);

   /// TODO : Possibly rewrite check redraw to use two map<WidgetBase* , list<WidgetBase*> >'s
   /// TODO : for widgets in front of and behind each widget
   std::set<unsigned int> CheckRedraw(UINT widget_index);
   void CheckRedraw();

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
   WidgetHandler(EagleGraphicsContext* window , std::string classname = "WidgetHandler" , std::string objname = "Nemo");
   
   ~WidgetHandler();

   /// Add a message to the widget event queue
   virtual void QueueUserMessage(WidgetMsg wmsg);// For the top level WidgetHandlers to store messages to the user from 
                                                        // the widgets it is handling. Messages are passed up the chain using
                                                        // the parent pointer.
	/// Sets up drawing background and buffer to draw to.
	void SetDrawWindow(EagleGraphicsContext* window);
	EagleGraphicsContext* GetDrawWindow();
	
	/// set up draw window first please, and set up buffers before drawing please!
	bool SetupBuffer(int w , int h , EagleGraphicsContext* window);/// throw (EagleException);
	void SetBufferShrinkOnResize(bool buffer_shrink_on_resize);// defaults to false
	void UseBackgroundImage(EagleImage* bg , bool stretch);// pass null to turn off the background image
	void UseBackgroundColor(EagleColor col);

   void FreeImageBuffers();// Frees the BitmapHandler buffer and background so global WidgetHandler destruction doesn't crash.

   bool      HasMessages();
   WidgetMsg TakeNextMessage();
   void      ClearMessages();// Clears the message queue of any messages remaining

   /// Functions for working with the base layout
   LayoutBase* GetRootLayout() {return root_layout;}
   void SetRootLayout(LayoutBase* l);
   
   /// Functions for adding widgets to the handler - automatically added to the layout
   void AddWidget(WidgetBase* widget);
   WidgetHandler& operator<<(WidgetBase* widget);
   WidgetHandler& operator<<(WidgetBase& widget);

   void RemoveWidget(WidgetBase* widget);
   void ClearLayout();// Removes all widgets from the dialog and destroys the ones marked for deletion. Global WH's should probably call this.



   /// Helper functions
   void PerformFullRedraw(EagleGraphicsContext* win);
   void PerformPartialRedraw(EagleGraphicsContext* win);

   void DrawBuffer(EagleGraphicsContext* win);
   void DrawToWindow(EagleGraphicsContext* win , int xpos , int ypos);

   virtual void SetBackgroundColor(const EagleColor color);

   void SyncLayoutPos();
   void SyncCamera();

   virtual void SetRedrawFlag();/// TODO : SetBgRedrawFlag() when necessary
   void SetFullRedraw();

   virtual void SetFocusState(bool state);

   bool GiveWidgetFocus(WidgetBase* widget , bool notify_parent = true);/// notify_parent for internal use only

   void BringToFront(WidgetBase* w);
   
   virtual void SetRedrawAllFlag();
   
   virtual bool HasGui();
   virtual WidgetHandler* GetGui();

   /// Used by widgets to mark old dirty areas that need to be refreshed
   void MakeAreaDirty(Rectangle dirty_area);


   /// Getters
   WidgetBase*   CurrentFocus();
   WidgetBase*   CurrentHover();

   EagleImage* BackgroundBitmap();/// For drawing the background only, pair with MakeAreaDirty().
   EagleImage* BufferBitmap();/// For saving purposes only. Don't draw to this, or destroy it.


   bool AreaFree(Rectangle r , WidgetBase* widget);
   
   bool InView(WidgetBase* w);
   
   WidgetBase* GetWidgetAt(const int absx , const int absy);
   
   
   
   int GetMouseX();// returns mouse x relative to gui's inner area
   int GetMouseY();// returns mouse y relative to gui's inner area
   
   /// Camera control
   void AccMoveViewTlxTo(int xpos , int ypos , double time = 0.0);// decelerated move to position, instant move if time = 0.0
   void AccMoveViewCenterTo(int xpos , int ypos , double time = 0.0);// decelerated move to position, instant move if time = 0.0
   void AccMoveViewBy(int dx , int dy , double time = 0.0);// decelerated move by position delta
   
   void MoveViewTlxTo(int xpos , int ypos , double time = 0.0);///< set velocity move to position, instant move if time = 0.0
   void MoveViewCenterTo(int xpos , int ypos , double time = 0.0);///< set velocity move to position, instant move if time = 0.0
   void MoveViewBy(int dx , int dy , double time = 0.0);///< Set velocity move by position delta

   void AllowMiddleMouseButtonScroll(bool allow);///< false by default, scroll moves the camera view

   /// Messages from camera
   const WidgetMsg ViewMovedMessage() {return WidgetMsg(&cam , TOPIC_CAMERA , CAMERA_VIEW_MOVED);}
   const WidgetMsg ViewDestReachedMessage() {return WidgetMsg(&cam , TOPIC_CAMERA , CAMERA_VIEW_DEST_REACHED);}


   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;
///   virtual void PrintTo(std::ostream& os , Indenter& indent) const;
///   friend std::ostream& operator<<(std::ostream& os , const WidgetHandler& wh);

};






#endif // EagleGuiWidgetHandler_HPP

