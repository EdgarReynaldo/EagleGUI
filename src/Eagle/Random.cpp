


#include "Eagle/Random.hpp"

#include <climits>

void MTRNG::Seed(unsigned int s) {
   wtf.seed(s);
   (void)s;
}



MTRNG::MTYPE MTRNG::Generate() {
   return wtf();
}



unsigned int MTRNG::URand() {
   double d = DRand();
   return (unsigned int)(d*UINT_MAX);
///   return wtf() & 0xffffffff;
}



int MTRNG::IRand() {
   return (int)URand();
}



float MTRNG::FRand() {
   return (float)DRand();
}



double MTRNG::DRand() {
   return (double)wtf()/wtf.max();
}



unsigned int MTRNG::Rand0toNM1(unsigned int n) {
   return URand()%n;
}



unsigned int MTRNG::Rand1toN(unsigned int n) {
   return Rand0toNM1(n) + 1;
}



float MTRNG::Percent() {
   return FRand();
}



double MTRNG::DPercent() {
   return DRand();
}





