



#ifndef PRNG_HPP
#define PRNG_HPP

#include <vector>
#include "Eagle/Random.hpp"

      
extern MTRNG PRNG;

int UINTCOMPARE(const void* p1 , const void* p2);

class Dice {

   unsigned int ndice;
   unsigned int dsize;
   std::vector<unsigned int> rolls;
   
   void Reroll();
//   void SortDescending();
   void SortDescending() {
      qsort(&rolls[0] , rolls.size() , sizeof(unsigned int) , UINTCOMPARE);
   }
public :
   Dice(unsigned int num , unsigned int sz);
   const std::vector<unsigned int>& GetRolls() {return rolls;}
};

int DiceBattle(Dice dfrom , Dice dto);/// Returns negative if attacker lost armies, positive if defender lost armies

#endif // PRNG_HPP

