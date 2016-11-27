
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



#ifndef EagleEvents_HPP
#define EagleEvents_HPP

#include <list>
#include <vector>
#include <string>


#include "Eagle/Gui/WidgetMessage.hpp"

#include "Eagle/Mutexes.hpp"
#include "Eagle/Exception.hpp"




class EagleGraphicsContext;

enum EAGLE_EVENT_TYPE {
   EAGLE_EVENT_NONE                        =  0,
   
   EAGLE_EVENT_JOYSTICK_AXIS               =  1,
   EAGLE_EVENT_JOYSTICK_BUTTON_DOWN        =  2,
   EAGLE_EVENT_JOYSTICK_BUTTON_UP          =  3,
   EAGLE_EVENT_JOYSTICK_CONFIGURATION      =  4,

   EAGLE_EVENT_KEY_DOWN                    = 10,
   EAGLE_EVENT_KEY_CHAR                    = 11,
   EAGLE_EVENT_KEY_UP                      = 12,

   EAGLE_EVENT_MOUSE_AXES                  = 20,
   EAGLE_EVENT_MOUSE_BUTTON_DOWN           = 21,
   EAGLE_EVENT_MOUSE_BUTTON_UP             = 22,
   EAGLE_EVENT_MOUSE_ENTER_DISPLAY         = 23,
   EAGLE_EVENT_MOUSE_LEAVE_DISPLAY         = 24,
   EAGLE_EVENT_MOUSE_WARPED                = 25,

   EAGLE_EVENT_TIMER                       = 30,

   EAGLE_EVENT_DISPLAY_EXPOSE              = 40,
   EAGLE_EVENT_DISPLAY_RESIZE              = 41,
   EAGLE_EVENT_DISPLAY_CLOSE               = 42,
   EAGLE_EVENT_DISPLAY_LOST                = 43,
   EAGLE_EVENT_DISPLAY_FOUND               = 44,
   EAGLE_EVENT_DISPLAY_SWITCH_IN           = 45,
   EAGLE_EVENT_DISPLAY_SWITCH_OUT          = 46,
   EAGLE_EVENT_DISPLAY_ORIENTATION         = 47,
   EAGLE_EVENT_DISPLAY_HALT_DRAWING        = 48,
   EAGLE_EVENT_DISPLAY_RESUME_DRAWING      = 49,

   EAGLE_EVENT_TOUCH_BEGIN                 = 50,
   EAGLE_EVENT_TOUCH_END                   = 51,
   EAGLE_EVENT_TOUCH_MOVE                  = 52,
   EAGLE_EVENT_TOUCH_CANCEL                = 53,

   EAGLE_EVENT_WIDGET                      = 60,

   EAGLE_EVENT_USER_START                  = 1024

};


std::string EagleEventName(int event_num);


int NextFreeEagleEventId();



struct KEYBOARD_EVENT_DATA {
   int keycode;
   EagleGraphicsContext* display;
   int unicode;
   int modifiers;
   bool repeat;
   
};

struct MOUSE_EVENT_DATA {
   int x;
   int y;
   int z;
   int w;
   int dx;
   int dy;
   int dz;
   int dw;
   unsigned int button;// numbers from 1,2,3...
   EagleGraphicsContext* display;
};

class EagleJoystickData;

struct JOYSTICK_EVENT_DATA {
   EagleJoystickData* id;
   int nid;
   int stick;
   int axis;
   int button;
   float pos;
};

struct TOUCH_EVENT_DATA {
   EagleGraphicsContext* display;
   int id;
   float x;
   float y;
   float dx;
   float dy;
   bool primary;
};

class EagleTimer;

struct TIMER_EVENT_DATA {
   EagleTimer* eagle_timer_source;
   void* raw_source;
   long long int count;
};

enum EAGLE_DISPLAY_ORIENTATION {
    EAGLE_DISPLAY_ORIENTATION_0_DEGREES = 0,
    EAGLE_DISPLAY_ORIENTATION_90_DEGREES,
    EAGLE_DISPLAY_ORIENTATION_180_DEGREES,
    EAGLE_DISPLAY_ORIENTATION_270_DEGREES,
    EAGLE_DISPLAY_ORIENTATION_FACE_UP,
    EAGLE_DISPLAY_ORIENTATION_FACE_DOWN
};

struct DISPLAY_EVENT_DATA {
   EagleGraphicsContext* source;
   int x;
   int y;
   int width;
   int height;
   int orientation;// an EAGLE_DISPLAY_ORIENTATION
};

struct WIDGET_EVENT_DATA {
   WidgetBase* from;
   unsigned int topic;
   int msgs;
};

struct USER_EVENT_DATA {
   int type;
   void* userdata;
};


class EagleEventSource;

class EagleEvent {

public :

   EAGLE_EVENT_TYPE type;
   EagleEventSource* source;
   double timestamp;// In seconds since program started
   union {
      KEYBOARD_EVENT_DATA keyboard;// keycode display unicode modifiers repeat
      MOUSE_EVENT_DATA mouse;// x y z w dx dy dz dw button display
      JOYSTICK_EVENT_DATA joystick;// id stick axis button pos
      TOUCH_EVENT_DATA touch;// display id x y dx dy primary
      TIMER_EVENT_DATA timer;// source count
      DISPLAY_EVENT_DATA display;// source x y width height orientation
      WIDGET_EVENT_DATA widget;
      USER_EVENT_DATA data;
   };
   
//   EagleEvent();
   EagleEvent() : 
         type(EAGLE_EVENT_NONE),
         source(0),
         timestamp(-1.0)
   {}

};


bool IsMouseEvent(EagleEvent e);
bool IsKeyboardEvent(EagleEvent e);
bool IsJoystickEvent(EagleEvent e);
bool IsTouchEvent(EagleEvent e);
bool IsDisplayEvent(EagleEvent e);




class EagleEventListener;



class EagleEventSource {

private :
   std::vector<EagleEventListener*> listeners;

   bool OnList(EagleEventListener* l);

public :
   EagleEventSource() : listeners() {}
   virtual ~EagleEventSource();
   
   void EmitEvent(EagleEvent e);
   
   void SubscribeListener(EagleEventListener* l);
   void UnsubscribeListener(EagleEventListener* l);
   bool HasListeners() {return !listeners.empty();}
   
   virtual void ReadEvents()=0;/// TODO : Has this been implemented in any subclasses? What was it for again? Polling?
};


class EagleEventListener {

private :
   std::vector<EagleEventSource*> sources;
   
   bool OnList(EagleEventSource* s);

public :
   EagleEventListener() : sources() {}
   virtual ~EagleEventListener();
   
   virtual void RespondToEvent(EagleEvent e)=0;
   
   void ListenTo(EagleEventSource* s);
   void StopListeningTo(EagleEventSource* s);
   
   bool HasSources() {return !sources.empty();}
   void CheckSources();
};



/// Abstract base class!
class EagleEventHandler : public EagleEventListener , public EagleEventSource {
protected :
   std::list<EagleEvent> queue;
   EagleMutex* mutex;
   bool emitter_delay;

   virtual EagleEvent PrivateWaitForEvent()=0;
   virtual EagleEvent PrivateWaitForEvent(double timeout)=0;
   virtual void PrivateRefreshQueue()=0;


public :
   EagleEventHandler(bool delay_emitted_events = true);
   virtual ~EagleEventHandler() {}
   
   virtual bool Create()=0;
   virtual void Destroy()=0;
   virtual bool Valid()=0;

   /// EagleEventSource
   void ReadEvents();

   /// EagleEventListener
   virtual void RespondToEvent(EagleEvent e);
   
///   void Clear();
   void Clear() {
      EAGLE_ASSERT(mutex);
      mutex->Lock();
      queue.clear();
      mutex->Unlock();
   }

   void PushEvent(EagleEvent e);

   bool HasEvent();
   EagleEvent TakeNextEvent();
   EagleEvent PeekNextEvent();
   
   std::vector<EagleEvent> FilterEvents(EAGLE_EVENT_TYPE etype);
   std::vector<EagleEvent> FilterEvents(EagleEventSource* esrc);
   std::vector<EagleEvent> FilterEvents(EAGLE_EVENT_TYPE etype , EagleEventSource* esrc);
   
   EagleEvent WaitForEvent();
   EagleEvent WaitForEvent(double timeout);
   EagleEvent WaitForEvent(EAGLE_EVENT_TYPE t);

   void RefreshQueue();


///   virtual void SubscribeToAllAvailableSources()=0;/// Should be system function
};



#endif // EagleEvents_HPP

