
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
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */

#include "Eagle/Random.hpp"
#include "Eagle/Math.hpp"



#include <climits>



MTRNG::MTRNG() : 
      mt()
{
   Seed();
}



void MTRNG::Seed(unsigned int s) {
   mt.seed(s);
}



MTRNG::MTYPE MTRNG::Generate() {
   return mt();
}



unsigned int MTRNG::URand() {
   return (unsigned int)(mt());
}



int MTRNG::IRand() {
   return (int)URand();
}



float MTRNG::FRand() {
   return (float)DRand();
}



double MTRNG::DRand() {
   ///< A double is 8 bytes of precision, and our rng outputs 4 bytes at a time, combine two random numbers to make a double
   MTYPE hiword = mt();
   MTYPE loword = mt();
   double d = ((unsigned long long)hiword << 32) | (unsigned long long)loword; 
   if (std::isinf(d) || std::isnan(d)) {
      return DRand();///< Try again
   }
   return d;
}



unsigned int MTRNG::Rand0toNM1(unsigned int n) {
   return URand()%n;
}



unsigned int MTRNG::Rand1toN(unsigned int n) {
   return Rand0toNM1(n) + 1;
}



float MTRNG::FPercent() {
   return (float)DPercent();
}



double MTRNG::DPercent() {
   return (double)mt()/mt.max();
}





