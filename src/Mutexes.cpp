

#include "Eagle/Mutexes.hpp"
#include "Eagle/Logging.hpp"
#include "Eagle/StringWork.hpp"



EagleMutex::EagleMutex() :
      recursive(false),
      locked(false),
      lock_thread_id(std::thread::id())
{}



void EagleMutex::Lock(std::string calling_function) {
   PrivateLock();
   locked = true;
   lock_thread_id = std::this_thread::get_id();
   EagleInfo() << StringPrintF("Mutex %p Locked   in function %s from thread " , this , calling_function.c_str()) << lock_thread_id << std::endl;
}



void EagleMutex::Unlock(std::string calling_function) {
   PrivateUnlock();
   locked = false;
   EagleInfo() << StringPrintF("Mutex %p Unlocked in function %s from thread " , this , calling_function.c_str()) << lock_thread_id << std::endl;
   lock_thread_id = std::thread::id();
}



bool EagleMutex::Locked() {
   return locked;
}



