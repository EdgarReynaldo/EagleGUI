


#include "Eagle/backends/Allegro5/Allegro5InputHandler.hpp"
#include "Eagle/backends/Allegro5/Allegro5EventHandler.hpp"

#include "allegro5/allegro.h"



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
   throw new EagleError("Sorry, not implemented yet. Keystates are up to date anyway, as long as you feed it your events");
}



void Allegro5InputHandler::GetMouseState() {
   throw new EagleError("Sorry, not implemented yet. Mousestates are up to date anyway, as long as you feed it your events");
}



void Allegro5InputHandler::GetTouchState() {
   throw new EagleError("Sorry, not implemented yet.");
}



void Allegro5InputHandler::RegisterKeyboardInput(EagleEventHandler* queue) {
	Allegro5EventHandler* a5event_handler = dynamic_cast<Allegro5EventHandler*>(queue);
	if (a5event_handler) {
		ALLEGRO_EVENT_QUEUE* a5queue = a5event_handler->AllegroQueue();
		al_register_event_source(a5queue , al_get_keyboard_event_source());
	}
}



void Allegro5InputHandler::RegisterMouseInput(EagleEventHandler* queue) {
	Allegro5EventHandler* a5event_handler = dynamic_cast<Allegro5EventHandler*>(queue);
	if (a5event_handler) {
		ALLEGRO_EVENT_QUEUE* a5queue = a5event_handler->AllegroQueue();
		al_register_event_source(a5queue , al_get_mouse_event_source());
	}
}



void Allegro5InputHandler::RegisterJoystickInput(EagleEventHandler* queue) {
	Allegro5EventHandler* a5event_handler = dynamic_cast<Allegro5EventHandler*>(queue);
	if (a5event_handler) {
		ALLEGRO_EVENT_QUEUE* a5queue = a5event_handler->AllegroQueue();
		al_register_event_source(a5queue , al_get_joystick_event_source());
	}
}



void Allegro5InputHandler::RegisterTouchInput(EagleEventHandler* queue) {
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

