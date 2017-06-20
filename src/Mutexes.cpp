

#include "Eagle/Mutexes.hpp"
#include "Eagle/Logging.hpp"
#include "Eagle/StringWork.hpp"



EagleMutex::EagleMutex() :
      recursive(false),
      locked(false),
      lock_thread_id(std::thread::id())
{}



void EagleMutex::DoLock(std::string calling_function) {
#ifdef EAGLE_DEBUG_MUTEX_LOCKS
   EagleInfo() << StringPrintF("Mutex %p WaitState in function %s from thread " , this , calling_function.c_str()) << lock_thread_id << std::endl;
#endif
   PrivateLock();
   locked = true;
   lock_thread_id = std::this_thread::get_id();
#ifdef EAGLE_DEBUG_MUTEX_LOCKS
   EagleInfo() << StringPrintF("Mutex %p Locked    in function %s from thread " , this , calling_function.c_str()) << lock_thread_id << std::endl;
#endif
}



void EagleMutex::DoUnlock(std::string calling_function) {
   PrivateUnlock();
   locked = false;
#ifdef EAGLE_DEBUG_MUTEX_LOCKS
   EagleInfo() << StringPrintF("Mutex %p Unlocked  in function %s from thread " , this , calling_function.c_str()) << lock_thread_id << std::endl;
#endif
   lock_thread_id = std::thread::id();
}



bool EagleMutex::Locked() {
   return locked;
}



