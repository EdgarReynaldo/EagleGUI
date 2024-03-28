
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
 *    Copyright 2009-2021+ by Edgar Reynaldo
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




int WidgetHandler::PrivateHandleEvent(EagleEvent e) {
   if (!(wlist.size())) {
      return DIALOG_OKAY;
   }

   /**
      Since WidgetHandlers have their own buffer, they need to offset the position they pass to their
      widgets since each widget stores its position on their own drawing target (the WidgetHandler buffer).
      This way, every widget that is called will have the relative mouse position on their target bitmap, and
      they won't have to adjust it.
   */
   EagleEvent rel_event = e;
   EagleEvent cam_event = e;
   /// Relative mouse position
   if (IsMouseEvent(e)) {
      /// Mouse position relative to buffer
      rel_event.mouse.x = e.mouse.x + (cam.ViewX() - InnerArea().X());
      rel_event.mouse.y = e.mouse.y + (cam.ViewY() - InnerArea().Y());
      /// Mouse position relative to camera
///      cam_event.mouse.x = e.mouse.x - InnerArea().X();
///      cam_event.mouse.y = e.mouse.y - InnerArea().Y();
///      cam_event.mouse.x = e.mouse.x;
///      cam_event.mouse.y = e.mouse.y;
   }

   /// Need to get mouse position relative to buffer
///   const int mx = GetMouseX();/// I don't know if these work, they should do the same thing, but maybe mouse_x and mouse_y are wrong.
///   const int my = GetMouseY();

   if (IsMouseEvent(e)) {
      /// WidgetHandlers will check whether the mouse is hovering over one of the widgets, and set the hover flag for
      /// that widget, as well as removing it from any widget that had it last.
      if (!wparent) {/// Only root gui checks for hover and focus
         WidgetBase* hoverwidget = GetWidgetAt(rel_event.mouse.x , rel_event.mouse.y);

         if (hoverwidget && !hoverwidget->DoIReallyHaveHover(rel_event.mouse.x , rel_event.mouse.y)) {
            hoverwidget = 0;
         }

         Rectangle clip = hoverwidget?hoverwidget->GetClipRectangle():BADRECTANGLE;

         if (clip == BADRECTANGLE || (clip != BADRECTANGLE && !clip.Contains(rel_event.mouse.x , rel_event.mouse.y))) {
            hoverwidget = 0;
         }

         /// Check widget hover
         if (hoverwidget != whover) {
            if (whover) {
               whover->SetHoverState(false);
            }
            if (hoverwidget) {
               hoverwidget->SetHoverState(true);
            }
            whover = hoverwidget;
/**
            EAGLE_DEBUG(
               std::string name = (hoverwidget?hoverwidget->ShortName():std::string(""));
               EagleInfo() << StringPrintF("Given hover to %s" , name.c_str()) << std::endl;
            );
//*/
         }

         /// Check widget focus
         if (e.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN) {
            if (e.mouse.button == 1) {
               if (whover != wfocus) {
                  GiveWidgetFocus(whover , true);
               }
            }
         }
      }
   }

///   bool gui_takes_focus = false;/// unused
   int msg;
   int retmsg = DIALOG_OKAY;
   bool focus_taken = false;


   /// Check the widget with focus first - inputlist is sorted so that the widget with focus is first in the list
   /// Debugging, remove
   if (rel_event.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN) {
      EagleInfo() << "Breakpoint" << std::endl;
   }

   for (UINT index = 0 ; index < inputlist.size() ; ++index) {
      WidgetBase* widget = inputlist[index];

      if (widget->Flags().FlagOff(ENABLED)) {continue;}

///      if (!widget->GetClipRectangle().Contains(rel_event.mouse.x , rel_event.mouse.y) && e.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN) {
/*
The previous line triggers uninitialized value warnings from valgrind, in Rectangle::Contains
*/
///         continue;Some widgets monitor clicks outside their clipping rectangle, like the widget mover
///      }
      msg = widget->HandleEvent(rel_event);

      /// Warning - All messages not related to a dialog will be ignored.
      /// Use QueueUserMessage or RaiseEvent for user notifications instead.

      if (msg == DIALOG_OKAY)      {continue;}
      if (msg & DIALOG_CLOSE)      {return msg;}// Pass the CLOSE message up the chain and back to the user
      ///if (msg & DIALOG_DISABLED) {}// Ignore disabled dialogs
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
         /// Don't check any of the other widget's inputs during this update
         return retmsg;
      }
      if (focus_taken) {
         return retmsg;
      }
   }
   /// Camera is at IndentX,IndentY
   msg = cam.HandleEvent(cam_event);
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
///   if (gui_takes_focus) {
///      return DIALOG_TAKE_FOCUS;
///   }
   return retmsg;
}



void WidgetHandler::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());

///   Pos2I pos(warea.InnerArea().X() + xpos , warea.InnerArea().Y() + ypos);/// wrong

   DrawBuffer(win);

   DrawToWindow(win , xpos , ypos);
}





int WidgetHandler::PrivateUpdate(double tsec) {

   int msg = 0;
   int retmsg = DIALOG_OKAY;

   cam.Update(tsec);

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

   return msg;
}




void WidgetHandler::OnAreaChanged() {
   /// INFO : Use OuterArea for camera. Camera is always at 0,0,OuterWidth,OuterHeight
   WIDGETAREA wa = GetWidgetArea();
   Rectangle r = wa.InnerArea();

   cam.SetWidgetArea(InnerArea());

   if (gwindow) {
      if (!buffer ||
          (buffer && ((buffer->W() < r.W()) || (buffer->H() < r.H()))) ||
           shrink_buffer_on_resize)
      {
         SetupBuffer(r.W() , r.H() , gwindow);
      }
      else {
         /// buffer has excess size but we don't shrink the buffer, so reset the camera to our area
      }
      SyncCamera();
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

   EagleInfo() << StringPrintF("WidgetHandler::TrackWidget (%s)" , widget->FullName()) << std::endl;

   LayoutBase* widget_is_layout = dynamic_cast<LayoutBase*>(widget);

   std::vector<WidgetBase*> layout_children;

   if (widget_is_layout) {
      layout_children = widget_is_layout->WChildren();
      for (unsigned int i = 0 ; i < layout_children.size() ; ++i) {
         TrackWidget(layout_children[i]);
      }
      widget_is_layout->SetWidgetHandler(this);
   }

   wlist.push_back(widget);
///   inputlist.push_back(widget);
///   drawlist.insert(drawlist.begin() , widget);
   inputlist.insert(inputlist.begin() , widget);/// Insert at top (front)
   drawlist.push_back(widget);/// Add to back (front)

   /// Set these before setting the focus, otherwise it thinks it has no parent
   widget->SetParent(this);
   widget->SetWidgetHandler(this);
   widget->SetBgRedrawFlag();

   /** First widget to accept focus gets it */
   if (!wfocus && widget->AcceptsFocus()) {
      GiveWidgetFocus(widget , true);
   }

   SortDrawListByPriority();
}


/// TODO : WARNING : Tracked widgets may have gone out of scope by now, so we have to
/// be careful not to call any widget functions through their pointers : WARNING : TODO

void WidgetHandler::StopTrackingWidget(WidgetBase* w) {
   EAGLE_ASSERT(w);
   EAGLE_ID eid = w->GetEagleId();
   if (!GetValidById(eid)) {
      /// TODO : This widget has already been destroyed somehow - note it and move on - figure it out later
      EagleError() << StringPrintF("WidgetHandler::StopTrackingWidget(%p) reports an invalid widget." , (void*)w) << endl;
      RemoveUniqueFromList<WidgetBase* , WIDGETLIST>(wlist , w);
      RemoveUniqueFromList<WidgetBase* , WIDGETLIST>(inputlist , w);
      RemoveUniqueFromList<WidgetBase* , WIDGETLIST>(drawlist , w);
      return;
   }

   EagleInfo() << StringPrintF("WidgetHandler::StopTrackingWidget (%s)" , w->FullName()) << std::endl;/// may crash if widget is out of scope, TODO , FIXME

   if (!OwnsWidget(w)) {
      EagleWarn() << StringPrintF("WidgetHandler::StopTrackingWidget - we do not own w (%s)\n" , w->FullName()) << std::endl;
      return;
   }
   EAGLE_ASSERT(OwnsWidget(w));

   LayoutBase* widget_is_layout = dynamic_cast<LayoutBase*>(w);
   std::vector<WidgetBase*> layout_children;

   if (widget_is_layout) {
      layout_children = widget_is_layout->WChildren();
      for (unsigned int i = 0 ; i < layout_children.size() ; ++i) {
         StopTrackingWidget(layout_children[i]);
      }
   }

   MakeAreaDirty(w->OuterArea());

   if (wfocus == w) {
      GiveWidgetFocus(0);/// The widget being removed had the focus
   } else {
      GiveWidgetFocus(wfocus);/// Reset the focus index
   }

   w->SetParent(0);
   w->SetWidgetHandler(0);

   RemoveUniqueFromList<WidgetBase* , WIDGETLIST>(wlist , w);
   RemoveUniqueFromList<WidgetBase* , WIDGETLIST>(inputlist , w);
   RemoveUniqueFromList<WidgetBase* , WIDGETLIST>(drawlist , w);

   if (w == root_layout) {
      root_layout = &dumb_layout;
      SyncLayoutPos();
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



WLIT WidgetHandler::DrawListIterator(WidgetBase* widget) {
   for (WLIT it = drawlist.begin() ; it != drawlist.end() ; ++it) {
      if (*it == widget) {return it;}
   }
   return drawlist.end();
}



std::set<unsigned int> WidgetHandler::CheckRedraw(UINT widget_index) {
   std::set<unsigned int> eset;
   if (widget_index >= drawlist.size()) {return eset;}
   WidgetBase* cwidget = drawlist[widget_index];
   UINT cflags = cwidget->Flags();
   Rectangle carea = cwidget->OuterArea();

   if (!(cflags & NEEDS_REDRAW)) {
      return eset;
   }

   /// If we need to be redrawn, so does every widget in front of us that overlaps
   /// Check every widget in front of us for overlap with our area
   std::set<unsigned int> checkset;
   for (unsigned int i = widget_index + 1 ; i < drawlist.size() ; ++i) {
      WidgetBase* w = drawlist[i];
      if (carea.Overlaps(w->OuterArea())) {
         if (w->Flags().FlagOff(NEEDS_REDRAW)) {
            w->SetRedrawFlag();
            checkset.insert(i);
         }
      }
   }
   return checkset;
}



void WidgetHandler::CheckRedraw() {
   /// Check from back to front
   typedef std::set<unsigned int> WSET;
   typedef list<Rectangle> RLIST;

   /// Prime the list of widgets to check
   WSET recheck;
   for (unsigned int i = 0 ; i < drawlist.size() ; ++i) {
      if (drawlist[i]->Flags().FlagOn(NEEDS_REDRAW)) {
         recheck.insert(i);
      }
   }

   /// Prime the dirty background list - it may be altered by CheckRedraw or if a widget sets the BG_REDRAW flag
   RLIST dirty = dbg_list;
   dbg_list.clear();

   /// For storing new areas that get marked as dirty by widget checks
   RLIST new_dbg_list;

   /// Run each list at least once
   do {
      WSET check = recheck;
      recheck.clear();

      /// Each widget that overlaps a dirty background area needs to be redrawn
      for (RLIST::iterator it = dirty.begin() ; it != dirty.end() ; ++it) {
         Rectangle r = *it;
         for (UINT i = 0 ; i < drawlist.size() ; ++i) {
            WidgetBase* w = drawlist[i];
            Rectangle area = w->OuterArea();
            if (r.Overlaps(area)) {
               if (w->Flags().FlagOff(NEEDS_REDRAW)) {
                  w->SetRedrawFlag();/// This may alter dbg_list
                  recheck.insert(i);
               }
            }
         }
      }

      /// Each widget that needs redraw needs to check for causing other widgets to redraw
      for (WSET::iterator it = check.begin() ; it != check.end() ; ++it) {
         WSET newcheck = CheckRedraw(*it);/// This may alter dbg_list
         recheck.insert(newcheck.begin() , newcheck.end());
      }

      /// Prepare the next set of dirty backgrounds
      new_dbg_list.insert(new_dbg_list.end() , dirty.begin() , dirty.end());
      dirty = dbg_list;
      dbg_list.clear();

   } while (recheck.size() || dirty.size());

   dbg_list = new_dbg_list;
}



void WidgetHandler::SortDrawListByPriority() {
   stable_sort(drawlist.begin() , drawlist.end() , DrawPriorityIsLess);
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
   if (use_bg_pic && user_bg_ptr) {
      if (stretch_bg) {
         // stretch bg to fill background
         gwindow->DrawStretchedRegion(user_bg_ptr,
                                      Rectangle(0,0,user_bg_ptr->W(),user_bg_ptr->H()),
                                      Rectangle(0,0,background->W(),background->H()),
                                      DRAW_NORMAL);
      }
      else {
         // draw centered
         gwindow->Draw(user_bg_ptr , (background->W() - user_bg_ptr->W())/2 , (background->H() - user_bg_ptr->H())/2);
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
		focus_index(-1),
		focus_start(true),
		wfocus(0),
		whover(0),
		mque(),
		clear_background(true),
		always_clear(false),
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



#warning - TODO : If eagle is shutdown before the destructor runs bad things happen
/// The background and buffer will be freed when the WidgetHandler destructor is run.
/// Global WidgetHandlers will need to have FreeImageBuffers() called before main exits.

WidgetHandler::~WidgetHandler() {
   FreeImageBuffers();

   ClearLayout();

   root_layout->DetachFromGui();

   root_layout = &dumb_layout;
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
      EmitEvent(MakeEagleEvent(wmsg));
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
   if (oldwindow) {
      if (oldbuffer) {
         oldwindow->FreeImage(oldbuffer);
      }
      if (oldbackground) {
         EagleImage* oldtarget = window->GetDrawingTarget();
         window->SetDrawingTarget(background);
         window->SetCopyBlender();
         window->Draw(oldbackground , 0 , 0);
         window->RestoreLastBlendingState();
         window->SetDrawingTarget(oldtarget);
         oldwindow->FreeImage(oldbackground);
      }
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
      /// Set default view for camera
      /// We may not know our size or camera position yet, so set the full view of the bitmap
      /// Successive resizes will not shrink the view, so check if our area is set
	   Rectangle vrect(0 , 0 , w , h);
	   cam.SetView(buffer , vrect);
	}
	return success;
}



void WidgetHandler::SetBufferShrinkOnResize(bool buffer_shrink_on_resize) {
   if (shrink_buffer_on_resize != buffer_shrink_on_resize) {
      if (buffer_shrink_on_resize) {
         Rectangle r = InnerArea();
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
      if (buffer) {
         gwindow->FreeImage(buffer);
      }
      if (background) {
         gwindow->FreeImage(background);
      }
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





void WidgetHandler::SetRootLayout(LayoutBase* l) {
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



void WidgetHandler::AddWidget(WidgetBase* widget) {
   root_layout->AddWidget(widget);
}



WidgetHandler& WidgetHandler::operator<<(WidgetBase* widget) {
   AddWidget(widget);
   return *this;
}



WidgetHandler& WidgetHandler::operator<<(WidgetBase& widget) {
   AddWidget(&widget);
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

   dbg_list.clear();
   for (UINT i = 0 ; i < drawlist.size() ; ++i) {
      WidgetBase* w = drawlist[i];
      if (w->Flags().FlagOn(VISIBLE)) {
         w->Display(win , 0 , 0);
      }
   }
}



void WidgetHandler::PerformPartialRedraw(EagleGraphicsContext* win) {

   CheckRedraw();/// May affect dbg_list

   /** Draw dirty backgrounds first */
   dbg_list = ConsolidateRectangles(dbg_list);

   win->SetFullCopyBlender();
   for (list<Rectangle>::iterator it = dbg_list.begin() ; it != dbg_list.end() ; ++it) {
      Rectangle& r = *it;
      win->DrawTintedRegion(background , r , r.X() , r.Y());
   }
   win->RestoreLastBlendingState();

   dbg_list.clear();

   /** Only widgets with the NEEDS_REDRAW flag should actually redraw */
   for (UINT i = 0 ; i < drawlist.size() ; ++i) {
      WidgetBase* w = drawlist[i];
      UINT flags = w->Flags();
      if ((flags & NEEDS_REDRAW) && (flags & VISIBLE)) {
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

   win->PushDrawingTarget(buffer);

   if (always_clear) {
      clear_background = true;
   }

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

      win->SetPMAlphaBlender();
      cam.Display(win , xpos , ypos);
      win->RestoreLastBlendingState();
   }
}



void WidgetHandler::SetBackgroundColor(const EagleColor color) {
   bg_col = color;
   RedrawBackgroundBuffer();
   clear_background = true;
}



void WidgetHandler::SyncLayoutPos() {
   EAGLE_ASSERT(root_layout->IsRootLayout());

   /// Layout is always at 0,0,buffer->w,buffer->h, so all children widgets are drawn correctly on the buffer
   if (buffer && buffer->Valid()) {
      Rectangle bufrect(0 , 0 , buffer->W() , buffer->H());
      root_layout->WidgetBase::SetWidgetArea(bufrect , false);
   }
}


void WidgetHandler::SyncCamera() {
   /// Reset the camera position as best we can
   Rectangle oldview = cam.ViewArea();
   const int iw = InnerArea().W();
   const int ih = InnerArea().H();

   /// Make the camera view as large as possible
   cam.SetViewArea(oldview.X() , oldview.Y() , iw , ih);
}



#if 0 /// OLD
void WidgetHandler::SyncCamera() {
   /// Reset the camera position as best we can
   Rectangle v = cam.ViewArea();
   int iw = warea.InnerAreaWidth();
   int ih = warea.InnerAreaHeight();
   if ((v.W() >= iw) || (v.H() >= ih)) {
      v.SetDimensions((v.W() >= iw)?iw:v.W() ,
                      (v.H() >= ih)?ih:v.H());
   }
   cam.SetViewArea(v);
}
#endif


void WidgetHandler::SetRedrawFlag() {
   SetBgRedrawFlag();
}



void WidgetHandler::SetFullRedraw() {
   SetRedrawFlag();
   clear_background = true;
}



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



/**
   /// Each widget handler stores the index of the widget with focus
*/




bool WidgetHandler::GiveWidgetFocus(WidgetBase* widget , bool notify_parent) {

   std::string name = "";
   if (widget) {name = widget->FullName();}
   EagleLog() << StringPrintF("Giving widget focus to [%s]\n" , name.c_str());

   if (!widget) {
      /// Setting the widget focus to NONE
      if (wfocus) {
         /// Remove current focus
         wfocus->SetFocusState(false);/// This propagates up the parent chain
      }
      wfocus = 0;
      /// focus index should remain unchanged
      return true;
   }

   if (widget == this) {
      return false;
   }

   WidgetHandler* pwh = dynamic_cast<WidgetHandler*>(widget->GetParent());

   EAGLE_ASSERT(pwh);

   if (notify_parent && pwh && pwh != this) {
      /// Send the request for focus up the parent widget handler chain
      return pwh->GiveWidgetFocus(widget , true);
   }

   EAGLE_ASSERT(HasWidget(widget));

   /// Remove old focus
   if (wfocus && wfocus != widget) {
      wfocus->SetFocusState(false);
   }
   /// Set new focus
   widget->SetFocusState(true);
   wfocus = widget;

   /// Things get tricky here, as we might not own the widget we're giving focus to
   /// Start with the widget and work up the parent chain
   WidgetBase* w = widget;

   while (pwh) {
      pwh->BringToFront(w);
      w = pwh;
      pwh = w->GetHandler();
   }
   return true;
}



void WidgetHandler::BringToFront(WidgetBase* w) {
   EAGLE_ASSERT(w);
   EAGLE_ASSERT(OwnsWidget(w));

   /// Move to front of input list
   WLIT it = InputListIterator(w);
   EAGLE_ASSERT(it != inputlist.end());
   inputlist.erase(it);
   inputlist.insert(inputlist.begin() , w);
   focus_index = WidgetIndex(w);

   /// Move to back of draw list (the front of the drawing)
   WLIT dit = DrawListIterator(w);
   EAGLE_ASSERT(dit != drawlist.end());
   drawlist.erase(dit);
   drawlist.push_back(w);
   SortDrawListByPriority();
}



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



///< Returns top most widget at x,y

WidgetBase* WidgetHandler::GetWidgetAt(const int absx , const int absy) {

   /// If the absolute position is outside of our area
   if (!OuterArea().Contains(absx , absy)) {
      return (WidgetBase*)0;
   }
   if (!InnerArea().Contains(absx , absy)) {
      return this;
   }
   /// Make the position relative to the buffer
   int relx = absx - InnerArea().X();
   int rely = absy - InnerArea().Y();

   /// Account for the camera
   relx += cam.ViewX();
   rely += cam.ViewY();

   /// Search drawlist from front to back
   for (int i = (int)drawlist.size() - 1 ; i >= 0 ; --i) {
      WidgetBase* w = drawlist[i];
///      bool visible = w->Flags().FlagOn(VISIBLE);
///      if (visible && w->OuterArea().Contains(relx , rely)) {/// TODO : FIXME?
      if (w->OuterArea().Contains(relx , rely)) {
         WidgetHandler* wh = dynamic_cast<WidgetHandler*>(w);
         if (!wh) {
            return w;
         }
         return wh->GetWidgetAt(relx , rely);
      }
   }

   /// We know the widget is inside our area, and outside of all others
   return this;
}



int WidgetHandler::GetMouseX() {

   WidgetHandler* handler = dynamic_cast<WidgetHandler*>(wparent);

   int mx = mouse_x;

   int absx = InnerArea().X();

   if (handler) {
      mx = handler->GetMouseX();
   }
   else if (wparent) {
      absx += AbsParentX();
   }

   const int cx = cam.ViewX();

   return (mx - absx) + cx;
}



int WidgetHandler::GetMouseY() {

   WidgetHandler* handler = dynamic_cast<WidgetHandler*>(wparent);

   int my = mouse_y;

   int absy = InnerArea().Y();

   if (handler) {
      my = handler->GetMouseY();
   }
   else if (wparent) {
      absy += AbsParentY();
   }

   const int cy = cam.ViewY();

   return (my - absy) + cy;
}



Pos2I WidgetHandler::ViewPos() const {
   return Pos2I(cam.ViewX() , cam.ViewY());
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



void WidgetHandler::AllowMiddleMouseButtonScroll(bool allow) {
   cam.AllowMiddleMouseButtonScroll(allow);
}



std::ostream& WidgetHandler::DescribeTo(std::ostream& os , Indenter indent) const {
   os << indent << "WidgetHandler class object [" << FullName() << "] {" << endl;
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
   os << StringPrintF("user_bg_ptr = %p (" , (void*)user_bg_ptr);
   if (user_bg_ptr) {
      user_bg_ptr->DescribeTo(os);
   }
   os << ")" << endl;
   os << indent << StringPrintF("wfocus = %p (%s) , whover = %p (%s)" ,
                                 (void*)wfocus , wfocus?wfocus->FullName():"" , (void*)whover , whover?whover->FullName():"") << endl;
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



void WidgetHandler::PrintDrawList(std::ostream& os) {
   os << std::endl;
   for (int i = 0 ; i < (int)drawlist.size() ; i++) {
      WidgetBase* w = drawlist[i];
      std::string s = StringPrintF("DrawWidget #%02d = %s\n" , i , w->FullName());
      os << s << std::endl;
   }
   os << std::endl;
}









