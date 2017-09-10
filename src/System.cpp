
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
 *    Copyright 2009-2017+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */


#include "Eagle/System.hpp"
#include "Eagle/Logging.hpp"
#include "Eagle/Exception.hpp"
#include "Eagle/Time.hpp"

#include <signal.h>

#include <string>
#include <sstream>


void shutdown_dll() {
   EagleInfo() << "EAGLE INFO : Shutting down Eagle DLL during atexit." << std::endl;
   return;
}



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





inline void signal_handler(int)
{
    EpicFail();
}
/**
inline void __cdecl invalid_parameter_handler(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t)
{
   EpicFail();
}
//*/


const char* const eagle_init_state_strs[12] = {
   "EAGLE_NOT_INSTALLED",
   "EAGLE_SYSTEM",
   "EAGLE_IMAGES",
   "EAGLE_FONTS",
   "EAGLE_TTF_FONTS",
   "EAGLE_AUDIO",
   "EAGLE_SHADERS",
   "EAGLE_PRIMITIVES",
   "EAGLE_KEYBOARD",
   "EAGLE_MOUSE",
   "EAGLE_JOYSTICK",
   "EAGLE_TOUCH"
};



std::string PrintEagleInitState(int state) {
   std::stringstream ss;
   if (state) {
      for (int bitshift = 0 ; bitshift < 11 ; ++bitshift) {
         int flag = 1 << bitshift;
         ss << ((state & flag)?"+":"-");
         ss << eagle_init_state_strs[bitshift + 1];
         state &= ~flag;
         if (state) {
            ss << " | ";
         }
      }
      return ss.str();
   }
   return eagle_init_state_strs[0];
}



std::string PrintFailedEagleInitStates(int desired_state , int actual_state) {
   std::stringstream ss;
   if (desired_state) {

      if (actual_state & ~desired_state) {
         ss << "EXTRA STATES TURNED ON!";
      }

      if (desired_state & ~actual_state) {
         ss << "FAILED STATES = ";
      }
      else {
         return "ALL DESIRED STATES SUCCEEDED TO INITIALIZE";
      }

      for (int bitshift = 0 ; bitshift < 11 ; ++bitshift) {
         int flag = 1 << bitshift;

         if ((desired_state & flag) && !(actual_state & flag)) {
            ss << "-" << eagle_init_state_strs[bitshift + 1] << "-";
         }
         desired_state &= ~flag;
         actual_state &= ~flag;

         if (desired_state ^ actual_state) {
            ss << " | ";
         }
      }
      return ss.str();
   }
   return eagle_init_state_strs[0];
}



EagleSystem* eagle_system = 0;



EagleEvent most_recent_system_event = EagleEvent();



const int EAGLE_STANDARD_INPUT = EAGLE_KEYBOARD | EAGLE_MOUSE | EAGLE_JOYSTICK | EAGLE_TOUCH;
const int EAGLE_STANDARD_SYSTEM = EAGLE_SYSTEM | EAGLE_IMAGES | EAGLE_FONTS | EAGLE_TTF_FONTS | EAGLE_AUDIO;
const int EAGLE_GENERAL_SETUP = EAGLE_STANDARD_INPUT | EAGLE_STANDARD_SYSTEM;
const int EAGLE_FULL_SETUP = EAGLE_GENERAL_SETUP | EAGLE_SHADERS | EAGLE_PRIMITIVES;



float EagleSystem::system_timer_rate = 1.0f/60.0f;



EagleSystem::EagleSystem(std::string objclass , std::string objname) :
   EagleObject(objclass , objname),
   queues(true),
   inputs(true),
   timers(true),
   threads(true),
   mutexes(true),
   clipboards(true),
   input_handler(0),
   system_timer(0),
   system_queue(0),
   system_clipboard(0),
   window_manager(0),
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
   atexit(shutdown_dll);
}



void EagleSystem::Shutdown() {

   EagleInfo() << "EagleSystem::Shutdown called" << std::endl;

   if (window_manager) {
      delete window_manager;
      window_manager = 0;
   }

   /// TODO : Manage destruction order carefully...
   inputs.FreeAll();
   clipboards.FreeAll();
   timers.FreeAll();
   threads.FreeAll();
   queues.FreeAll();
   mutexes.FreeAll();
}



int EagleSystem::Initialize(int state) {
   state |= EAGLE_SYSTEM;/// System is non-optional

   if (state & EAGLE_SYSTEM)     {InitializeSystem();}
   if (state & EAGLE_IMAGES)     {InitializeImages();}
   if (state & EAGLE_FONTS)      {InitializeFonts();}
   if (state & EAGLE_TTF_FONTS)  {InitializeTTFFonts();}
   if (state & EAGLE_AUDIO)      {InitializeAudio();}
   if (state & EAGLE_SHADERS)    {InitializeShaders();}
   if (state & EAGLE_PRIMITIVES) {InitializePrimitives();}

   state |= EAGLE_KEYBOARD | EAGLE_MOUSE | EAGLE_JOYSTICK | EAGLE_TOUCH;/// Kludge making them non-optional TODO : DO something about it

   if (state & EAGLE_KEYBOARD)   {InstallKeyboard();}
   if (state & EAGLE_MOUSE)      {InstallMouse();}
   if (state & EAGLE_JOYSTICK)   {InstallJoystick();}
   if (state & EAGLE_TOUCH)      {InstallTouch();}

   FinalizeSystem();

   return EagleInitState();
}



bool EagleSystem::InitializeSystem() {

///   int success = (atexit(EagleShutdown) == 0);
///   EagleInfo() << StringPrintF("EagleSystem::InitializeSystem - atexit ptr is %p and atexit(EagleShutdown) returned %d\n" ,
///                              atexit , success);

   signal(SIGABRT, signal_handler);
   // _set_abort_behavior(0, _WRITE_ABORT_MSG|_CALL_REPORTFAULT);

    std::set_terminate(EpicFail );
    std::set_unexpected(EpicFail );
   // _set_purecall_handler( &terminator );
   // _set_invalid_parameter_handler( &invalid_parameter_handler );



   system_up = PrivateInitializeSystem();
   if (!system_up) {
      EagleError() << "Eagle : Failed to initialize the system." << std::endl;
   }
   else {
      EagleInfo() << "Eagle : Initialized system." << std::endl;
   }

   return system_up;
}



bool EagleSystem::FinalizeSystem() {
   if (!input_handler)    {input_handler    = CreateInputHandler();}
   if (!system_timer)     {system_timer     = CreateTimer();}
   if (!system_queue)     {system_queue     = CreateEventHandler(false);}
   if (!system_clipboard) {system_clipboard = CreateClipboard();}
   if (!window_manager)   {window_manager   = CreateWindowManager();}

   system_up = (system_up && input_handler && system_timer && system_queue && system_clipboard && window_manager);

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
      EagleInfo() << "Eagle : Finalized the system state." << std::endl;
//      register_system_shutdown_function();
   }
   else {
      EagleError() << "Eagle : System state not finalized." << std::endl;
   }

   return system_up;
}



bool EagleSystem::InitializeImages() {
   images_up = PrivateInitializeImages();
   if (!images_up) {
      EagleError() << "Eagle : Failed to initialize images." << std::endl;
   }
   else {
      EagleInfo() << "Eagle : Initialized images." << std::endl;
   }
   return images_up;
}



bool EagleSystem::InitializeFonts() {
   fonts_up = PrivateInitializeFonts();
   if (!fonts_up) {
      EagleError() << "Eagle : Failed to initialize fonts." << std::endl;
   }
   else {
      EagleInfo() << "Eagle : Initialized fonts." << std::endl;
   }
   return fonts_up;
}



bool EagleSystem::InitializeTTFFonts() {
   ttf_fonts_up = PrivateInitializeTTFFonts();
   if (!ttf_fonts_up) {
      EagleError() << "Eagle : Failed to initialize ttf fonts." << std::endl;
   }
   else {
      EagleInfo() << "Eagle : Initialized TTF fonts." << std::endl;
   }
   return ttf_fonts_up;
}



bool EagleSystem::InitializeAudio() {
   audio_up = PrivateInitializeAudio();
   if (!audio_up) {
      EagleError() << "Eagle : Failed to initialize audio." << std::endl;
   }
   else {
      EagleInfo() << "Eagle : Initialized audio." << std::endl;
   }
   return audio_up;
}



bool EagleSystem::InitializeShaders() {
   shaders_up = PrivateInitializeShaders();
   if (!shaders_up) {
      EagleError() << "Eagle : Failed to initialize shaders." << std::endl;
   }
   else {
      EagleInfo() << "Eagle : Initialized shaders." << std::endl;
   }
   return shaders_up;
}



bool EagleSystem::InitializePrimitives() {
   primitives_up = PrivateInitializePrimitives();
   if (!primitives_up) {
      EagleError() << "Eagle : Failed to initilialize primitives." << std::endl;
   }
   else {
      EagleInfo() << "Eagle : Initialized primitives." << std::endl;
   }
   return primitives_up;
}



bool EagleSystem::InstallKeyboard() {
   keyboard_running = PrivateInstallKeyboard();
   if (!keyboard_running) {
      EagleError() << "Eagle : Failed to install keyboard." << std::endl;
   }
   else {
      EagleInfo() << "Eagle : Installed keyboard." << std::endl;
   	EagleInputHandler* input = GetInputHandler();
      EagleEventHandler* queue = GetSystemQueue();

   	if (input) {
   		input->InitializeKeyboardInput();
   		input->StartKeyboardEventHandler();
   	}
      if (queue) {
         queue->ListenTo(input);
      }
   }
   return keyboard_running;
}



bool EagleSystem::InstallMouse() {
   mouse_running = PrivateInstallMouse();
   if (!mouse_running) {
      EagleError() << "Eagle : Failed to install mouse." << std::endl;
   }
   else {
      EagleInfo() << "Eagle : Installed mouse." << std::endl;
   	EagleInputHandler* input = GetInputHandler();
   	if (input) {
   		input->InitializeMouseInput();
   		input->StartMouseEventHandler();
         EagleEventHandler* queue = GetSystemQueue();
   		if (queue) {
				queue->ListenTo(input);
   		}
   	}
   }
   return mouse_running;
}



bool EagleSystem::InstallJoystick() {
   joystick_running = PrivateInstallJoystick();
   if (!joystick_running) {
      EagleError() << "Eagle : Failed to install joystick." << std::endl;
   }
   else {
      EagleInfo() << "Eagle : Installed joystick." << std::endl;
   	EagleInputHandler* input = GetInputHandler();
   	if (input) {
   		input->InitializeJoystickInput();
   		input->StartJoystickEventHandler();
   		EagleEventHandler* queue = GetSystemQueue();
   		if (queue) {
				queue->ListenTo(input);
   		}
   	}
   }
   return joystick_running;
}



bool EagleSystem::InstallTouch() {
   touch_running = PrivateInstallTouch();
   if (!touch_running) {
      EagleError() << "Eagle : Failed to install touch input." << std::endl;
   }
   else {
      EagleInfo() << "Eagle : Installed touch input." << std::endl;
   	EagleInputHandler* input = GetInputHandler();
   	if (input) {
   		input->InitializeTouchInput();
   		input->StartTouchEventHandler();
   		EagleEventHandler* queue = GetSystemQueue();
   		if (queue) {
				queue->ListenTo(input);
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
         throw EagleException("Failed to retrieve input handler.");
      }
   }
   return input_handler;
}



EagleEventHandler* EagleSystem::GetSystemQueue() {
	if (!system_queue) {
		system_queue = CreateEventHandler(false);
		if (!system_queue) {
		   throw EagleException("Failed to retrieve system event handler.");
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
         throw EagleException("EagleSystem::GetSystemTimer - Failed to retrieve system timer.");
		}
	}
	return system_timer;
}



EagleClipboard* EagleSystem::GetSystemClipboard() {
   return system_clipboard;
}



EagleWindowManager* EagleSystem::GetWindowManager() {
   return window_manager;
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


/**
EagleEventHandler* EagleSystem::CreateEventHandlerDuplicate(EagleEventHandler* handler) {
   EagleEventHandler* clone = handler->CloneEventHandler();
   if (clone) {
      queues.Add(clone);
   }
   return clone;
}
//*/


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
         system_queue->ListenTo(win);
///         win->RegisterDisplayInput(system_queue);
      }
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



EagleMutex* EagleSystem::CreateMutex(bool recursive , bool timed) {
   EAGLE_ASSERT(system_up);
   EagleMutex* emutex = PrivateCreateMutex(recursive , timed);
   if (emutex) {
      mutexes.Add(emutex);
   }
   return emutex;
}



EagleClipboard* EagleSystem::CreateClipboard() {
   EAGLE_ASSERT(system_up);
   EagleClipboard* cb = PrivateCreateClipboard();
   if (cb) {
      clipboards.Add(cb);
   }
   return cb;
}



void EagleSystem::FreeInputHandler(EagleInputHandler* handler) {
   inputs.Free(handler);
}



void EagleSystem::FreeEventHandler(EagleEventHandler* event_handler) {
   queues.Free(event_handler);
}



void EagleSystem::FreeTimer(EagleTimer* timer) {
   timers.Free(timer);
}



void EagleSystem::FreeGraphicsContext(EagleGraphicsContext* window) {
   window_manager->DestroyWindow(window->GetEagleId());
}



void EagleSystem::FreeThread(EagleThread* thread) {
   threads.Free(thread);
}



void EagleSystem::FreeMutex(EagleMutex* mutex) {
   mutexes.Free(mutex);
}



void EagleSystem::FreeClipboard(EagleClipboard* clipboard) {
   clipboards.Free(clipboard);
}


/*
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
		queue->ListenTo(input);
///		input->RegisterInputs(queue);
	}
}

*/

bool EagleSystem::UpToDate() {
   EAGLE_ASSERT(system_up);
   EAGLE_ASSERT(system_queue);
   return !system_queue->HasEvent(0);
}



EagleEvent EagleSystem::UpdateSystemState() {
   EAGLE_ASSERT(system_up);
   EAGLE_ASSERT(system_queue);

   if (!UpToDate()) {
      EagleEvent e = system_queue->TakeNextEvent(0);
      most_recent_system_event = e;
      input_handler->HandleInputEvent(e);
      return e;
   }
   else {
      return EagleEvent();
   }
}



EagleEvent EagleSystem::WaitForSystemEventAndUpdateState() {
   EAGLE_ASSERT(system_up);
   EAGLE_ASSERT(system_queue);
   EagleEvent e = system_queue->WaitForEvent(0);
   most_recent_system_event = e;
   input_handler->HandleInputEvent(e);
   return e;
}



EagleEvent EagleSystem::TimedWaitForSystemEventAndUpdateState(double timeout) {
   EAGLE_ASSERT(system_up);
   EAGLE_ASSERT(system_queue);
   EagleEvent e = system_queue->WaitForEvent(timeout , 0);
   if (e.type == EAGLE_EVENT_NONE) {return e;}
   most_recent_system_event = e;
   input_handler->HandleInputEvent(e);
   return e;
}



double EagleSystem::GetProgramTime() {
   return ProgramTime::Now() - ProgramTime::ProgramStart();
}



EagleGraphicsContext* EagleSystem::GetActiveWindow() {
   return window_manager->GetActiveWindow();
}



