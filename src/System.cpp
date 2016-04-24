
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



#include "Eagle/System.hpp"
#include "Eagle/Logging.hpp"




/// Temporary functions
/*
void EagleShutdown() {
   /// TODO : Destroy each system, not just eagle_system
   printf("EagleShutdown called.\n");
   if (eagle_system) {
      printf("Calling eagle_system->Shutdown().\n");
      eagle_system->Shutdown();
   }
}



void system_shutdown_function() {
   printf("Eagle atexit function system_shutdown_function called\n");
}



#include <cstdlib>
int register_system_shutdown_function() {
   int ret = 0;
   printf("register system shutdown function called. Address of atexit is %p\n" , (void*)atexit);
   printf("atexit(system_shutdown_function) returned %d\n" , ret = atexit(system_shutdown_function));
//   return atexit(shutdown_function);
   return ret;
}
*/







EagleSystem* eagle_system = 0;

EagleEvent most_recent_system_event = EagleEvent();

const int EAGLE_STANDARD_INPUT = EAGLE_KEYBOARD | EAGLE_MOUSE | EAGLE_JOYSTICK;
const int EAGLE_STANDARD_SYSTEM = EAGLE_SYSTEM | EAGLE_IMAGES | EAGLE_FONTS | EAGLE_TTF_FONTS | EAGLE_AUDIO;
const int EAGLE_GENERAL_SETUP = EAGLE_STANDARD_INPUT | EAGLE_STANDARD_SYSTEM;
const int EAGLE_FULL_SETUP = EAGLE_GENERAL_SETUP | EAGLE_SHADERS | EAGLE_PRIMITIVES;


float EagleSystem::system_timer_rate = 1.0f/60.0f;



EagleSystem::EagleSystem(std::string name) :
   EagleObject(name),
   windows(true),
   queues(true),
   inputs(true),
   timers(true),
   threads(true),
   mutexes(true),
   input_handler(0),
   system_timer(0),
   system_queue(0),
   system_up(false),
   images_up(false),
   fonts_up(false),
   ttf_fonts_up(false),
   audio_up(false),
   shaders_up(false),
   primitives_up(false),
   keyboard_running(false),
   mouse_running(false),
   joystick_running(false),
   touch_running(false)
{
   if (!eagle_system) {
      eagle_system = this;
   }
}



void EagleSystem::Shutdown() {
   
   EagleLog() << "EagleSystem::Shutdown called" << std::endl;
   
   timers.FreeAll();
   inputs.FreeAll();
   windows.FreeAll();
   queues.FreeAll();
   threads.FreeAll();
   mutexes.FreeAll();
   
   /// TODO : Keep list of eagle systems
   if (eagle_system && eagle_system == this) {
		eagle_system = 0;
   }
}



int EagleSystem::Initialize(int state) {
   if (state & EAGLE_SYSTEM)     {InitializeSystem();}
   if (state & EAGLE_IMAGES)     {InitializeImages();}
   if (state & EAGLE_FONTS)      {InitializeFonts();}
   if (state & EAGLE_TTF_FONTS)  {InitializeTTFFonts();}
   if (state & EAGLE_AUDIO)      {InitializeAudio();}
   if (state & EAGLE_SHADERS)    {InitializeShaders();}
   if (state & EAGLE_PRIMITIVES) {InitializePrimitives();}
   if (state & EAGLE_KEYBOARD)   {InstallKeyboard();}
   if (state & EAGLE_MOUSE)      {InstallMouse();}
   if (state & EAGLE_JOYSTICK)   {InstallJoystick();}
   if (state & EAGLE_TOUCH)      {InstallTouch();}
   return EagleInitState();
}



bool EagleSystem::InitializeSystem() {

///   int success = (atexit(EagleShutdown) == 0);
///   EagleLog() << StringPrintF("EagleSystem::InitializeSystem - atexit ptr is %p and atexit(EagleShutdown) returned %d\n" ,
///                              atexit , success);

   system_up = PrivateInitializeSystem();
   if (!system_up) {
      EagleLog() << "Eagle : Failed to initialize the system." << std::endl;
   }
   else {
      EagleLog() << "Eagle : Initialized system." << std::endl;
   }
   
   if (!input_handler) {input_handler = CreateInputHandler();}
   if (!system_timer)  {system_timer  = CreateTimer();}
   if (!system_queue)  {system_queue  = CreateEventHandler(false);}

   system_up = (system_up && input_handler && system_timer && system_queue);

   if (system_timer) {
      bool created_system_timer = system_timer->Create(system_timer_rate);
      system_up = system_up && created_system_timer;
      if (created_system_timer) {
         SetInputTimer(system_timer);
         if (system_queue) {
            system_timer->RegisterTimerInput(system_queue);
         }
      }
   }
   if (system_up) {
      EagleLog() << "Eagle : Initialized the system state." << std::endl;
//      register_system_shutdown_function();
   }
   else {
      EagleLog() << "Eagle : System state not fully initialized." << std::endl;
   }
   
   return system_up;
}



bool EagleSystem::InitializeImages() {
   images_up = PrivateInitializeImages();
   if (!images_up) {
      EagleLog() << "Eagle : Failed to initialize images." << std::endl;
   }
   else {
      EagleLog() << "Eagle : Initialized images." << std::endl;
   }
   return images_up;
}



bool EagleSystem::InitializeFonts() {
   fonts_up = PrivateInitializeFonts();
   if (!fonts_up) {
      EagleLog() << "Eagle : Failed to initialize fonts." << std::endl;
   }
   else {
      EagleLog() << "Eagle : Initialized fonts." << std::endl;
   }
   return fonts_up;
}



bool EagleSystem::InitializeTTFFonts() {
   ttf_fonts_up = PrivateInitializeTTFFonts();
   if (!ttf_fonts_up) {
      EagleLog() << "Eagle : Failed to initialize ttf fonts." << std::endl;
   }
   else {
      EagleLog() << "Eagle : Initialized TTF fonts." << std::endl;
   }
   return ttf_fonts_up;
}



bool EagleSystem::InitializeAudio() {
   audio_up = PrivateInitializeAudio();
   if (!audio_up) {
      EagleLog() << "Eagle : Failed to initialize audio." << std::endl;
   }
   else {
      EagleLog() << "Eagle : Initialized audio." << std::endl;
   }
   return audio_up;
}



bool EagleSystem::InitializeShaders() {
   shaders_up = PrivateInitializeShaders();
   if (!shaders_up) {
      EagleLog() << "Eagle : Failed to initialize shaders." << std::endl;
   }
   else {
      EagleLog() << "Eagle : Initialized shaders." << std::endl;
   }
   return shaders_up;
}



bool EagleSystem::InitializePrimitives() {
   primitives_up = PrivateInitializePrimitives();
   if (!primitives_up) {
      EagleLog() << "Eagle : Failed to initilialize primitives." << std::endl;
   }
   else {
      EagleLog() << "Eagle : Initialized primitives." << std::endl;
   }
   return primitives_up;
}



bool EagleSystem::InstallKeyboard() {
   keyboard_running = PrivateInstallKeyboard();
   if (!keyboard_running) {
      EagleLog() << "Eagle : Failed to install keyboard." << std::endl;
   }
   else {
      EagleLog() << "Eagle : Installed keyboard." << std::endl;
   	EagleInputHandler* input = GetInputHandler();
      EagleEventHandler* queue = GetSystemQueue();
   	if (input) {
   		input->InitializeKeyboardInput();
   		if (queue) {
				input->RegisterKeyboardInput(queue);
   		}
   	}
   	if (!input || !queue) {
         if (!input) {
            EagleLog() << "EagleSystem::InstallKeyboard : Failed to retrieve input handler." << std::endl;
         }
         if (!queue) {
            EagleLog() << "EagleSystem::InstallKeyboard : Failed to retrieve system queue." << std::endl;
         }
   	}
   }
   return keyboard_running;
}



bool EagleSystem::InstallMouse() {
   mouse_running = PrivateInstallMouse();
   if (!mouse_running) {
      EagleLog() << "Eagle : Failed to install mouse." << std::endl;
   }
   else {
      EagleLog() << "Eagle : Installed mouse." << std::endl;
   	EagleInputHandler* input = GetInputHandler();
   	if (input) {
   		input->InitializeMouseInput();
   		EagleEventHandler* queue = GetSystemQueue();
   		if (queue) {
				input->RegisterMouseInput(queue);
   		}
   	}
   }
   return mouse_running;
}



bool EagleSystem::InstallJoystick() {
   joystick_running = PrivateInstallJoystick();
   if (!joystick_running) {
      EagleLog() << "Eagle : Failed to install joystick." << std::endl;
   }
   else {
      EagleLog() << "Eagle : Installed joystick." << std::endl;
   	EagleInputHandler* input = GetInputHandler();
   	if (input) {
   		input->InitializeJoystickInput();
   		EagleEventHandler* queue = GetSystemQueue();
   		if (queue) {
				input->RegisterJoystickInput(queue);
   		}
   	}
   }
   return joystick_running;
}



bool EagleSystem::InstallTouch() {
   touch_running = PrivateInstallTouch();
   if (!touch_running) {
      EagleLog() << "Eagle : Failed to install touch input." << std::endl;
   }
   else {
      EagleLog() << "Eagle : Installed touch input." << std::endl;
   	EagleInputHandler* input = GetInputHandler();
   	if (input) {
   		input->InitializeTouchInput();
   		EagleEventHandler* queue = GetSystemQueue();
   		if (queue) {
				input->RegisterTouchInput(queue);
   		}
   	}
   }
   return touch_running;
}



bool EagleSystem::InitializeAll() {
   if (!InitializeSystem()) {return false;}
   InitializeImages();
   InitializeFonts();
   InitializeAudio();
   InitializeTTFFonts();
   InitializeShaders();
   InitializePrimitives();
   return (system_up &&
           images_up &&
           fonts_up &&
           audio_up &&
           ttf_fonts_up &&
           shaders_up &&
           primitives_up);
}



bool EagleSystem::InstallAll() {
   InstallKeyboard();
   InstallMouse();
   InstallJoystick();
   InstallTouch();
   return keyboard_running && mouse_running && joystick_running && touch_running;
}



int EagleSystem::EagleInitState() {
   int state = 0;
   if (system_up)        {state |= EAGLE_SYSTEM;}
   if (images_up)        {state |= EAGLE_IMAGES;}
   if (fonts_up)         {state |= EAGLE_FONTS;}
   if (ttf_fonts_up)     {state |= EAGLE_TTF_FONTS;}
   if (audio_up)         {state |= EAGLE_AUDIO;}
   if (shaders_up)       {state |= EAGLE_SHADERS;}
   if (primitives_up)    {state |= EAGLE_PRIMITIVES;}
   if (keyboard_running) {state |= EAGLE_KEYBOARD;}
   if (mouse_running)    {state |= EAGLE_MOUSE;}
   if (joystick_running) {state |= EAGLE_JOYSTICK;}
   if (touch_running)    {state |= EAGLE_TOUCH;}
   return state;
}



/// TODO,DESIGN : Try catch block? PrivateCreateInputHandler will most likely throw bad_alloc if it fails
/// TODO,DESIGN : This applies to GetInputHandler, GetSystemQueue , and GetSystemTimer
/// TODO,DESIGN : As well as the Create functions

EagleInputHandler* EagleSystem::GetInputHandler() {
   if (!input_handler) {
      input_handler = CreateInputHandler();
      if (!input_handler) {
         EagleLog() << "Failed to retrieve input handler." << std::endl;
      }
   }
   return input_handler;
}



EagleEventHandler* EagleSystem::GetSystemQueue() {
	if (!system_queue) {
		system_queue = CreateEventHandler(false);
		if (!system_queue) {
		   EagleLog() << "Failed to retrieve system event handler." << std::endl;
		}
	}
	return system_queue;
}



EagleTimer* EagleSystem::GetSystemTimer() {
	if (!system_timer) {
		system_timer = CreateTimer();
		if (system_timer) {
			system_timer->Create(system_timer_rate);
		}
		else {
         EagleLog() << "Failed to retrieve system timer." << std::endl;
		}
	}
	return system_timer;
}



EagleInputHandler* EagleSystem::CreateInputHandler() {
   EAGLE_ASSERT(system_up);
   EagleInputHandler* input = PrivateCreateInputHandler();
   if (input) {
      inputs.Add(input);
   }
   return input;
}



EagleEventHandler* EagleSystem::CreateEventHandler(bool delay_events) {
   EAGLE_ASSERT(system_up);
   EagleEventHandler* q = PrivateCreateEventHandler(delay_events);
   if (q) {
      queues.Add(q);
   }
   return q;
}



EagleTimer* EagleSystem::CreateTimer() {
   EAGLE_ASSERT(system_up);
   EagleTimer* timer = PrivateCreateTimer();
   if (timer) {
      timers.Add(timer);
   }
   return timer;
}



EagleGraphicsContext* EagleSystem::CreateGraphicsContext(int width , int height , int flags) {
   EAGLE_ASSERT(system_up);
   EagleGraphicsContext* win = PrivateCreateGraphicsContext(width,height,flags);
   if (win) {
      if (system_queue) {
         win->RegisterDisplayInput(system_queue);
      }
      windows.Add(win);
   }
   return win;
}



EagleThread* EagleSystem::CreateThread(void* (*process)(EagleThread* , void*) , void* data) {
   EAGLE_ASSERT(system_up);
   EagleThread* ethread = PrivateCreateThread(process , data);
   if (ethread) {
      threads.Add(ethread);
   }
   return ethread;
}



EagleMutex* EagleSystem::CreateMutex(bool recursive) {
   EAGLE_ASSERT(system_up);
   EagleMutex* emutex = PrivateCreateMutex(recursive);
   if (emutex) {
      mutexes.Add(emutex);
   }
   return emutex;
}



void EagleSystem::RegisterKeyboardInput(EagleEventHandler* queue) {
	EagleInputHandler* input = GetInputHandler();
	if (input) {
		input->RegisterKeyboardInput(queue);
	}
}



void EagleSystem::RegisterMouseInput   (EagleEventHandler* queue) {
	EagleInputHandler* input = GetInputHandler();
	if (input) {
		input->RegisterMouseInput(queue);
	}
}



void EagleSystem::RegisterJoystickInput(EagleEventHandler* queue) {
	EagleInputHandler* input = GetInputHandler();
	if (input) {
		input->RegisterJoystickInput(queue);
	}
}



void EagleSystem::RegisterTouchInput   (EagleEventHandler* queue) {
	EagleInputHandler* input = GetInputHandler();
	if (input) {
		input->RegisterTouchInput(queue);
	}
}


	
void EagleSystem::RegisterInputs(EagleEventHandler* queue) {
	EagleInputHandler* input = GetInputHandler();
	EAGLE_ASSERT(input);
	if (input) {
		input->RegisterInputs(queue);
	}
}



bool EagleSystem::UpToDate() {
   EAGLE_ASSERT(system_up);
   EAGLE_ASSERT(system_queue);
   return !system_queue->HasEvent();
}



EagleEvent EagleSystem::UpdateSystemState() {
   EAGLE_ASSERT(system_up);
   EAGLE_ASSERT(system_queue);
   
   if (!UpToDate()) {
      EagleEvent e = system_queue->TakeNextEvent();
      most_recent_system_event = e;
      HandleInputEvent(e);
//
//      eagle_gui_ptr->CheckInputs();
//      if (e.type == ALLEGRO_EVENT_TIMER && e.timer.source == system_timer) {
//         eagle_gui_ptr->Update(system_timer->SecondsPerTick());
//      }
      return e;
   }
   else {
      return EagleEvent();
   }
}



EagleEvent EagleSystem::WaitForSystemEventAndUpdateState() {
   EAGLE_ASSERT(system_up);
   EAGLE_ASSERT(system_queue);
   EagleEvent e = system_queue->WaitForEvent();
   most_recent_system_event = e;
   HandleInputEvent(e);
//   eagle_gui_ptr->CheckInputs();
//   if (e.type == ALLEGRO_EVENT_TIMER && e.timer.source == system_timer) {
//      eagle_gui_ptr->Update(system_timer->SecondsPerTick());
//   }
   return e;
}



EagleEvent EagleSystem::TimedWaitForSystemEventAndUpdateState(double timeout) {
   EAGLE_ASSERT(system_up);
   EAGLE_ASSERT(system_queue);
   EagleEvent e = system_queue->WaitForEvent(timeout);
   if (e.type == EAGLE_EVENT_NONE) {return e;}
   most_recent_system_event = e;
   HandleInputEvent(e);
//   eagle_gui_ptr->CheckInputs();
//   if (e.type == ALLEGRO_EVENT_TIMER && e.timer.source == system_timer) {
//      eagle_gui_ptr->Update(system_timer->SecondsPerTick());
//   }
   return e;
}






