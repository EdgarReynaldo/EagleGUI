

#include "Eagle/WindowManager.hpp"
#include "Eagle/System.hpp"
#include "Eagle/Mutexes.hpp"
#include "Eagle/Threads.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/Object.hpp"



void EagleWindowManager::SwitchIn(EagleGraphicsContext* window) {
   EAGLE_ASSERT(window);
   EAGLE_ASSERT(manager_mutex && manager_mutex->Valid());

   ThreadLockMutex(our_thread , manager_mutex);

   WMIT it = window_map.find(window->GetEagleId());
   EAGLE_ASSERT(it != window_map.end());
   EAGLE_ASSERT(it->second);
   active_window = window;

   ThreadUnLockMutex(our_thread , manager_mutex);
}



void EagleWindowManager::SwitchOut(EagleGraphicsContext* window) {
   EAGLE_ASSERT(window);
   EAGLE_ASSERT(manager_mutex && manager_mutex->Valid());

   ThreadLockMutex(our_thread , manager_mutex);

   EAGLE_ASSERT(window_map.find(window->GetEagleId()) != window_map.end());

   if (active_window == window) {
      active_window = 0;/// We don't know which window will become active, but this one was, so zero the active_window
   }

   ThreadUnLockMutex(our_thread , manager_mutex);
}






EagleWindowManager::EagleWindowManager(EagleSystem* sys , std::string objclass , std::string objname) :
      EagleObject(objclass , objname),
      EagleEventSource(),
      parent_system(sys),
      manager_thread(0),
      manager_mutex(0),
      window_map(),
      window_count(0),
      active_window(0),
      our_thread(0)
{}



EagleWindowManager::~EagleWindowManager() {
   CloseWindows();
}



void EagleWindowManager::CloseWindows() {
   if (window_count < 1) {
      return;
   }

   WINMAP winmap;

   EAGLE_ASSERT(manager_mutex && manager_mutex->Valid());

   ThreadLockMutex(our_thread , manager_mutex);
   winmap = window_map;
   ThreadUnLockMutex(our_thread , manager_mutex);

   for (WMIT it = winmap.begin() ; it != winmap.end() ; ++it) {
      if (it->second != 0) {
         EAGLE_ASSERT(GetValidById(it->first));
         EagleLog() << StringPrintF("EagleWindowManager::CloseWindows - About to close window %s." , it->second->FullName()) << std::endl;
         DestroyWindow(it->first);
      }
   }

   EAGLE_ASSERT(window_count == 0);
}



EagleGraphicsContext* EagleWindowManager::CreateWindow(int width , int height , int flags) {
   EagleGraphicsContext* window = PrivateCreateWindow(width , height , flags);
   if (window) {
      if (!window->Valid()) {
         throw EagleException("EagleWindowManager::CreateWindow - Failed to create valid window!");
      }

      EAGLE_ASSERT(manager_mutex && manager_mutex->Valid());

      ThreadLockMutex(our_thread , manager_mutex);

      window_map[window->GetEagleId()] = window;
      ++window_count;
      active_window = window;

      ThreadUnLockMutex(our_thread , manager_mutex);

      EAGLE_ASSERT(parent_system);
      
      parent_system->GetSystemQueue()->ListenTo(window);
      
      return window;
   }
   return 0;
}



void EagleWindowManager::DestroyWindow(int window_eid) {

   EAGLE_ASSERT(manager_mutex && manager_mutex->Valid());


   ThreadLockMutex(our_thread , manager_mutex);

   WMIT it = window_map.find(window_eid);
   if (it == window_map.end()) {
      EagleWarn() << "EagleWindowManager::DestroyWindow - attempting to destroy an unregistered window. Ignored." << std::endl;
      ThreadUnLockMutex(our_thread , manager_mutex);
      return;
   }
   if (it->second == (EagleGraphicsContext*)0) {
      EagleWarn() << "EagleWindowManager::DestroyWindow - attempting to destroy null window. Ignored." << std::endl;
      ThreadUnLockMutex(our_thread , manager_mutex);
      return;
   }

   EagleGraphicsContext* window = window_map[window_eid];

   ThreadUnLockMutex(our_thread , manager_mutex);

   EAGLE_ASSERT(parent_system);
   
   parent_system->GetSystemQueue()->StopListeningTo(window);

   EagleInfo() << StringPrintF("EagleWindowManager::DestroyWindow - destroying %s" , window->FullName()) << std::endl;

   EAGLE_ASSERT(window);

   delete window;/// Must destroy window outside of lock! Or else deadlock!



   ThreadLockMutex(our_thread , manager_mutex);
   
   window_map[window_eid] = 0;/// mark destroyed windows as null

   --window_count;

   ThreadUnLockMutex(our_thread , manager_mutex);

}



EagleGraphicsContext* EagleWindowManager::GetActiveWindow() {

   EAGLE_ASSERT(manager_mutex && manager_mutex->Valid());
   EagleGraphicsContext* window = 0;

   ThreadLockMutex(our_thread , manager_mutex);
   window = active_window;
   ThreadUnLockMutex(our_thread , manager_mutex);

   return window;
}



int EagleWindowManager::GiveWindowFocus(int window_eid) {
   return PrivateGiveWindowFocus(window_eid);
}


