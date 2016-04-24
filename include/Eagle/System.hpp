
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



#ifndef EagleSystem_HPP
#define EagleSystem_HPP


#include "Eagle/Color.hpp"
#include "Eagle/Events.hpp"
#include "Eagle/GraphicsContext.hpp"
#include "Eagle/Image.hpp"
#include "Eagle/InputHandler.hpp"
#include "Eagle/Threads.hpp"
#include "Eagle/Mutexes.hpp"



enum EAGLE_INIT_STATE {
   EAGLE_SYSTEM =     1 << 0,
   EAGLE_IMAGES =     1 << 1,
   EAGLE_FONTS =      1 << 2,
   EAGLE_TTF_FONTS =  1 << 3,
   EAGLE_AUDIO =      1 << 4,
   EAGLE_SHADERS =    1 << 5,
   EAGLE_PRIMITIVES = 1 << 6,
   EAGLE_KEYBOARD =   1 << 7,
   EAGLE_MOUSE =      1 << 8,
   EAGLE_JOYSTICK =   1 << 9,
   EAGLE_TOUCH =      1 << 10
};

class EagleSystem;

extern EagleSystem* eagle_system;

extern EagleEvent most_recent_system_event;

extern const int EAGLE_STANDARD_INPUT;// keyboard mouse joystick
extern const int EAGLE_STANDARD_SYSTEM;// system images fonts ttf fonts audio
extern const int EAGLE_GENERAL_SETUP;// standard system and input together
extern const int EAGLE_FULL_SETUP;// all bits on muthafucka!





/// Temporary functions

///void EagleShutdown();

//void system_shutdown_function();

//int register_system_shutdown_function();







class EagleSystem : public EagleObject {

protected :

   PointerManager<EagleGraphicsContext> windows;
   PointerManager<EagleEventHandler> queues;
   PointerManager<EagleInputHandler> inputs;
   PointerManager<EagleTimer> timers;
   PointerManager<EagleThread> threads;
   PointerManager<EagleMutex> mutexes;

   EagleInputHandler* input_handler;
   EagleTimer* system_timer;
   static float system_timer_rate;
   
   EagleEventHandler* system_queue;
   
   bool system_up;
   bool images_up;
   bool fonts_up;
   bool ttf_fonts_up;
   bool audio_up;
   bool shaders_up;
   bool primitives_up;
   
   bool keyboard_running;
   bool mouse_running;
   bool joystick_running;
   bool touch_running;
   


   virtual bool PrivateInitializeSystem()=0;

   virtual bool PrivateInitializeImages()=0;
   virtual bool PrivateInitializeFonts()=0;
   virtual bool PrivateInitializeTTFFonts()=0;
   virtual bool PrivateInitializeAudio()=0;
   virtual bool PrivateInitializeShaders()=0;
   virtual bool PrivateInitializePrimitives()=0;

   virtual bool PrivateInstallKeyboard()=0;
   virtual bool PrivateInstallMouse()=0;
   virtual bool PrivateInstallJoystick()=0;
   virtual bool PrivateInstallTouch()=0;

	virtual EagleInputHandler*    PrivateCreateInputHandler()=0;
	virtual EagleEventHandler*    PrivateCreateEventHandler(bool delay_events = true)=0;
	virtual EagleTimer*           PrivateCreateTimer()=0;
   virtual EagleGraphicsContext* PrivateCreateGraphicsContext(int width , int height , int flags)=0;
   virtual EagleThread*          PrivateCreateThread(void* (*process)(EagleThread* , void*) , void* data)=0;
   virtual EagleMutex*           PrivateCreateMutex(bool recursive)=0;

public :

   EagleSystem(std::string name);
   virtual ~EagleSystem() {}// Call Shutdown from your destructor please

   virtual void Shutdown();// Call EagleSystem::Shutdown from your overridden Shutdown methods please...



   virtual int Initialize(int state);
   
   virtual bool InitializeSystem();

   bool InitializeImages();
   bool InitializeFonts();
   bool InitializeTTFFonts();
   bool InitializeAudio();
   bool InitializeShaders();
   bool InitializePrimitives();

   virtual bool InitializeAll();// call this in overridden method calls
   
   bool InstallKeyboard();
   bool InstallMouse();
   bool InstallJoystick();
   bool InstallTouch();

   virtual bool InstallAll();// call this in overridden method calls
   

   bool SystemUp()        {return system_up;}
   bool ImagesUp()        {return images_up;}
   bool FontsUp()         {return fonts_up;}
   bool TTFFontsUp()      {return ttf_fonts_up;}
   bool AudioUp()         {return audio_up;}
   bool ShadersUp()       {return shaders_up;}
   bool PrimitivesUp()    {return primitives_up;}

   bool KeyboardRunning() {return keyboard_running;}
   bool MouseRunning()    {return mouse_running;}
   bool JoystickRunning() {return joystick_running;}
   bool TouchRunning()    {return touch_running;}

   int EagleInitState();

///   virtual EagleGraphicsContext* MakeGraphicsContext(int width , int height , int flags)=0;

   EagleInputHandler* GetInputHandler();
   EagleEventHandler* GetSystemQueue();
   EagleTimer*        GetSystemTimer();

	EagleInputHandler*    CreateInputHandler();
	EagleEventHandler*    CreateEventHandler(bool delay_events);
	EagleTimer*           CreateTimer();
   EagleGraphicsContext* CreateGraphicsContext(int width , int height , int flags);
   EagleThread*          CreateThread(void* (*process)(EagleThread* , void*) , void* data);
   EagleMutex*           CreateMutex(bool recursive);

	void RegisterKeyboardInput(EagleEventHandler* queue);
	void RegisterMouseInput   (EagleEventHandler* queue);
	void RegisterJoystickInput(EagleEventHandler* queue);
	void RegisterTouchInput   (EagleEventHandler* queue);
	
   void RegisterInputs(EagleEventHandler* queue);
   


   bool UpToDate();
   EagleEvent UpdateSystemState();/// Keep calling this until UpToDate() returns true!
   EagleEvent WaitForSystemEventAndUpdateState();   /// If you don't install any input or create any displays
                                                    /// WaitForSystemEventAndUpdateState might never return
   EagleEvent TimedWaitForSystemEventAndUpdateState(double timeout);

	virtual double GetProgramTime()=0;// Seconds since system initialization
	virtual void Rest(double time)=0;// Rest for time seconds
	
};



#endif // EagleSystem_HPP





