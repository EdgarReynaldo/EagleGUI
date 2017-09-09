
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
#include <list>
#include <map>

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
   MTX_FAILLOCK = 2,
   MTX_ISLOCKED = 3,
   NUM_MUTEX_STATES = 4
};



const char* EagleMutexStateStr(EAGLE_MUTEX_STATE s);



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

   friend class EagleThread;
   friend class MutexManager;
   
   void DoLock       (EagleThread* callthread , std::string callfunc);
   bool DoLockWaitFor(EagleThread* callthread , std::string callfunc , double timeout);
   bool DoTryLock    (EagleThread* callthread , std::string callfunc);
   void DoUnLock     (EagleThread* callthread , std::string callfunc);

   void DoLock       (std::string callfunc);
   bool DoLockWaitFor(std::string callfunc , double timeout);
   bool DoTryLock    (std::string callfunc);
   void DoUnLock     (std::string callfunc);

   void LogThreadState(EagleThread* callthread , const char* func , EAGLE_MUTEX_STATE tstate);

public :

   EagleMutex();
   virtual ~EagleMutex();

   virtual bool Create(bool multi_lockable , bool is_timed)=0;
   virtual void Destroy()=0;/// NOTE : Must call Destroy in YOUR destructor for inherited mutex classes

   bool Valid();
   bool Timed();
   bool Recursive();

   /** Mutex class has no public methods for locking and unlocking mutexes. Use MutexManager static methods instead. */
   
   EAGLE_MUTEX_STATE GetMutexState();
   
   EagleThread* Owner() {return owner;}
   
};



/// -------------------     MutexReporter      ------------------------

   typedef std::map<int , EagleMutex*> EAGLE_MUTEX_MAP;
   typedef EAGLE_MUTEX_MAP::iterator EMMIT;

class MutexReporter {
   
   typedef std::map<EagleMutex* , std::list<EagleThread*> > MUTEX_LOCK_STATE_MAP;
   typedef MUTEX_LOCK_STATE_MAP::iterator LSMIT;
   typedef std::list<EagleThread*>::iterator ETLIT;
   
   
///   static std::string CreateReport(EAGLE_MUTEX_MAP mtx_map);
std::string CreateReport(EAGLE_MUTEX_MAP mtx_map) {
   MUTEX_LOCK_STATE_MAP lockmap;
   MUTEX_LOCK_STATE_MAP waitmap;
   for (EMMIT it = mtx_map.begin() ; it != mtx_map.end() ; ++it) {
      EagleMutex* m = it->second;
      EagleThread* t = m->Owner();
      EAGLE_MUTEX_STATE s = m->GetMutexState();
      if (s == MTX_ISLOCKED) {
         lockmap[m].push_back(t);
      }
      else if (s == MTX_WAITLOCK) {
         waitmap[m].push_back(t);
      }
   }
   for (EMMIT it = mtx_map.begin() ; it != mtx_map.end() ; ++it) {
///      EagleMutex* m = it->second;
      
   }
   return "";
}
   
};



/// ----------------     MutexManager      ---------------------------------



class MutexManager {
   
   friend class EagleMutex;
   
protected :
   
   static MutexManager* mutex_man;
   
   EAGLE_MUTEX_MAP mutex_map;

   MutexManager();
   
   void RegisterMutex(EagleMutex* m);
   void UnRegisterMutex(EagleMutex* m);
   
   
   
public :
   static MutexManager* Instance();
   
   
   static void DoThreadLockOnMutex    (EagleThread* t , EagleMutex* m , const char* callfunc);
   static bool DoThreadTryLockOnMutex (EagleThread* t , EagleMutex* m , const char* callfunc);
   static bool DoThreadLockWaitOnMutex(EagleThread* t , EagleMutex* m , const char* callfunc , double timeout);
   static void DoThreadUnLockOnMutex  (EagleThread* t , EagleMutex* m , const char* callfunc);
   
   static void DoLockOnMutex    (EagleMutex* m , const char* callfunc);
   static bool DoTryLockOnMutex (EagleMutex* m , const char* callfunc);
   static bool DoLockWaitOnMutex(EagleMutex* m , const char* callfunc , double timeout);
   static void DoUnLockOnMutex  (EagleMutex* m , const char* callfunc);
   
   
};

/** Some helper macros to make threading and mutex locks cleaner and easier to use */
#define ThreadLockMutex(t,m) \
        MutexManager::DoThreadLockOnMutex(t,m,EAGLE__FUNC)

#define ThreadTryLockMutex(t,m) \
        MutexManager::DoThreadTryLockOnMutex(t,m,EAGLE__FUNC)

#define ThreadWaitLockMutex(t,m) \
        MutexManager::DoThreadWaitLockOnMutex(t,m,EAGLE__FUNC)

#define ThreadUnLockMutex(t,m) \
        MutexManager::DoThreadUnLockOnMutex(t,m,EAGLE__FUNC)

#define LockMutex(m)\
        MutexManager::DoLockOnMutex(m,EAGLE__FUNC)

#define TryLockMutex(m)\
        MutexManager::DoTryLockOnMutex(m,EAGLE__FUNC)

#define WaitLockMutex(m,d)\
        MutexManager::DoWaitLockOnMutex(m,EAGLE__FUNC,d)

#define UnLockMutex(m)\
        MutexManager::DoUnLockOnMutex(m,EAGLE__FUNC)

        
        
        
#endif // EagleMutex_HPP





