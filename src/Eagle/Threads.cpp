
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
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */


#include "Eagle/Exception.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/Threads.hpp"
#include "Eagle/Mutexes.hpp"



int EagleThread::new_thread_id = 1;/// 0 is reserved for Main, and -1 is reserved for non-threads (free for all)



void* const THREAD_NOT_VALID = (void*)-1;



EagleThread* const MAIN_THREAD = (EagleThread*)0;



EagleThread* const NOT_A_THREAD = (EagleThread*)-1;



void EagleThread::SetState(EAGLE_MUTEX_STATE s) {
   mutex_state = s;
}



void EagleThread::SetCaller(const char* caller) {
   latest_func_caller = caller;
}



EagleThread::EagleThread(std::string objclass , std::string objname) :
      EagleObject(objclass , objname),
      thread_id(new_thread_id++),
      mutex_state(MTX_UNLOCKED),
      our_mutex(0),
      latest_func_caller(0)
{
   ThreadManager::Instance()->RegisterThread(this);
}



EagleThread::~EagleThread() {
   ThreadManager::Instance()->UnRegisterThread(this);
}



void EagleThread::DoLockOnMutex(EagleMutex* m , const char* func) {
   EAGLE_ASSERT(m);
   MutexManager::Instance()->DoThreadLockOnMutex(this , m , func);
/*   
   mutex_state = MTX_WAITLOCK;
   our_mutex = m;
   m->DoLock(this , func);
   mutex_state = MTX_ISLOCKED;
*/
}



bool EagleThread::DoTryLockOnMutex(EagleMutex* m , const char* func) {
   EAGLE_ASSERT(m);
   return MutexManager::Instance()->DoThreadTryLockOnMutex(this , m , func);
/*
   mutex_state = MTX_WAITLOCK;
   our_mutex = m;
   bool locked = m->DoTryLock(this , func);
   if (locked) {
      mutex_state = MTX_ISLOCKED;
   }
   else {
      mutex_state = MTX_UNLOCKED;
   }
   return locked;
*/
}



bool EagleThread::DoLockWaitOnMutex(EagleMutex* m , const char* func , double timeout) {
   EAGLE_ASSERT(m);
   
   return MutexManager::Instance()->DoThreadLockWaitOnMutex(this , m , func , timeout);
/* OLD KEEP?
   mutex_state = MTX_WAITLOCK;
   our_mutex = m;
   bool locked = m->DoLockWaitFor(this , func , timeout);
   if (locked) {
      mutex_state = MTX_ISLOCKED;
   }
   else {
      mutex_state = MTX_UNLOCKED;
      our_mutex = 0;
   }
   return locked;
*/
}



void EagleThread::DoUnLockOnMutex(EagleMutex* m , const char* func) {
   EAGLE_ASSERT(m);
   EAGLE_ASSERT(m == our_mutex);
   MutexManager::Instance()->DoThreadUnLockOnMutex(this , m , func);
/*
   m->DoUnLock(this , func);
   mutex_state = MTX_UNLOCKED;
   our_mutex = 0;
*/
}



/// --------------    ThreadManager     -------------------------------------



ThreadManager* ThreadManager::threadman = 0;



///EagleThread* ThreadManager::main_thread = 0;



ThreadManager::ThreadManager() :
   thread_map(),
   thread_log()
{}






void ThreadManager::Create() {
   thread_log.open("EagleThreadLog.txt" , std::ios::out);
   EAGLE_ASSERT(thread_log.good());
   atexit(DestroyThreadManager);
}



void ThreadManager::Destroy() {
   if (threadman) {
      delete threadman;
   }
   threadman = 0;
}



void ThreadManager::RegisterThread(EagleThread* t) {
   EAGLE_ASSERT(t);
   thread_map[t->GetEagleId()] = t;
}



void ThreadManager::UnRegisterThread(EagleThread* t) {
   auto it = thread_map.find(t->GetEagleId());
   if (it != thread_map.end()) {
      thread_map.erase(it);
   }
}



ThreadManager* ThreadManager::Instance() {
   if (!threadman) {
      threadman = new ThreadManager();
      threadman->Create();
   }
   return threadman;
}



void DestroyThreadManager() {
   ThreadManager::Instance()->Destroy();
}



std::ostream& ThreadLog() {
   return ThreadManager::Instance()->thread_log;
}



