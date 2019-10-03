



#include "PRNG.hpp"
#include "Eagle/Exception.hpp"



MTRNG PRNG;



int UINTCOMPARE(const void* p1 , const void* p2) {
   const unsigned int* pa = (const unsigned int*)p1;
   const unsigned int* pb = (const unsigned int*)p2;
   if (!pa || !pb) {
      EAGLE_ASSERT(pa && pb);
      return 0;
   }
   return (*pa > *pb)?1:((*pa == *pb)?0:-1);
}



void Dice::Reroll() {
   rolls.clear();
   rolls.resize(ndice);
   for (unsigned int i = 0 ; i < ndice ; ++i) {
      rolls[i] = PRNG.Rand1toN(dsize);
   }
   SortDescending();
}



Dice::Dice(unsigned int num , unsigned int sz) :
      ndice(num),
      dsize(sz),
      rolls()
{
   Reroll();
}



int DiceBattle(Dice dfrom , Dice dto) {
   /// Returns negative if attacker lost armies, positive if defender lost armies
   const std::vector<unsigned int>& defrolls = dto.GetRolls();
   const std::vector<unsigned int>& offrolls = dfrom.GetRolls();
   if (!defrolls.size()) {
      return offrolls.size();
   }
   int casualties = 0;
   unsigned int d = 0;
   unsigned int o = 0;
   unsigned int i = 0;
   unsigned int oldi = 0;
   unsigned int j = 0;
   unsigned int oldj = 0;
   for (i = 0 ; i < defrolls.size() ; ++i) {
      oldi = i;
      d = defrolls[i];
      j = oldj;
      while (d >= offrolls[j] && j < offrolls.size()) {
         ++j;
      }
      oldj = j;
      if (j < offrolls.size()) {
         o = offrolls[j];
         /// Defender wins this one
         casualties -= 1;
      }
   }
   
   i = oldi;
   for (j = oldj ; j < offrolls.size() ; ++j) {
      o = offrolls[j];
      d = defrolls[i];
      if (o > d) {
         /// Attacker wins this one
         casualties += 1;
      }
      ++i;
      if (i >= defrolls.size()) {break;}
      ++j;
   }
   
   return casualties;
}

