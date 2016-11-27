

#ifndef Allegro5_InputHandler_HPP
#define Allegro5_InputHandler_HPP



#include "Eagle/Exception.hpp"
#include "Eagle/InputHandler.hpp"



class Allegro5InputHandler : public EagleInputHandler {

private :
   virtual void PrivateInitializeJoystickInput();// called by InitializeJoysticksInput
   virtual void PrivateInitializeKeyboardInput();// called by InitializeKeyboardinput
   virtual void PrivateInitializeMouseInput();// called by InitializeMouseInput
   virtual void PrivateInitializeTouchInput();// called by InitializeTouchInput
   
public :
   
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
