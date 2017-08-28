


#ifndef Threads_HPP
#define Threads_HPP


#include "Eagle/Object.hpp"
#include "Eagle/Mutexes.hpp"

#include <fstream>



class EagleThread;

/// Your process will be given access to the eagle thread, and the ptr specified in EagleThread::Create
typedef void* (*EAGLE_THREAD_PROCESS)(EagleThread* , void*);



class EagleThread : public EagleObject {
   
private :
   static int new_thread_id;
   
   int thread_id;
   EAGLE_MUTEX_STATE mutex_state;
   EagleMutex* our_mutex;
   
   
public :
   EagleThread();
   virtual ~EagleThread();
   
   virtual bool Create(void* (*process_to_run)(EagleThread* , void*) , void* arg)=0;
   virtual void Destroy()=0;
   virtual void Start()=0;
   virtual void SignalToStop()=0;

   virtual void* Join()=0;/// Implicitly signal stop
   virtual void* FinishThread()=0;/// Wait for thread to complete

   virtual bool ShouldStop()=0;
   virtual bool Running()=0;
   virtual bool Valid()=0;

   int ID() {return thread_id;}

   void DoLockOnMutex(EagleMutex* m , const char* func);
   bool DoTryLockOnMutex(EagleMutex* m , const char* func);
   bool DoLockWaitOnMutex(EagleMutex* m , const char* func , double timeout);
   void DoUnLockOnMutex(EagleMutex* m , const char* func);
   
   
   
};

/**
   ThreadManager tracks which 
*/
class ThreadManager {

   friend class EagleThread;

private :
   
   static ThreadManager* threadman;
   
   
   std::map<int , EagleThread*> thread_map;
   
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
   

};



void DestroyThreadManager();

std::ostream& ThreadLog();








#endif // Threads_HPP




