


#include "Eagle/backends/Allegro5/Allegro5InputHandler.hpp"
#include "Eagle/backends/Allegro5/Allegro5EventHandler.hpp"
#include "Eagle/backends/Allegro5/Allegro5GraphicsContext.hpp"


#include "allegro5/allegro.h"



EagleEvent GetEagleInputEvent(ALLEGRO_EVENT ev) {
   EagleEvent ee;

   ee.type = (EAGLE_EVENT_TYPE)(int)ev.type;/// TODO : Translate allegro events into eagle events once they are actually different
   ee.timestamp = ev.any.timestamp;
/*
   EAGLE_EVENT_TYPE type;

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
      WIDGET_EVENT_DATA widget;
      USER_EVENT_DATA data;
   };

*/

   switch (ev.type) {
      case ALLEGRO_EVENT_JOYSTICK_AXIS :
      case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN :
      case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP :
      case ALLEGRO_EVENT_JOYSTICK_CONFIGURATION :
         ee.joystick.nid = get_joystick_n(ev.joystick.id);
         if (ee.joystick.nid == -1) {
            ee.joystick.id = 0;
         }
         else {
				ee.joystick.id = &joysticks[ee.joystick.nid];
         }
         ee.joystick.stick = ev.joystick.stick;
         ee.joystick.axis = ev.joystick.axis;
         ee.joystick.button = ev.joystick.button;
         ee.joystick.pos = ev.joystick.pos;
         break;
      case ALLEGRO_EVENT_KEY_DOWN :
      case ALLEGRO_EVENT_KEY_UP :
      case ALLEGRO_EVENT_KEY_CHAR :
         ee.keyboard.keycode = ev.keyboard.keycode;
         ee.keyboard.unicode = ev.keyboard.unichar;
         ee.keyboard.modifiers = ev.keyboard.modifiers;
         ee.keyboard.repeat = ev.keyboard.repeat;
         break;
      case ALLEGRO_EVENT_MOUSE_AXES :
      case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN :
      case ALLEGRO_EVENT_MOUSE_BUTTON_UP :
      case ALLEGRO_EVENT_MOUSE_WARPED :
      case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY :
      case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY :
         ee.mouse.x = ev.mouse.x;
         ee.mouse.y = ev.mouse.y;
         ee.mouse.z = ev.mouse.z;
         ee.mouse.w = ev.mouse.w;
         ee.mouse.dx = ev.mouse.dx;
         ee.mouse.dy = ev.mouse.dy;
         ee.mouse.dz = ev.mouse.dz;
         ee.mouse.dw = ev.mouse.dw;
         ee.mouse.button = ev.mouse.button;
         break;
      case ALLEGRO_EVENT_TIMER :
         ee.timer.eagle_timer_source = 0;// did not come from an Eagletimer
         ee.timer.raw_source = ev.timer.source;
         ee.timer.count = ev.timer.count;
         break;
      case ALLEGRO_EVENT_DISPLAY_EXPOSE :
      case ALLEGRO_EVENT_DISPLAY_RESIZE :
      case ALLEGRO_EVENT_DISPLAY_CLOSE :
      case ALLEGRO_EVENT_DISPLAY_LOST :
      case ALLEGRO_EVENT_DISPLAY_FOUND :
      case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT :
      case ALLEGRO_EVENT_DISPLAY_SWITCH_IN :
      case ALLEGRO_EVENT_DISPLAY_ORIENTATION :
      case ALLEGRO_EVENT_DISPLAY_HALT_DRAWING :
      case ALLEGRO_EVENT_DISPLAY_RESUME_DRAWING :
         ee.display.x = ev.display.x;
         ee.display.y = ev.display.y;
         ee.display.width = ev.display.width;
         ee.display.height = ev.display.height;
         ee.display.orientation = ev.display.orientation;
         break;

      case ALLEGRO_EVENT_TOUCH_BEGIN :
      case ALLEGRO_EVENT_TOUCH_END :
      case ALLEGRO_EVENT_TOUCH_MOVE :
      case ALLEGRO_EVENT_TOUCH_CANCEL :
         ee.touch.id = ev.touch.id;
         ee.touch.x = ev.touch.x;
         ee.touch.y = ev.touch.y;
         ee.touch.dx = ev.touch.dx;
         ee.touch.dy = ev.touch.dy;
         ee.touch.primary = ev.touch.primary;
         break;
      default : break;
   }
   return ee;
}



void* A5InputThreadProcess(EagleThread* thread , void* input_handler) {
   EagleInputHandler* handler = (EagleInputHandler*)input_handler;
   Allegro5InputHandler* a5_input_handler = dynamic_cast<Allegro5InputHandler*>(handler);
   
   if (!a5_input_handler) {
      throw EagleException("A5InputThreadProcess : Input handler is invalid.");
   }
   
   while (!thread->ShouldStop()) {
      ALLEGRO_EVENT ev;
      al_wait_for_event(a5_input_handler->input_queue , &ev);
   
      if (ev.any.source == &a5_input_handler->input_extra_event_source &&
          ev.type == EAGLE_EVENT_USER_START &&
          A5INPUT_THREAD_SHOULD_STOP == (A5INPUT_THREAD_MESSAGE)ev.user.data1) {
         break;
      }
      EagleEvent ee = GetEagleInputEvent(ev);

      ee.window = Allegro5GraphicsContext::GetActiveWindow();
      ee.source = a5_input_handler;
      
      if (IsKeyboardEvent(ee)) {
         handler->EmitEvent(ee);
         a5_input_handler->keyboard_event_handler.RespondToEvent(ee);
         a5_input_handler->keyboard_event_handler.TakeNextEvent();/// discard event after emitting
      }
      else if (IsMouseEvent(ee)) {
         handler->EmitEvent(ee);
         a5_input_handler->mouse_event_handler.RespondToEvent(ee);
         a5_input_handler->mouse_event_handler.TakeNextEvent();/// discard event after emitting
      }
      else if (IsTouchEvent(ee)) {
         handler->EmitEvent(ee);
         a5_input_handler->touch_event_handler.RespondToEvent(ee);
         a5_input_handler->touch_event_handler.TakeNextEvent();/// discard event after emitting
      }
      else if (IsJoystickEvent(ee)) {
         handler->EmitEvent(ee);
         a5_input_handler->joystick_event_handler.RespondToEvent(ee);
         a5_input_handler->joystick_event_handler.TakeNextEvent();/// discard event after emitting
      }
   }
   return input_handler;
}



/// -------------------     Allegro5InputHandler     --------------------------



void Allegro5InputHandler::PrivateInitializeJoystickInput() {
   // do nothing
   (void)0;
   return;
}



void Allegro5InputHandler::PrivateInitializeKeyboardInput() {
   // do nothing
   (void)0;
   return;
}



void Allegro5InputHandler::PrivateInitializeMouseInput() {
   // do nothing
   (void)0;
   return;
}



void Allegro5InputHandler::PrivateInitializeTouchInput() {
   // do nothing
   (void)0;
   return;
}



Allegro5InputHandler::Allegro5InputHandler() :
      input_extra_event_source(),
      input_queue(0),
      input_thread(),
      keyboard_event_handler(false),
      joystick_event_handler(false),
      mouse_event_handler(false),
      touch_event_handler(false)
{
   if (!input_thread.Create(A5InputThreadProcess , this)) {
      throw EagleException("Allegro5InputHandler::Allegro5InputHandler : Failed to create input thread.");
   }

   al_init_user_event_source(&input_extra_event_source);

   input_queue = al_create_event_queue();

   if (!input_queue) {
      throw EagleException("Allegro5InputHandler::Allegro5InputHandler : Failed to create input_queue.");
   }

   al_register_event_source(input_queue , &input_extra_event_source);
   
   input_thread.Start();
}



Allegro5InputHandler::~Allegro5InputHandler() {
   ALLEGRO_EVENT ev;
   ev.type = EAGLE_EVENT_USER_START;
   ev.any.source = &input_extra_event_source;
   ev.any.timestamp = al_get_time();
   ev.user.data1 = A5INPUT_THREAD_SHOULD_STOP;
   al_emit_user_event(&input_extra_event_source , &ev , 0);
   
   input_thread.Join();
   
   al_destroy_event_queue(input_queue);
   al_destroy_user_event_source(&input_extra_event_source);
}



int Allegro5InputHandler::NumJoysticksInstalled() {
   return num_joysticks = al_get_num_joysticks();
}



void Allegro5InputHandler::GetJoystickStates() {
   const int numjoy = NumJoysticksInstalled();
   for (int n = 0 ; n < numjoy ; ++n) {
// ALLEGRO_JOYSTICK * al_get_joystick(int num)
      ALLEGRO_JOYSTICK* joystick = al_get_joystick(n);
      
      int num_sticks = al_get_joystick_num_sticks(joystick);
      joysticks[n].num_sticks = num_sticks;
      for (int i = 0 ; i < num_sticks ; ++i) {
         joysticks[n].num_axes[i] = al_get_joystick_num_axes(joystick , i);
      }
      joysticks[n].num_buttons = al_get_joystick_num_buttons(joystick);
      
      ALLEGRO_JOYSTICK_STATE state;
      al_get_joystick_state(joystick , &state);
      
      for (int stick = 0 ; stick < num_sticks ; ++stick) {
         for (int axis = 0 ; axis < joysticks[n].num_axes[stick] ; ++axis) {
            joysticks[n].axes[stick][axis] = state.stick[stick].axis[axis];
         }
      }
      for (int button = 0 ; button < joysticks[n].num_buttons ; ++button) {
         if (state.button[button]) {
            joysticks[n].buttonstates[button] = HELD;
         } else {
            joysticks[n].buttonstates[button] = OPEN;
         }
         joysticks[n].button_held_duration[button] = 0.0f;
         joysticks[n].since_last_jspress[button] = 0.0f;
      }
      
      
   }
}



void Allegro5InputHandler::GetKeyboardState() {
   throw EagleException("Allegro5InputHandler::GetKeyboardState : Sorry, not implemented yet. Keystates are up to date anyway, as long as you feed it your events");
}



void Allegro5InputHandler::GetMouseState() {
   throw EagleException("Allegro5InputHandler::GetMouseState : Sorry, not implemented yet. Mousestates are up to date anyway, as long as you feed it your events");
}



void Allegro5InputHandler::GetTouchState() {
   throw EagleException("Allegro5InputHandler::GetTouchState : Sorry, not implemented yet.");
}




void Allegro5InputHandler::RegisterKeyboardInput(EagleEventHandler* queue) {

   queue->ListenTo(&keyboard_event_handler);

	Allegro5EventHandler* a5event_handler = dynamic_cast<Allegro5EventHandler*>(queue);
	if (a5event_handler) {
		ALLEGRO_EVENT_QUEUE* a5queue = a5event_handler->AllegroQueue();
		al_register_event_source(a5queue , al_get_keyboard_event_source());
	}
}



void Allegro5InputHandler::RegisterMouseInput(EagleEventHandler* queue) {
   queue->ListenTo(&mouse_event_handler);

	Allegro5EventHandler* a5event_handler = dynamic_cast<Allegro5EventHandler*>(queue);
	if (a5event_handler) {
		ALLEGRO_EVENT_QUEUE* a5queue = a5event_handler->AllegroQueue();
		al_register_event_source(a5queue , al_get_mouse_event_source());
	}
}



void Allegro5InputHandler::RegisterJoystickInput(EagleEventHandler* queue) {
   queue->ListenTo(&joystick_event_handler);

	Allegro5EventHandler* a5event_handler = dynamic_cast<Allegro5EventHandler*>(queue);
	if (a5event_handler) {
		ALLEGRO_EVENT_QUEUE* a5queue = a5event_handler->AllegroQueue();
		al_register_event_source(a5queue , al_get_joystick_event_source());
	}
}



void Allegro5InputHandler::RegisterTouchInput(EagleEventHandler* queue) {
   queue->ListenTo(&touch_event_handler);

	Allegro5EventHandler* a5event_handler = dynamic_cast<Allegro5EventHandler*>(queue);
	if (a5event_handler) {
		ALLEGRO_EVENT_QUEUE* a5queue = a5event_handler->AllegroQueue();
		al_register_event_source(a5queue , al_get_touch_input_event_source());
	}
}



/**

SAVE for A5 backend...

void EagleJoystickData::Initialize(ALLEGRO_JOYSTICK* joy) {
   joystick = joy;
   if (!joystick) {
      num_sticks = 0;
      for (int i = 0 ; i < JS_MAX_NUM_STICKS ; ++i) {
         num_axes[i] = 0;
      }
      num_buttons = 0;
      return;
   }
   
   num_sticks = al_get_joystick_num_sticks(joystick);
   for (int i = 0 ; i < num_sticks ; ++i) {
      num_axes[i] = al_get_joystick_num_axes(joystick , i);
   }
   num_buttons = al_get_joystick_num_buttons(joystick);
   
   GetState();
}


void EagleJoystickData::GetState() {

   ALLEGRO_JOYSTICK_STATE state;
   al_get_joystick_state(joystick , &state);

// struct {
//    float axis[num_axes];             // -1.0 to 1.0 
// } stick[num_sticks];
// int button[num_buttons]; 

   for (int stick = 0 ; stick < num_sticks ; ++stick) {
      for (int axis = 0 ; axis < num_axes[stick] ; ++axis) {
         axes[stick][axis] = state.stick[stick].axis[axis];
      }
   }
   for (int button = 0 ; button < num_buttons ; ++button) {
      if (state.button[button]) {
         buttonstates[button] = HELD;
      } else {
         buttonstates[button] = OPEN;
      }
      button_held_duration[button] = 0.0f;
      since_last_jspress[button] = 0.0f;
   }
}
*/

