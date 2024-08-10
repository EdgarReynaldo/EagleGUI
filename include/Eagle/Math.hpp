
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
 *    Copyright 2009-2024+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Math.hpp
 * @brief Interface for working with math
 *
 * Currently only defines M_PI
 */

#ifndef Math_HPP
#define Math_HPP



#include <cmath>



#ifndef M_PI
   #define M_PI (3.14159265358979323846)
#endif

template <class Type>
Type Sign(Type value) {
   if (value < 0) {return (Type)-1;}
   else if (value > 0) {return (Type)1;}
   else {
      return (Type)0;
   }
}

template <class Type>
Type Magnitude(const Type& value) {return (Type)fabs(value);}

template <class Type>
Type MIN(const Type& t1 , const Type& t2) {(t1<t2)?t1:t2;}

template <class Type>
Type MAX(const Type& t1 , const Type& t2) {(t1>t2)?t1:t2;}

#endif // Math_HPP



