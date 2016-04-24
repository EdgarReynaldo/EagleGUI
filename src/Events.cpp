
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
#include "Eagle/Error.hpp"


/// ------------------     GLOBALS     ----------------------------


std::string EagleEventName(int event_num) {

   int n = event_num;
   std::string s;

   if (n >= 512) {s += StringPrintF("EAGLE_EVENT_USER_START + %d" , event_num - 512);}
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
   return (e.type == EAGLE_EVENT_MOUSE_AXES ||
           e.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN ||
           e.type == EAGLE_EVENT_MOUSE_BUTTON_UP ||
           e.type == EAGLE_EVENT_MOUSE_ENTER_DISPLAY ||
           e.type == EAGLE_EVENT_MOUSE_LEAVE_DISPLAY ||
           e.type == EAGLE_EVENT_MOUSE_WARPED);
}



bool IsKeyboardEvent(EagleEvent e) {
   return (e.type == EAGLE_EVENT_KEY_DOWN ||
           e.type == EAGLE_EVENT_KEY_UP ||
           e.type == EAGLE_EVENT_KEY_CHAR);
}



bool IsJoystickEvent(EagleEvent e) {

   return (e.type == EAGLE_EVENT_JOYSTICK_AXIS ||
           e.type == EAGLE_EVENT_JOYSTICK_BUTTON_DOWN ||
           e.type == EAGLE_EVENT_JOYSTICK_BUTTON_UP ||
           e.type == EAGLE_EVENT_JOYSTICK_CONFIGURATION);
   
}



bool IsTouchEvent(EagleEvent e) {
   return (e.type == EAGLE_EVENT_TOUCH_BEGIN ||
           e.type == EAGLE_EVENT_TOUCH_END ||
           e.type == EAGLE_EVENT_TOUCH_MOVE ||
           e.type == EAGLE_EVENT_TOUCH_CANCEL);
}



bool IsDisplayEvent(EagleEvent e) {

   return (e.type == EAGLE_EVENT_DISPLAY_EXPOSE ||
           e.type == EAGLE_EVENT_DISPLAY_RESIZE ||
           e.type == EAGLE_EVENT_DISPLAY_CLOSE ||
           e.type == EAGLE_EVENT_DISPLAY_LOST ||
           e.type == EAGLE_EVENT_DISPLAY_FOUND ||
           e.type == EAGLE_EVENT_DISPLAY_SWITCH_IN ||
           e.type == EAGLE_EVENT_DISPLAY_SWITCH_OUT ||
           e.type == EAGLE_EVENT_DISPLAY_ORIENTATION ||
           e.type == EAGLE_EVENT_DISPLAY_HALT_DRAWING ||
           e.type == EAGLE_EVENT_DISPLAY_RESUME_DRAWING);
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



EagleEventSource::~EagleEventSource() {
   for (std::vector<EagleEventListener*>::iterator it = listeners.begin() ; it != listeners.end() ; ) {
      EagleEventListener* l = *it;
      it = listeners.erase(it);
      l->StopListeningTo(this);
   }
}



void EagleEventSource::EmitEvent(EagleEvent e) {
   for (unsigned int n = 0 ; n < listeners.size(); ++n) {
      EagleEventListener* l = listeners[n];
      l->RespondToEvent(e);
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



void EagleEventListener::CheckSources() {
   for (unsigned int i = 0 ; i < sources.size() ; ++i) {
      sources[i]->ReadEvents();
   }
}



/// ---------------------     EagleEventHandler     -------------------------------



EagleEventHandler::EagleEventHandler(bool delay_emitted_events) : 
      EagleEventListener(),
      EagleEventSource(),
      queue(),
      mutex(0),
      emitter_delay(delay_emitted_events)
{}



void EagleEventHandler::ReadEvents() {
   RefreshQueue();
}



void EagleEventHandler::RespondToEvent(EagleEvent e) {
   
   EAGLE_ASSERT(mutex);
   
   
   if (!emitter_delay && HasListeners()) {
      EmitEvent(e);// notify our listeners to deal with it...
   }
   
   mutex->Lock();
   queue.push_back(e);// and store the event ourselves...
   mutex->Unlock();
   
}



void EagleEventHandler::PushEvent(EagleEvent e) {
   RespondToEvent(e);
}



bool EagleEventHandler::HasEvent() {
   EAGLE_ASSERT(Valid());
   
   RefreshQueue();
   
   bool has_event = false;
   mutex->Lock();
   has_event = !queue.empty();
   mutex->Unlock();
   return has_event;
   
}



EagleEvent EagleEventHandler::TakeNextEvent() {
   EAGLE_ASSERT(Valid());
   
   RefreshQueue();
   
   EagleEvent e;
   
   mutex->Lock();
   if (!queue.empty()) {
      e = queue.front();
      queue.pop_front();
   }
   mutex->Unlock();
   if (emitter_delay) {
      EmitEvent(e);
   }
   return e;
}



EagleEvent EagleEventHandler::PeekNextEvent() {
   EAGLE_ASSERT(Valid());
   
   RefreshQueue();
   
   EagleEvent e;
   mutex->Lock();
   if (!queue.empty()) {e = queue.front();}
   mutex->Unlock();
   return e;
}



std::vector<EagleEvent> EagleEventHandler::FilterEvents(EAGLE_EVENT_TYPE etype) {
   std::vector<EagleEvent> events;
   mutex->Lock();
   for (std::list<EagleEvent>::iterator it = queue.begin() ; it != queue.end() ; ) {
      EagleEvent e = *it;
      if (e.type == etype) {
         events.push_back(e);
         it = queue.erase(it);
      }
      else {
         ++it;
      }
   }
   mutex->Unlock();
   return events;
}



std::vector<EagleEvent> EagleEventHandler::FilterEvents(EagleEventSource* esrc) {
   std::vector<EagleEvent> events;
   mutex->Lock();
   for (std::list<EagleEvent>::iterator it = queue.begin() ; it != queue.end() ; ) {
      EagleEvent e = *it;
      if (e.source == esrc) {
         events.push_back(e);
         it = queue.erase(it);
      }
      else {
         ++it;
      }
   }
   mutex->Unlock();
   return events;
}



std::vector<EagleEvent> EagleEventHandler::FilterEvents(EAGLE_EVENT_TYPE etype , EagleEventSource* esrc) {
   std::vector<EagleEvent> events;
   mutex->Lock();
   for (std::list<EagleEvent>::iterator it = queue.begin() ; it != queue.end() ; ) {
      EagleEvent e = *it;
      if (e.type == etype && e.source == esrc) {
         events.push_back(e);
         it = queue.erase(it);
      }
      else {
         ++it;
      }
   }
   mutex->Unlock();
   return events;
}



EagleEvent EagleEventHandler::WaitForEvent() {
   EAGLE_ASSERT(Valid());
   
   RefreshQueue();
   
   EagleEvent e;
   mutex->Lock();
   if (!queue.empty()) {
      // eagle event in queue
      e = queue.front();
      queue.pop_front();
      mutex->Unlock();
      if (emitter_delay) {
         EmitEvent(e);
      }
      return e;
   }
   mutex->Unlock();
   // wait for event from event thread saying there is a message in the queue
   e = PrivateWaitForEvent();
   if (emitter_delay) {
      EmitEvent(e);
   }
   return e;
}



EagleEvent EagleEventHandler::WaitForEvent(double timeout) {
   EAGLE_ASSERT(Valid());
   
   RefreshQueue();
   
   EagleEvent e;
   mutex->Lock();
   if (!queue.empty()) {
      // eagle event in queue
      e = queue.front();
      queue.pop_front();
      mutex->Unlock();
      return e;
   }
   mutex->Unlock();
   // wait for event from event thread saying there is a message in the queue
   return PrivateWaitForEvent(timeout);
}



void EagleEventHandler::RefreshQueue() {
   PrivateRefreshQueue();
   CheckSources();
}



