
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
 * @file NDimensionArray.hpp
 * @brief Utility for making N dimensional arrays
 *
 * TODO : Uhh, I don't even remember this code anymore. Make sure it still works....
 */

#ifndef NDimensionArray_HPP
#define NDimensionArray_HPP


#error UNUSED

/** TODO : UNUSED?

template <class O>
class NDA {
protected :
   struct DIM {
      int i;
      int sz;
      DIM() : i(0) , sz(0) {}
   };
   DIM* dimen;
   
   int ndim;
   O* oarray;
   
   int dim_index;
   
public :
///   Index(O** object_array , int* dimensions , int ndimensions);
   NDA(int ndimensions , int* dimensions , O** object_array = 0) :
         dimen(0),
         ndim(0),
         oarray(0),
         dim_index(0)
   {
      Allocate(ndimensions , dimensions , object_array);
   }
   void Allocate(int ndimensions , int* dimensions , O** object_array = 0) {
      Free();
      dimen = new DIM[ndim = ndimensions];
      for (int i = 0 ; i < ndim ; ++i) {
         dimen[i].sz = dimensions[i];
      }
      oarray = new O[Volume()];
      if (object_array) {
         *object_array = oarray;
      }
      dim_index = 0;
   }
   ~NDA() {
      Free();
   }
   void ResetIndices() {
      for (int dim = 0 ; dim < ndim ; ++dim) {
         dimen[dim].i = 0;
      }
   }
   int Size(int dindex) {
      return dimen[dindex % ndim].sz;
   }
   int Size() {return Size(0);}
   int Volume(int dindex) {
      if (dindex < 0) {return 0;}
      if (dindex >= ndim) {return 1;}
      return dimen[dindex].sz*Volume(dindex + 1);
   }
   int Volume() {return Volume(0);}
   void Free() {
      if (dimen) {
         delete [] dimen;
         dimen = 0;
      }
      if (oarray) {
         delete [] oarray;
         oarray = 0;
      }
   }
   O* PTR() {
      return &oarray[GetFullIndex()];
   }
   int GetThisIndex(int dindex) {
      return dimen[dindex%ndim].i;
   }
   int GetFullIndex() {
      int total = 0;
      for (int dim = 0 ; dim < ndim ; ++dim) {
         total += GetThisIndex(dim)*Volume(dim + 1);
      }
      return total;
   }
   NDA& operator[](int n) {
      if (dim_index == 0) {
         ResetIndices();
      }
      DIM* d = &dimen[dim_index];
      if (n >= d->sz) {
         n %= d->sz;
      }
      d->i = n;
      ++dim_index;
      if (dim_index >= ndim) {
         dim_index = 0;
      }
      return *this;
   }
};

*/

#endif // NDimensionArray_HPP



