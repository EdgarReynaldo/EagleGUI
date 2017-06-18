

#include "Eagle/Mutexes.hpp"
#include "Eagle/Logging.hpp"
#include "Eagle/StringWork.hpp"



EagleMutex::EagleMutex() :
      recursive(false),
      locked(false),
      lock_thread_id(std::thread::id())
{}



void EagleMutex::DoLock(std::string calling_function) {
   EagleInfo() << StringPrintF("Mutex %p WaitState in function %s from thread " , this , calling_function.c_str()) << lock_thread_id << std::endl;
   PrivateLock();
   locked = true;
   lock_thread_id = std::this_thread::get_id();
   EagleInfo() << StringPrintF("Mutex %p Locked    in function %s from thread " , this , calling_function.c_str()) << lock_thread_id << std::endl;
}



void EagleMutex::DoUnlock(std::string calling_function) {
   PrivateUnlock();
   locked = false;
   EagleInfo() << StringPrintF("Mutex %p Unlocked  in function %s from thread " , this , calling_function.c_str()) << lock_thread_id << std::endl;
   lock_thread_id = std::thread::id();
}



bool EagleMutex::Locked() {
   return locked;
}



