


#include "CombinationLock.hpp"


#include <cstdlib>



Safe::Safe() :
      rng(),
      pill_recipe(""),
      pill_recipe_hex(""),
      alpha_to_pill(),
      pill_to_alpha()
{
   GenerateRecipe(0);/// TODO : CHANGE THIS
}
   


void Safe::GenerateRecipe(int rng_seed) {
   rng.SeedRng(rng_seed);
   std::vector<char> digits = {'0' , '1' , '2' , '3' , '4' , '5' , '6' , '7' , 
                               '8' , '9' , 'a' , 'b' , 'c' , 'd' , 'e' , 'f' , ' '};
   std::vector<char> digits2 = digits;
   
   alpha_to_pill.clear();
   pill_to_alpha.clear();
   
   /// Generate pill recipe string (public)
   pill_recipe = "";
   while (digits2.size()) {
      int index = rng.Rand0toNminus1((int)digits2.size());
      char digit[2] = {digits2[index] , '\0'};
      pill_recipe.append(digit);
      digits2.erase(digits2.begin() + index);
   }
   /// Generate pill font encoding (private)
   while (digits.size()) {
      int index = rng.Rand0toNminus1((int)digits.size());
      char digit[2] = {digits[index] , '\0'};
      pill_recipe_hex.append(digit);
      digits.erase(digits.begin() + index);
   }
   /// Generate encoding / decoding maps
   for (int i = 0 ; i < NCHAR ; ++i) {
      alpha_to_pill[pill_recipe[i]] = pill_recipe_hex[i];
      pill_to_alpha[pill_recipe_hex[i]] = pill_recipe[i];
   }
}



std::string Safe::GetAlphaRecipe() {
   return pill_recipe;
}



bool Safe::OpenSafe(std::string recipe) {
   return (pill_recipe_hex.compare(recipe) == 0);
}



char Safe::GetPillEncoding(char hex_char) {
   return alpha_to_pill[hex_char];
}



std::string Safe::GetPillEncoding(std::string hex_string) {
   std::string s;
   for (unsigned int i = 0 ; i < hex_string.length() ; ++i) {
      char e[2] = {GetPillEncoding(hex_string[i]) , '\0'};
      s.append(e);
   }
   return s;
}





