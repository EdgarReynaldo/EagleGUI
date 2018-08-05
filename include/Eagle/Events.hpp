
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
#include <deque>
#include <vector>
#include <string>


#include "Eagle/Gui/WidgetMessage.hpp"

#include "Eagle/Mutexes.hpp"
#include "Eagle/Exception.hpp"
#include "Eagle/Conditions.hpp"
#include "Eagle/Object.hpp"


class EagleGraphicsContext;

enum EAGLE_EVENT_TYPE {
   EAGLE_EVENT_NONE                        =  0,

   EAGLE_EVENT_JOYSTICK_EVENT_START        =  1,

   EAGLE_EVENT_JOYSTICK_AXIS               =  1,
   EAGLE_EVENT_JOYSTICK_BUTTON_DOWN        =  2,
   EAGLE_EVENT_JOYSTICK_BUTTON_UP          =  3,
   EAGLE_EVENT_JOYSTICK_CONFIGURATION      =  4,

   EAGLE_EVENT_JOYSTICK_EVENT_STOP         =  4,

   EAGLE_EVENT_KEYBOARD_EVENT_START        = 10,

   EAGLE_EVENT_KEY_DOWN                    = 10,
   EAGLE_EVENT_KEY_CHAR                    = 11,
   EAGLE_EVENT_KEY_UP                      = 12,

   EAGLE_EVENT_KEYBOARD_EVENT_STOP         = 12,

   EAGLE_EVENT_MOUSE_EVENT_START           = 20,

   EAGLE_EVENT_MOUSE_AXES                  = 20,
   EAGLE_EVENT_MOUSE_BUTTON_DOWN           = 21,
   EAGLE_EVENT_MOUSE_BUTTON_UP             = 22,
   EAGLE_EVENT_MOUSE_ENTER_DISPLAY         = 23,
   EAGLE_EVENT_MOUSE_LEAVE_DISPLAY         = 24,
   EAGLE_EVENT_MOUSE_WARPED                = 25,

   EAGLE_EVENT_MOUSE_EVENT_STOP            = 25,

   EAGLE_EVENT_TIMER_EVENT_START           = 30,

   EAGLE_EVENT_TIMER                       = 30,

   EAGLE_EVENT_TIMER_EVENT_STOP            = 30,

   EAGLE_EVENT_DISPLAY_EVENT_START         = 37,

   EAGLE_EVENT_DISPLAY_CREATE              = 37,/// No matching allegro event
   EAGLE_EVENT_DISPLAY_DESTROY             = 38,/// No matching allegro event
   EAGLE_EVENT_DISPLAY_HALT_DRAWING_ACKNOWLEDGED = 39,/// No matching allegro event

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

   EAGLE_EVENT_DISPLAY_EVENT_STOP          = 49,

   EAGLE_EVENT_TOUCH_EVENT_START           = 50,

   EAGLE_EVENT_TOUCH_BEGIN                 = 50,
   EAGLE_EVENT_TOUCH_END                   = 51,
   EAGLE_EVENT_TOUCH_MOVE                  = 52,
   EAGLE_EVENT_TOUCH_CANCEL                = 53,

   EAGLE_EVENT_TOUCH_EVENT_STOP            = 53,

   EAGLE_EVENT_ANIMATION_LOOP_COMPLETE     = 60,
   EAGLE_EVENT_ANIMATION_COMPLETE          = 61,
   
   EAGLE_EVENT_WIDGET_EVENT_START          = 70,
   EAGLE_EVENT_WIDGET                      = 70,
   EAGLE_EVENT_WIDGET_EVENT_STOP           = 70,

   EAGLE_EVENT_USER_START                  = 1024
};


std::string EagleEventName(int event_num);


int NextFreeEagleEventId();



struct KEYBOARD_EVENT_DATA {
   int keycode;
   int unicode;
   int modifiers;
   bool repeat;

   KEYBOARD_EVENT_DATA() :
         keycode(-1),
         unicode(-1),
         modifiers(-1),
         repeat(false)
   {}
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

   MOUSE_EVENT_DATA() :
         x(-1),
         y(-1),
         z(-1),
         w(-1),
         dx(0),
         dy(0),
         dz(0),
         dw(0),
         button(-1)
   {}
};

class EagleJoystickData;

struct JOYSTICK_EVENT_DATA {
   EagleJoystickData* id;
   int nid;
   int stick;
   int axis;
   int button;
   float pos;

   JOYSTICK_EVENT_DATA() :
         id(0),
         nid(-1),
         stick(-1),
         axis(-1),
         button(-1),
         pos(-1.0f)
   {}
};

struct TOUCH_EVENT_DATA {
   int id;
   float x;
   float y;
   float dx;
   float dy;
   bool primary;

   TOUCH_EVENT_DATA() :
         id(-1),
         x(-1.0f),
         y(-1.0f),
         dx(0.0f),
         dy(0.0f),
         primary(false)
   {}
};

class EagleTimer;

struct TIMER_EVENT_DATA {
   EagleTimer* eagle_timer_source;
   void* raw_source;
   long long int count;

   TIMER_EVENT_DATA() :
         eagle_timer_source(0),
         raw_source(0),
         count(-1)
   {}
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
   int x;
   int y;
   int width;
   int height;
   int orientation;// an EAGLE_DISPLAY_ORIENTATION

   DISPLAY_EVENT_DATA() :
         x(-1),
         y(-1),
         width(0),
         height(0),
         orientation(EAGLE_DISPLAY_ORIENTATION_0_DEGREES)
   {}
};


class AnimationBase;

struct ANIMATION_EVENT_DATA {
   AnimationBase* source;
   int loops_complete;
   bool animation_complete;
   
   ANIMATION_EVENT_DATA() :
         source(0),
         loops_complete(-1),
         animation_complete(false)
   {}
};


enum AVSTATE {

};

class EagleAudio;

struct AUDIO_EVENT_DATA {
   EagleAudio* audio_source;
   AVSTATE avstate;
};

class EagleVideo;

struct VIDEO_EVENT_DATA {
   EagleVideo* video_source;
   AVSTATE avstate;
};

class WidgetBase;

struct WIDGET_EVENT_DATA {
   WidgetBase* from;
   unsigned int topic;
   int msgs;

   WIDGET_EVENT_DATA() :
         from(0),
         topic(TOPIC_NONE),
         msgs(0)
   {}
};

struct USER_EVENT_DATA {
   int type;
   void* userdata;

   USER_EVENT_DATA() :
         type(EAGLE_EVENT_USER_START),
         userdata(0)
   {}
};


class EagleEventSource;

class EagleEvent {

public :

   int type;

   EagleEventSource* source;

   EagleGraphicsContext* window;

   double timestamp;// In seconds since program started
   union {
      KEYBOARD_EVENT_DATA keyboard;// keycode display unicode modifiers repeat
      MOUSE_EVENT_DATA mouse;// x y z w dx dy dz dw button display
      JOYSTICK_EVENT_DATA joystick;// id stick axis button pos
      TOUCH_EVENT_DATA touch;// display id x y dx dy primary
      TIMER_EVENT_DATA timer;// source count
      DISPLAY_EVENT_DATA display;// source x y width height orientation
      ANIMATION_EVENT_DATA animation;
      AUDIO_EVENT_DATA audio;
      VIDEO_EVENT_DATA video;
      WIDGET_EVENT_DATA widget;
      USER_EVENT_DATA data;
   };

//   EagleEvent();
   EagleEvent() :
         type(EAGLE_EVENT_NONE),
         source(0),
         window(0),
         timestamp(-1.0)
   {}

};



/// Bitfield describing event type
enum EAGLE_EVENT_GROUP_TYPE {
   EAGLE_EVENT_TYPE_NONE      = 0,
   EAGLE_MOUSE_EVENT_TYPE     = 1 << 0,
   EAGLE_KEYBOARD_EVENT_TYPE  = 1 << 1,
   EAGLE_JOYSTICK_EVENT_TYPE  = 1 << 2,
   EAGLE_TOUCH_EVENT_TYPE     = 1 << 3,
   EAGLE_DISPLAY_EVENT_TYPE   = 1 << 4,
   EAGLE_WIDGET_EVENT_TYPE    = 1 << 5,
   EAGLE_SYSTEM_EVENT_TYPE    = 1 << 6,
   EAGLE_USER_EVENT_TYPE      = 1 << 7,
   EAGLE_ANY_EVENT_TYPE       = 1 << 8,
   EAGLE_UNDEFINED_EVENT_TYPE = 1 << 9
};



bool IsMouseEvent(EagleEvent e);
bool IsKeyboardEvent(EagleEvent e);
bool IsJoystickEvent(EagleEvent e);
bool IsTouchEvent(EagleEvent e);
bool IsDisplayEvent(EagleEvent e);
bool IsWidgetEvent(EagleEvent e);

bool IsSystemEvent(EagleEvent e);
bool IsUserEvent(EagleEvent e);



int EagleEventGroupType(EagleEvent e);


EagleEvent MakeEagleEvent(WidgetMsg msg);






class EagleEventListener;



class EagleEventSource {

public :
   typedef std::vector<EagleEventListener*> LISTENERS;
   typedef LISTENERS::iterator LIT;

protected :
   LISTENERS listeners;

   LIT FindListener(EagleEventListener* l);

   friend class EagleEventListener;
   
   virtual void SubscribeListener(EagleEventListener* l);
   virtual void UnsubscribeListener(EagleEventListener* l);

   virtual void StopBroadcasting();
   
public :

   EagleEventSource();
   virtual ~EagleEventSource();

   virtual void EmitEvent(EagleEvent e , EagleThread* thread);

   bool HasListeners() {return !listeners.empty();}

   std::vector<EagleEventListener*> Listeners();

};


class EagleEventListener {

private :
   
   typedef std::vector<EagleEventSource*> SOURCES;
   typedef SOURCES::iterator SIT;
   
   SOURCES sources;

   SIT FindSource(EagleEventSource* s);

protected :
   void StopListening();
   
public :
   EagleEventListener();
   virtual ~EagleEventListener();

   virtual void RespondToEvent(EagleEvent e , EagleThread* thread = 0)=0;

   virtual void ListenTo(EagleEventSource* s);
   virtual void StopListeningTo(EagleEventSource* s);
   
   bool HasSources() {return sources.size();}
};


/// Abstract base class!
class EagleEventHandler : public EagleObject , public EagleEventListener , public EagleEventSource {

protected :
   std::deque<EagleEvent> queue;
   EagleMutex* mutex;
   EagleConditionVar* cond_var;
   bool emitter_delay;/// To decide whether events are emitted immediately upon receipt,
                      /// or whether they are emitted as they are taken off the queue
   
   EagleThread* our_thread;
   
   
   
   void SetOurThread(EagleThread* t);
   
   void StopHandlingEvents();
   
///   void LockOurMutex(EagleThread* t);
///   void UnLockOurMutex(EagleThread* t);

   /// EagleEventSource

   void SubscribeListener(EagleEventListener* l);
   void UnsubscribeListener(EagleEventListener* l);

   virtual void StopBroadcasting();

   /// EagleEventListener
   
public :
   EagleEventHandler(std::string objclass , std::string objname , bool delay_emitted_events = true);
   virtual ~EagleEventHandler() {}

   virtual bool Create()=0;
   virtual void Destroy()=0;
   virtual bool Valid()=0;
   
   /// EagleEventSource
   void EmitEvent(EagleEvent e , EagleThread* thread);

   /// EagleEventListener

   void RespondToEvent(EagleEvent e , EagleThread* thread);

   void ListenTo(EagleEventSource* s);
   void StopListeningTo(EagleEventSource* s);

   void ListenTo(EagleEventSource* s , EagleThread* t);
   void StopListeningTo(EagleEventSource* s , EagleThread* t);


   /// EagleEventHandler
   void Clear(EagleThread* thread);

   void PushEvent(EagleEvent e , EagleThread* thread);

   bool HasEvent(EagleThread* thread);
   EagleEvent TakeNextEvent(EagleThread* thread);
   EagleEvent PeekNextEvent(EagleThread* thread);

   void InsertEventFront(EagleEvent e , EagleThread* thread);/// Does not EmitEvent...merely adds the event to the front of the queue
                                       /// Allows you to 'put back' an event

   std::vector<EagleEvent> FilterEvents(EAGLE_EVENT_TYPE etype , EagleThread* thread);
   std::vector<EagleEvent> FilterEvents(EagleEventSource* esrc , EagleThread* thread);
   std::vector<EagleEvent> FilterEvents(EAGLE_EVENT_TYPE etype , EagleEventSource* esrc , EagleThread* thread);

   EagleEvent WaitForEvent(EagleThread* thread);
   EagleEvent WaitForEvent(double timeout , EagleThread* thread);
   EagleEvent WaitForEvent(EAGLE_EVENT_TYPE type , EagleThread* thread);

///   EagleEventHandler* CloneEventHandler();TODO : Make this abstract and virtual and move it to Allegro5EventHandler
};



#endif // EagleEvents_HPP

