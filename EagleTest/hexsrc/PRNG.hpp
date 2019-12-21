



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
   
   friend class DiceBattle;
   
   void Reroll();
//   void SortDescending();
   void SortDescending() {
      qsort(&rolls[0] , rolls.size() , sizeof(unsigned int) , UINTCOMPARE);
   }
public :
   Dice() : ndice(0) , dsize(0) , rolls() {}
   
   Dice(unsigned int num , unsigned int sz);
   const std::vector<unsigned int>& GetRolls() {return rolls;}
};



class DiceBattle {
   
   Dice defdice;
   Dice offdice;
   int result;
   
   void CalcResult();
public :
   
   DiceBattle() : defdice() , offdice() , result(0) {}
   
   DiceBattle(Dice dfrom , Dice dto);
   /// Returns negative if attacker lost armies, positive if defender lost armies

   void SetDice(Dice def , Dice off);

   int GetResult();

   const std::vector<unsigned int>& GetOffRolls() {return offdice.rolls;}
   const std::vector<unsigned int>& GetDefRolls() {return defdice.rolls;}
};



#endif // PRNG_HPP


