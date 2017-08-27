

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
      owner((EagleThread*)-1),
      log(true)
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

   if (state == MTX_ISLOCKED) {
      if (callthread == owner) {
         EAGLE_ASSERT(Recursive());
      }
   }

   state = MTX_WAITLOCK;
   
   LogThreadState(callthread , callfunc.c_str() , "WaitLock");

   PrivateLock();
   state = MTX_ISLOCKED;
   ++lock_count;
   owner = callthread;
   
   LogThreadState(callthread , callfunc.c_str() , "IsLocked");
}



bool EagleMutex::DoLockWaitFor(EagleThread* callthread , std::string callfunc , double timeout) {

   if (state == MTX_ISLOCKED) {
      if (callthread == owner) {
         EAGLE_ASSERT(Recursive());
      }
   }

   state = MTX_WAITLOCK;

   LogThreadState(callthread , callfunc.c_str() , "WaitLock");

   bool ret = PrivateLockWaitFor(timeout);
   if (ret) {
      state = MTX_ISLOCKED;
      ++lock_count;
      owner = callthread;
      LogThreadState(callthread , callfunc.c_str() , "IsLocked");
   }
   else {
      state = MTX_UNLOCKED;
      LogThreadState(callthread , callfunc.c_str() , "FailLock");
   }
   return ret;
}



bool EagleMutex::DoTryLock(EagleThread* callthread , std::string callfunc) {

   if (state == MTX_ISLOCKED) {
      if (callthread == owner) {
         EAGLE_ASSERT(Recursive());
      }
   }

   state = MTX_WAITLOCK;

   LogThreadState(callthread , callfunc.c_str() , "WaitLock");

   bool ret = PrivateTryLock();

   if (ret) {
      state = MTX_ISLOCKED;
      ++lock_count;
      owner = callthread;
      LogThreadState(callthread , callfunc.c_str() , "IsLocked");
   }
   else {
      state = MTX_UNLOCKED;
      LogThreadState(callthread , callfunc.c_str() , "FailLock");
   }

   return ret;

}



void EagleMutex::DoUnlock(EagleThread* callthread , std::string callfunc) {

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
   
   LogThreadState(callthread , callfunc.c_str() , "UnLocked");

}



EAGLE_MUTEX_STATE EagleMutex::GetMutexState() {
   return state;
}



void EagleMutex::LogThreadState(EagleThread* t , const char* func , const char* tstate) {
   (void)t;
   (void)func;
   (void)tstate;
#ifdef EAGLE_DEBUG_MUTEX_LOCKS
   if (log) {
      const char* threadname = t?t->GetName().c_str():"Main Thread";
      int threadid = t?t->ID():-1;

      ThreadLog() << 
         StringPrintF("Mutex '%50s' %8s on Thread #%4d %50s in function %50s" ,
                      GetName().c_str() , tstate , threadid , threadname , func) << std::endl;
   }
#endif
   return;
}





