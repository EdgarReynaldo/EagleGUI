
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




#ifndef SharedMemory_HPP
#define SharedMemory_HPP


#include <memory>

template <class T>
class DELETER { 

   bool deleteme;

public :

   DELETER() : deleteme(true) {}
   
   DELETER(bool destroy_when_done) : deleteme(destroy_when_done) {}
   
   void operator()(T* p) const {
      if (deleteme) {
         delete p;
      }
      return;
   }
};

template <typename T>
class SHAREDOBJECT;

template <typename T>
SHAREDOBJECT<T> StackObject(T* t);

template <typename T>
SHAREDOBJECT<T> StackObject(T& t);

template <typename T>
SHAREDOBJECT<T> HeapObject(T* t);

template <typename T>
class SHAREDOBJECT {
   std::shared_ptr<T> sptr;
   SHAREDOBJECT(T* t, bool delete_me) :
         sptr(t , DELETER<T>(delete_me))
   {}
   friend SHAREDOBJECT StackObject<T>(T* t);
   friend SHAREDOBJECT StackObject<T>(T& t);
   friend SHAREDOBJECT HeapObject<T>(T* t);

public :
   
   SHAREDOBJECT() :
         sptr(0)
   {}

   void reset() {sptr.reset();}
   T* get() {return sptr.get();}
   const std::shared_ptr<T>& ptr() {return sptr;}
   const T* get() const {return sptr.get();}
   
   operator T* () {return sptr.get();}
   operator const T* () const {return sptr.get();}
   
   T* operator->() {return sptr.get();}
   const T* operator->() const {return sptr.get();}
   
   T& operator*() {return *sptr.get();}
   const T& operator*() const {return *sptr.get();}

   operator bool() const {return (bool)sptr.get();}
};










template <class T>
SHAREDOBJECT<T> StackObject(T* t) {
   return SHAREDOBJECT<T>(t,false);
}
template <class T>
SHAREDOBJECT<T> StackObject(T& t) {
   return SHAREDOBJECT<T>(&t,false);
}
template <class T>
SHAREDOBJECT<T> HeapObject(T* t) {
   return SHAREDOBJECT<T>(t,true);
}



#endif // SharedMemory_HPP

