


#include "Eagle/CXX11Mutexes.hpp"
#include "Eagle/Exception.hpp"
#include "Eagle/StringWork.hpp"


#include <chrono>



void CXX11Mutex::PrivateLock() {
   EAGLE_ASSERT(Valid());
   switch (type) {
   case MTX_PLAIN :
      MTX.mtx->lock();
      break;
   case MTX_TIMED :
      MTX.mtx_timed->lock();
      break;
   case MTX_RECURSIVE :
      MTX.mtx_recursive->lock();
      break;
   case MTX_RECURSIVE_TIMED :
      MTX.mtx_recursive_timed->lock();
      break;
   case MTX_INVALID :
      EAGLE_ASSERT(false);
      break;
   }
}



bool CXX11Mutex::PrivateLockWaitFor(double timeout) {
   EAGLE_ASSERT(Valid());

   if (!Timed()) {
      throw EagleException("CXX11Mutex::PrivateLockWaitFor(double) : This is not a Timed() mutex!");
   }

   int ms = int(1000.0*timeout);
   if (ms < 1) {ms = 1;}
   auto duration = std::chrono::milliseconds(ms);

   switch (type) {
   case MTX_TIMED :
      return MTX.mtx_timed->try_lock_for(duration);
      break;
   case MTX_RECURSIVE_TIMED :
      return MTX.mtx_recursive_timed->try_lock_for(duration);
      break;
   default:
      break;
   }
   return false;
}



bool CXX11Mutex::PrivateTryLock() {
   EAGLE_ASSERT(Valid());
   switch (type) {
   case MTX_PLAIN :
      return MTX.mtx->try_lock();
      break;
   case MTX_TIMED :
      return MTX.mtx_timed->try_lock();
      break;
   case MTX_RECURSIVE :
      return MTX.mtx_recursive->try_lock();
      break;
   case MTX_RECURSIVE_TIMED :
      return MTX.mtx_recursive_timed->try_lock();
      break;
   case MTX_INVALID :
      EAGLE_ASSERT(false);
      break;
   }
   return false;
}

void CXX11Mutex::PrivateUnlock() {
   EAGLE_ASSERT(Valid());
   switch (type) {
   case MTX_PLAIN :
      MTX.mtx->unlock();
      break;
   case MTX_TIMED :
      MTX.mtx_timed->unlock();
      break;
   case MTX_RECURSIVE :
      MTX.mtx_recursive->unlock();
      break;
   case MTX_RECURSIVE_TIMED :
      MTX.mtx_recursive_timed->unlock();
      break;
   case MTX_INVALID :
      EAGLE_ASSERT(false);
      break;
   }
}



CXX11Mutex::CXX11Mutex() :
      EagleMutex()
{
///   SetShortName(StringPrintF("CXX11Mutex(EID = %d) at %p" , GetEagleId() , this));
}



CXX11Mutex::~CXX11Mutex() {
   /// NOTE : Must call Destroy in YOUR destructor for inherited mutex classes
   Destroy();
}



bool CXX11Mutex::Create(bool multi_lockable , bool is_timed) {
   Destroy();

   if (is_timed) {
      /// Timed mutexes
      if (multi_lockable) {
         type = MTX_RECURSIVE_TIMED;
         MTX.mtx_recursive_timed = new std::recursive_timed_mutex();
      }
      else {
         type = MTX_TIMED;
         MTX.mtx_timed = new std::timed_mutex();
      }
   }
   else {
      /// Non-timed mutexes
      if (multi_lockable) {
         type = MTX_RECURSIVE;
         MTX.mtx_recursive = new std::recursive_mutex();
      }
      else {
         type = MTX_PLAIN;
         MTX.mtx = new std::mutex();
      }
   }
   return true;
}



void CXX11Mutex::Destroy() {
   if (Valid()) {
      switch (type) {
      case MTX_PLAIN :
         delete MTX.mtx;
         MTX.mtx = 0;
         break;
      case MTX_TIMED :
         delete MTX.mtx_timed;
         MTX.mtx_timed = 0;
         break;
      case MTX_RECURSIVE :
         delete MTX.mtx_recursive;
         MTX.mtx_recursive = 0;
         break;
      case MTX_RECURSIVE_TIMED :
         delete MTX.mtx_recursive_timed;
         MTX.mtx_recursive_timed = 0;
         break;
      case MTX_INVALID :
         EAGLE_ASSERT(false);
         break;
      };
      type = MTX_INVALID;
   }
}

