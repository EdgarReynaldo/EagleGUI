

#ifndef Allegro5_InputHandler_HPP
#define Allegro5_InputHandler_HPP



#include "Eagle/Exception.hpp"
#include "Eagle/InputHandler.hpp"
#include "Eagle/backends/Allegro5/Allegro5Threads.hpp"
#include "Eagle/backends/Allegro5/Allegro5EventHandler.hpp"


#include "allegro5/allegro.h"


EagleEvent GetEagleInputEvent(ALLEGRO_EVENT ev);


enum A5INPUT_THREAD_MESSAGE {
   A5INPUT_THREAD_SHOULD_STOP = -1
};

void* A5InputThreadProcess(EagleThread* thread , void* input_handler);



class Allegro5InputHandler : public EagleInputHandler {

private :

   ALLEGRO_EVENT_SOURCE input_extra_event_source;
   ALLEGRO_EVENT_QUEUE* input_queue;
   
   Allegro5Thread input_thread;
   
   Allegro5EventHandler keyboard_event_handler;
   Allegro5EventHandler joystick_event_handler;
   Allegro5EventHandler mouse_event_handler;
   Allegro5EventHandler touch_event_handler;
   
   
   
   friend void* A5InputThreadProcess(EagleThread* thread , void* input_handler);

   virtual void PrivateInitializeJoystickInput();// called by InitializeJoysticksInput
   virtual void PrivateInitializeKeyboardInput();// called by InitializeKeyboardinput
   virtual void PrivateInitializeMouseInput();// called by InitializeMouseInput
   virtual void PrivateInitializeTouchInput();// called by InitializeTouchInput
   
public :
   
   Allegro5InputHandler();
   ~Allegro5InputHandler();
   
   virtual void RespondToEvent(EagleEvent e);
   
   virtual int NumJoysticksInstalled();

   virtual void GetJoystickStates();
   virtual void GetKeyboardState();
   virtual void GetMouseState();
   virtual void GetTouchState();

	virtual void RegisterKeyboardInput(EagleEventHandler* queue);
	virtual void RegisterMouseInput(EagleEventHandler* queue);
	virtual void RegisterJoystickInput(EagleEventHandler* queue);
	virtual void RegisterTouchInput(EagleEventHandler* queue);
	
};



#endif // Allegro5_InputHandler_HPP
