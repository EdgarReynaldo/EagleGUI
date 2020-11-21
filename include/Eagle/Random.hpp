
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
 * @file Random.hpp
 * @brief A simple implementation of the Mersenne Twister 19937 capabilities using C++11
 */

#ifndef Random_HPP
#define Random_HPP



#include <random>



/**! @class MTRNG
 *   @brief A 19937 implementation of the mersenne twister class using C++11
 */

class MTRNG {
   
   typedef std::mt19937 MT;
public :
   typedef uint_fast32_t MTYPE;

protected :
   MT mt;

public :
   MTRNG();
   
   void Seed(unsigned int s = 0);
   
   MTYPE Generate();///
   
   /// These return all possible values (2^32 - 1), signed or unsigned

   unsigned int URand();///< Returns a random unsigned int from 0 to 2^32 - 1

   int IRand();///< Returns a random signed int from INT_MIN to INT_MAX

   /**! @fn FRand @fn DRand
    *   @brief Returns a random, valid, non-inf , non-nan double or float
    */
   float FRand();
   double DRand();
   
   /**! @fn Rand0toNM1 <unsigned int>
    *   @brief Returns a random unsigned integer from 0 to N minus 1 (like rolling an N-sided die)
    */
   unsigned int Rand0toNM1(unsigned int n);

   /**! @fn Rand1toN <unsigned int>
    *   @brief Returns a random unsigned integer from 1 to N
    */
   unsigned int Rand1toN(unsigned int n);

   /**! @fn FPercent
    *   @brief Returns a float from [0.0f,1.0f]
    */
   float FPercent();

   /**! @fn DPercent
    *   @brief Returns a double from [0.0,1.0]
    */
   double DPercent();

};




#endif // Random_HPP




