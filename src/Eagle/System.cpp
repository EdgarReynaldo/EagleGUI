
/**
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
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */


#include "Eagle/System.hpp"
#include "Eagle/Logging.hpp"
#include "Eagle/Exception.hpp"
#include "Eagle/Time.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/FileSystem.hpp"
#include "Eagle/ResourceLib.hpp"


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





inline void signal_handler(int sig)
{
   EagleError() << StringPrintF("EAGLE caught signal %d" , sig) << std::endl;
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



float EagleSystem::system_timer_rate = 1.0f/60.0f;



EagleSystem::EagleSystem(std::string objclass , std::string objname) :
   EagleObject(objclass , objname),
   queues(),
   inputs(),
   timers(),
   threads(),
   mutexes(),
   clipboards(),
   input_handler(0),
   system_timer(0),
   system_queue(0),
   system_clipboard(0),
   window_manager(0),
   system_transformer(0),
   file_system(0),
   resource_library(0),
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

   if (system_transformer) {
      delete system_transformer;
      system_transformer = 0;
   }
   if (file_system) {
      delete file_system;
      file_system = 0;
   }
   
   if (resource_library) {
      delete resource_library;
      resource_library = 0;
   }

   /// TODO : Manage destruction order carefully...
   inputs.RemoveAll();
   clipboards.RemoveAll();
   timers.RemoveAll();
   threads.RemoveAll();
   queues.RemoveAll();
   mutexes.RemoveAll();
}



int EagleSystem::Initialize(int state) {
   int desired_state = state;

   state |= EAGLE_SYSTEM;/// System is non-optional

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

   FinalizeSystem();

   PrintFailedEagleInitStates(desired_state , EagleInitState());

   return EagleInitState();
}



bool EagleSystem::InitializeSystem() {

///   int success = (atexit(EagleShutdown) == 0);
///   EagleInfo() << StringPrintF("EagleSystem::InitializeSystem - atexit ptr is %p and atexit(EagleShutdown) returned %d\n" ,
///                              atexit , success);

   signal(SIGABRT, signal_handler);
   signal(SIGTERM, signal_handler);
   // _set_abort_behavior(0, _WRITE_ABORT_MSG|_CALL_REPORTFAULT);

//    std::set_terminate(EpicFail );
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
   if (!input_handler)      {input_handler    = GetSystemInput();}
   if (!system_timer)       {system_timer     = GetSystemTimer();}
   if (!system_queue)       {system_queue     = GetSystemQueue();}
   if (!system_clipboard)   {system_clipboard = CreateClipboard();}
   if (!window_manager)     {window_manager   = CreateWindowManager();}
   if (!system_transformer) {system_transformer = PrivateCreateTransformer();}

   system_up = (system_up && input_handler && system_timer && system_queue && system_clipboard && window_manager);

   if (system_timer) {
      if (system_timer->Valid()) {
         SetInputTimer(system_timer);
         if (system_queue) {
            system_queue->ListenTo(system_timer);
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
   	EagleInputHandler* input = GetSystemInput();
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
   	EagleInputHandler* input = GetSystemInput();
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
   	EagleInputHandler* input = GetSystemInput();
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
   	EagleInputHandler* input = GetSystemInput();
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



EagleInputHandler* EagleSystem::GetSystemInput() {
   if (!input_handler) {
      std::string input_name_str = StringPrintF("%sSystem::input_handler" , GetSystemName());
      input_handler = CreateInputHandler(input_name_str);
      if (!input_handler) {
         throw EagleException("Failed to retrieve input handler.");
      }
   }
   return input_handler;
}



EagleEventHandler* EagleSystem::GetSystemQueue() {
	if (!system_queue) {
      std::string queue_name_str = StringPrintF("%sSystem::system_queue" , GetSystemName());
		system_queue = CreateEventHandler(queue_name_str , false);
		if (!system_queue) {
		   throw EagleException("Failed to retrieve system event handler.");
		}
	}
	return system_queue;
}



EagleTimer* EagleSystem::GetSystemTimer() {
	if (!system_timer) {
      std::string timer_name_str = StringPrintF("%sSystem::system_timer" , GetSystemName());
		system_timer = CreateTimer(timer_name_str);
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



Transformer* EagleSystem::GetSystemTransformer() {
   return system_transformer;
}



FileSystem* EagleSystem::GetFileSystem() {
   if (!file_system) {
      file_system = CreateFileSystem();
   }
   return file_system;
}



ResourceLibrary* EagleSystem::GetResourceLibrary() {
   if (!resource_library) {
      resource_library = CreateResourceLibrary();
   }
   return resource_library;
}



EagleInputHandler* EagleSystem::CreateInputHandler(std::string objname) {
   EAGLE_ASSERT(system_up);
   EagleInputHandler* input = PrivateCreateInputHandler(objname);
   if (input) {
      inputs.Add(HeapObject(input));
   }
   return input;
}



EagleEventHandler* EagleSystem::CreateEventHandler(std::string objname , bool delay_events) {
   EAGLE_ASSERT(system_up);
   EagleEventHandler* q = PrivateCreateEventHandler(objname , delay_events);
   if (q) {
      queues.Add(HeapObject(q));
   }
   return q;
}



EagleTimer* EagleSystem::CreateTimer(std::string objname) {
   EAGLE_ASSERT(system_up);
   EagleTimer* timer = PrivateCreateTimer(objname);
   if (timer) {
      timers.Add(HeapObject(timer));
   }
   return timer;
}



EagleGraphicsContext* EagleSystem::CreateGraphicsContext(std::string objname , int width , int height , int flags) {
   EAGLE_ASSERT(system_up);
   EagleGraphicsContext* win = PrivateCreateGraphicsContext(objname , width , height , flags);
   if (win) {
      if (system_queue) {
         system_queue->ListenTo(win);
      }
   }
   return win;
}



EagleGraphicsContext* EagleSystem::CreatePopupWindow(std::string objname , int width , int height , int flags) {
   flags |= EAGLE_NOFRAME;/// Specify no frame
   flags &= ~EAGLE_FULLSCREEN;/// Remove fullscreen
   flags |= EAGLE_WINDOWED;/// Always windowed
   return CreateGraphicsContext(objname , width , height , flags);
}



EagleThread* EagleSystem::CreateThread(std::string objname , void* (*process)(EagleThread* , void*) , void* data) {
   EAGLE_ASSERT(system_up);
   EagleThread* ethread = PrivateCreateThread(objname , process , data);
   if (ethread) {
      threads.Add(HeapObject(ethread));
   }
   return ethread;
}



EagleMutex* EagleSystem::CreateMutex(std::string objname , bool recursive , bool timed) {
   EAGLE_ASSERT(system_up);
   EagleMutex* emutex = PrivateCreateMutex(objname , recursive , timed);
   if (emutex) {
      mutexes.Add(HeapObject(emutex));
   }
   return emutex;
}



EagleClipboard* EagleSystem::CreateClipboard(std::string objname) {
   EAGLE_ASSERT(system_up);
   EagleClipboard* cb = PrivateCreateClipboard(objname);
   if (cb) {
      clipboards.Add(HeapObject(cb));
   }
   return cb;
}



void EagleSystem::FreeInputHandler(EagleInputHandler* handler) {
   inputs.Remove(handler);
}



void EagleSystem::FreeEventHandler(EagleEventHandler* event_handler) {
   queues.Remove(event_handler);
}



void EagleSystem::FreeTimer(EagleTimer* timer) {
   timers.Remove(timer);
}



void EagleSystem::FreeGraphicsContext(EagleGraphicsContext* window) {
   window_manager->DestroyWindow(window->GetEagleId());
}



void EagleSystem::FreeThread(EagleThread* thread) {
   threads.Remove(thread);
}



void EagleSystem::FreeMutex(EagleMutex* mutex) {
   mutexes.Remove(mutex);
}



void EagleSystem::FreeClipboard(EagleClipboard* clipboard) {
   clipboards.Remove(clipboard);
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
   EagleEvent e = system_queue->WaitForEvent(MAIN_THREAD);
   most_recent_system_event = e;
   input_handler->HandleInputEvent(e);
   return e;
}



EagleEvent EagleSystem::TimedWaitForSystemEventAndUpdateState(double timeout) {
   EAGLE_ASSERT(system_up);
   EAGLE_ASSERT(system_queue);
   EagleEvent e = system_queue->WaitForEvent(timeout , MAIN_THREAD);
   if (e.type == EAGLE_EVENT_NONE) {return e;}
   most_recent_system_event = e;
   input_handler->HandleInputEvent(e);
   return e;
}



EagleGraphicsContext* EagleSystem::GetActiveWindow() {
   return window_manager->GetActiveWindow();
}



double EagleSystem::GetProgramTime() {
   return (double)ProgramTime::Now() - ProgramTime::ProgramStart();
}



