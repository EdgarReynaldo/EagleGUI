
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
 *    Copyright 2009-2018+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#include "Eagle/ObjectRegistry.hpp"
#include "Eagle/Logging.hpp"
#include "Eagle/InputHandler.hpp"
#include "Eagle/StringWork.hpp"

#include "Eagle/Gui/WidgetHandler.hpp"
///#include "Eagle/Gui/Decorators/Decorator.hpp"


#include <algorithm>

using std::list;
using std::vector;
using std::map;
using std::string;
using std::endl;
using std::stable_sort;

typedef vector<WidgetBase*> WIDGETLIST;
typedef WIDGETLIST::iterator WLIT;

typedef list<WidgetMsg> MESSAGEQUEUE;
typedef MESSAGEQUEUE::iterator MQIT;

typedef map<WidgetBase* , bool> MEMTRACKER;
typedef MEMTRACKER::iterator MTIT;



/** Used by WidgetHandler::TakeNextMessage to signify an empty message queue */
const WidgetMsg NoMessages((WidgetBase*)0 , TOPIC_DIALOG , DIALOG_OKAY);



const unsigned int TOPIC_GUI = NextFreeTopicId();




/// TODO : Alter mouse event for relative mouse coordinates
int  WidgetHandler::PrivateHandleEvent(EagleEvent e) {
   if (!(wlist.size())) {
      return DIALOG_OKAY;
   }
   /**
      Since WidgetHandlers have their own buffer, they need to offset the position they pass to their
      widgets since each widget stores its position on their own drawing target (the WidgetHandler buffer).
      This way, every widget that is called will have the relative mouse position on their target bitmap, and
      they won't have to adjust it.
   */

   /// Need to get mouse position relative to buffer
   const int mx = GetMouseX();
   const int my = GetMouseY();
   
   if (IsMouseEvent(e)) {
      e.mouse.x = mx;
      e.mouse.y = my;
   }
   
//   WidgetMsg retval;
   bool gui_takes_focus = false;
   int msg;
   int retmsg = DIALOG_OKAY;
   bool focus_taken = false;

   /// GUIs can take the focus too
   if (wparent && OuterArea().Contains(mouse_x , mouse_y) && input_mouse_press(LMB)) {
      /// TODO : this is kind of a relic of a gui being a window... but they're not anymore, sort of
      /// TODO : Leave this to the EagleGuiWindow subclass
//      gui_takes_focus = true;
   }

   /** WidgetHandlers will check whether the mouse is hovering over one of the widgets, and set the hover flag for
         that widget, as well as removing it from any widget that had it last.
   */
   // Only root gui checks for hover
   if (!wparent) {
      WidgetBase* new_whover = 0;
      for (int i = (int)drawlist.size() - 1 ; i >= 0 ; --i) {
         WidgetBase* w = drawlist[i];
         Rectangle abswidgetpos = w->AbsoluteArea().OuterArea();
         bool hover = abswidgetpos.Contains(mouse_x , mouse_y);
///         bool hover = w->OuterArea().Contains(mx,my);
         if (hover) {
            new_whover = w;
            break;
         }
      }
      if (new_whover != whover) {
         if (whover) {
            whover->SetHoverState(false);
         }
         /**
         if (new_whover) {
            new_whover->SetHoverState(true);
         }
         whover = new_whover;
         */
      }
      /// Always set hover??? Need this to make some widgets work (like TextDecorator and Decorator)
      if (new_whover) {
         new_whover->SetHoverState(true);
      }
      whover = new_whover;
   }
   /**
      Check the widget with focus first - inputlist is sorted so that the widget with focus is first in the list
   */

   for (UINT index = 0 ; index < inputlist.size() ; ++index) {
      WidgetBase* widget = inputlist[index];

      if (!(widget->Flags().FlagOn(ENABLED))) {continue;}

      msg = widget->HandleEvent(e);

      /// Warning - All messages not related to a dialog will be ignored.
      /// Use QueueUserMessage or RaiseEvent for user notifications instead.
      
      if (msg == DIALOG_OKAY)      {continue;}
      if (msg & DIALOG_CLOSE)      {return msg;}// Pass the CLOSE message up the chain and back to the user
      //if (msg & DIALOG_DISABLED) {}// Ignore disabled dialogs
      if (msg & DIALOG_REDRAW_ALL) {
         clear_background = true;
         SetRedrawFlag();
      }
      if (msg & DIALOG_REMOVE_ME) {
         AddToRemoveList(widget);
      }
      if (msg & DIALOG_TAKE_FOCUS) {
         /// The GiveWidgetFocus function will rearrange the widgets in the list, so stop processing then
         GiveWidgetFocus(widget);
         focus_taken = true;
      }
      if (msg & DIALOG_INPUT_USED) {
         retmsg |= DIALOG_INPUT_USED;
         // Don't check any of the other widget's inputs during this update
         return retmsg;
      }
      if (focus_taken) {
         return retmsg;
      }
   }
   
   msg = cam.HandleEvent(e);
   if (msg & DIALOG_INPUT_USED) {
      retmsg |= DIALOG_INPUT_USED;
      return retmsg;
   }

   /// Now that all the widgets have been given priority, check for gui focus changes by key input
   if (focus_cycle_forward) {
      CycleFocusForward();
   }
   if (focus_cycle_backward) {
      CycleFocusBackward();
   }
   if (gui_takes_focus) {
      return DIALOG_TAKE_FOCUS;
   }
   return retmsg;
}



void WidgetHandler::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());

   DrawBuffer(win);
   
   DrawToWindow(win , xpos , ypos);
}





int WidgetHandler::PrivateUpdate(double tsec) {

   int msg = 0;
   int retmsg = DIALOG_OKAY;
   
   for (unsigned int i = 0 ; i < inputlist.size() ; ++i) {
      WidgetBase* widget = inputlist[i];
      msg = widget->Update(tsec);
      
      if (msg == DIALOG_OKAY)      {continue;}

      if (msg & DIALOG_CLOSE)      {return retmsg | DIALOG_CLOSE;}/// Pass the CLOSE message up the chain and back to the user

      ///if (msg & DIALOG_DISABLED) {}/// Ignore disabled dialogs

      if (msg & DIALOG_REDRAW_ALL) {
         clear_background = true;
         SetRedrawFlag();
      }

      if (msg & DIALOG_TAKE_FOCUS) {
         GiveWidgetFocus(widget);/// This rearranges the widget lists!!! Stop processing them!
         return retmsg;
      }

      if (msg & DIALOG_INPUT_USED) {
         throw EagleException("Your widget's Update function should not be returning DIALOG_INPUT_USED.\n");
      }
      
      if (msg & DIALOG_REMOVE_ME) {
         AddToRemoveList(widget);
      }
   }
   cam.Update(tsec);
   
   return msg;
}




void WidgetHandler::OnAreaChanged() {
   /// INFO : Use OuterArea for camera. Camera is always at 0,0,OuterWidth,OuterHeight
   Rectangle r = OuterArea();
   cam.SetWidgetArea(r);

   if (!buffer || (buffer && ((buffer->W() < r.W()) || (buffer->H() < r.H()))) || shrink_buffer_on_resize) {
      SetupBuffer(r.W() , r.H() , gwindow);
   }
   else {
      // buffer has excess size but we don't shrink the buffer, so reset the camera to our area
      cam.SetView(buffer , 0 , 0 , r.W() , r.H());
   }
   SyncLayoutPos();
   clear_background = true;
}



void WidgetHandler::OnColorChanged() {
   if (!use_bg_pic) {
      RedrawBackgroundBuffer();
   }
   clear_background = true;
}



void WidgetHandler::TrackWidget(WidgetBase* widget) {
   /// Only add unique widgets and never itself.
   /// widget may be a layout, in which case all of its children get tracked as well

   if (!widget) {return;}
   if (widget == this) {return;}
   if (HasWidget(widget)) {return;}

///   widget = widget->GetDecoratorRoot();
   
   Layout* widget_is_layout = dynamic_cast<Layout*>(widget);
   
   std::vector<WidgetBase*> layout_children;
   
   if (widget_is_layout) {
      layout_children = widget_is_layout->WChildren();
      for (unsigned int i = 0 ; i < layout_children.size() ; ++i) {
         TrackWidget(layout_children[i]);
      }
      widget_is_layout->SetWidgetHandler(this);
   }

   wlist.push_back(widget);
   inputlist.push_back(widget);
   drawlist.insert(drawlist.begin() , widget);
   /** First widget always has the focus */
   if (wlist.size() == 1) {
      GiveWidgetFocus(widget);
   } else {
      if (widget->HasGui()) {
         widget->GetGui()->GiveWidgetFocus(0,false);
      }
/**
      WidgetHandler* wh = widget->GetGui();
      if (wh) {
         wh->GiveWidgetFocus(0,false);
      }
*/
   }
   SortDrawListByPriority();
   widget->SetParent(this);
   widget->SetBgRedrawFlag();
}


/// TODO : WARNING : Tracked widgets may have gone out of scope by now, so we have to
/// be careful not to call any widget functions through their pointers : WARNING : TODO

void WidgetHandler::StopTrackingWidget(WidgetBase* w) {
   EAGLE_ASSERT(w);
   if (!GetValidByAddress(w)) {
      /// TODO : This widget has already been destroyed somehow - note it and move on - figure it out later
      EagleError() << StringPrintF("WidgetHandler::StopTrackingWidget(%p) reports an invalid widget." , w) << endl;
      RemoveUniqueFromList<WidgetBase* , WIDGETLIST>(wlist , w);
      RemoveUniqueFromList<WidgetBase* , WIDGETLIST>(inputlist , w);
      RemoveUniqueFromList<WidgetBase* , WIDGETLIST>(drawlist , w);
      return;
   }
   
   /// TODO : I think this is fixed? NOTE : Uncomment the next line to crash
   EagleInfo() << StringPrintF("WidgetHandler::StopTrackingWidget %s" , w->FullName());
   
   if (!OwnsWidget(w)) {
      EagleWarn() << StringPrintF("WidgetHandler::StopTrackingWidget - we do not own w (%s)\n" , w->FullName());
      return;
   }
   EAGLE_ASSERT(OwnsWidget(w));
   
   Layout* widget_is_layout = dynamic_cast<Layout*>(w);
   std::vector<WidgetBase*> layout_children;
   
   if (widget_is_layout) {
      layout_children = widget_is_layout->WChildren();
      for (unsigned int i = 0 ; i < layout_children.size() ; ++i) {
         StopTrackingWidget(layout_children[i]);
      }
      widget_is_layout->SetWidgetHandler(0);
   }
   
   MakeAreaDirty(w->OuterArea());
   
   RemoveUniqueFromList<WidgetBase* , WIDGETLIST>(wlist , w);
   RemoveUniqueFromList<WidgetBase* , WIDGETLIST>(inputlist , w);
   RemoveUniqueFromList<WidgetBase* , WIDGETLIST>(drawlist , w);

   if (wfocus == w) {
      GiveWidgetFocus(0);
   } else {
      GiveWidgetFocus(wfocus);
   }
   
   w->SetParent(0);
   if (w == root_layout) {
      root_layout = &dumb_layout;
   }
}



bool WidgetHandler::OwnsWidget(WidgetBase* widget) {
   for (UINT i = 0 ; i < wlist.size() ; ++i) {
      if (wlist[i] == widget) {return true;}
   }
   return false;
}



unsigned int WidgetHandler::WidgetIndex(WidgetBase* widget) {
   for (unsigned int index = 0 ; index < wlist.size() ; ++index) {
      WidgetBase* w = wlist[index];
      WidgetHandler* wh = w->GetGui();
      
      if (w == widget) {return index;}
      if (wh && wh->HasWidget(widget)) {return index;}
   }
   return wlist.size();
}


WLIT WidgetHandler::InputListIterator(WidgetBase* widget) {
   for (WLIT it = inputlist.begin() ; it != inputlist.end() ; ++it) {
      if (*it == widget) {return it;}
   }
   return inputlist.end();
}



void WidgetHandler::CheckRedraw(UINT widget_index) {
   if (widget_index >= drawlist.size()) {return;}
   WidgetBase* widget = drawlist[widget_index];
   UINT widgetflags = widget->Flags();
   Rectangle widgetarea = widget->OuterArea();

   /// For widgets that need their background redrawn, this means all widgets behind them need to be redrawn as well
   if ((widgetflags & NEEDS_BG_REDRAW) ||
      ((widgetflags & NEEDS_REDRAW) && !(widgetflags & VISIBLE))) {
      /// Check whether each widget behind it overlaps
      for (UINT i = 0 ; i < widget_index ; ++i) {
         WidgetBase* w = drawlist[i];
         Rectangle area = w->OuterArea();
         if (widgetarea.Overlaps(area)) {
            if (w->Flags().FlagOff(NEEDS_REDRAW)) {// w doesn't have the redraw flag set
               w->SetRedrawFlag();
               CheckRedraw(i);
            }
         }
      }
   }
   /// For widgets that need to be redrawn, this means all widgets in front of them need to be redrawn as well
   if ((widgetflags & NEEDS_REDRAW) && (widgetflags & VISIBLE)) {
      /// Check whether each widget in front of it overlaps
      for (UINT i = widget_index + 1 ; i < drawlist.size() ; ++i) {
         WidgetBase* w = drawlist[i];
         Rectangle area = w->OuterArea();
         if (widgetarea.Overlaps(area)) {
            if (w->Flags().FlagOff(NEEDS_REDRAW)) {
               w->SetRedrawFlag();
               CheckRedraw(i);
            }
         }
      }
   }
}



void WidgetHandler::CheckRedraw() {
   /// Each widget that needs its background redrawn needs all overlapping widgets before it to be redrawn as well
   /// Each widget that needs to be redrawn needs all overlapping widgets after it to be redrawn as well.
   /// Each time a widget is set to be redrawn by this algorithm it needs to have the same two checks performed on it as well.
   /// Each widget that overlaps a dirty background area needs to be redrawn
   for (list<Rectangle>::iterator it = dbg_list.begin() ; it != dbg_list.end() ; ++it) {
      Rectangle r = *it;
      for (UINT i = 0 ; i < drawlist.size() ; ++i) {
         WidgetBase* w = drawlist[i];
#warning FIXME IM BROKEN - RELATIVE VS ABSOLUTE POSITION
         Rectangle area = w->OuterArea();
         if (r.Overlaps(area)) {
            if (w->Flags().FlagOff(NEEDS_REDRAW)) {
               w->SetRedrawFlag();
            }
         }
      }
   }
   /// TODO : From back to front? Or front to back?
   for (UINT i = 0 ; i < drawlist.size() ; ++i) {
      CheckRedraw(i);
   }
}



void WidgetHandler::SortDrawListByPriority() {
   
   stable_sort(drawlist.begin() , drawlist.end() , DrawPriorityIsLess);
   
   
/* KEEP FOR NOW
   vector<WidgetBase*> plist;
   for (unsigned int i = 0 ; i < sortlist.size() ; ++i) {
      WidgetBase* w = sortlist[i];
      MenuBar* mbar = dynamic_cast<MenuBar*>(w);
      if (mbar) {
         plist.push_back(w);
      }
   }
   for (unsigned int i = 0 ; i < sortlist.size() ; ++i) {
      WidgetBase* w = sortlist[i];
      WidgetHandler* wh = dynamic_cast<WidgetHandler*>(w);
      if (wh) {
         plist.push_back(w);
      }
   }
   for (unsigned int i = 0 ; i < sortlist.size() ; ++i) {
      WidgetBase* w = sortlist[i];
      MenuBar* mbar = dynamic_cast<MenuBar*>(w);
      WidgetHandler* wh = dynamic_cast<WidgetHandler*>(w);
      if (!mbar && !wh) {
         plist.push_back(w);
      }
   }
   sortlist = plist;
*/
///   vector<WidgetBase*> plist;
///   plist.push_back(&titlebar);
/*
   for (unsigned int i = 0 ; i < sortlist.size() ; ++i) {
      WidgetBase* w = sortlist[i];
      if (w == titlebar) {
         plist.push_back(w);
      }
   }
*/
/**
   for (unsigned int i = 0 ; i < sortlist.size() ; ++i) {
      WidgetBase* w = sortlist[i];
      WidgetHandler* wh = dynamic_cast<WidgetHandler*>(w);
      if (wh) {
         plist.push_back(w);
      }
   }
   for (unsigned int i = 0 ; i < sortlist.size() ; ++i) {
      WidgetBase* w = sortlist[i];
      WidgetHandler* wh = dynamic_cast<WidgetHandler*>(w);
      if (!wh && (w != &titlebar)) {
         plist.push_back(w);
      }
   }
   sortlist = plist;
*/
   
}



void WidgetHandler::AddToRemoveList(WidgetBase* w) {
   bool onlist = false;
   for (unsigned int i = 0 ; i < removelist.size() ; ++i) {
      if (removelist[i] == w) {
         onlist = true;
         break;
      }
   }
   if (!onlist && OwnsWidget(w)) {
      removelist.push_back(w);
   }
}



void WidgetHandler::RemoveOldWidgets() {
   for (unsigned int i = 0 ; i < removelist.size() ; ++i) {
      RemoveWidget(removelist[i]);
   }
   removelist.clear();
}



void WidgetHandler::RedrawBackgroundBuffer() {
   EAGLE_ASSERT(gwindow);
   if (!background || !buffer) {return;}
   if(!(background->W() && background->H())) {return;}
   
   gwindow->PushDrawingTarget(background);
   gwindow->SetFullCopyBlender();
   gwindow->Clear(bg_col);
   if (user_bg_ptr) {
      if (stretch_bg) {
         // stretch bg to fill background
         gwindow->DrawStretchedRegion(user_bg_ptr,
                                      Rectangle(0,0,user_bg_ptr->W(),user_bg_ptr->H()),
                                      Rectangle(0,0,background->W(),background->H()),
                                      DRAW_NORMAL);
      }
      else {
         // draw centered
         gwindow->Draw(user_bg_ptr , (background->W() - user_bg_ptr->W())/2 , (background->H() - user_bg_ptr->H())/2 , DRAW_NORMAL);
      }
   }
   gwindow->RestoreLastBlendingState();
   gwindow->PopDrawingTarget();
}




bool WidgetHandler::HasWidget(WidgetBase* widget) {
   for (UINT i = 0 ; i < wlist.size() ; ++i) {
      WidgetBase* w = wlist[i];
      WidgetHandler* wh = w->GetGui();
      
      if (w == widget) {return true;}
      if (wh && wh->HasWidget(widget)) {return true;}
   }
   return false;
}



void WidgetHandler::FocusStart() {
   focus_start = true;
}



WidgetBase* WidgetHandler::NextFocus() {
   /// focus index holds the index of the current widget with focus in the wlist
   /// return the next widget that accepts focus if there is one in the list
   /// can't skip to the next one if it is a WidgetHandler - need to use NextFocus() instead
   /// skip the WidgetHandler if it returns 0 from NextFocus()
   /// NextFocus() should return 0 when it reaches the end of it's wlist
   int start_index = focus_index;
   if (focus_start) {
      start_index = -1;
      focus_start = false;
   } else if (wlist.size()) {
      WidgetBase* w = wlist[focus_index];
      WidgetHandler* wh = w->GetGui();
      if (wh) {
         WidgetBase* next = wh->NextFocus();
         if (next) {return next;}
      }
   }
   for (int index = start_index + 1 ; index < (int)wlist.size() ; ++index) {
      WidgetBase* widget = wlist[index];
      UINT flags = widget->Flags();
      if ((flags & ENABLED) && (flags & VISIBLE) && InView(widget)) {
         if (widget->HasGui()) {
            WidgetHandler* handler = widget->GetGui();
            handler->FocusStart();
            WidgetBase* next = handler->NextFocus();
            if (next) {
               return next;
            }
         } else {
            if (widget->AcceptsFocus()) {
               return widget;
            }
         }
      }
   }
   // if next focus reaches the end of the wlist it returns 0 and starts over next time
   focus_start = true;
   return (WidgetBase*)0;
}



WidgetBase* WidgetHandler::PreviousFocus() {
   int start_index = focus_index;
   if (focus_start) {
      start_index = (int)wlist.size();
      focus_start = false;
   } else if (wlist.size()) {
      WidgetBase* w = wlist[focus_index];
      WidgetHandler* wh = w->GetGui();
      if (wh) {
         WidgetBase* previous = wh->PreviousFocus();
         if (previous) {return previous;}
      }
   }
   for (int index = start_index - 1 ; index >= 0 ; --index) {
      WidgetBase* widget = wlist[index];
      UINT flags = widget->Flags();
      if ((flags & ENABLED) && (flags & VISIBLE) && InView(widget)) {
         if (widget->HasGui()) {
            WidgetHandler* handler = widget->GetGui();
            handler->FocusStart();
            WidgetBase* previous = handler->PreviousFocus();
            if (previous) {
               return previous;
            }
         } else {
            if (widget->AcceptsFocus()) {
               return widget;
            }
         }
      }
   }
   focus_start = true;
   return (WidgetBase*)0;
}



void WidgetHandler::CycleFocusForward() {
   if (!wparent) {
      WidgetBase* next = NextFocus();
      if (!next) {
         next = NextFocus();
      }
      GiveWidgetFocus(next);
   }
}



void WidgetHandler::CycleFocusBackward() {
   if (!wparent) {
      WidgetBase* previous = PreviousFocus();
      if (!previous) {
         previous = PreviousFocus();
      }
      GiveWidgetFocus(previous);
   }
}



WidgetHandler::WidgetHandler(EagleGraphicsContext* window , std::string classname , std::string objname) :
		WidgetBase(classname , objname),
		gwindow(window),
		buffer(0),
		background(0),
		user_bg_ptr(0),
		stretch_bg(false),
		use_bg_pic(false),
		bg_col((*(ColorRegistry::GlobalColorRegistry()->GetDefaultColorset()))[BGCOL]),
		cam(),
		shrink_buffer_on_resize(false),
		dumb_layout(),
		root_layout(0),
		wlist(),
		inputlist(),
		drawlist(),
		removelist(),
//		memtrack(),
		focus_index(-1),
		focus_start(true),
		wfocus(0),
		whover(0),
		mque(),
		clear_background(true),
		focus_cycle_forward(input_key_press(EAGLE_KEY_TAB) && input_key_held(EAGLE_KEY_NO_MOD)),
		focus_cycle_backward(input_key_press(EAGLE_KEY_TAB) && input_key_held(EAGLE_KEY_ONLY_SHIFT)),
		dbg_list()
{
   SetRootLayout(&dumb_layout);
   cam.SetParent(this);
   cam.TakesFocus(false);
   SetZOrder(ZORDER_PRIORITY_HIGH);
   SetRedrawFlag();
}



/// The background and buffer will be freed when the WidgetHandler destructor is run.
/// Global WidgetHandlers will need to have FreeImageBuffers() called before main exits.
WidgetHandler::~WidgetHandler() {
   FreeImageBuffers();

   ClearLayout();
   
   root_layout->DetachFromGui();
   
   root_layout = &dumb_layout;
/**
   if (root_layout == &dumb_layout) {
      root_layout->ClearLayout();
   }
   else {
      StopTrackingWidget(root_layout);
      root_layout = 0;
   }
//*/
}



void WidgetHandler::QueueUserMessage(const WidgetMsg& wmsg) {
   
   const WidgetMsg cammovemsg(&cam , TOPIC_CAMERA , CAMERA_VIEW_MOVED);

   if (wmsg == cammovemsg) {
      SetRedrawFlag();
      return;
   }
   
   if (wparent) {
      WidgetBase::QueueUserMessage(wmsg);
   } else {
      mque.push_back(wmsg);
   }
}


void WidgetHandler::SetDrawWindow(EagleGraphicsContext* window) {
   EAGLE_ASSERT(window);
   
   if (window == gwindow) {return;}
   
   EagleGraphicsContext* oldwindow = gwindow;
   EagleImage* oldbuffer = buffer;
   EagleImage* oldbackground = background;
   
   if (window) {
      buffer = window->EmptyImage();
      background = window->EmptyImage();
   }
   
   gwindow = window;
   
   if (oldbuffer && oldbuffer->W() && oldbuffer->H()) {
      SetupBuffer(oldbuffer->W() , oldbuffer->H() , gwindow);
   }

   if (oldwindow && oldbuffer && oldbackground) {
      oldwindow->FreeImage(oldbuffer);
      oldwindow->FreeImage(oldbackground);
   }
   
}



EagleGraphicsContext* WidgetHandler::GetDrawWindow() {
   return gwindow;
}



bool WidgetHandler::SetupBuffer(int w , int h , EagleGraphicsContext* window) {
	bool success = true;

	if (!window) {
      window = gwindow;
	}
	
	gwindow = window;
	
   if (!gwindow) {throw EagleException(StringPrintF("WidgetHandler::SetupBuffer : graphics window not set!\n"));}

   if (!buffer) {
      buffer = gwindow->EmptyImage();
   }
   if (!background) {
      background = gwindow->EmptyImage();
   }
   
   
	if (w <= 0 || h <= 0) {
		buffer->Free();
		background->Free();
		return true;
	}
	else {
	   success = buffer->Allocate(w,h) && background->Allocate(w,h);
	   EAGLE_ASSERT(success);
	}
	if (success) {
	   RedrawBackgroundBuffer();
	   cam.SetView(buffer , Rectangle(0 , 0 , buffer->W() , buffer->H()));
	}
	return success;
}



void WidgetHandler::SetBufferShrinkOnResize(bool buffer_shrink_on_resize) {
   if (shrink_buffer_on_resize != buffer_shrink_on_resize) {
      if (buffer_shrink_on_resize) {
         Rectangle r = OuterArea();
         if ((buffer->W() > r.W()) || (buffer->H() > r.H())) {
            SetupBuffer(r.W() , r.H() , gwindow);
         }
      }
      shrink_buffer_on_resize = buffer_shrink_on_resize;
   }
}



void WidgetHandler::UseBackgroundImage(EagleImage* bg , bool stretch) {
	user_bg_ptr = bg;
	stretch_bg = stretch;
	use_bg_pic = (bool)user_bg_ptr;
	if (background && background->W() && background->H()) {
      RedrawBackgroundBuffer();
	}
	MakeAreaDirty(OuterArea());
}



void WidgetHandler::UseBackgroundColor(EagleColor col) {
	bg_col = col;
	use_bg_pic = false;
	if (background->W() && background->H()) {
	   RedrawBackgroundBuffer();
	}
	MakeAreaDirty(OuterArea());
}



void WidgetHandler::FreeImageBuffers() {
   
   if (gwindow) {
      gwindow->FreeImage(buffer);
      gwindow->FreeImage(background);
   }
   buffer = 0;
   background = 0;

}



bool WidgetHandler::HasMessages() {
   return (!mque.empty());
}



WidgetMsg WidgetHandler::TakeNextMessage() {
   WidgetMsg message;
   if (mque.empty()) {
      message = NoMessages;
   } else {
      message = mque.front();
      mque.pop_front();
   }
   return message;
}
   


void WidgetHandler::ClearMessages() {
   mque.clear();
}





void WidgetHandler::SetRootLayout(Layout* l) {
   if (!l) {
      SetRootLayout(&dumb_layout);
      return;
   }
   
   EAGLE_ASSERT(l->IsRootLayout());
   
   if (root_layout) {
      StopTrackingWidget(root_layout);
   }
   root_layout = l;
   TrackWidget(root_layout);
   SyncLayoutPos();
}



void WidgetHandler::AddWidget(SHAREDWIDGET widget) {
   root_layout->AddWidget(widget);
}



WidgetHandler& WidgetHandler::operator<<(SHAREDWIDGET widget) {
   AddWidget(widget);
   return *this;
}



void WidgetHandler::RemoveWidget(WidgetBase* widget) {
   root_layout->RemoveWidget(widget);
}



void WidgetHandler::ClearLayout() {

   EAGLE_ASSERT(root_layout);
   
   root_layout->ClearWidgets();

   wlist.clear();
   inputlist.clear();
   drawlist.clear();
   wfocus = 0;
   focus_index = -1;
   focus_start = true;
   mque.clear();
   
   MakeAreaDirty(OuterArea());
}

void WidgetHandler::PerformFullRedraw(EagleGraphicsContext* win) {
   /// WidgetHandler needs to be fully redrawn, so redraw every contained widget as well, regardless
   /// of the state of their NEEDS_REDRAW flag.
   win->SetFullCopyBlender();
   
   win->Draw(background , 0 , 0);

   win->RestoreLastBlendingState();
//      blit(background , buffer , 0 , 0 , 0 , 0 , background.W() , background.H());
   
   dbg_list.clear();
   for (UINT i = 0 ; i < drawlist.size() ; ++i) {
      WidgetBase* w = drawlist[i];
      if (w->Flags().FlagOn(VISIBLE)) {
///            Rectangle a = area.InnerArea();
         w->Display(win , 0 , 0);
      }
   }
}



void WidgetHandler::PerformPartialRedraw(EagleGraphicsContext* win) {
   /** Draw dirty backgrounds first */
   dbg_list = ConsolidateRectangles(dbg_list);

   CheckRedraw();

   win->SetFullCopyBlender();
   for (list<Rectangle>::iterator it = dbg_list.begin() ; it != dbg_list.end() ; ++it) {
      Rectangle& r = *it;
      win->DrawRegion(background , r , r.X() , r.Y());
   }
   dbg_list.clear();
   win->RestoreLastBlendingState();

   /** Only widgets with the NEEDS_REDRAW flag should actually redraw */
   for (UINT i = 0 ; i < drawlist.size() ; ++i) {
      WidgetBase* w = drawlist[i];
      UINT flags = w->Flags();
      if (flags & NEEDS_REDRAW) {
         w->Display(win , 0 , 0);
      }
   }
}



void WidgetHandler::DrawBuffer(EagleGraphicsContext* win) {
   EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());
   EAGLE_ASSERT(buffer);
   EAGLE_ASSERT(buffer->Valid());
   EAGLE_ASSERT(background);
   EAGLE_ASSERT(background->Valid());

   RemoveOldWidgets();

///   if (!(WidgetBase::flags & NEEDS_REDRAW)) {return;}

   win->PushDrawingTarget(buffer);
   
   if (clear_background) {
      PerformFullRedraw(win);
      clear_background = false;
   } else {
      PerformPartialRedraw(win);
   
   }

   win->PopDrawingTarget();

}



void WidgetHandler::DrawToWindow(EagleGraphicsContext* win , int xpos , int ypos) {
   
   if (Flags().FlagOn(VISIBLE)) {
         
      cam.SetRedrawFlag();
   
///      win->DrawRegion(buffer , Rectangle(0,0,buffer->W(),buffer->H()) , xpos , ypos);
      //** TODO : IMPORTANT : FIX CAMERA CODE FOR WIDGETHANDLER
///      win->SetPMAlphaBlender();
///      Rectangle a = area.InnerArea();
///      cam.Display(win , xpos + a.X() , ypos + a.Y());

      win->SetPMAlphaBlender();
      cam.Display(win , xpos , ypos);
      win->RestoreLastBlendingState();
      
   }
   
   ClearRedrawFlag();
}



void WidgetHandler::SetBackgroundColor(const EagleColor color) {
   bg_col = color;
   use_bg_pic = false;
   RedrawBackgroundBuffer();
}



void WidgetHandler::SyncLayoutPos() {
   EAGLE_ASSERT(root_layout->IsRootLayout());
   root_layout->WidgetBase::SetWidgetArea(InnerArea(),false);
}



void WidgetHandler::SyncCamera() {
   cam.SetViewArea(cam.ViewArea());
}



void WidgetHandler::SetRedrawFlag() {
   SetBgRedrawFlag();
}



void WidgetHandler::SetFullRedraw() {
   SetRedrawFlag();
   clear_background = true; 
}




/** Revised but buggy SetFocusState and GiveWidgetFocus

/// One widget handler has the focus at a time, and it may have one of its widgets have the focus
/// 
void WidgetHandler::SetFocusState(bool state) {
   WidgetBase::SetFocusState(state);
   titlebar.SetFocusState(state);
   mintitlebar.SetFocusState(state);
   if (!state) {
      if (wfocus) {
         wfocus->SetFocusState(false);
         /// The widget handler that owns the wfocus needs to be notified it has lost focus as well
         WidgetBase* pfocus = wfocus->Parent();
         WidgetHandler* pwh = dynamic_cast<WidgetHandler*>(pfocus);
         if (pwh && (pwh != this)) {
            pwh->SetFocusState(false);
         }
         wfocus = 0;
      }
   }
}




///   How to keep the widgets sorted?
///   - Have the focused widget always be first in the sorted list by shifting it there from
///      its last position.
///   - Keep track of the index of the widget with focus and check input forward from there
///      and draw backwards from the end to the focused widget.

bool WidgetHandler::GiveWidgetFocus(WidgetBase* widget) {
//   EagleInfo() << "Giving widget focus to " << widget << endl;
   if (parent) {
      return parent->GiveWidgetFocus(widget);
   }
   if (!widget) {
      SetFocusState(false);
      wfocus = 0;
      focus_index = wlist.size();
      return true;
   }
   if (HasWidget(widget)) {
      bool accepts_focus = widget->AcceptsFocus();
      if (!accepts_focus) {return false;}
      if (wfocus != widget) {
         if (wfocus) {
            WidgetBase* pfocus = wfocus->Parent();
            WidgetHandler* pwh = dynamic_cast<WidgetHandler*>(pfocus);
            if (pwh) {
               pwh->SetFocusState(false);
            } else {
               wfocus->SetFocusState(false);
            }
         }
         WidgetBase* pwidget = widget->Parent();
         WidgetHandler* pwh = dynamic_cast<WidgetHandler*>(pwidget);
         if (pwh) {
            pwh->SetFocusState(true);
         }
         wfocus = widget;
         focus_index = WidgetIndex(widget);
         wfocus->SetFocusState(true);

         // Move the widget with focus to the beginning of the sorted list
         WLIT focus_iter = SortListIterator(wfocus);
         if (focus_iter == sortlist.end()) {
            // See if any of the top level widgets is a WidgetHandler and whether it has the widget wfocus
            for (UINT i = 0 ; i < sortlist.size() ; ++i) {
               WidgetBase* w = sortlist[i];
               WidgetHandler* wh = dynamic_cast<WidgetHandler*>(w);
               TabGroup* tg = dynamic_cast<TabGroup*>(w);
               if (wh) {
                  if (wh->HasWidget(wfocus)) {
                     focus_iter = SortListIterator(w);
                     i = sortlist.size();
                  }
               }
               if (tg) {
                  if (tg->HasWidget(wfocus)) {
                     focus_iter = SortListIterator(w);
                     i = sortlist.size();
                  }
               }
            }
         }
         ASSERT(focus_iter != sortlist.end());
         WidgetBase* w = *focus_iter;
         sortlist.erase(focus_iter);
         sortlist.insert(sortlist.begin() , w);
         return true;
      }
   }
   return false;
}
//*/


//** OLD SetFocusState and GiveWidgetFocus

void WidgetHandler::SetFocusState(bool state) {
   WidgetBase::SetFocusState(state);
   if (!state) {
      if (wfocus) {
         wfocus->SetFocusState(false);
         /// The widget handler that owns the wfocus needs to be notified it has lost focus as well
         WidgetHandler* pwh = wfocus->GetHandler();
         if (pwh && (pwh != this)) {
            pwh->GiveWidgetFocus(0 , false);
         }
         wfocus = 0;
      }
   }
}




///   How to keep the widgets sorted?
///   - Have the focused widget always be first in the sorted list by shifting it there from
///      its last position.
///   - Keep track of the index of the widget with focus and check input forward from there
///      and draw backwards from the end to the focused widget.

bool WidgetHandler::GiveWidgetFocus(WidgetBase* widget , bool notify_parent) {
//   EagleInfo() << "Giving widget focus to " << widget << endl;

   WidgetHandler* pwh = dynamic_cast<WidgetHandler*>(widget->GetParent());
   if (notify_parent && pwh && pwh != this) {
      return pwh->GiveWidgetFocus(widget , false);
   }
   
   if (!widget) {
      SetFocusState(false);
      return true;
   }
   
   if (HasWidget(widget) && widget->AcceptsFocus()) {
/** TODO : WARNING : Do not enable this line, if you do it will break RemoveWidget because it depends
              on being able to set the same focus again (this resets the focus_index properly).
   if ((widget != wfocus) && HasWidget(widget) && widget->AcceptsFocus()) {
      
      TODO : I think this is fixed now, by the "if (wfocus && wfocus != widget)" line below
*/      
      /// Remove focus from previous widget
      if (wfocus && wfocus != widget) {
         /// The widget handler that owns the wfocus needs to be notified it has lost focus as well
         wfocus->SetFocusState(false);
         WidgetHandler* pwh2 = dynamic_cast<WidgetHandler*>(wfocus->GetParent());
         if (pwh2) {
            pwh2->GiveWidgetFocus(0 , false);
         }
      }
      if (OwnsWidget(widget)) {
         SetFocusState(true);
      }
      wfocus = widget;
      focus_index = WidgetIndex(widget);
      wfocus->SetFocusState(true);

      // Move the widget with focus to the beginning of the sorted list
      WLIT focus_iter = InputListIterator(wfocus);
      if (focus_iter == inputlist.end()) {
         // See if any of the top level widgets is a WidgetHandler and whether it has the widget wfocus
         for (UINT i = 0 ; i < inputlist.size() ; ++i) {
            WidgetBase* w = inputlist[i];
            WidgetHandler* wh = w->GetGui();
            if (wh) {
               if (wh->HasWidget(wfocus)) {
                  wh->GiveWidgetFocus(wfocus , false);
                  focus_iter = InputListIterator(w);
                  i = inputlist.size();
               }
            }
         }
      }
      EAGLE_ASSERT(focus_iter != inputlist.end());
      // move to front of input list
      WidgetBase* w = *focus_iter;
      inputlist.erase(focus_iter);
      inputlist.insert(inputlist.begin() , w);
      WLIT draw_iter = drawlist.end();
      for (draw_iter = drawlist.begin() ; draw_iter != drawlist.end() ; ++draw_iter) {
         if (*draw_iter == w) {break;}
      }
      EAGLE_ASSERT(draw_iter != drawlist.end());
      // move to end of draw list
      drawlist.erase(draw_iter);
      drawlist.push_back(w);
      SortDrawListByPriority();
      return true;
   }
   return false;
}

//*/


void WidgetHandler::SetRedrawAllFlag() {
   clear_background = true;
   SetRedrawFlag();
}



bool WidgetHandler::HasGui() {
   return true;
}



WidgetHandler* WidgetHandler::GetGui() {
   return this;
}



void WidgetHandler::MakeAreaDirty(Rectangle dirty_area) {
   dbg_list.push_back(dirty_area);
   SetRedrawFlag();
}



WidgetBase* WidgetHandler::CurrentFocus() {
   return wfocus;
}



WidgetBase* WidgetHandler::CurrentHover() {
   return whover;
}



EagleImage* WidgetHandler::BackgroundBitmap() {
   return background;
}



EagleImage* WidgetHandler::BufferBitmap() {
   return buffer;
}



bool WidgetHandler::AreaFree(Rectangle r , WidgetBase* widget) {
   if (!OuterArea().Contains(r)) {return false;}
   for (UINT i = 0 ; i < wlist.size() ; ++i) {
      WidgetBase* w = wlist[i];
      if ((w != widget) && r.Overlaps(w->OuterArea())) {
         return false;
      }
   }
   return true;
}



bool WidgetHandler::InView(WidgetBase* w) {
   if (!w) {return false;}
   Rectangle view = cam.ViewArea();
   Rectangle area = w->OuterArea();
   return (view.Overlaps(area));
   /// TODO : Perhaps add in checking to see if the widget w is fully obscured by other widgets?
}



/// Returns top most widget at x,y
#warning TODO : FIXME, WORKING HERE
WidgetBase* WidgetHandler::GetWidgetAt(int x , int y) {
   for (int i = (int)drawlist.size() - 1 ; i >= 0 ; --i) {
      WidgetBase* w = drawlist[i];
      if (w->OuterArea().Contains(x,y)) {
         WidgetHandler* wh = dynamic_cast<WidgetHandler*>(w);
         if (wh) {
            return wh->GetWidgetAt(x - wh->OuterArea().X() , y - wh->OuterArea().Y());
         } else {
            return w;
         }
      }
   }
   if (OuterArea().Contains(x,y)) {return this;}
   return 0;
}



int WidgetHandler::GetMouseX() {
   
   WidgetHandler* handler = dynamic_cast<WidgetHandler*>(wparent);
   
   int mx = mouse_x;
   
   int absx = OuterArea().X();
   
   if (handler) {
      mx = handler->GetMouseX();
   }
   else if (wparent) {
      absx += AbsParentX();
   }
   
   const int cx = cam.X();
   
   return (mx - absx) + cx;
/**   
   if (whandler) {
      return (whandler->GetMouseX() - area.OuterArea().X()) + cam.X();
   }
   else if (wparent) {
      return (mouse_x - (AbsParentX() + area.OuterArea().X())) + cam.X();
   }
   else {
      return (mouse_x - area.OuterArea().X()) + cam.X();
   }
   return 0;
*/
}


#warning FIXME WHAT ARE THESE FOR AND HOW DO THEY WORK
int WidgetHandler::GetMouseY() {
   
   WidgetHandler* handler = dynamic_cast<WidgetHandler*>(wparent);
   
   int my = mouse_y;
   
   int absy = OuterArea().Y();
   
   if (handler) {
      my = handler->GetMouseY();
   }
   else if (wparent) {
      absy += AbsParentY();
   }
   
   const int cy = cam.Y();
   
   return (my - absy) + cy;

/**
   if (whandler) {
      return (whandler->GetMouseY() - area.OuterArea().Y()) + cam.Y();
   }
   else if (wparent) {
      return (mouse_y  - (area.OuterArea().Y() + AbsParentY())) + cam.Y();
   }
   else {
      return (mouse_y  - area.OuterArea().Y()) + cam.Y();
   }
*/
   return 0;
}



void WidgetHandler::AccMoveViewTlxTo(int xpos , int ypos , double time) {
   cam.AccMoveViewTlxTo(xpos , ypos , time);
}



void WidgetHandler::AccMoveViewCenterTo(int xpos , int ypos , double time) {
   cam.AccMoveViewCenterTo(xpos , ypos , time);
}



void WidgetHandler::AccMoveViewBy(int dx , int dy , double time) {
   cam.AccMoveViewBy(dx , dy , time);
}



void WidgetHandler::MoveViewTlxTo(int xpos , int ypos , double time) {
   cam.MoveViewTlxTo(xpos , ypos , time);
}



void WidgetHandler::MoveViewCenterTo(int xpos , int ypos , double time) {
   cam.MoveViewCenterTo(xpos , ypos , time);
}



void WidgetHandler::MoveViewBy(int dx , int dy , double time) {
   cam.MoveViewBy(dx , dy , time);
}



void WidgetHandler::AllowMiddleMouseButtonDrag(bool allow) {
   cam.AllowMiddleMouseButtonDrag(allow);
}



std::ostream& WidgetHandler::DescribeTo(std::ostream& os , Indenter indent) const {
   os << indent << "WidgetHandler class object - address " << this << " {" << endl;
   ++indent;
   os << indent << "Buffer Data :" << endl;
   ++indent;
   if (buffer) {
      buffer->DescribeTo(os , indent);
      os << endl;
   }
   else {
      os << indent << "NULL Buffer." << endl;
   }
   --indent;
   os << indent << "Background Data :" << endl;
   ++indent;
   if (background) {
      background->DescribeTo(os , indent);
      os << endl;
   }
   else {
      os << indent << "NULL Background." << endl;
   }
   --indent;
   os << indent;
   os << "[clear_background = " << (clear_background?"true] ":"false] ");
   os << "[use_bg_pic = " << (use_bg_pic?"true] ":"false] ");
   os << "[stretch_bg = " << (stretch_bg?"true] ":"false] ");
   os << endl;
   os << indent << "Background color : [" << bg_col << "] ";
   os << StringPrintF("user_bg_ptr = %p (",user_bg_ptr);
   if (user_bg_ptr) {
      user_bg_ptr->DescribeTo(os);
   }
   os << ")" << endl;
   os << indent << StringPrintF("wfocus = %p (%s) , whover = %p (%s)" ,
                                 wfocus , wfocus?wfocus->FullName():"" , whover , whover?whover->FullName():"") << endl;
   os << indent << "Widget Camera info :" << endl;
   ++indent;
   cam.DescribeTo(os,indent);
   --indent;
   WidgetBase::DescribeTo(os , indent);
   os << indent << "Root Layout at " << root_layout << " is named " << (root_layout?root_layout->FullName():"NULL") << endl;
   os << indent << "Widgets used by this dialog : {" << endl;
   ++indent;
   unsigned int nwidgets = wlist.size();
   for (unsigned int i = 0 ; i < nwidgets ; ++i) {
      os << indent << "Widget # " << i << endl;
      wlist[i]->DescribeTo(os , indent);
   }
   --indent;
   os << indent << "}" << endl;
   --indent;
   os << indent << "}" << endl;
   return os;
}












