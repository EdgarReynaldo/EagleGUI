
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
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Events.hpp
 * @brief The interface for the Eagle event system
 */

#ifndef EagleEvents_HPP
#define EagleEvents_HPP



#include <list>
#include <deque>
#include <vector>
#include <string>



#include "Eagle/Threads.hpp"
#include "Eagle/Mutexes.hpp"
#include "Eagle/Exception.hpp"
#include "Eagle/Conditions.hpp"
#include "Eagle/Object.hpp"
#include "Eagle/Gui/WidgetMessage.hpp"



class EagleGraphicsContext;


/**! @enum EAGLE_EVENT_TYPE
 *   @brief An enumeration to distinguish event types
 */

enum EAGLE_EVENT_TYPE {
   EAGLE_EVENT_NONE                        =  0,///< Used to signify there is no event, an empty event basically

   EAGLE_EVENT_JOYSTICK_EVENT_START        =  1,///< Start id for joystick events

   EAGLE_EVENT_JOYSTICK_AXIS               =  1,///< A joystick axis moved
   EAGLE_EVENT_JOYSTICK_BUTTON_DOWN        =  2,///< A joystick button was pressed
   EAGLE_EVENT_JOYSTICK_BUTTON_UP          =  3,///< A joystick button was released
   EAGLE_EVENT_JOYSTICK_CONFIGURATION      =  4,///< A joystick configuration event occurred, such as hotplugging a joystick

   EAGLE_EVENT_JOYSTICK_EVENT_STOP         =  4,///< End id for joystick events

   EAGLE_EVENT_KEYBOARD_EVENT_START        = 10,///< Start id for keyboard events

   EAGLE_EVENT_KEY_DOWN                    = 10,///< A key was pressed
   EAGLE_EVENT_KEY_CHAR                    = 11,///< A character was generated from the keyboard
   EAGLE_EVENT_KEY_UP                      = 12,///< A key was released

   EAGLE_EVENT_KEYBOARD_EVENT_STOP         = 12,///< End id for keyboard events

   EAGLE_EVENT_MOUSE_EVENT_START           = 20,///< Start id for mouse events

   EAGLE_EVENT_MOUSE_AXES                  = 20,///< The mouse was moved, or the scroll wheel(s) were moved
   EAGLE_EVENT_MOUSE_BUTTON_DOWN           = 21,///< A mouse button was pressed
   EAGLE_EVENT_MOUSE_BUTTON_UP             = 22,///< A mouse button was released
   EAGLE_EVENT_MOUSE_ENTER_DISPLAY         = 23,///< The mouse entered the display area
   EAGLE_EVENT_MOUSE_LEAVE_DISPLAY         = 24,///< The mouse left the display area
   EAGLE_EVENT_MOUSE_WARPED                = 25,///< The mouse was moved manually

   EAGLE_EVENT_MOUSE_EVENT_STOP            = 25,///< End id for mouse events

   EAGLE_EVENT_TIMER_EVENT_START           = 30,///< Start id for timer events

   EAGLE_EVENT_TIMER                       = 30,///< A timer ticked

   EAGLE_EVENT_TIMER_EVENT_STOP            = 30,///< End id for timer events

   EAGLE_EVENT_DISPLAY_EVENT_START         = 37,///< Start id for display events

   EAGLE_EVENT_DISPLAY_CREATE              = 37,///< A display was created. There is no matching allegro event
   EAGLE_EVENT_DISPLAY_DESTROY             = 38,///< A display was destroyed. There is no matching allegro event
   EAGLE_EVENT_DISPLAY_HALT_DRAWING_ACKNOWLEDGED = 39,///< The display has acknowledged a drawing halt. There is no matching allegro event

   EAGLE_EVENT_DISPLAY_EXPOSE              = 40,///< Part of the display was exposed
   EAGLE_EVENT_DISPLAY_RESIZE              = 41,///< The display was resized
   EAGLE_EVENT_DISPLAY_CLOSE               = 42,///< The close button was pressed
   EAGLE_EVENT_DISPLAY_LOST                = 43,///< The display was lost. Only occurs with the DX driver
   EAGLE_EVENT_DISPLAY_FOUND               = 44,///< The display was found. Only occurs with the DX driver
   EAGLE_EVENT_DISPLAY_SWITCH_IN           = 45,///< This display regained focus
   EAGLE_EVENT_DISPLAY_SWITCH_OUT          = 46,///< This display lost focus
   EAGLE_EVENT_DISPLAY_ORIENTATION         = 47,///< This display was re-oriented
   EAGLE_EVENT_DISPLAY_HALT_DRAWING        = 48,///< This display must not continue to draw
   EAGLE_EVENT_DISPLAY_RESUME_DRAWING      = 49,///< This display may resume drawing

   EAGLE_EVENT_DISPLAY_EVENT_STOP          = 49,///< End id for display events

   EAGLE_EVENT_TOUCH_EVENT_START           = 50,///< Start id for touch events

   EAGLE_EVENT_TOUCH_BEGIN                 = 50,///< A touch has begun
   EAGLE_EVENT_TOUCH_END                   = 51,///< A touch has completed
   EAGLE_EVENT_TOUCH_MOVE                  = 52,///< Signal for a move event during a touch
   EAGLE_EVENT_TOUCH_CANCEL                = 53,///< This touch was cancelled

   EAGLE_EVENT_TOUCH_EVENT_STOP            = 53,///< End id for touch events

   EAGLE_EVENT_ANIMATION_EVENT_START       = 60,///< Start id for animation events

   EAGLE_EVENT_ANIMATION_LOOP_COMPLETE     = 60,///< An animation loop completed

   EAGLE_EVENT_ANIMATION_COMPLETE          = 61,///< An animation completed

   EAGLE_EVENT_ANIMATION_EVENT_STOP        = 61,///< End id for animation events
   
   EAGLE_EVENT_WIDGET_EVENT_START          = 70,///< Start id for widget events

   EAGLE_EVENT_WIDGET                      = 70,///< This event came from a widget

   EAGLE_EVENT_WIDGET_DRAG_AND_DROP        = 71,///< This event came from a mover widget

   EAGLE_EVENT_WIDGET_EVENT_STOP           = 71,///< End id for widget events
   
   EAGLE_EVENT_NETWORK_EVENT_START         = 80,///< Start id for network events
   
   EAGLE_EVENT_NETWORK_RECV_MSG            = 80,///< This event is a network data received event
   
   EAGLE_EVENT_NETWORK_CONNECT             = 81,///< This event is a network connection event
   EAGLE_EVENT_NETWORK_DISCONNECT          = 82,///< This event is a network disconnect event
   EAGLE_EVENT_NETWORK_SERVERUP            = 83,///< This event means a server is running now
   EAGLE_EVENT_NETWORK_SERVERDOWN          = 84,///< This event means a server has shut down
   
   EAGLE_EVENT_NETWORK_EVENT_STOP          = 84,///< Stop id for network events

   EAGLE_EVENT_AUDIO_EVENT_START           = 100,
   
   EAGLE_EVENT_AUDIO_RECORDER_FRAGMENT     = 100,/// audio recorder fragment is ready
   EAGLE_EVENT_AUDIO_BUFFER_READY          = 101,/// audio buffer is ready to be filled
   EAGLE_EVENT_AUDIO_SAMPLE_COMPLETE       = 102,/// Sample has finished playing
   
   EAGLE_EVENT_AUDIO_EVENT_STOP            = 102,
   
   EAGLE_EVENT_VIDEO_EVENT_START           = 110,
   
   EAGLE_EVENT_VIDEO_AUDIO_BUFFER_READY    = 110,
   EAGLE_EVENT_VIDEO_FRAME_READY           = 111,
   EAGLE_EVENT_VIDEO_COMPLETE              = 112,
   
   EAGLE_EVENT_VIDEO_EVENT_STOP            = 112,
   
   
   EAGLE_EVENT_USER_START                  = 1024///< Event id's above this value are free to use for user events
};


std::string EagleEventName(int event_num);///< Returns a name string for an event id



int NextFreeEagleEventId();///< Returns the next available user event id


/**! @struct KEYBOARD_EVENT_DATA
 *   @brief Holds data for keyboard events
 */

struct KEYBOARD_EVENT_DATA {
   int keycode;///< EAGLE_KEY_XXX
   int unicode;///< UTF8
   int modifiers;///< Modifiers held
   bool repeat;///< If this is a keyboard repeat event

   KEYBOARD_EVENT_DATA() :
         keycode(-1),
         unicode(-1),
         modifiers(-1),
         repeat(false)
   {}
};


/**! @struct MOUSE_EVENT_DATA
 *   @brief Holds data for mouse events
 */

struct MOUSE_EVENT_DATA {
   int x;///< Mouse x relative to display
   int y;///< Mouse y relative to display
   int z;///< Mouse z for vertical scroll wheel
   int w;///< Mouse w for horizontal scroll wheel
   int dx;///< Delta mouse x value
   int dy;///< Delta mouse y value
   int dz;///< Delta mouse z value
   int dw;///< Delta mouse w value

   ///< numbers from 1,2,3.../// TODO : make these correspond to an enum like LMB,RMB,MMB,ETC
   unsigned int button;

   MOUSE_EVENT_DATA() :
         x(-1),
         y(-1),
         z(-1),
         w(-1),
         dx(0),
         dy(0),
         dz(0),
         dw(0),
         button(0)
   {}
};



class EagleJoystickData;

/**! @struct JOYSTICK_EVENT_DATA
 *   @brief Data for joystick events
 */

struct JOYSTICK_EVENT_DATA {
   EagleJoystickData* id;///< A pointer to joystick data
   int nid;///< Joystick id number
   int stick;///< Stick number
   int axis;///< Axis number
   int button;///< Button number
   float pos;///< Floating point position

   JOYSTICK_EVENT_DATA() :
         id(0),
         nid(-1),
         stick(-1),
         axis(-1),
         button(-1),
         pos(-1.0f)
   {}
};


/**! @struct TOUCH_EVENT_DATA
 *   @brief Holds data for touch events
 */

struct TOUCH_EVENT_DATA {
   int id;///< The id of the touch event, multiple touch events may come from the same id
   float x;///< The touch x value
   float y;///< The touch y value
   float dx;///< The delta x value for the touch
   float dy;///< The delta y value for the touch
   bool primary;///< What the hell is this? TODO : FIND OUT

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

/**! @struct TIMER_EVENT_DATA
 *   @brief The data for timer events
 */

struct TIMER_EVENT_DATA {
   EagleTimer* eagle_timer_source;///< The address of the timer that generated this event
   void* raw_source;///< The actual source of the event, such as an ALLEGRO_TIMER*, varies by system driver
   long long int count;///< The tick count for this timer

   TIMER_EVENT_DATA() :
         eagle_timer_source(0),
         raw_source(0),
         count(-1)
   {}
};


/**! @enum EAGLE_DISPLAY_ORIENTATION
 *   @brief An enumeration to define the different orientations a screen may take, such as on Android, iOS, etc...
 */

enum EAGLE_DISPLAY_ORIENTATION {
    EAGLE_DISPLAY_ORIENTATION_0_DEGREES   = 0,///< This display is upright
    EAGLE_DISPLAY_ORIENTATION_90_DEGREES  = 1,///< This display is tilted right 90 degrees
    EAGLE_DISPLAY_ORIENTATION_180_DEGREES = 2,///< This display is upside down
    EAGLE_DISPLAY_ORIENTATION_270_DEGREES = 4,///< This display is tilted left 90 degrees
    EAGLE_DISPLAY_ORIENTATION_FACE_UP     = 8,///< This display is face up
    EAGLE_DISPLAY_ORIENTATION_FACE_DOWN   = 16///< This display is face down
};


/**! @struct DISPLAY_EVENT_DATA
 *   @brief The data for display events
 */

struct DISPLAY_EVENT_DATA {
   int x;///< The screen x position of the display
   int y;///< The screen y position of the display
   int width;///< The current display width
   int height;///< The current display height
   int orientation;/// Bitfield of EAGLE_DISPLAY_ORIENTATION values - a rotation, and whether it is face up or down

   DISPLAY_EVENT_DATA() :
         x(-1),
         y(-1),
         width(0),
         height(0),
         orientation(EAGLE_DISPLAY_ORIENTATION_0_DEGREES | EAGLE_DISPLAY_ORIENTATION_FACE_UP)
   {}
};



class AnimationBase;

/**! @struct ANIMATION_EVENT_DATA
 *   @brief Data for animation events
 */

struct ANIMATION_EVENT_DATA {
   AnimationBase* source;///< The source of the animation
   int loops_complete;///< Number of loops complete
   bool animation_complete;///< Whether the full animation is complete
   
   ANIMATION_EVENT_DATA() :
         source(0),
         loops_complete(-1),
         animation_complete(false)
   {}
};

/**! @enum AVSTATE
 *   @brief Enumeration to describe the state of this audio video object
 */

enum AVSTATE {
   AV_STATE_NOTREADY    = -1,///< This audio video object is stopped
   AV_STATE_READY       =  0,///< This audio video object is ready to play
   AV_STATE_PLAYING     =  1,///< This audio video object is currently playing
   AV_STATE_PAUSED      =  2 ///< This audio video object is paused
};



class EagleSound;

/**! @struct AUDIO_EVENT_DATA
 *   @brief Data for audio events
 */

struct AUDIO_EVENT_DATA {
   EagleSound* sound_source;///< The audio source
   char* byte_buffer;
   int sample_count;
};



class EagleVideo;
class EagleImage;
/**! @struct VIDEO_EVENT_DATA
 *   @brief Data for video events
 */

struct VIDEO_EVENT_DATA {
   /// Video and audio are played separately
   EagleVideo* video_source;///< The video source
   EagleSound* sound_source;///< The audio source
   EagleImage** buffered_frames;
   int frame_buffer_count;
};



class WidgetBase;

/**! @struct WIDGET_EVENT_DATA
 *   @brief Data for widget events
 */

struct DRAG_AND_DROP_DATA {
   WidgetBase*  mwidget;///< The widget being moved, IF this is a drag and drop event, otherwise, NULL

   int          startx;///< The start x of the widget
   int          starty;///< The start y of the widget
   int          startw;///< The start width of the widget
   int          starth;///< The start height of the widget

   int          dropx;///< Where the widget was dropped, only for WIDGET_MOVER_DROP and SIZING messages
   int          dropy;///< Where the widget was dropped, only for WIDGET_MOVER_DROP and SIZING messages
   int          dropw;///< How wide the widget was when dropped, only for WIDGET_MOVER_DROP and SIZING messages
   int          droph;///< How tall the widget was when dropped, only for WIDGET_MOVER_DROP and SIZING messages
   
   DRAG_AND_DROP_DATA() :
         mwidget(0),
         startx(-1),
         starty(-1),
         startw(-1),
         starth(-1),
         dropx(-1),
         dropy(-1),
         dropw(-1),
         droph(-1)
   {}
   void Reset() {
      mwidget = 0;
      startx = -1;
      starty = -1;
      startw = -1;
      starth = -1;
      dropx  = -1;
      dropy  = -1;
      dropw  = -1;
      droph  = -1;
   }
};



struct WIDGET_EVENT_DATA {
   WidgetBase*  from;///< The widget that fired this event
   unsigned int topic;///< The widget topic
   int          msgs;///< The messages for the topic

   DRAG_AND_DROP_DATA dnd;///< Drag and drop data if this is a drag and drop widget event
   
   WIDGET_EVENT_DATA() :
         from(0),
         topic(TOPIC_NONE),
         msgs(0),
         dnd()
   {}
};



/**! @struct NETWORK_EVENT_DATA
 *   @brief Data for network events
 */

class Network;
struct NETWORK_EVENT_DATA {
   
   void SetFields(std::string IP , std::string PORT , unsigned char* bytes , unsigned int NBYTES);
   
   Network* srcNETWORK;
   std::string srcIP;
   std::string srcPORT;
   void* data;
   unsigned int data_size;
   
   NETWORK_EVENT_DATA();
   NETWORK_EVENT_DATA(Network* net , std::string IP , std::string PORT , unsigned char* bytes , unsigned int NBYTES);
   ~NETWORK_EVENT_DATA();
   void Free();

};




/**! @struct USER_EVENT_DATA
 *   @brief Data for user events
 */

struct USER_EVENT_DATA {
   int type;///< The user event subtype, in case you have more than one
   void* userdata;///< A void pointer to whatever data you want to send with this event

   USER_EVENT_DATA() :
         type(-1),
         userdata(0)
   {}
};



class EagleEventSource;

/**! @class EagleEvent
 *   @brief The master class for all eagle events
 */

class EagleEvent {

public :

   EAGLE_EVENT_TYPE type;///< The event type

   EagleEventSource* source;///< The event source

   EagleGraphicsContext* window;///< The graphics window in focus during the event

   double timestamp;///< The timestamp in seconds since program started, generated from ProgramTime::Now() - ProgramTime::Start()
   
   /**! @union EVENT_DATA
    *   @brief A union to hold all the different event data types
    */
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
      NETWORK_EVENT_DATA* network;
      USER_EVENT_DATA data;
   };

   EagleEvent() :
         type(EAGLE_EVENT_NONE),
         source(0),
         window(0),
         timestamp(-1.0)
   {}
};



/**! @enum EAGLE_EVENT_GROUP_TYPE
 *   @brief Bitfield describing event group type
 */

enum EAGLE_EVENT_GROUP_TYPE {
   EAGLE_EVENT_TYPE_NONE      = 0,     ///< Default event type is NONE
   EAGLE_MOUSE_EVENT_TYPE     = 1 << 0,///< Mouse event
   EAGLE_KEYBOARD_EVENT_TYPE  = 1 << 1,///< Keyboard event
   EAGLE_JOYSTICK_EVENT_TYPE  = 1 << 2,///< Joystick event
   EAGLE_TOUCH_EVENT_TYPE     = 1 << 3,///< Touch event
   EAGLE_DISPLAY_EVENT_TYPE   = 1 << 4,///< Display event
   EAGLE_WIDGET_EVENT_TYPE    = 1 << 5,///< Widget event
   EAGLE_NETWORK_EVENT_TYPE   = 1 << 6,///< Network event
   EAGLE_AUDIO_EVENT_TYPE     = 1 << 7,///< Network event
   EAGLE_VIDEO_EVENT_TYPE     = 1 << 8,///< Network event
   EAGLE_SYSTEM_EVENT_TYPE    = 1 << 9,///< System event
   EAGLE_USER_EVENT_TYPE      = 1 << 10,///< User event
   EAGLE_ANY_EVENT_TYPE       = 1 << 11,///< Generic event
   EAGLE_UNDEFINED_EVENT_TYPE = 1 << 12 ///< Undefined event
};



bool IsMouseEvent   (EagleEvent e);///< True if e is a mouse event
bool IsKeyboardEvent(EagleEvent e);///< True if e is a keyboard event
bool IsJoystickEvent(EagleEvent e);///< True if e is a joystick event
bool IsTouchEvent   (EagleEvent e);///< True if e is a touch event
bool IsDisplayEvent (EagleEvent e);///< True if e is a display event
bool IsWidgetEvent  (EagleEvent e);///< True if e is a widget event
bool IsNetworkEvent (EagleEvent e);///< True if e is a network event
bool IsSystemEvent  (EagleEvent e);///< True if e is a system event
bool IsUserEvent    (EagleEvent e);///< True if e is a user event



EAGLE_EVENT_GROUP_TYPE EagleEventGroupType(EagleEvent e);///< Get the event group type


EagleEvent MakeEagleEvent(WidgetMsg msg);///< Make an eagle event from a widget message



class EagleEventListener;

/**! @class EagleEventSource
 *   @brief The base class for all classes that wish to emit eagle events
 */

class EagleEventSource {

protected :
   typedef std::vector<EagleEventListener*> LISTENERS;
   typedef LISTENERS::iterator LIT;

   mutable LISTENERS listeners;

   LIT FindListener(EagleEventListener* l);

   friend class EagleEventListener;
   
   virtual void SubscribeListener(EagleEventListener* l);
   virtual void UnsubscribeListener(EagleEventListener* l);

   virtual void StopBroadcasting();
   
public :
   EagleEventSource();
   virtual ~EagleEventSource();
   
   EagleEventSource& operator=(const EagleEventSource& rhs);

   virtual void EmitEvent(EagleEvent e , EagleThread* thread = MAIN_THREAD);///< Emit an event from this source on the specified thread

   bool HasListeners() {return !listeners.empty();}///< True if anyone is listening to this source

   std::vector<EagleEventListener*> Listeners();///< Returns a list of listeners attached to this source
};



/**! @class EagleEventListener
 *   @brief The base class for all eagle event listeners
 */

class EagleEventListener {

private :
   typedef std::vector<EagleEventSource*> SOURCES;
   typedef SOURCES::iterator SIT;
   
   mutable SOURCES sources;

   SIT FindSource(EagleEventSource* s);

protected :
   void StopListening();
   
public :
   EagleEventListener();
   virtual ~EagleEventListener();
   
   EagleEventListener& operator=(const EagleEventListener& l);
   

   ///< Virtual function for responding to events. Override if you need different behavior
   virtual void RespondToEvent(EagleEvent e , EagleThread* thread = MAIN_THREAD)=0;

   virtual void ListenTo(EagleEventSource* s);///< Start listening to s
   virtual void StopListeningTo(EagleEventSource* s);///< Stop listening to s
   
   bool HasSources() {return sources.size();}///< True if we are listening to any event sources
};


/**! @class EagleEventHandler
 *   @brief Abstract base class for dealing with eagle events
 *
 *   
 *   EagleEventHandler's are thread safe, as long as you tell it which thread you're on. For general purposes, and single threaded apps
 *   you can use the default value for an @ref EagleThread*, @ref MAIN_THREAD . The mutexes used need to know which thread they're on
 *   to keep everything synchronized properly.
 *
 *   An EagleEventHandler enqueues-back events to its message queue (deque) for in order retrieval and event handling.
 *   As an EagleEventListener, it's event handling does several things. One it enqueues the event. Two, at the time of
 *   your choosing, it will emit the event to any objects listening to it. You can set this with the @ref EagleEventHandler::emitter_delay
 *   variable. Pass true to the constructor to delay emitting events until removed from the queue. Pass false to emit events immediately
 *   upon receipt. This determines which thread they will be run on. If events are delayed, they will be run on the thread where you take
 *   the event off of the queue. If events are not delayed, they will be run on the thread currently listening and waiting for an event.
 */

class EagleEventHandler : public EagleObject , public EagleEventListener , public EagleEventSource {

protected :

   ///< The event de(queue)
   std::deque<EagleEvent> queue;

   ///< Our guard mutex
   EagleMutex* mutex;

   ///< For signalling other threads
   EagleConditionVar* cond_var;

   ///< To decide whether events are emitted immediately upon receipt (emitter_delay = false),
   ///< or whether they are emitted as they are taken off the queue (emitter_delay = true)
   bool emitter_delay;
   
   ///< The thread we're running on, for comparison and mutex locking purposes
   EagleThread* our_thread;
   
   


   ///< To set the thread we're reported to be using...
   void SetOurThread(EagleThread* t);
   
   ///< Shutdown
   void StopHandlingEvents();
   


   /// EagleEventSource

   ///< Subscribes a listener to this queue
   void SubscribeListener(EagleEventListener* l);
   
   ///< Unsubscribes a listener to this queue
   void UnsubscribeListener(EagleEventListener* l);

   ///< Unsubscribe all listeners
   virtual void StopBroadcasting();

public :
   EagleEventHandler(std::string objclass , std::string objname , bool delay_emitted_events = true);///< See @ref emitter_delay
   virtual ~EagleEventHandler() {}

   virtual bool Create()=0;///< Abstract virtual function that must be implemented in derived classes
   virtual void Destroy()=0;///< Abstract virtual function that must be implemented in derived classes
   virtual bool Valid()=0;///< Abstract virtual function that must be implemented in derived classes
   
   /// EagleEventSource
   
   ///< Emits an event without enqueueing it
   void EmitEvent(EagleEvent e , EagleThread* thread = MAIN_THREAD);

   /// EagleEventListener

   ///< Enqueues an event and handles it
   void RespondToEvent(EagleEvent e , EagleThread* thread = MAIN_THREAD);

   void ListenTo(EagleEventSource* s) override;///< Start listening to s on main thread
   void StopListeningTo(EagleEventSource* s) override;///< Stop listening to s on main thread

   void ListenTo(EagleEventSource* s , EagleThread* t);///< Start listening to s on thread t
   void StopListeningTo(EagleEventSource* s , EagleThread* t);///< Stop listening to s on thread t


   /// EagleEventHandler
   
   ///< Clears the event queue immediately, without handling any events
   void Clear(EagleThread* thread = MAIN_THREAD);

   ///< Pushes an event and responds to it. Does same thing as @ref RespondToEvent
   void PushEvent(EagleEvent e , EagleThread* thread = MAIN_THREAD);

   ///< True if there is an event in the queue
   bool HasEvent(EagleThread* thread = MAIN_THREAD);

   ///< Take the next event. The @ref EagleEvent may be @ref EAGLE_EVENT_NONE
   EagleEvent TakeNextEvent(EagleThread* thread = MAIN_THREAD);

   ///< Peek at the next event. The @ref EagleEvent may be @ref EAGLE_EVENT_NONE
   EagleEvent PeekNextEvent(EagleThread* thread = MAIN_THREAD);

   ///< Does not EmitEvent...merely adds the event to the front of the queue
   ///< Allows you to 'put back' an event, but you should really be using @ref PeekNextEvent instead
   ///< if you don't need to take the event, or you're not using it.
   void InsertEventFront(EagleEvent e , EagleThread* thread = MAIN_THREAD);

   /**! FilterEvents allows you to collect a filtered set of events from an event queue.
    *   You may filter events by @ref EAGLE_EVENT_TYPE , @ref EagleEventSource pointer , or by both.
    */
   std::vector<EagleEvent> FilterEvents(EAGLE_EVENT_TYPE etype , EagleThread* thread = MAIN_THREAD);
   std::vector<EagleEvent> FilterEvents(EagleEventSource* esrc , EagleThread* thread = MAIN_THREAD);
   std::vector<EagleEvent> FilterEvents(EAGLE_EVENT_TYPE etype , EagleEventSource* esrc , EagleThread* thread = MAIN_THREAD);

   /**! WaitForEvent will wait for an event to be received. You may listen for a specific @ref EAGLE_EVENT_TYPE, for
    *   a specified duration, or you may wait indefinitely.
    */
   EagleEvent WaitForEvent(EagleThread* thread = MAIN_THREAD);
   EagleEvent WaitForEvent(double timeout , EagleThread* thread = MAIN_THREAD);
   EagleEvent WaitForEvent(EAGLE_EVENT_TYPE type , EagleThread* thread = MAIN_THREAD);
};



#endif // EagleEvents_HPP


