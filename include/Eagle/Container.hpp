
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
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Container.hpp
 * @brief Simple container of shared pointers for ease of storage and removal
 */

#ifndef EagleContainer_HPP
#define EagleContainer_HPP



#include "Eagle/SharedMemory.hpp"

#include <vector>



/**! @brief Removes a single unique object of class Type from a class ListType container */
template <class Type , class ListType>
void RemoveUniqueFromList(ListType& list , Type obj_to_remove) {
   for (typename ListType::iterator it = list.begin() ; it != list.end() ; ++it) {
      if (*it == obj_to_remove) {
         list.erase(it);
         return;
      }
   }
}



/**! @class PointerManager<Type>
 *   @brief A simple templated class for managing shared pointers
 */

template <class Type>
class PointerManager {

protected :
   /**! @typedef PTYPE
    *   @brief A shared object of template class Type
    */
   typedef SHAREDOBJECT<Type> PTYPE;
   
   /**! @typedef PTRLIST
    *   @brief A vector of @ref PTYPE objects
    */
   typedef std::vector<PTYPE> PTRLIST;
   
   /**! @typedef PLIT
    *   @brief A @ref PTRLIST iterator
    */
   typedef typename PTRLIST::iterator PLIT;

private :
   PTRLIST ptrs;///< The pointer list
   
   PLIT GetListIterator(Type* ptype);///< Get an iterator for the specified pointer

public :
   PointerManager();///< Empty constructor
   ~PointerManager();///< Destructor that clears the shared pointer list

   PTYPE Add(PTYPE sp);///< Add a @ref PTYPE object to the list
   void Remove(Type* sp);///< Remove a Type pointer from the list
   void RemoveAll();///< Empty the list
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
   if (it == ptrs.end()) {
      ptrs.push_back(sp);
   }
   return sp;
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



