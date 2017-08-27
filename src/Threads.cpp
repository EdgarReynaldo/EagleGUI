

#include "Eagle/Exception.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/Threads.hpp"


int EagleThread::new_thread_id = 0;



EagleThread::EagleThread() :
      EagleObject(),
      thread_id(new_thread_id++)
{
   SetName(StringPrintF("EagleThread at %p" , this));
   ThreadManager::Instance()->RegisterThread(this);
}



EagleThread::~EagleThread() {
   ThreadManager::Instance()->UnRegisterThread(this);
}



/// --------------    ThreadManager     -------------------------------------



ThreadManager* ThreadManager::threadman = 0;



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



