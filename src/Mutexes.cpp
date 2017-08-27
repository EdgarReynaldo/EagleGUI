

#include "Eagle/Exception.hpp"
#include "Eagle/Mutexes.hpp"
#include "Eagle/Logging.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/Threads.hpp"



EagleMutex::EagleMutex() :
      EagleObject(),
      type(MTX_INVALID),
      state(MTX_UNLOCKED),
      lock_count(0),
      owner((EagleThread*)-1)
{
   SetName(StringPrintF("EagleMutex at %p" , this));
}



bool EagleMutex::Valid() {
   return type != MTX_INVALID;
}
bool EagleMutex::Timed() {
   return ((type == MTX_TIMED) || (type == MTX_RECURSIVE_TIMED));
}
bool EagleMutex::Recursive() {
   return ((type == MTX_RECURSIVE) || (type == MTX_RECURSIVE_TIMED));
}



void EagleMutex::DoLock(EagleThread* callthread , std::string callfunc) {

   (void)callfunc;

   if (state == MTX_ISLOCKED) {
      if (callthread == owner) {
         EAGLE_ASSERT(Recursive());
      }
   }

   state = MTX_WAITLOCK;
   
#ifdef EAGLE_DEBUG_MUTEX_LOCKS
   const char* threadname = callthread?callthread->GetName().c_str():"Main Thread";
   int threadid = callthread?callthread->Id():-1;
   ThreadLog() << 
      StringPrintF("Mutex '%s' WaitLock on Thread #%d %s in function %s",
                   GetName().c_str() , threadid , threadname , callfunc.c_str()) << std::endl;
#endif

   PrivateLock();
   state = MTX_ISLOCKED;
   ++lock_count;
   owner = callthread;
   
#ifdef EAGLE_DEBUG_MUTEX_LOCKS
      ThreadLog() << 
         StringPrintF("Mutex '%s' IsLocked on Thread #%d %s in function %s" ,
                      GetName().c_str() , threadid , threadname , callfunc.c_str()) << std::endl;
#endif
}



bool EagleMutex::DoLockWaitFor(EagleThread* callthread , std::string callfunc , double timeout) {

   if (state == MTX_ISLOCKED) {
      if (callthread == owner) {
         EAGLE_ASSERT(Recursive());
      }
   }

   state = MTX_WAITLOCK;

#ifdef EAGLE_DEBUG_MUTEX_LOCKS
   const char* threadname = callthread?callthread->GetName().c_str():"Main Thread";
   int threadid = callthread?callthread->Id():-1;
   ThreadLog() << 
      StringPrintF("Mutex '%s' WaitLock on Thread #%d %s in function %s",
                   GetName().c_str() , threadid , threadname , callfunc.c_str()) << std::endl;
#endif

   bool ret = PrivateLockWaitFor(timeout);
#ifdef EAGLE_DEBUG_MUTEX_LOCKS
   if (ret) {
      state = MTX_ISLOCKED;
      ++lock_count;
      owner = callthread;
      ThreadLog() << 
         StringPrintF("Mutex '%s' IsLocked on Thread #%d %s in function %s" ,
                      GetName().c_str() , threadid , threadname , callfunc.c_str()) << std::endl;
   }
   else {
      state = MTX_UNLOCKED;
      ThreadLog() << 
         StringPrintF("Mutex '%s' FailLock on Thread #%d %s in function %s",
                      GetName().c_str() , threadid , threadname , callfunc.c_str()) << std::endl;
   }
#endif
   return ret;
}



bool EagleMutex::DoTryLock(EagleThread* callthread , std::string callfunc) {

   if (state == MTX_ISLOCKED) {
      if (callthread == owner) {
         EAGLE_ASSERT(Recursive());
      }
   }

   state = MTX_WAITLOCK;

#ifdef EAGLE_DEBUG_MUTEX_LOCKS
   const char* threadname = callthread?callthread->GetName().c_str():"Main Thread";
   int threadid = callthread?callthread->Id():-1;
   ThreadLog() << 
      StringPrintF("Mutex '%s' WaitLock on Thread #%d %s in function %s",
                   GetName().c_str() , threadid , threadname , callfunc.c_str()) << std::endl;
#endif

   bool ret = PrivateTryLock();

#ifdef EAGLE_DEBUG_MUTEX_LOCKS
   if (ret) {
      state = MTX_ISLOCKED;
      ++lock_count;
      owner = callthread;
      ThreadLog() << 
         StringPrintF("Mutex '%s' IsLocked on Thread #%d %s in function %s" ,
                      GetName().c_str() , threadid , threadname , callfunc.c_str()) << std::endl;
   }
   else {
      state = MTX_UNLOCKED;
      ThreadLog() << 
         StringPrintF("Mutex '%s' FailLock on Thread #%d %s in function %s",
                      GetName().c_str() , threadid , threadname , callfunc.c_str()) << std::endl;
   }
#endif

   return ret;

}



void EagleMutex::DoUnlock(EagleThread* callthread , std::string callfunc) {

   (void)callthread;
   (void)callfunc;

   PrivateUnlock();
   state = MTX_UNLOCKED;
   --lock_count;
   EAGLE_ASSERT(lock_count >= 0);
   if (!Recursive()) {
      EAGLE_ASSERT(lock_count == 0);
   }
   if (lock_count == 0) {
      owner = (EagleThread*)-1;
   }
   
#ifdef EAGLE_DEBUG_MUTEX_LOCKS
   const char* threadname = callthread?callthread->GetName().c_str():"Main Thread";
   int threadid = callthread?callthread->Id():-1;
   ThreadLog() << 
      StringPrintF("Mutex '%s' Unlocked on Thread #%d %s in function %s" ,
                   GetName().c_str() , threadid , threadname , callfunc.c_str()) << std::endl;
#endif

}



EAGLE_MUTEX_STATE EagleMutex::GetMutexState() {
   return state;
}


