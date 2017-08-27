
/*
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
 *    EAGLE
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2017+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */

#ifndef EagleMutex_HPP
#define EagleMutex_HPP


#include "Eagle/Object.hpp"

#include <mutex>
#include <thread>
#include <string>

enum EAGLE_MUTEX_TYPE {
   MTX_INVALID         = 0,
   MTX_PLAIN           = 1,
   MTX_TIMED           = 2,
   MTX_RECURSIVE       = 3,
   MTX_RECURSIVE_TIMED = 4
};



enum EAGLE_MUTEX_STATE {
   MTX_UNLOCKED = 0,
   MTX_WAITLOCK = 1,
   MTX_ISLOCKED = 2
};



class EagleThread;



/// Abstract Base class

class EagleMutex : public EagleObject {

protected :
   EAGLE_MUTEX_TYPE type;
   EAGLE_MUTEX_STATE state;
   
   union {
      std::mutex*                 mtx;
      std::timed_mutex*           mtx_timed;
      std::recursive_mutex*       mtx_recursive;
      std::recursive_timed_mutex* mtx_recursive_timed;
   } MTX;

///   bool locked;

   int lock_count;
   EagleThread* owner;
   bool log;


   virtual void PrivateLock()=0;

   virtual bool PrivateLockWaitFor(double timeout)=0;

   virtual bool PrivateTryLock()=0;

   virtual void PrivateUnlock()=0;


public :

   EagleMutex();
   virtual ~EagleMutex() {}

   virtual bool Create(bool multi_lockable , bool is_timed)=0;
   virtual void Destroy()=0;/// NOTE : Must call Destroy in YOUR destructor for inherited mutex classes

   bool Valid();
   bool Timed();
   bool Recursive();


   /// void Lock();/// There is a macro for Lock() that calls Lock(__function__) in your code
   /// void Unlock();/// There is a macro for Unlock() that calls Unlock(__function__) in your code

   /// DoLockWaitFor will throw an error if this is not a timed mutex
   /// callthread may be NULL if calling from main
   /// Pass EAGLE__FUNC for callfunc
   
   void DoLock       (EagleThread* callthread , std::string callfunc);
   bool DoLockWaitFor(EagleThread* callthread , std::string callfunc , double timeout);
   bool DoTryLock    (EagleThread* callthread , std::string callfunc);
   void DoUnlock     (EagleThread* callthread , std::string callfunc);

   EAGLE_MUTEX_STATE GetMutexState();
   
   void LogThreadState(EagleThread* t , const char* func , const char* state);
   
};






#endif // EagleMutex_HPP





