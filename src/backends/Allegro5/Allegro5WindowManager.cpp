


#include "Eagle/backends/Allegro5/Allegro5WindowManager.hpp"

#include "Eagle/Lib.hpp"
#include "Eagle/StringWork.hpp"


#include "Eagle/backends/Allegro5/Allegro5GraphicsContext.hpp"
#include "Eagle/backends/Allegro5/Allegro5Mutex.hpp"
#include "Eagle/backends/Allegro5/Allegro5Threads.hpp"



static const unsigned int EAGLE_EVENT_WM_CLOSE_WINDOW = NextFreeEagleEventId();
static const unsigned int EAGLE_EVENT_WM_CLOSE_WINDOW_RECEIVED = NextFreeEagleEventId();
static const unsigned int EAGLE_EVENT_WM_DESTROY = NextFreeEagleEventId();



void* A5WindowManagerProcess(EagleThread* thread , void* manager) {

   Allegro5WindowManager* a5man = dynamic_cast<Allegro5WindowManager*>((EagleWindowManager*)manager);
   EAGLE_ASSERT(a5man);

   ALLEGRO_EVENT_QUEUE* window_queue = a5man->window_queue;

   while (1) {
      if (thread->ShouldStop()) {
         EagleInfo() << "A5WindowManagerProcess - thread has been signaled to stop." << std::endl;
         break;
      }
      ALLEGRO_EVENT ev;
      al_wait_for_event(window_queue , &ev);

      if (ev.type == EAGLE_EVENT_WM_CLOSE_WINDOW) {

         EagleInfo() << "EAGLE_EVENT_WM_CLOSE_WINDOW received by A5WindowManagerProcess" << std::endl;

         Allegro5GraphicsContext* window = (Allegro5GraphicsContext*)ev.user.data1;
         EAGLE_ASSERT(window);
         al_unregister_event_source(window_queue , al_get_display_event_source(window->AllegroDisplay()));

         ALLEGRO_EVENT_SOURCE* response_event_source = &(a5man->response_event_source);

         ALLEGRO_EVENT ev2;
         ev2.type = EAGLE_EVENT_WM_CLOSE_WINDOW_RECEIVED;
         ev2.user.data1 = (intptr_t)window;
         al_emit_user_event(response_event_source , &ev2 , 0);

         continue;
      }

      if (ev.type == EAGLE_EVENT_WM_DESTROY) {

         EAGLE_ASSERT(a5man->WindowCount() == 0);

         EagleInfo() << "EAGLE_EVENT_WM_DESTROY received by A5WindowManagerProcess" << std::endl;

         if (ev.user.data1 == -1) {
            /// Received signal to close, stop listening
            break;
         }
      }

      EagleEvent ee = a5man->GetEagleDisplayEvent(ev);
      EagleInfo() << "Allegro5WindowManagerProcess received event " << EagleEventName(ee.type) << std::endl;

      if (ee.type == EAGLE_EVENT_DISPLAY_SWITCH_IN) {
         a5man->SwitchIn(ee.window);
      }
      else if (ee.type == EAGLE_EVENT_DISPLAY_SWITCH_OUT) {
         a5man->SwitchOut(ee.window);
      }

      ee.window->EmitEvent(ee);

   }
   return 0;
}



/// ----------------------------      Allegro5WindowManager    -----------------------




void Allegro5WindowManager::AddDisplay(EagleGraphicsContext* window , ALLEGRO_DISPLAY* display) {
   if (!display || !window) {
      if (!window) {
         throw EagleException("Allegro5WindowManager::AddDisplay - window is null!");
      }
      if (!display) {
         throw EagleException("Allegro5WindowManager::AddDisplay - display is null!");
      }
   }
   EAGLE_ASSERT(manager_mutex && manager_mutex->Valid());
   manager_mutex->Lock();
   display_context_map[display] = window;
   EagleInfo() << StringPrintF("Allegro5WindowManager::AddDisplay - adding ALLEGRO_DISPLAY* %p ." , display) << std::endl;
   manager_mutex->Unlock();
}



void Allegro5WindowManager::RemoveDisplay(ALLEGRO_DISPLAY* display) {
   if (!display) {return;}
   EAGLE_ASSERT(manager_mutex && manager_mutex->Valid());
   manager_mutex->Lock();
   std::map<ALLEGRO_DISPLAY* , EagleGraphicsContext*>::iterator it = display_context_map.find(display);
   if (it != display_context_map.end()) {
      display_context_map.erase(it);
      EagleInfo() << StringPrintF("Allegro5WindowManager::RemoveDisplay - removing ALLEGRO_DISPLAY* %p ." , display) << std::endl;
   }
   manager_mutex->Unlock();
}



void Allegro5WindowManager::SignalClose(Allegro5GraphicsContext* window) {
   ALLEGRO_EVENT ev;
   ev.type = EAGLE_EVENT_WM_CLOSE_WINDOW;
   ev.user.data1 = (intptr_t)window;/// TODO : Not safe for 64 bit pointers
   al_emit_user_event(&window_event_source , &ev , 0);

   al_wait_for_event(response_queue , &ev);
   EAGLE_ASSERT(ev.type == EAGLE_EVENT_WM_CLOSE_WINDOW_RECEIVED);
   EAGLE_ASSERT(ev.user.data1 == (intptr_t)window);

   EagleInfo() << StringPrintF("SignalClose acknowledged for window %p" , window) << std::endl;
}



EagleEvent Allegro5WindowManager::GetEagleDisplayEvent(ALLEGRO_EVENT ev) {
   EagleEvent ee;

   Allegro5GraphicsContext* a5win = 0;

   a5win = dynamic_cast<Allegro5GraphicsContext*>(GetAssociatedContext(ev.display.source));

   ee.display.x = ev.display.x;
   ee.display.y = ev.display.y;
   ee.display.width = ev.display.width;
   ee.display.height = ev.display.height;
   ee.display.orientation = ev.display.orientation;

   EAGLE_ASSERT(a5win);

   ee.source = a5win;
   ee.window = a5win;


   switch(ev.type) {
/*
   case EAGLE_EVENT_DISPLAY_CREATE :
      ee.type = EAGLE_EVENT_DISPLAY_CREATE;
      break;
   case EAGLE_EVENT_DISPLAY_DESTROY :
      ee.type = EAGLE_EVENT_DISPLAY_DESTROY;
      break;
*/
   case ALLEGRO_EVENT_DISPLAY_EXPOSE :
      ee.type = EAGLE_EVENT_DISPLAY_EXPOSE;
      break;
   case ALLEGRO_EVENT_DISPLAY_RESIZE :
      ee.type = EAGLE_EVENT_DISPLAY_RESIZE;
      break;
   case ALLEGRO_EVENT_DISPLAY_CLOSE :
      ee.type = EAGLE_EVENT_DISPLAY_CLOSE;
      break;
   case ALLEGRO_EVENT_DISPLAY_LOST :
      ee.type = EAGLE_EVENT_DISPLAY_LOST;
      break;
   case ALLEGRO_EVENT_DISPLAY_FOUND :
      ee.type = EAGLE_EVENT_DISPLAY_FOUND;
      break;
   case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT :
      ee.type = EAGLE_EVENT_DISPLAY_SWITCH_OUT;
      break;
   case ALLEGRO_EVENT_DISPLAY_SWITCH_IN :
      ee.type = EAGLE_EVENT_DISPLAY_SWITCH_IN;
      break;
   case ALLEGRO_EVENT_DISPLAY_ORIENTATION :
      ee.type = EAGLE_EVENT_DISPLAY_ORIENTATION;
      break;
   case ALLEGRO_EVENT_DISPLAY_HALT_DRAWING :
      ee.type = EAGLE_EVENT_DISPLAY_HALT_DRAWING;
      break;
   case ALLEGRO_EVENT_DISPLAY_RESUME_DRAWING :
      ee.type = EAGLE_EVENT_DISPLAY_RESUME_DRAWING;
      break;

   default :
      throw EagleException(StringPrintF("A5WindowProcess : We don't monitor this kind of event (%d)." , ev.type));
   }
   return ee;
}



EagleGraphicsContext* Allegro5WindowManager::PrivateCreateWindow(int width , int height , int flags) {
   Allegro5GraphicsContext* a5win = new Allegro5GraphicsContext(width , height , flags);
   EAGLE_ASSERT(a5win && a5win->Valid());
   al_register_event_source(window_queue , al_get_display_event_source(a5win->AllegroDisplay()));
   return a5win;
}





Allegro5WindowManager::Allegro5WindowManager(EagleSystem* sys) :
      EagleWindowManager(sys),
      display_context_map(),
      window_queue(0),
      window_event_source(),
      response_queue(0),
      response_event_source()
{
   Create();
}



Allegro5WindowManager::~Allegro5WindowManager() {
   Destroy();
}



bool Allegro5WindowManager::Create() {

   Destroy();

   al_init_user_event_source(&window_event_source);
   al_init_user_event_source(&response_event_source);

   window_queue = al_create_event_queue();
   response_queue = al_create_event_queue();

   if (!window_queue) {
      throw EagleException("Allegro5WindowManager::Create - Failed to create allegro event queue.");
   }
   if (!response_queue) {
      throw EagleException("Allegro5WindowManager::Create - Failed to create allegro event queue.");
   }

   al_register_event_source(window_queue , &window_event_source);
   al_register_event_source(response_queue , &response_event_source);

   manager_mutex = new Allegro5Mutex();

   manager_thread = new Allegro5Thread();

   if (!manager_mutex->Create(true)) {
      throw EagleException("Allegro5WindowManager::Allegro5WindowManager - failed to create the window manager mutex.");
   }

   if (!manager_thread->Create(A5WindowManagerProcess , this)) {
      throw EagleException("Allegro5WindowManager::Allegro5WindowManager - failed to create the window manager thread.");
   }

   manager_thread->Start();

   return window_queue && manager_mutex && manager_mutex->Valid() && manager_thread && manager_thread->Running();
}



void Allegro5WindowManager::Destroy() {

   CloseWindows();

   ALLEGRO_EVENT ev;
   ev.type = EAGLE_EVENT_WM_DESTROY;
   ev.user.data1 = -1;

   al_emit_user_event(&window_event_source , &ev , 0);

   if (manager_thread && manager_thread->Running()) {
      manager_thread->FinishThread();
   }

   if (window_queue) {
      al_destroy_event_queue(window_queue);
      window_queue = 0;
   }
   if (response_queue) {
      al_destroy_event_queue(response_queue);
      response_queue = 0;
   }
   if (manager_mutex) {
      delete manager_mutex;
      manager_mutex = 0;
   }
   if (manager_thread) {
      delete manager_thread;
      manager_thread = 0;
   }
}



EagleGraphicsContext* Allegro5WindowManager::GetAssociatedContext(ALLEGRO_DISPLAY* display) {

   EagleGraphicsContext* window = 0;

   EAGLE_ASSERT(manager_mutex && manager_mutex->Valid());

   manager_mutex->Lock();

   std::map<ALLEGRO_DISPLAY* , EagleGraphicsContext*>::iterator it = display_context_map.find(display);
   if (it != display_context_map.end()) {
      window = it->second;
   }
   else {
      EagleWarn() << StringPrintF("Failed to retrieve window for ALLEGRO_DISPLAY* %p.\n" , display);
   }

   manager_mutex->Unlock();

   return window;
}



Allegro5WindowManager* GetAllegro5WindowManager() {
   return dynamic_cast<Allegro5WindowManager*>(Eagle::EagleLibrary::System("Allegro5")->GetWindowManager());
}


#undef Lock
#undef Unlock

#include "Eagle/Platform.hpp"

#ifdef EAGLE_WIN32
   #include "allegro5/allegro_windows.h"
///   #include "windows.h"
#endif




int Allegro5WindowManager::PrivateGiveWindowFocus(int window_eid) {
   EagleGraphicsContext* win = 0;
   EAGLE_ASSERT(manager_mutex && manager_mutex->Valid());
   manager_mutex->DoLock(EAGLE__FUNC);
   win = window_map[window_eid];
   manager_mutex->DoUnlock(EAGLE__FUNC);
   Allegro5GraphicsContext* a5win = dynamic_cast<Allegro5GraphicsContext*>(win);
   if (!a5win) {
      return -1;
   }
#ifdef EAGLE_WIN32
   HWND hwnd = al_get_win_window_handle(a5win->AllegroDisplay());
   SetFocus(hwnd);
#endif
   return -1;
}




