

#include "Eagle/Exception.hpp"
#include "Eagle/Mutexes.hpp"
#include "Eagle/Logging.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/Threads.hpp"




const char* EagleMutexStateStr(EAGLE_MUTEX_STATE s) {
   static const char* strs[NUM_MUTEX_STATES] = {
      "MTX_UNLOCKED",
      "MTX_WAITLOCK",
      "MTX_FAILLOCK",
      "MTX_ISLOCKED"
   };
   return strs[s];
}



/// ----------------------     EagleMutex      --------------------------------------




void EagleMutex::DoLock(EagleThread* callthread , std::string callfunc) {

   EAGLE_MUTEX_STATE oldstate = state;
   EagleThread* oldowner = owner;

   LogThreadState(callthread , callfunc.c_str() , MTX_WAITLOCK);
   
   if (NOT_A_THREAD != callthread) {/// don't track state, for power users only
      if (oldstate == MTX_ISLOCKED) {
         if (callthread == oldowner) {/// Already locked by this thread
            EAGLE_ASSERT(Recursive());/// Must be a recursive mutex
         }
      }
   }

   PrivateLock();
   
   LogThreadState(callthread , callfunc.c_str() , MTX_ISLOCKED);
}



bool EagleMutex::DoLockWaitFor(EagleThread* callthread , std::string callfunc , double timeout) {

   if (NOT_A_THREAD != callthread) {/// don't track state, for power users only
      if (state == MTX_ISLOCKED) {
         if (callthread == owner) {
            EAGLE_ASSERT(Recursive());
         }
      }
   }

   LogThreadState(callthread , callfunc.c_str() , MTX_WAITLOCK);/// TODO : Add wait time to logging

   bool ret = PrivateLockWaitFor(timeout);

   if (ret) {
      LogThreadState(callthread , callfunc.c_str() , MTX_ISLOCKED);
   }
   else {
      LogThreadState(callthread , callfunc.c_str() , MTX_FAILLOCK);
   }

   return ret;
}



bool EagleMutex::DoTryLock(EagleThread* callthread , std::string callfunc) {

   if (NOT_A_THREAD != callthread) {/// don't track state, for power users only
      if (state == MTX_ISLOCKED) {
         if (callthread == owner) {
            EAGLE_ASSERT(Recursive());
         }
      }
   }
      
   LogThreadState(callthread , callfunc.c_str() , MTX_WAITLOCK);

   bool ret = PrivateTryLock();

   if (ret) {
      LogThreadState(callthread , callfunc.c_str() , MTX_ISLOCKED);
   }
   else {
      LogThreadState(callthread , callfunc.c_str() , MTX_FAILLOCK);
   }

   return ret;

}



void EagleMutex::DoUnLock(EagleThread* callthread , std::string callfunc) {
   PrivateUnlock();
   LogThreadState(callthread , callfunc.c_str() , MTX_UNLOCKED);
}



EagleMutex::EagleMutex(std::string objclass , std::string objname , bool use_log) :
      EagleObject(objclass , objname , use_log),
      type(MTX_INVALID),
      state(MTX_UNLOCKED),
      lock_count(0),
      owner(NOT_A_THREAD),
      log(use_log)
{
   
///   SetName(StringPrintF("EagleMutex at %p" , this));
   MutexManager::Instance()->RegisterMutex(this);
}



EagleMutex::~EagleMutex() {
   MutexManager::Instance()->UnRegisterMutex(this);
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



void EagleMutex::DoLock(std::string callfunc) {
   DoLock(0 , callfunc);
}



bool EagleMutex::DoLockWaitFor(std::string callfunc , double timeout) {
   return DoLockWaitFor(0 , callfunc , timeout);
}



bool EagleMutex::DoTryLock(std::string callfunc) {
   return DoTryLock(0 , callfunc);
}



void EagleMutex::DoUnLock(std::string callfunc) {
   DoUnLock(0 , callfunc);
}



EAGLE_MUTEX_STATE EagleMutex::GetMutexState() {
   return state;
}



void EagleMutex::LogThreadState(EagleThread* t , const char* func , EAGLE_MUTEX_STATE tstate) {
   if (NOT_A_THREAD == t) {/// Free for all - Thread state not tracked
      state = tstate;
      if (state == MTX_ISLOCKED) {
         owner = NOT_A_THREAD;
      }
   }
   else {
       if (t) {
         t->SetState(tstate);
         if (tstate == MTX_UNLOCKED) {
            t->SetCaller("Unknown function");
         }
         else {
            t->SetCaller(func);
         }
      }
      state = tstate;
      if (state == MTX_ISLOCKED) {
         owner = t;
         ++lock_count;
      }
      if (state == MTX_UNLOCKED) {
         --lock_count;

         EAGLE_ASSERT(lock_count >= 0);

         if (!Recursive()) {
            EAGLE_ASSERT(lock_count == 0);
         }
         if (lock_count == 0) {
            owner = NOT_A_THREAD;
         }
      }
   }
#ifdef EAGLE_DEBUG_MUTEX_LOCKS
   if (log) {
      std::string threadname = "Main Thread";
      int threadid = 0;
      if (t) {
         if (t == NOT_A_THREAD) {
            threadname = "NOT_A_THREAD";
            threadid = -1;
         }
         else {
            threadname = t->FullName();
            threadid = t->ID();
         }
      }
      std::string our_name = FullName();
      ThreadLog() << StringPrintF("%50s ID %3d %s on %60s in function %-80s" ,
                                  threadname.c_str() , threadid , EagleMutexStateStr(tstate) , our_name.c_str() , func) << std::endl;
   }
#endif
   return;
}



void EagleMutex::TurnLogOn() {
   log = true;
}



void EagleMutex::TurnLogOff() {
   log = false;
}



/// ---------------------      MutexManager      -------------------------------------



MutexManager* MutexManager::mutex_man = 0;


MutexManager::MutexManager() :
   mutex_map()
{}



void MutexManager::RegisterMutex(EagleMutex* m) {
   mutex_map[m->GetEagleId()] = m;
}



void MutexManager::UnRegisterMutex(EagleMutex* m) {
   auto it = mutex_map.find(m->GetEagleId());
   if (it != mutex_map.end()) {
      mutex_map.erase(it);
   }
}



MutexManager* MutexManager::Instance() {
   if (!mutex_man) {
      mutex_man = new MutexManager();
   }
   return mutex_man;
}



void MutexManager::DoThreadLockOnMutex(EagleThread* t , EagleMutex* m , const char* callfunc) {
   EAGLE_ASSERT(m);
   EAGLE_ASSERT(callfunc);
   
   m->DoLock(t , callfunc);
}



bool MutexManager::DoThreadTryLockOnMutex(EagleThread* t , EagleMutex* m , const char* callfunc) {
   EAGLE_ASSERT(m);
   EAGLE_ASSERT(callfunc);
   
   return m->DoTryLock(t , callfunc);
}



bool MutexManager::DoThreadLockWaitOnMutex(EagleThread* t , EagleMutex* m , const char* callfunc , double timeout) {
   EAGLE_ASSERT(m);
   EAGLE_ASSERT(callfunc);
   
   return m->DoLockWaitFor(t , callfunc , timeout);
}



void MutexManager::DoThreadUnLockOnMutex(EagleThread* t , EagleMutex* m , const char* callfunc) {
   EAGLE_ASSERT(m);
   EAGLE_ASSERT(callfunc);
   
   m->DoUnLock(t , callfunc);
}



void MutexManager::DoLockOnMutex(EagleMutex* m , const char* callfunc) {
   DoThreadLockOnMutex(0 , m , callfunc);
}



bool MutexManager::DoTryLockOnMutex (EagleMutex* m , const char* callfunc) {
   return DoThreadTryLockOnMutex(0 , m , callfunc);
}



bool MutexManager::DoLockWaitOnMutex(EagleMutex* m , const char* callfunc , double timeout) {
   return DoThreadLockWaitOnMutex(0 , m , callfunc , timeout);
}



void MutexManager::DoUnLockOnMutex  (EagleMutex* m , const char* callfunc) {
   DoThreadUnLockOnMutex(0 , m , callfunc);
}








