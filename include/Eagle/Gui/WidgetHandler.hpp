
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file WidgetHandler.hpp
 * @brief This file holds the interface for the main widget handler class
 * 
 */

#ifndef EagleGuiWidgetHandler_HPP
#define EagleGuiWidgetHandler_HPP



#include "Eagle/Exception.hpp"
#include "Eagle/Image.hpp"
#include "Eagle/GraphicsContext.hpp"
#include "Eagle/InputHandler.hpp"

#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/Gui/Layout/Layout.hpp"
#include "Eagle/Gui/Layout/DumbLayout.hpp"
#include "Eagle/Gui/Camera.hpp"



#include <vector>
#include <list>
#include <map>
#include <set>



extern const WidgetMsg NoMessages;///< Used by WidgetHandler::TakeNextMessage to signify an empty message queue



extern const unsigned int TOPIC_GUI;///< Main gui topic value




/**! @enum GUI_MSGS
 *   @brief A simple enum for gui message values
 *  
 *   Currently unused
 */

enum GUI_MSGS {
   GUI_RESIZED = 0///< Only gui message right now
};



REGISTER_WIDGET_MESSAGE(TOPIC_GUI , GUI_RESIZED);///< You can check for this @ref WidgetMsg to respond to it



/*! @class WidgetHandler
 *  @brief The WidgetHandler class supports all aspects of handling widgets and serves as the main GUI object 
 *  See also @ref WidgetBase
 *
 * WidgetHandler handles routine widget tasks like collected input and drawing and events. Handles z order, focus, 
 * tabbed focus changes, background images or colors, transparency, dirty rectangle buffering mechanism,
 * layout sizing (always the size of the buffer), full camera and view control, and message queues for child widgets
 * to store events sent to the user.
 
 * Before calling WidgetBase::Display, you must set up the widget handler buffers by calling SetupBuffer . If the draw window
 * has not been set it must be set at that time. TODO : In the future, widget handlers will be able to function without a buffer.
 */

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
   virtual int  PrivateHandleEvent(EagleEvent e);///< Overridden event handler
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);///< Overridden display routine
   virtual int PrivateUpdate(double tsec);///< Overridden update routine

   virtual void OnAreaChanged();///< Area change callback
   virtual void OnColorChanged();///< Color change callback

   /// New API, for layout class to use privately
   friend class LayoutBase;
   
   void TrackWidget(WidgetBase* widget);///< For layouts
   void StopTrackingWidget(WidgetBase* widget);///< For layouts

private :
   typedef unsigned int UINT;
   
   bool OwnsWidget(WidgetBase* widget);
   UINT WidgetIndex(WidgetBase* widget);
   WLIT InputListIterator(WidgetBase* widget);
   WLIT DrawListIterator(WidgetBase* widget);

   std::set<unsigned int> CheckRedraw(UINT widget_index);
   void CheckRedraw();

   void SortDrawListByPriority();///< sorts from least drawing priority to highest drawing priority (back to front)
   
   void AddToRemoveList(WidgetBase* w);
   void RemoveOldWidgets();
   
   void RedrawBackgroundBuffer();



public :

   /// These 4 functions are only public because they have to be for things to work right
   /// TODO : Find a way to make these protected instead of public
   bool        HasWidget(WidgetBase* widget);///< call this if you want, but you should already know the answer, includes sub GUIs.
   void        FocusStart();   ///< starts the focus search over at the beginning(also the end)
   WidgetBase* NextFocus();    ///< don't call directly, alters internal state
   WidgetBase* PreviousFocus();///< don't call directly, alters internal state
   
   /// The focus cycling functions will only function if this is the top level widget handler
   void CycleFocusForward();///< Like pressing the tab key
   void CycleFocusBackward();///< Like pressing shift tab


   /// Constructors
   WidgetHandler(EagleGraphicsContext* window , std::string classname = "WidgetHandler" , std::string objname = "Nemo");
   
   ~WidgetHandler();

   ///< For the top level WidgetHandlers to store messages to the user from 
   ///< the widgets it is handling. Messages are passed up the chain using
   ///< the parent pointer.
   virtual void QueueUserMessage(const WidgetMsg& wmsg) override;///< Add a message to the widget event queue


	/// Sets up drawing background and buffer to draw to.
	void SetDrawWindow(EagleGraphicsContext* window);///< Sets the window to draw into
	EagleGraphicsContext* GetDrawWindow();///< Returns the current drawing window
	
	/// set up draw window first please, and set up buffers before drawing please!
	bool SetupBuffer(int w , int h , EagleGraphicsContext* window);///< Set the buffer size and window. Pass null
                                                                  ///< to reuse a previously set window. May throw an EagleException
                                                                  ///< if the graphics window is invalid.
	void SetBufferShrinkOnResize(bool buffer_shrink_on_resize);///< Defaults to false, pass true if you want the buffer to shrink
                                                              ///< to fit the widget area
	void UseBackgroundImage(EagleImage* bg , bool stretch);///< Pass null to turn off the background image
	void UseBackgroundColor(EagleColor col);///< Use the specified color for a background color. Transparency is supported.

   void FreeImageBuffers();///< Frees the BitmapHandler buffer and background so global WidgetHandler destruction doesn't crash.
                           ///< Must be called before Eagle::Shutdown - TODO : FIXME
   bool      HasMessages();    ///< True if there are messages waiting in the queue
   WidgetMsg TakeNextMessage();///< Take the next message. Returns NoMessages if empty.
   void      ClearMessages();  ///< Clears the message queue of any messages remaining

   /// Functions for working with the base layout

   LayoutBase* GetRootLayout() {return root_layout;}///< Returns the root layout for this widget handler
   void SetRootLayout(LayoutBase* l);///< Sets the root layout for this widget handler
   
   /// Functions for adding widgets to the handler - automatically added to the layout
   
   void AddWidget(WidgetBase* widget);///< Adds a widget at the end of the widget list
   WidgetHandler& operator<<(WidgetBase* widget);///< Stream operator adds the widget pointed to to the gui
   WidgetHandler& operator<<(WidgetBase& widget);///< Stream operator adds the widget reference to the gui

   /// Functions to remove widgets and clear the layout
   
   void RemoveWidget(WidgetBase* widget);///< Remove a widget from this gui
   void ClearLayout();///< Removes all widgets from the dialog. Global WH's should probably call this.



   /// Helper functions
   void PerformFullRedraw(EagleGraphicsContext* win);///< Perform a full redraw, ignoring and clearing redraw flags
   void PerformPartialRedraw(EagleGraphicsContext* win);///< Perform a partial redraw of the widgets and bg areas that are dirty

   void DrawBuffer(EagleGraphicsContext* win);///< Draws the buffer
   void DrawToWindow(EagleGraphicsContext* win , int xpos , int ypos);///< Draws the buffer to the window drawing target

   virtual void SetBackgroundColor(const EagleColor color);///< Set the background color

   void SyncLayoutPos();///< TODO : Make private?
   void SyncCamera();///< TODO : Make private?

   virtual void SetRedrawFlag();///< Set the redraw flag. TODO : SetBgRedrawFlag() when necessary
   void SetFullRedraw();///< Call to fully redraw everything

   virtual void SetFocusState(bool state);///< Sets the focus state

   bool GiveWidgetFocus(WidgetBase* widget , bool notify_parent = true);///< Give widget the focus. notify_parent is for internal use only

   void BringToFront(WidgetBase* w);///< Bring the specified widget to the front
   
   virtual void SetRedrawAllFlag();///< Call to completely redraw
   
   virtual bool HasGui();///< Returns true
   virtual WidgetHandler* GetGui();///< Returns this


   void MakeAreaDirty(Rectangle dirty_area);///< Used by widgets to mark old dirty areas that need to be refreshed


   /// Getters
   WidgetBase*   CurrentFocus();///< Returns the widget if any that has the focus
   WidgetBase*   CurrentHover();///< Returns the widget if any that has the hover

   EagleImage* BackgroundBitmap();///< For drawing the background only, pair with MakeAreaDirty().
   EagleImage* BufferBitmap();    ///< For saving purposes only. Don't draw to this, or destroy it.


   bool AreaFree(Rectangle r , WidgetBase* widget);///< True if r is a free area. The specified widget is ignored.
   
   bool InView(WidgetBase* w);///< True if w is in view
   
   WidgetBase* GetWidgetAt(const int absx , const int absy);///< Returns the widget at the absolute coordinates
   
   int GetMouseX();///< Returns mouse x relative to gui's inner area
   int GetMouseY();///< Returns mouse y relative to gui's inner area
   
   virtual Pos2I ViewPos() const;///< Returns the camera offset


   /// Camera control
   void AccMoveViewTlxTo   (int xpos , int ypos , double time = 0.0);///< Decelerated move to position, instant move if time = 0.0
   void AccMoveViewCenterTo(int xpos , int ypos , double time = 0.0);///< Decelerated move to position, instant move if time = 0.0
   void AccMoveViewBy      (int dx   , int dy   , double time = 0.0);///< Decelerated move by position delta, instant move if time = 0.0
   
   void MoveViewTlxTo   (int xpos , int ypos , double time = 0.0);///< Linear timed move to position, instant move if time = 0.0
   void MoveViewCenterTo(int xpos , int ypos , double time = 0.0);///< Linear timed move to position, instant move if time = 0.0
   void MoveViewBy      (int dx   , int dy   , double time = 0.0);///< Linear timed move by position delta, instant move if time = 0.0

   void AllowMiddleMouseButtonScroll(bool allow);///< False by default, MMB drag scrolls the camera view

   /// Messages from camera
   inline const WidgetMsg ViewMovedMessage() {///< Check for this message to see if the view moved
      return WidgetMsg(&cam , TOPIC_CAMERA , CAMERA_VIEW_MOVED);
   }
   inline const WidgetMsg ViewDestReachedMessage() {///< Check for this message to see if the view arrived
      return WidgetMsg(&cam , TOPIC_CAMERA , CAMERA_VIEW_DEST_REACHED);
   }

   virtual std::ostream& DescribeTo(std::ostream& os , Indenter indent = Indenter()) const ;///< Output to stream
};



#endif // EagleGuiWidgetHandler_HPP



