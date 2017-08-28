
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



#include "Eagle/Events.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/Exception.hpp"


/// ------------------     GLOBALS     ----------------------------


std::string EagleEventName(int event_num) {

   /// TODO : Rework to use an array or a map or something faster

   int n = event_num;
   std::string s;

   if (n >= EAGLE_EVENT_USER_START) {s += StringPrintF("EAGLE_EVENT_USER_START + %d" , event_num - (int)EAGLE_EVENT_USER_START);}
   else if (n == EAGLE_EVENT_NONE)                   {s = "EAGLE_EVENT_NONE";}
   else if (n == EAGLE_EVENT_JOYSTICK_AXIS)          {s = "EAGLE_EVENT_JOYSTICK_AXIS";}
   else if (n == EAGLE_EVENT_JOYSTICK_BUTTON_DOWN)   {s = "EAGLE_EVENT_JOYSTICK_BUTTON_DOWN";}
   else if (n == EAGLE_EVENT_JOYSTICK_BUTTON_UP)     {s = "EAGLE_EVENT_JOYSTICK_BUTTON_UP";}
   else if (n == EAGLE_EVENT_JOYSTICK_CONFIGURATION) {s = "EAGLE_EVENT_JOYSTICK_CONFIGURATION";}
   else if (n == EAGLE_EVENT_KEY_DOWN)               {s = "EAGLE_EVENT_KEY_DOWN";}
   else if (n == EAGLE_EVENT_KEY_CHAR)               {s = "EAGLE_EVENT_KEY_CHAR";}
   else if (n == EAGLE_EVENT_KEY_UP)                 {s = "EAGLE_EVENT_KEY_UP";}
   else if (n == EAGLE_EVENT_MOUSE_AXES)             {s = "EAGLE_EVENT_MOUSE_AXES";}
   else if (n == EAGLE_EVENT_MOUSE_BUTTON_DOWN)      {s = "EAGLE_EVENT_MOUSE_BUTTON_DOWN";}
   else if (n == EAGLE_EVENT_MOUSE_BUTTON_UP)        {s = "EAGLE_EVENT_MOUSE_BUTTON_UP";}
   else if (n == EAGLE_EVENT_MOUSE_ENTER_DISPLAY)    {s = "EAGLE_EVENT_MOUSE_ENTER_DISPLAY";}
   else if (n == EAGLE_EVENT_MOUSE_LEAVE_DISPLAY)    {s = "EAGLE_EVENT_MOUSE_LEAVE_DISPLAY";}
   else if (n == EAGLE_EVENT_MOUSE_WARPED)           {s = "EAGLE_EVENT_MOUSE_WARPED";}
   else if (n == EAGLE_EVENT_TIMER)                  {s = "EAGLE_EVENT_TIMER";}
   else if (n == EAGLE_EVENT_DISPLAY_CREATE)         {s = "EAGLE_EVENT_DISPLAY_CREATE";}
   else if (n == EAGLE_EVENT_DISPLAY_DESTROY)        {s = "EAGLE_EVENT_DISPLAY_DESTROY";}
   else if (n == EAGLE_EVENT_DISPLAY_EXPOSE)         {s = "EAGLE_EVENT_DISPLAY_EXPOSE";}
   else if (n == EAGLE_EVENT_DISPLAY_RESIZE)         {s = "EAGLE_EVENT_DISPLAY_RESIZE";}
   else if (n == EAGLE_EVENT_DISPLAY_CLOSE)          {s = "EAGLE_EVENT_DISPLAY_CLOSE";}
   else if (n == EAGLE_EVENT_DISPLAY_LOST)           {s = "EAGLE_EVENT_DISPLAY_LOST";}
   else if (n == EAGLE_EVENT_DISPLAY_FOUND)          {s = "EAGLE_EVENT_DISPLAY_FOUND";}
   else if (n == EAGLE_EVENT_DISPLAY_SWITCH_IN)      {s = "EAGLE_EVENT_DISPLAY_SWITCH_IN";}
   else if (n == EAGLE_EVENT_DISPLAY_SWITCH_OUT)     {s = "EAGLE_EVENT_DISPLAY_SWITCH_OUT";}
   else if (n == EAGLE_EVENT_DISPLAY_ORIENTATION)    {s = "EAGLE_EVENT_DISPLAY_ORIENTATION";}
   else if (n == EAGLE_EVENT_DISPLAY_HALT_DRAWING)   {s = "EAGLE_EVENT_DISPLAY_HALT_DRAWING";}
   else if (n == EAGLE_EVENT_DISPLAY_RESUME_DRAWING) {s = "EAGLE_EVENT_DISPLAY_RESUME_DRAWING";}
   else if (n == EAGLE_EVENT_TOUCH_BEGIN)            {s = "EAGLE_EVENT_TOUCH_BEGIN";}
   else if (n == EAGLE_EVENT_TOUCH_END)              {s = "EAGLE_EVENT_TOUCH_END";}
   else if (n == EAGLE_EVENT_TOUCH_MOVE)             {s = "EAGLE_EVENT_TOUCH_MOVE";}
   else if (n == EAGLE_EVENT_TOUCH_CANCEL)           {s = "EAGLE_EVENT_TOUCH_CANCEL";}
   else if (n == EAGLE_EVENT_WIDGET)                 {s = "EAGLE_EVENT_WIDGET";}
   else {
      s = "EAGLE_EVENT_UNDEFINED";
   }
   return s;
}



int NextFreeEagleEventId() {
   static int id = EAGLE_EVENT_USER_START;
   return id++;
}



bool IsMouseEvent(EagleEvent e) {
   return (e.type >= EAGLE_EVENT_MOUSE_EVENT_START && e.type <= EAGLE_EVENT_MOUSE_EVENT_STOP);
}



bool IsKeyboardEvent(EagleEvent e) {
   return (e.type >= EAGLE_EVENT_KEYBOARD_EVENT_START && e.type <= EAGLE_EVENT_KEYBOARD_EVENT_STOP);
}



bool IsJoystickEvent(EagleEvent e) {
   return (e.type >= EAGLE_EVENT_JOYSTICK_EVENT_START && e.type <= EAGLE_EVENT_JOYSTICK_EVENT_STOP);
}



bool IsTouchEvent(EagleEvent e) {
   return (e.type >= EAGLE_EVENT_TOUCH_EVENT_START && e.type <= EAGLE_EVENT_TOUCH_EVENT_STOP);
}



bool IsDisplayEvent(EagleEvent e) {
   return (e.type >= EAGLE_EVENT_DISPLAY_EVENT_START && e.type <= EAGLE_EVENT_DISPLAY_EVENT_STOP);
}



bool IsWidgetEvent(EagleEvent e) {
   return (e.type >= EAGLE_EVENT_WIDGET_EVENT_START && e.type <= EAGLE_EVENT_WIDGET_EVENT_STOP);
}



bool IsSystemEvent(EagleEvent e) {
   return (e.type >= EAGLE_EVENT_NONE && e.type < EAGLE_EVENT_USER_START);
}



bool IsUserEvent(EagleEvent e) {
   return (e.type >= EAGLE_EVENT_USER_START);
}



int EagleEventGroupType(EagleEvent e) {
   if (e.type == EAGLE_EVENT_NONE) {
      return EAGLE_EVENT_TYPE_NONE;
   }
   int type =( (IsMouseEvent(e)?EAGLE_MOUSE_EVENT_TYPE:0) |
               (IsKeyboardEvent(e)?EAGLE_KEYBOARD_EVENT_TYPE:0) |
               (IsJoystickEvent(e)?EAGLE_JOYSTICK_EVENT_TYPE:0) |
               (IsTouchEvent(e)?EAGLE_TOUCH_EVENT_TYPE:0) |
               (IsDisplayEvent(e)?EAGLE_DISPLAY_EVENT_TYPE:0) |
               (IsWidgetEvent(e)?EAGLE_WIDGET_EVENT_TYPE:0) |
               (IsSystemEvent(e)?EAGLE_SYSTEM_EVENT_TYPE:0) |
               (IsUserEvent(e)?EAGLE_USER_EVENT_TYPE:0) );
   if (type) {
      type |= EAGLE_ANY_EVENT_TYPE;
   }
   else {
      type = EAGLE_UNDEFINED_EVENT_TYPE;
   }
   return type;
}



/// -----------------------     EagleEventSource     -------------------------------



bool EagleEventSource::OnList(EagleEventListener* l) {
   for (unsigned int n = 0 ; n < listeners.size() ; ++n) {
      if (listeners[n] == l) {
         return true;
      }
   }
   return false;
}



void EagleEventSource::EmitEvent(EagleEvent e , EagleThread* thread) {
   for (unsigned int n = 0 ; n < listeners.size(); ++n) {
      EagleEventListener* l = listeners[n];
      l->RespondToEvent(e , thread);
   }
}



EagleEventSource::~EagleEventSource() {
   for (std::vector<EagleEventListener*>::iterator it = listeners.begin() ; it != listeners.end() ; ) {
      EagleEventListener* l = *it;
      it = listeners.erase(it);
      l->StopListeningTo(this);
   }
}



void EagleEventSource::SubscribeListener(EagleEventListener* l) {
   if (!l) {return;}
   if (!OnList(l)) {
      listeners.push_back(l);
   }
}



void EagleEventSource::UnsubscribeListener(EagleEventListener* l) {
   if (!l) {return;}
   for (std::vector<EagleEventListener*>::iterator it = listeners.begin() ; it != listeners.end() ; ) {
      if (l == *it) {
         it = listeners.erase(it);
      }
      else {
         ++it;
      }
   }
}



std::vector<EagleEventListener*> EagleEventSource::Listeners() {
   return listeners;
}



/// ---------------      EagleEventListener         --------------------------------------



bool EagleEventListener::OnList(EagleEventSource* s) {
   for (unsigned int n = 0 ; n < sources.size() ; ++n) {
      if (sources[n] == s) {
         return true;
      }
   }
   return false;
}



EagleEventListener::~EagleEventListener() {
   std::vector<EagleEventSource*> sources_copy = sources;
   for (unsigned int n = 0 ; n < sources_copy.size() ; ++n) {
      StopListeningTo(sources_copy[n]);
   }
}



void EagleEventListener::ListenTo(EagleEventSource* s) {
   if (!OnList(s)) {
      sources.push_back(s);
      s->SubscribeListener(this);
   }
}



void EagleEventListener::StopListeningTo(EagleEventSource* s) {
   if (OnList(s)) {
      for (std::vector<EagleEventSource*>::iterator it = sources.begin() ; it != sources.end() ; ) {
         if (*it == s) {
            it = sources.erase(it);
            s->UnsubscribeListener(this);
         }
         else {
            ++it;
         }
      }
   }
}


/**
void EagleEventListener::CheckSources() {
   for (unsigned int i = 0 ; i < sources.size() ; ++i) {
      sources[i]->ReadEvents();
   }
}
//*/


/// ---------------------     EagleEventHandler     -------------------------------



EagleEventHandler::EagleEventHandler(bool delay_emitted_events) :
      EagleObject(),
      EagleEventListener(),
      EagleEventSource(),
      queue(),
      mutex(0),
      cond_var(0),
      emitter_delay(delay_emitted_events),
      our_thread(0)
{
   SetName(StringPrintF("EagleEventHandler (EID = %d) at %p" , GetEagleId() , this));
}



void EagleEventHandler::SetOurThread(EagleThread* t) {
   our_thread = t;
}



void EagleEventHandler::RespondToEvent(EagleEvent e , EagleThread* thread) {

   EAGLE_ASSERT(mutex);

   /// Store the event
   ThreadLockMutex(thread , mutex);
   queue.push_back(e);
   ThreadUnLockMutex(thread , mutex);

   /// Wake any threads waiting on us
   cond_var->SignalCondition();

   /// Notify our listeners
   if (!emitter_delay && HasListeners()) {
      EmitEvent(e , thread);
   }
}



void EagleEventHandler::Clear(EagleThread* thread) {
   EAGLE_ASSERT(mutex);
   ThreadLockMutex(thread , mutex);
   queue.clear();
   ThreadUnLockMutex(thread , mutex);
}



void EagleEventHandler::PushEvent(EagleEvent e , EagleThread* thread) {
   RespondToEvent(e , thread);
}



bool EagleEventHandler::HasEvent(EagleThread* thread) {
   EAGLE_ASSERT(Valid());

   bool has_event = false;

   ThreadLockMutex(thread , mutex);

   has_event = !queue.empty();

   ThreadUnLockMutex(thread , mutex);

   return has_event;

}



EagleEvent EagleEventHandler::TakeNextEvent(EagleThread* thread) {
   EAGLE_ASSERT(Valid());

   EagleEvent e;

   ThreadLockMutex(thread , mutex);

   if (!queue.empty()) {
      e = queue.front();
      queue.pop_front();
   }

   ThreadUnLockMutex(thread , mutex);

   if (emitter_delay) {
      EmitEvent(e , thread);
   }

   return e;
}



EagleEvent EagleEventHandler::PeekNextEvent(EagleThread* thread) {
   EAGLE_ASSERT(Valid());

   EagleEvent e;
   
   ThreadLockMutex(thread , mutex);
   
   if (!queue.empty()) {e = queue.front();}
   
   ThreadUnLockMutex(thread , mutex);
   
   return e;
}



void EagleEventHandler::InsertEventFront(EagleEvent e , EagleThread* thread) {
   EAGLE_ASSERT(mutex);

   ThreadLockMutex(thread , mutex);

   queue.push_front(e);

   ThreadUnLockMutex(thread , mutex);
}



std::vector<EagleEvent> EagleEventHandler::FilterEvents(EAGLE_EVENT_TYPE etype , EagleThread* thread) {

   std::vector<EagleEvent> events;

   ThreadLockMutex(thread , mutex);

   for (std::deque<EagleEvent>::iterator it = queue.begin() ; it != queue.end() ; ) {
      EagleEvent e = *it;
      if (e.type == etype) {
         events.push_back(e);
         it = queue.erase(it);
      }
      else {
         ++it;
      }
   }

   ThreadUnLockMutex(thread , mutex);

   return events;
}



std::vector<EagleEvent> EagleEventHandler::FilterEvents(EagleEventSource* esrc , EagleThread* thread) {

   std::vector<EagleEvent> events;

   ThreadLockMutex(thread , mutex);

   for (std::deque<EagleEvent>::iterator it = queue.begin() ; it != queue.end() ; ) {
      EagleEvent e = *it;
      if (e.source == esrc) {
         events.push_back(e);
         it = queue.erase(it);
      }
      else {
         ++it;
      }
   }
   ThreadUnLockMutex(thread , mutex);
   
   return events;
}



std::vector<EagleEvent> EagleEventHandler::FilterEvents(EAGLE_EVENT_TYPE etype , EagleEventSource* esrc , EagleThread* thread) {
   
   std::vector<EagleEvent> events;
   
   ThreadLockMutex(thread , mutex);
   
   for (std::deque<EagleEvent>::iterator it = queue.begin() ; it != queue.end() ; ) {
      EagleEvent e = *it;
      if (e.type == etype && e.source == esrc) {
         events.push_back(e);
         it = queue.erase(it);
      }
      else {
         ++it;
      }
   }
   
   ThreadUnLockMutex(thread , mutex);
   
   return events;
}



EagleEvent EagleEventHandler::WaitForEvent(EagleThread* thread) {
   EAGLE_ASSERT(Valid());
   EAGLE_ASSERT(HasSources());

   EagleEvent e;

   ThreadLockMutex(thread , mutex);

   if (!queue.empty()) {
      // eagle event in queue
      e = queue.front();
      queue.pop_front();
      ThreadUnLockMutex(our_thread , mutex);
      if (emitter_delay) {
         EmitEvent(e , thread);
      }
      return e;
   }

   ThreadUnLockMutex(thread , mutex);

   /// wait for event from event thread saying there is a message in the queue
   cond_var->WaitForCondition();
   e = TakeNextEvent(thread);

   if (emitter_delay) {
      EmitEvent(e , thread);
   }
   return e;
}



EagleEvent EagleEventHandler::WaitForEvent(double timeout , EagleThread* thread) {
   EAGLE_ASSERT(Valid());
   EAGLE_ASSERT(HasSources());

   EagleEvent e;

   ThreadLockMutex(thread , mutex);

   if (!queue.empty()) {
      // eagle event in queue
      e = queue.front();
      queue.pop_front();
      ThreadUnLockMutex(thread , mutex);
      return e;
   }

   ThreadUnLockMutex(thread , mutex);

   // wait for event from event thread saying there is a message in the queue
   int ret = 0;
   ret = cond_var->WaitForConditionUntil(timeout);
   if (!ret) {
      /// signalled - there is an event now
      return TakeNextEvent(thread);
   }
   /// timed out, return default event
   return e;
}



EagleEvent EagleEventHandler::WaitForEvent(EAGLE_EVENT_TYPE type , EagleThread* thread) {
   EagleEvent ev;
   do {
      ev = WaitForEvent(thread);
   } while (ev.type != type);
   return ev;
}



/**
EagleEventHandler* EagleEventHandler::CloneEventHandler() {
   EAGLE_ASSERT(mutex);
   EagleEventHandler* new_queue = new EagleEventHandler();
   if (!new_queue->Create() || (new_queue && !new_queue->Valid())) {
      delete new_queue;
      return 0;
   }

   mutex->Lock();
   new_queue->queue = queue;
   for (int i = 0 ; i < (int)sources.size() ; ++i) {
      new_queue->ListenTo(sources[i]);
   }
   mutex->Unlock();

   return new_queue;
}
//*/



