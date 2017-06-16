

#include "Eagle/WindowManager.hpp"
#include "Eagle/System.hpp"
#include "Eagle/Mutexes.hpp"
#include "Eagle/Threads.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/Object.hpp"



void EagleWindowManager::SwitchIn(EagleGraphicsContext* window) {
   EAGLE_ASSERT(window);
   EAGLE_ASSERT(manager_mutex && manager_mutex->Valid());
   manager_mutex->Lock();
   WMIT it = window_map.find(window->GetEagleId());
   EAGLE_ASSERT(it != window_map.end());
   EAGLE_ASSERT(it->second);
   active_window = window;
   manager_mutex->Unlock();
}



void EagleWindowManager::SwitchOut(EagleGraphicsContext* window) {
   EAGLE_ASSERT(window);
   EAGLE_ASSERT(manager_mutex && manager_mutex->Valid());

   manager_mutex->Lock();

   EAGLE_ASSERT(window_map.find(window->GetEagleId()) != window_map.end());

   if (active_window == window) {
      active_window = 0;/// We don't know which window will become active, but this one was, so zero the active_window
   }

   manager_mutex->Unlock();
}






EagleWindowManager::EagleWindowManager(EagleSystem* sys) :
      parent_system(sys),
      manager_thread(0),
      manager_mutex(0),
      window_map(),
      window_count(0),
      active_window(0)
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

   manager_mutex->Lock();
   winmap = window_map;
   manager_mutex->Unlock();

   for (WMIT it = winmap.begin() ; it != winmap.end() ; ++it) {
      if (it->second != 0) {
         EAGLE_ASSERT(GetValidById(it->first));
         EagleLog() << StringPrintF("EagleWindowManager::CloseWindows - About to close window %s." , it->second->GetName().c_str()) << std::endl;
///         manager_mutex->Unlock();
         DestroyWindow(it->first);
///         manager_mutex->Lock();
      }
   }

   manager_mutex->Lock();
   window_map.clear();
   active_window = 0;
   manager_mutex->Unlock();

   EAGLE_ASSERT(window_count == 0);
}



EagleGraphicsContext* EagleWindowManager::CreateWindow(int width , int height , int flags) {
   EagleGraphicsContext* window = PrivateCreateWindow(width , height , flags);
   if (window) {
      if (!window->Valid()) {
         throw EagleException("EagleWindowManager::CreateWindow - Failed to create valid window!");
      }

      EAGLE_ASSERT(manager_mutex && manager_mutex->Valid());
      manager_mutex->Lock();
      window_map[window->GetEagleId()] = window;
      ++window_count;
      active_window = window;
      manager_mutex->Unlock();

      return window;
   }
   return 0;
}



void EagleWindowManager::DestroyWindow(int window_eid) {

   WMIT it = window_map.find(window_eid);
   if (it == window_map.end()) {return;}
   if (it->second == (EagleGraphicsContext*)0) {return;}

   EAGLE_ASSERT(manager_mutex && manager_mutex->Valid());

   manager_mutex->Lock();

   EagleGraphicsContext* window = window_map[window_eid];

   EAGLE_ASSERT(window);

   EagleInfo() << StringPrintF("EagleWindowManager::DestroyWindow - destroying window %p with eid %d" , window , window_eid) << std::endl;

   delete window;

   window_map[window_eid] = 0;/// mark destroyed windows as null

   --window_count;

   manager_mutex->Unlock();

}



EagleGraphicsContext* EagleWindowManager::GetActiveWindow() {

   EAGLE_ASSERT(manager_mutex && manager_mutex->Valid());
   EagleGraphicsContext* window = 0;

   manager_mutex->Lock();
   window = active_window;
   manager_mutex->Unlock();

   return window;
}



int EagleWindowManager::GiveWindowFocus(int window_eid) {
   return PrivateGiveWindowFocus(window_eid);
}


