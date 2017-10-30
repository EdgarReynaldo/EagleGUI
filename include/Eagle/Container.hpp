
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
 *    Copyright 2009-2013+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#ifndef EagleContainer_HPP
#define EagleContainer_HPP


#include <vector>



template <class Type , class ListType>
void RemoveUniqueFromList(ListType& list , Type obj_to_remove) {
   for (typename ListType::iterator it = list.begin() ; it != list.end() ; ++it) {
      if (*it == obj_to_remove) {
         list.erase(it);
         return;
      }
   }
}



template <class Type>
class PointerManager {

private :
   std::vector<Type*> ptrs;
   bool free_mem;
   
   bool OnList(Type* mem);

public :
   PointerManager(bool free_memory) : ptrs() , free_mem(free_memory) {}
   
   ~PointerManager();
   
   Type* Add(Type* p);
   
   void Free(Type* p);// 
                        // Be careful with sub bitmap order
   void FreeAll();// Frees in reverse order, so safe for sub bitmaps. 
                  // Don't be retarded and create bitmaps in one pointer manager
                  // and sub btmaps in another and then free the parents first :P

   void Remove(Type* p);// simply removes from list, does not delete pointer
   void RemoveAll();


   Type*& operator[](unsigned int index) {return ptrs[index];}
   unsigned int size() {return ptrs.size();}

   
   const std::vector<Type*>& Ptrs() {return ptrs;}
};



template <class Type>
bool PointerManager<Type>::OnList(Type* mem) {
   for (unsigned int i = 0 ; i < ptrs.size() ; ++i) {
      if (ptrs[i] == mem) {return true;}
   }
   return false;
}



template <class Type>
PointerManager<Type>::~PointerManager() {
   if (free_mem) {
      FreeAll();
   }
   else {
      RemoveAll();
   }
}



template <class Type>
Type* PointerManager<Type>::Add(Type* new_mem) {
   if (!new_mem) {return (Type*)0;}
   if (!OnList(new_mem)) {
      ptrs.push_back(new_mem);
   }
   return new_mem;
}



template <class Type>
void PointerManager<Type>::Free(Type* p) {
   if (OnList(p)) {
      for (typename std::vector<Type*>::iterator it = ptrs.begin() ; it != ptrs.end() ; ++it) {
         if (*it == p) {
            delete p;
            ptrs.erase(it);
            return;
         }
      }
   }
}



template <class Type>
void PointerManager<Type>::FreeAll() {
   for (int i = (int)ptrs.size() - 1 ; i >= 0 ; --i) {
      Type* t = ptrs[i];
      delete t;
   }
   ptrs.clear();

}



template <class Type>
void PointerManager<Type>::Remove(Type* p) {
   if (OnList(p)) {
      for (typename std::vector<Type*>::iterator it = ptrs.begin() ; it != ptrs.end() ; ++it) {
         if (*it == p) {
            ptrs.erase(it);
            return;
         }
      }
   }
}



template <class Type>
void PointerManager<Type>::RemoveAll() {
   ptrs.clear();
}



#endif // EagleContainer_HPP


