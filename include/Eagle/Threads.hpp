
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



#ifndef Threads_HPP
#define Threads_HPP


#include "Eagle/Object.hpp"
#include "Eagle/Mutexes.hpp"

#include <fstream>



class EagleThread;

/// Your process will be given access to the eagle thread, and the ptr specified in EagleThread::Create
typedef void* (*EAGLE_THREAD_PROCESS)(EagleThread* , void*);

extern void* const THREAD_NOT_VALID;

extern EagleThread* const MAIN_THREAD;
extern EagleThread* const NOT_A_THREAD;



class EagleThread : public EagleObject {
   
   friend class EagleMutex;
   
private :
   static int new_thread_id;
   
   int thread_id;
   EAGLE_MUTEX_STATE mutex_state;
   EagleMutex* our_mutex;
   const char* latest_func_caller;
   
   void SetState(EAGLE_MUTEX_STATE s);
   void SetCaller(const char* caller);
   
public :
   EagleThread(std::string objclass = "EagleThread" , std::string objname = "Nemo");
   virtual ~EagleThread();
   
   virtual bool Create(void* (*process_to_run)(EagleThread* , void*) , void* arg)=0;
   virtual void Destroy()=0;
   virtual void Start()=0;
   virtual void SignalToStop()=0;

   virtual void* Join()=0;/// Implicitly signal stop
   virtual void* FinishThread()=0;/// Wait for thread to complete

   virtual bool ShouldStop()=0;
   virtual bool Running()=0;
   virtual bool Complete()=0;
   virtual bool Valid()=0;

   int ID() {return thread_id;}
   EAGLE_MUTEX_STATE State() {return mutex_state;}
   EagleMutex* OurMutex() {return our_mutex;}
   
   void DoLockOnMutex(EagleMutex* m , const char* func);
   bool DoTryLockOnMutex(EagleMutex* m , const char* func);
   bool DoLockWaitOnMutex(EagleMutex* m , const char* func , double timeout);
   void DoUnLockOnMutex(EagleMutex* m , const char* func);
   
   
   
};






class ThreadManager {

   friend class EagleThread;

private :
   
   static ThreadManager* threadman;
   
   
   typedef std::map<int , EagleThread*> THREAD_MAP;
   typedef THREAD_MAP::iterator TMIT;
   
   THREAD_MAP thread_map;
   
   std::ofstream thread_log;
   
   ThreadManager();
   
   void Create();
   void Destroy();

   void RegisterThread(EagleThread* t);
   void UnRegisterThread(EagleThread* t);
   
   friend void DestroyThreadManager();
   friend std::ostream& ThreadLog();
   
public :

   static ThreadManager* Instance();
   
/**
///   void PrintThreadReport();
   void PrintThreadReport() {
      MutexReporter rpt;
      TMIT it = thread_map.begin();
      while (it != thread_map.end()) {
         EagleThread* t = it->second;
         EagleMutex* m = t->OurMutex();
         if (m) {
            rpt.TrackMutexState(
         }
         ++it;
      }
   }
*/
};



void DestroyThreadManager();

std::ostream& ThreadLog();








#endif // Threads_HPP




