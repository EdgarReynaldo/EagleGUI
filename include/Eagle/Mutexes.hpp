
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
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 *    @file Mutexes.hpp
 *    @brief Interface for the abstract base class for Mutexes
 */


#ifndef EagleMutex_HPP
#define EagleMutex_HPP

#include "Eagle/Platform.hpp"
#include "Eagle/Object.hpp"

#include <mutex>
#include <thread>
#include <string>
#include <list>
#include <map>
#include <atomic>



/**! @enum EAGLE_MUTEX_TYPE
 *   @brief Enumeration to define mutex type
 */

enum EAGLE_MUTEX_TYPE {
   MTX_INVALID         = -1,///< This is not a valid mutex type
   MTX_PLAIN           = 0, ///< This is a plain, non-timed, non-recursive mutex
   MTX_TIMED           = 1, ///< This is a timed, non-recursive mutex
   MTX_RECURSIVE       = 2, ///< This is a recursive, non-timed mutex
   MTX_RECURSIVE_TIMED = 3  ///< This is a timed, recursive mutex
};



enum EAGLE_MUTEX_STATE {
   MTX_UNLOCKED = 0,///< This mutex is unlocked
   MTX_WAITLOCK = 1,///< This mutex is waiting for a lock
   MTX_FAILLOCK = 2,///< This mutex failed to get a lock
   MTX_ISLOCKED = 3,///< This mutex is locked
   NUM_MUTEX_STATES = 4 ///< Number of mutex states
};



const char* EagleMutexStateStr(EAGLE_MUTEX_STATE s);///< Get a string name for the mutex state s



class EagleThread;



/**! @class EagleMutex
 *   @brief Abstract Base class for working with eagle mutexes
 *
 *   Mutex class has no public methods for locking and unlocking mutexes. Use @ref MutexManager static methods instead
 */

class EagleMutex  : public EagleObject {

protected :
   EAGLE_MUTEX_TYPE type;  ///< The type of mutex
   volatile std::atomic<EAGLE_MUTEX_STATE> state_atomic;///< The state of the mutex, is an atomic type for thread safety

   /**! @union MTX
    *   @brief This is to make our lives easier, and store the mutex in a single variable even if it is a different type
    */

   union {
      std::mutex*                 mtx;                ///< Plain mutex
      std::timed_mutex*           mtx_timed;          ///< Timed mutex
      std::recursive_mutex*       mtx_recursive;      ///< Recursive mutex
      std::recursive_timed_mutex* mtx_recursive_timed;///< Timed recursive mutex
   } MTX;

   std::atomic<int> lock_count;///<  Count of the number of times we've been locked. 0 and 1 are valid for all mutexes. Greater than 1 is only valid for recursive mutices.

   EagleThread* owner;///< The thread that owns us, may be @ref MAIN_THREAD . If invalid, it will be NOT_A_THREAD

   bool log;///< Whether or not to log this mutex's state



   virtual void PrivateLock()=0;///< Pure virtual function to lock the mutex

   virtual bool PrivateLockWaitFor(double timeout)=0;///< Pure virtual function to lock the mutex using a timeout

   virtual bool PrivateTryLock()=0;///< Pure virtual function to try to lock the mutex

   virtual void PrivateUnlock()=0;///< Pure virtual function to unlock the mutex



   friend class EagleThread;
   friend class MutexManager;

   /**! @fn DoLock <EagleThread* , std::string>
    *   @brief Locks the mutex, private
    */
   void DoLock       (EagleThread* callthread , std::string callfunc);

   /**! @fn DoLockWaitFor <EagleThread* , std::string , double>
    *   @brief Locks the mutex, but gives up if timeout is reached, private
    *   @retval True if successful, false if timed out
    */
   bool DoLockWaitFor(EagleThread* callthread , std::string callfunc , double timeout);

   /**! @fn DoTryLock <EagleThread* , std::string>
    *   @brief Tries to lock the mutex, private
    *   @retval True if successful
    */
   bool DoTryLock    (EagleThread* callthread , std::string callfunc);

   /**! @fn DoUnLock <EagleThread* , std::string>
    *   @brief Unlocks the mutex, private
    */
   void DoUnLock     (EagleThread* callthread , std::string callfunc);

   ///< Like @ref DoLock <EagleThread* , std::string> but uses the @ref MAIN_THREAD
   void DoLock       (std::string callfunc);

   ///< Like @ref DoLockWaitFor<EagleThread* , std::string , double> but uses the MAIN_THREAD
   bool DoLockWaitFor(std::string callfunc , double timeout);

   ///< Like @ref DoTryLock <EagleThread* , std::string> but uses the @ref MAIN_THREAD
   bool DoTryLock    (std::string callfunc);

   ///< Like @ref DoUnLock <EagleThread* , std::string> but uses the @ref MAIN_THREAD
   void DoUnLock     (std::string callfunc);

   ///< Helper function to log the thread state
   void LogThreadState(EagleThread* callthread , const char* func , const EAGLE_MUTEX_STATE& state);

public :

   /**! @fn EagleMutex::EagleMutex <std:;string , std::string , bool>
    *   @brief Main constructor for EagleMutex objects, takes a class name, and object name, and whether or not to use the log.
    *   @param use_log Used to prevent logging of the very first mutex created during EagleObjectRegistry startup
    */
   EagleMutex(std::string objclass = "EagleMutex" , std::string objname = "Nemo" , bool use_log = true);

   virtual ~EagleMutex();///< Virtual destructor (base class)

   /**! @fn Create <bool , bool>
    *   @brief Pure virtual function to create a mutex
    *   @param multi_lockable Whether this should be a recursive mutex
    *   @param is_timed Whether this should be a timed mutex
    */
   virtual bool Create(bool multi_lockable , bool is_timed)=0;

   /**! @fn Destroy
    *   @brief Destroys the current mutex
    *   NOTE : Must call Destroy in YOUR destructor for inherited mutex classes
    */
   virtual void Destroy()=0;

   void TurnLogOn();///< Turn the log on, only affects things if EAGLE_DEBUG_MUTEX_LOCKS is defined
   void TurnLogOff();///< Turn the log off, on by default, does nothing if EAGLE_DEBUG_MUTEX_LOCKS is not defined

   bool Valid();///< True if this mutex is valid
   bool Timed();///< True if this mutex is timed
   bool Recursive();///< True if this mutex is recursive

   EAGLE_MUTEX_STATE GetMutexState();///< Get the mutex state (may not always be accurate!!!)

   EagleThread* Owner() {return owner;}///< Return a pointer to the owning @ref EagleThread object
};



/// -------------------     MutexReporter      ------------------------

/**! @typedef EAGLE_MUTEX_MAP
 *   @brief A map of eagle mutexes
 */

typedef std::map<int , EagleMutex*> EAGLE_MUTEX_MAP;

/**! @typedef EMMIT
 *   An iterator for @ref EAGLE_MUTEX_MAP
 */

typedef EAGLE_MUTEX_MAP::iterator EMMIT;



/**! @class MutexReporter
 *   @brief A class to report on the current state of all mutexes
 *   TODO : Finish me
 */

class MutexReporter {

   typedef std::map<EagleMutex* , std::list<EagleThread*> > MUTEX_LOCK_STATE_MAP;
   typedef MUTEX_LOCK_STATE_MAP::iterator LSMIT;
   typedef std::list<EagleThread*>::iterator ETLIT;

public :
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


/**! @class MutexManager
 *   @brief The class responsible for managing all mutexes
 */

class MutexManager {

   friend class EagleMutex;///< Give private access to the @ref EagleMutex class

protected :

   static MutexManager* mutex_man;///< A singleton for the mutex manager

   EAGLE_MUTEX_MAP mutex_map;///< A map of all the mutexes

   MutexManager();///< Protected constructor

   void RegisterMutex(EagleMutex* m);///< Protected registration function for EagleMutex objects
   void UnRegisterMutex(EagleMutex* m);///< Protected deregistration function for EagleMutex objects



public :
   static MutexManager* Instance();///< Get the singleton

   ///< Locks the mutex m on @ref EagleThread *t , from function callfunc
   static void DoThreadLockOnMutex    (EagleThread* t , EagleMutex* m , const char* callfunc);

   ///< Tries to lock the mutex m on @ref EagleThread *t , from function callfunc
   static bool DoThreadTryLockOnMutex (EagleThread* t , EagleMutex* m , const char* callfunc);

   ///< Wait locks the mutex m on @ref EagleThread *t , from function callfunc, failing if timeout is reached
   static bool DoThreadLockWaitOnMutex(EagleThread* t , EagleMutex* m , const char* callfunc , double timeout);

   ///< Unlocks the mutex m on @ref EagleThread *t , from function callfunc
   static void DoThreadUnLockOnMutex  (EagleThread* t , EagleMutex* m , const char* callfunc);


   ///< Locks the mutex m on the @ref MAIN_THREAD , from function callfunc
   static void DoLockOnMutex    (EagleMutex* m , const char* callfunc);

   ///< Try locks the mutex m on the @ref MAIN_THREAD , from function callfunc
   static bool DoTryLockOnMutex (EagleMutex* m , const char* callfunc);

   ///< Wait locks the mutex m on the @ref MAIN_THREAD , from function callfunc, failing if timeout is reached
   static bool DoLockWaitOnMutex(EagleMutex* m , const char* callfunc , double timeout);

   ///< Unlocks the mutex m on the @ref MAIN_THREAD , from function callfunc
   static void DoUnLockOnMutex  (EagleMutex* m , const char* callfunc);


};

/**! @defgroup MutexLocks MutexLocks
 *   @brief Some helper macros to make threading and mutex locks cleaner and easier to use
 *
 *   @def ThreadLockMutex(t,m)
 *   @brief Locks the mutex m on thread t
 *   @def LockMutex(m)
 *   @brief Locks the mutex m on the @ref MAIN_THREAD
 *
 *   @def ThreadTryLockMutex(t,m)
 *   @brief Tries to lock the mutex m on thread t
 *   @def TryLockMutex(m)
 *   @brief Tries to lock the mutex m on the @ref MAIN_THREAD
 *
 *   @def ThreadWaitLockMutex(t,m,timeout)
 *   @brief Tries to lock the mutex m on thread t, waiting for timeout seconds
 *   @def WaitLockMutex(m,timeout)
 *   @brief Tries to lock the mutex m on the @ref MAIN_THREAD waiting for timeout seconds
 *
 *   @def ThreadUnLockMutex(t,m)
 *   @brief Unlocks the mutex m on thread t
 *   @def UnLockMutex(m)
 *   @brief Unlocks the mutex m on the @ref MAIN_THREAD
 */

#define ThreadLockMutex(t,m) \
        MutexManager::DoThreadLockOnMutex(t,m,EAGLE__FUNC)

#define ThreadTryLockMutex(t,m) \
        MutexManager::DoThreadTryLockOnMutex(t,m,EAGLE__FUNC)

#define ThreadWaitLockMutex(t,m,timeout) \
        MutexManager::DoThreadWaitLockOnMutex(t,m,EAGLE__FUNC,timeout)

#define ThreadUnLockMutex(t,m) \
        MutexManager::DoThreadUnLockOnMutex(t,m,EAGLE__FUNC)

#define LockMutex(m)\
        MutexManager::DoLockOnMutex(m,EAGLE__FUNC)

#define TryLockMutex(m)\
        MutexManager::DoTryLockOnMutex(m,EAGLE__FUNC)

#define WaitLockMutex(m,timeout)\
        MutexManager::DoWaitLockOnMutex(m,EAGLE__FUNC,timeout)

#define UnLockMutex(m)\
        MutexManager::DoUnLockOnMutex(m,EAGLE__FUNC)




#endif // EagleMutex_HPP



