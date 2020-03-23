



#include "PRNG.hpp"
#include "Eagle/Exception.hpp"

#include "Math.hpp"

MTRNG PRNG;



int UINTCOMPARE(const void* p1 , const void* p2) {
   const unsigned int* pa = (const unsigned int*)p1;
   const unsigned int* pb = (const unsigned int*)p2;
   if (!pa || !pb) {
      EAGLE_ASSERT(pa && pb);
      return 0;
   }
   return (*pa > *pb)?-1:((*pa == *pb)?0:1);
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



void DiceBattle::SetDice(Dice def , Dice off) {
   defdice = def;
   offdice = off;
   CalcResult();
}



/** Returns negative if attacker lost armies, positive if defender lost armies */
void DiceBattle::CalcResult() {

   result = 0;
   const std::vector<unsigned int>& defrolls = defdice.GetRolls();
   const std::vector<unsigned int>& offrolls = offdice.GetRolls();
   if (!defrolls.size()) {
      result = offrolls.size();
      return;
   }
   if (!offrolls.size()) {result = 0;return;}

      
   unsigned int nmatches = MIN(defrolls.size() , offrolls.size());
   
   for (unsigned int dnum = 0 ; dnum < nmatches ; ++dnum) {
      int off = offrolls[dnum];
      int def = defrolls[dnum];
      result += (off>def)?1:-1;
   }
   /// Leftover dice win automatically
   result += (offrolls.size() - defrolls.size());
}



/**
void DiceBattle::CalcResult() {
   result = 0;
   /// Returns negative if attacker lost armies, positive if defender lost armies
   const std::vector<unsigned int>& defrolls = defdice.GetRolls();
   const std::vector<unsigned int>& offrolls = offdice.GetRolls();
   if (!defrolls.size()) {
      return offrolls.size();
   }
   if (!offrolls) {return 0;}
   
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
   
   result = casualties;
}
//*/


DiceBattle::DiceBattle(Dice dfrom , Dice dto) {
   SetDice(dfrom , dto);
}



int DiceBattle::GetResult() {
   return result;
}




