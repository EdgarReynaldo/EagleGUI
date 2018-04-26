



#ifndef Random_HPP
#define Random_HPP

#include <random>

/** uses mt19937 mersenne twister from C++ 11 */


class MTRNG {
   
///   MT* mt;
   
public :
   typedef uint_fast32_t MTYPE;
   typedef std::mt19937 MT;

protected :
   MT wtf;

public :
   MTRNG() : wtf() {Seed();}
   
   void Seed(unsigned int s = 0);
   
   MTYPE Generate();
   
   /// These should return all possible values (2^32 - 1), signed or unsigned
   unsigned int URand();
   int IRand();

   /// I think these two range from -1.0 to 1.0
   float FRand();
   double DRand();
   
   unsigned int Rand0toNM1(unsigned int n);
   unsigned int Rand1toN(unsigned int n);

   float Percent();
   double DPercent();

};

#endif // Random_HPP
