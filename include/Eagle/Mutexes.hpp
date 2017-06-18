
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
 *    Copyright 2009-2014+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */

#ifndef EagleMutex_HPP
#define EagleMutex_HPP


#include <thread>
#include <string>




/// Abstract Base class

class EagleMutex {

protected :
   bool recursive;
   bool locked;
   std::thread::id lock_thread_id;

   virtual void PrivateLock()=0;
   virtual void PrivateUnlock()=0;


public :

   EagleMutex();
   virtual ~EagleMutex() {}

   virtual bool Create(bool multi_lockable)=0;
   virtual void Destroy()=0;/// NOTE : Must call Destroy in YOUR destructor for inherited mutex classes
   virtual bool Valid()=0;

   /// void Lock();/// There is a macro for Lock() that calls Lock(__function__) in your code
   /// void Unlock();/// There is a macro for Unlock() that calls Unlock(__function__) in your code

   void DoLock(std::string calling_function);
   void DoUnlock(std::string calling_function);

   bool Locked();/// TODO : NOTE : Not reliable yet
};


#include "Eagle/Platform.hpp"

#define Lock() DoLock(EAGLE__FUNC)
#define Unlock() DoUnlock(EAGLE__FUNC)




#endif // EagleMutex_HPP





