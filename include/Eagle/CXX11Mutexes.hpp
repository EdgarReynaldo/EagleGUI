
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




#ifndef CXX11Mutexes_HPP
#define CXX11Mutexes_HPP

#include "Eagle/Mutexes.hpp"



class CXX11Mutex : public EagleMutex {

protected :
   virtual void PrivateLock();

   virtual bool PrivateLockWaitFor(double timeout);

   virtual bool PrivateTryLock();

   virtual void PrivateUnlock();


public :

   CXX11Mutex(std::string objname = "Nemo" , bool use_log = true);
   ~CXX11Mutex();

   virtual bool Create(bool multi_lockable , bool is_timed);
   virtual void Destroy();/// NOTE : Must call Destroy in YOUR destructor for inherited mutex classes





};


#endif // CXX11Mutexes_HPP

