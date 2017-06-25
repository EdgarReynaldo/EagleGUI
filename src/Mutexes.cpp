

#include "Eagle/Mutexes.hpp"
#include "Eagle/Logging.hpp"
#include "Eagle/StringWork.hpp"



EagleMutex::EagleMutex() :
      type(MTX_INVALID),
///      locked(false),
      lock_thread_id(std::thread::id())
{}



bool EagleMutex::Valid() {
   return type != MTX_INVALID;
}
bool EagleMutex::Timed() {
   return ((type == MTX_TIMED) || (type == MTX_RECURSIVE_TIMED));
}
bool EagleMutex::Recursive() {
   return ((type == MTX_RECURSIVE) || (type == MTX_RECURSIVE_TIMED));
}



void EagleMutex::DoLock(std::string calling_function) {
#ifdef EAGLE_DEBUG_MUTEX_LOCKS
   EagleInfo() << StringPrintF("Mutex %p WaitState         in function %s from thread " , this , calling_function.c_str()) << lock_thread_id << std::endl;
#endif
   PrivateLock();
///   locked = true;
   lock_thread_id = std::this_thread::get_id();
#ifdef EAGLE_DEBUG_MUTEX_LOCKS
   EagleInfo() << StringPrintF("Mutex %p Locked            in function %s from thread " , this , calling_function.c_str()) << lock_thread_id << std::endl;
#endif
}



bool EagleMutex::DoTryLock(std::string calling_function) {
#ifdef EAGLE_DEBUG_MUTEX_LOCKS
   EagleInfo() << StringPrintF("Mutex %p WaitState         in function %s from thread " , this , calling_function.c_str()) << lock_thread_id << std::endl;
#endif
   bool ret = PrivateTryLock();
   if (ret) {
      lock_thread_id = std::this_thread::get_id();
#ifdef EAGLE_DEBUG_MUTEX_LOCKS
      EagleInfo() << StringPrintF("Mutex %p Locked         in function %s from thread " , this , calling_function.c_str()) << lock_thread_id << std::endl;
#endif
   }
   else {
      EagleInfo() << StringPrintF("Mutex %p Already Locked in function %s from thread " , this , calling_function.c_str()) << lock_thread_id << std::endl;
   }
   return ret;
}



void EagleMutex::DoUnlock(std::string calling_function) {
   PrivateUnlock();
///   locked = false;
#ifdef EAGLE_DEBUG_MUTEX_LOCKS
   EagleInfo() << StringPrintF("Mutex %p Unlocked          in function %s from thread " , this , calling_function.c_str()) << lock_thread_id << std::endl;
#endif
   lock_thread_id = std::thread::id();
}



///bool EagleMutex::Locked() {
///   return locked;
///}



