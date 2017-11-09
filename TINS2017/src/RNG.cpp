

#include <cstdlib>

#include "RNG.hpp"


RNG rng;



void RNG::SeedRng(int seed) {srand(seed);}

int RNG::Rand0toNminus1(int n) {
   return rand()%n;
}
int RNG::Rand0toN(int n) {
   return Rand0toNminus1(n+1);
}

