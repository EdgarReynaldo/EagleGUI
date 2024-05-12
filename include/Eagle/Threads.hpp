
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Threads.hpp
 * @brief The main interface for threading in Eagle
 */

#ifndef Threads_HPP
#define Threads_HPP


#include <atomic>
#include <fstream>

#include "Eagle/Object.hpp"
#include "Eagle/Mutexes.hpp"



class EagleThread;

/**! @typedef EAGLE_THREAD_PROCESS
 *   @brief Typedef for a function you wish to run inside an @ref EagleThread
 *
 *   Your process will be given access to the eagle thread, and the ptr specified in EagleThread::Create
 */

typedef void* (*EAGLE_THREAD_PROCESS)(EagleThread* , void*);

extern void* const THREAD_NOT_VALID;///< Used by @ref EagleThread::FinishThread to signal no thread is active

extern EagleThread* const MAIN_THREAD;///< Use MAIN_THREAD for any EagleThread* that should run on main

extern EagleThread* const NOT_A_THREAD;///< Use NOT_A_THREAD for any EagleThread* that you don't care about synchonizing. Be wary.



/**! @class EagleThread
 *   @brief A class to handle a thread process and make it easier to run code in separate threads
 */

class EagleThread : public EagleObject {

   friend class EagleMutex;

private :
   static int new_thread_id;

   int thread_id;
   std::atomic<EAGLE_MUTEX_STATE> mutex_state;
   EagleMutex* our_mutex;
   const char* latest_func_caller;

   void SetState(EAGLE_MUTEX_STATE state);
   void SetCaller(const char* caller);

public :
   ///< Default constructor supplying class and object name
   EagleThread(std::string objclass = "EagleThread" , std::string objname = "Nemo");

   ///< Virtual destructor
   virtual ~EagleThread();


   ///< Pure virtual function to create a thread using the specified process and argument
   virtual bool Create(EAGLE_THREAD_PROCESS process_to_run , void* arg)=0;

   ///< Pure virtual function to destroy a thread - NOTE : You must call Destroy in your derived class destructors!!!
   virtual void Destroy()=0;

   ///< Pure virtual function to start a created thread
   virtual void Start()=0;

   ///< Pure virtual function to signal a thread to stop
   virtual void SignalToStop()=0;

   /**! @fn Join
    *   @brief Pure virtual function to implicitly signal this thread to stop
    *   @retval May return @ref THREAD_NOT_VALID if there is no thread to run
    *           otherwise returns the result of the process
    */
   virtual void* Join()=0;

   /**! @fn FinishThread
    *   @brief Pure virtual function to wait for this thread to complete,
    *   @retval May return @ref THREAD_NOT_VALID if there is no thread to run
    *           otherwise returns the result of the process
    */
   virtual void* FinishThread()=0;

   virtual bool ShouldStop()=0;///< Pure virtual function to check if the thread should stop
   virtual bool Running()=0;///< Pure virtual function to see if the thread is running
   virtual bool Complete()=0;///< Pure virtual function to check if the thread is complete

   virtual bool Valid()=0;///< Pure virtual function to check if the thread is valid

   int ID() {return thread_id;}///< Get the THREAD_ID of this thread

///   EAGLE_MUTEX_STATE State() {return mutex_state;}///< Return the state of our mutex, for what??? Hmm..

///   EagleMutex* OurMutex() {return our_mutex;}///< Get a pointer to our mutex??? Why...

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
   ~ThreadManager();



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




