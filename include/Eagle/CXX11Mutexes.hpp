



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

   CXX11Mutex();
   ~CXX11Mutex();

   virtual bool Create(bool multi_lockable , bool is_timed);
   virtual void Destroy();/// NOTE : Must call Destroy in YOUR destructor for inherited mutex classes





};


#endif // CXX11Mutexes_HPP

