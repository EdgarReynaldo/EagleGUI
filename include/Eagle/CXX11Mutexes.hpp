
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
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file CXX11Mutexes
 * @brief The interface for working with C++11 mutexes
 */



#ifndef CXX11Mutexes_HPP
#define CXX11Mutexes_HPP



#include "Eagle/Mutexes.hpp"



/**! @class CXX11Mutex
 *   @brief A simple class for working with mutexes using the C++11 implementation
 *          See @ref EagleMutex for details and a list of inherited functions
 */

class CXX11Mutex : public EagleMutex {

protected :
   virtual void PrivateLock();

   virtual bool PrivateLockWaitFor(double timeout);

   virtual bool PrivateTryLock();

   virtual void PrivateUnlock();


public :

   CXX11Mutex(std::string objname = "Nemo" , bool use_log = true);///< Create a named mutex - use_log is really only for internal purposes
   ~CXX11Mutex();///< Destructor that frees the mutex

   ///< Create a new mutex with the specified attributes, returns true on success
   virtual bool Create(bool multi_lockable , bool is_timed);

   ///< Destroy this mutex, will be called on destruction
   ///< NOTE : Must call Destroy in YOUR destructor for inherited mutex classes
   virtual void Destroy();
};



#endif // CXX11Mutexes_HPP



