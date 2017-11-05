


#ifndef CombinationLock_HPP
#define CombinationLock_HPP



#include "PillFont.hpp"




class RNG {
public :
   
   void SeedRng(int seed) {srand(seed);}
   
   int Rand0toNminus1(int n) {
      return rand()%n;
   }
   int Rand0toN(int n) {
      return Rand0toNminus1(n+1);
   }
};

class Safe {
   
   RNG rng;
   
   std::string pill_recipe;/// 17 digit base 17 number composed of a-f, 0-9, and ' ', once each. 17! combinations
   std::string pill_recipe_hex;
   
   std::map<char , char> alpha_to_pill;
   std::map<char , char> pill_to_alpha;
   
   std::map<char , char> known_alpha_to_pill;
   std::map<char , char> known_pill_to_alpha;
   
   std::string DecodePillString(std::string pill_string);
public :
   
   Safe();
   void GenerateRecipe(int rng_seed);
   
   std::string GetAlphaRecipe();

   bool OpenSafe(std::string recipe);

   char GetPillEncoding(char hex_char);

   std::string GetPillEncoding(std::string hex_string);

   bool Decode(std::string hex_string , std::string pill_string);
};




#endif // CombinationLock_HPP
