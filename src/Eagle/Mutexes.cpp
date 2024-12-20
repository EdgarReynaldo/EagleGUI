
/**
 *
 *     _______       ___       ____      __       _______
 *    /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *    \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *     \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *      \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *       \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *        \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2024+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */


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

   const EAGLE_MUTEX_STATE& oldstate = GetMutexState();/// Atomic assignment
   EagleThread* oldowner = owner;/// After unlock, owner is not a thread
   LogThreadState(callthread , callfunc.c_str() , MTX_WAITLOCK);

   if (oldstate == MTX_ISLOCKED) {
      if (callthread == oldowner) {/// Already locked by this thread
         EAGLE_ASSERT(Recursive());/// Must be a recursive mutex
      }
   }

   PrivateLock();

   LogThreadState(callthread , callfunc.c_str() , MTX_ISLOCKED);
}



bool EagleMutex::DoLockWaitFor(EagleThread* callthread , std::string callfunc , double timeout) {

   if (state_atomic == MTX_ISLOCKED) {
      if (callthread == owner) {
         EAGLE_ASSERT(Recursive());
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

   if (state_atomic == MTX_ISLOCKED) {
      if (callthread == owner) {
         EAGLE_ASSERT(Recursive());
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
   EAGLE_ASSERT(callthread == owner);
   /**
???      In order to protect our lock count, we must process the unlock while the mutex is still locked!

??? Really? is this true....?

   */
   LogThreadState(callthread , callfunc.c_str() , MTX_UNLOCKED);
   PrivateUnlock();
}



EagleMutex::EagleMutex(std::string objclass , std::string objname , bool use_log) :
      EagleObject(objclass , objname , use_log),
      type(MTX_INVALID),
      state_atomic(MTX_UNLOCKED),
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
   DoLock(MAIN_THREAD , callfunc);
}



bool EagleMutex::DoLockWaitFor(std::string callfunc , double timeout) {
   return DoLockWaitFor(MAIN_THREAD , callfunc , timeout);
}



bool EagleMutex::DoTryLock(std::string callfunc) {
   return DoTryLock(MAIN_THREAD , callfunc);
}



void EagleMutex::DoUnLock(std::string callfunc) {
   DoUnLock(MAIN_THREAD , callfunc);
}


EAGLE_MUTEX_STATE EagleMutex::GetMutexState() {
   return state_atomic;/// Atomic read, always up to date at time of calling but may change at any time thereafter
}



void EagleMutex::LogThreadState(EagleThread* t , const char* func , const EAGLE_MUTEX_STATE& state) {
   std::atomic<int> lcount(-1);

/** CRITICAL SECTION */
   if (state == MTX_ISLOCKED) {
      lcount = ++lock_count;/// Atomic reading and assignment
   }
   else if (state == MTX_UNLOCKED) {
      lcount = --lock_count;/// Atomic reading and assignment
   }
/** END_CRITICAL_SECTION */

/** Bookkeeping */
   if (t && (t != NOT_A_THREAD)) {
      t->SetState(state);
      if (state == MTX_UNLOCKED) {
         t->SetCaller("Unknown function");
      }
      else {
         t->SetCaller(func);
      }
   }

   /// Assign new owner
   if (state == MTX_UNLOCKED) {
      if (lcount > 0) {
         EAGLE_ASSERT(Recursive());
         /// owner doesn't change
      }
      else if (lcount == 0) {
         owner = NOT_A_THREAD;
      }
      else {
         ///< Illegal value for lock_count, a negative value indicates unmatched unlocks.
         EAGLE_ASSERT(lcount > -1);
         /// owner should be NOT_A_THREAD already
      }
   }
   else if (state == MTX_ISLOCKED) {
      owner = t;
   }
   else if (state == MTX_FAILLOCK) {
      owner = NOT_A_THREAD;
   }
   else {//(state == MTX_WAITLOCK) {
      /// owner doesn't change until they get a lock
      (void)0;
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
                                  threadname.c_str() , threadid , EagleMutexStateStr(state) , our_name.c_str() , func) << std::endl;
   }
#endif

   state_atomic = state;///< Atomic assignment of thread state. Does not change volatility of state_atomic.
                        ///< state is const though, so use state_atomic as late as possible to keep it up to date
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








