



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

template <class T>
typedef std::shared_ptr<T,DELETER = DELETER()> SHAREDOBJECT;

template <class T>
SHAREDOBJECT<T> StackObject(T* t);
template <class T>
SHAREDOBJECT<T> StackObject(T& t);

template <class T>
SHAREDOBJECT<T> StackObject(T* t) {
   return SHAREDOBJECT<T>(t,DELETER(false));
}
template <class T>
SHAREDOBJECT<T> StackObject(T& t) {
   return SHAREDOBJECT<T>(&t,DELETER(false));
}



#endif // SharedMemory_HPP

