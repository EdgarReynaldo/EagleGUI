
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

protected :
   typedef std::shared_ptr<Type> PTYPE;
   typedef std::vector<PTYPE> PTRLIST;
   typedef typename PTRLIST::iterator PLIT;

private :
   PTRLIST ptrs;
   
   PLIT GetListIterator(Type* ptype);

public :
   PointerManager();
   ~PointerManager();

   PTYPE Add(PTYPE sp);
   void Remove(Type* sp);
   void RemoveAll();   
};



template <class Type>
typename PointerManager<Type>::PLIT PointerManager<Type>::GetListIterator(Type* ptype) {
   int i = 0;
   PLIT it = ptrs.begin();
   while (i < (int)ptrs.size()) {
      Type* p = ptrs[i].get();
      if (p == ptype) {
         break;
      }
      ++it;
      ++i;
   }
   return it;
}



template <class Type>
PointerManager<Type>::PointerManager() : ptrs() {}


   
template <class Type>
PointerManager<Type>::~PointerManager() {
   RemoveAll();
}


   
template <class Type>
typename PointerManager<Type>::PTYPE PointerManager<Type>::Add(PTYPE sp) {
   Type* p = sp.get();
   PLIT it = GetListIterator(p);
   if (it != ptrs.end()) {
      ptrs.push_back(sp);
   }
}
   


template <class Type>
void PointerManager<Type>::Remove(Type* sp) {
   PLIT it = GetListIterator(sp);
   if (it != ptrs.end()) {ptrs.erase(it);}
}



template <class Type>
void PointerManager<Type>::RemoveAll() {
   /// Need to free the elements in the reverse order they were created
   while (!ptrs.empty()) {
      ptrs.pop_back();
   }
}



#endif // EagleContainer_HPP







