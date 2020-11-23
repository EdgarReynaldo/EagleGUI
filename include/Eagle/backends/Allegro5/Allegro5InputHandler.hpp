
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
 * @file Allegro5InputHandler.hpp
 * @brief The interface for the Allegro 5 input handler
 */

#ifndef Allegro5_InputHandler_HPP
#define Allegro5_InputHandler_HPP



#include "Eagle/Exception.hpp"
#include "Eagle/InputHandler.hpp"

#include "Eagle/backends/Allegro5/Allegro5Threads.hpp"
#include "Eagle/backends/Allegro5/Allegro5EventHandler.hpp"

#include "allegro5/allegro.h"



EagleEvent GetEagleInputEvent(ALLEGRO_EVENT ev);///< Get an EagleEvent from an ALLEGRO_EVENT input



enum A5INPUT_THREAD_MESSAGE {
   A5INPUT_THREAD_SHOULD_STOP = -1
};



void* A5InputThreadProcess(EagleThread* thread , void* input_handler);///< The main input thread process



/**! @class Allegro5InputHandler 
 *   @brief Handles setting up all the allegro 5 input
 * 
 *   You don't need to create an InputHandler, it will be handled automatically for you by the system
 */

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
   
   Allegro5InputHandler(std::string objname = "Nemo");///< Public constructor, but you don't need it
   ~Allegro5InputHandler();///< Destructor
   
   virtual void RespondToEvent(EagleEvent e , EagleThread* thread);///< For system
   
   virtual int NumJoysticksInstalled();///< Get the number of joysticks installed

   virtual void GetJoystickStates();///< Not needed yet
   virtual void GetKeyboardState();///< Not needed yet
   virtual void GetMouseState();///< Not needed yet
   virtual void GetTouchState();///< Not needed yet

   
   virtual void StartKeyboardEventHandler();///< Automatically called by system
   virtual void StartJoystickEventHandler();///< Automatically called by system
   virtual void StartMouseEventHandler();///< Automatically called by system
   virtual void StartTouchEventHandler();///< Automatically called by system
};



#endif // Allegro5_InputHandler_HPP
