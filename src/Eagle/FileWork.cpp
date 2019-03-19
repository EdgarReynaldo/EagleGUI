
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
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */




#include "Eagle/FileWork.hpp"
#include "Eagle/Exception.hpp"

#include <cstdlib>

using std::string;


/// Get Line Platform Neutral
std::istream& GetLinePN(std::istream& is , std::string& s) {
   s = "";
   char c;
   if (!is.good()) {return is;}
   while (!((is.get(c)).eof())) {
      if (c == '\r') {// WINDOWS OR OSX
         int c2 = is.peek();
         if (c2 == '\n') {
            is.get(c);// use up \n
         }
         return is;
      }
      if (c == '\n') {// NIX
         return is;
      }
      s += c;
   }
   return is;
}



FILE* GetLinePN(FILE* f , std::string& s) {
   s = "";
   if (!f || feof(f) || ferror(f)) {return f;}
   int c;
   while ((c = fgetc(f)) != EOF) {
      if (c == '\r') {// WINDOWS OR OSX
         int c2 = fpeek(f);
         if (c2 == '\n') {
            c = fgetc(f);// use up \n
         }
         return f;
      }
      if (c == '\n') {// NIX
         return f;
      }
      s += (char)c;
   }
   return f;
}



void SkipWhiteSpace(FILE* f) {
   int c = 0;
   while (1) {
      c = fgetc(f);
      if (!((c == ' ') ||
          (c == '\t') ||
          (c == '\n') ||
          (c == '\f') ||
          (c == '\r'))) {
         // not whitespace
         if (c != EOF) {
            ungetc(c , f);
         }
         break;
      }
   }
}


bool ReadTextInteger(FILE* f , int* store) {
   EAGLE_ASSERT(f);
   EAGLE_ASSERT(store);
   return (1 == fscanf(f , "%d" , store));
}
/* OLD
bool ReadTextInteger(FILE* f , int* store) {
   EAGLE_ASSERT(store);

   string s;
   int i = 0;
   bool isnumber = false;

   while (1) {
      int c = fgetc(f);
      if (isdigit(c)) {
         isnumber = true;
         s += (char)c;
      } else {
         if (c != EOF) {
            ungetc(c , f);
         }
         break;
      }
   }

   if (isnumber) {
      i = atoi(s.c_str());
      *store = i;
      return true;
   }

   return false;
}
*/

int fpeek(FILE* f) {
   if (!f || feof(f) || ferror(f)) {return EOF;}
   long pos = ftell(f);
   int c = fgetc(f);
   fseek(f , pos , SEEK_SET);
   EAGLE_ASSERT(pos == ftell(f));
   return c;
}



unsigned int SkipSeparators(const std::string& p , unsigned int index) {
   unsigned int stop = p.length();
   while ((index < stop) && (p[index] == '\\' || p[index] == '/')) {
      ++index;
   }
   return index;
}





