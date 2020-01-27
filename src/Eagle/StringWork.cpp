
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



#include "Eagle/Exception.hpp"
#include "Eagle/Platform.hpp"
#include "Eagle/StringWork.hpp"

#include <iostream>
#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <cstring>

using std::vector;
using std::string;
using std::list;




unsigned int STRINGPRINTF_BUFFER_SIZE = 1024;

   
string StringPrintF(const char* format_str , ...) {
   char buffer[STRINGPRINTF_BUFFER_SIZE];
   va_list args;
   va_start(args , format_str);
///int vsnprintf (char * s, size_t n, const char * format, va_list arg );
   vsnprintf(buffer , STRINGPRINTF_BUFFER_SIZE , format_str , args);
   va_end(args);
   return std::string(buffer);
}




int CountNewLines(std::string s) {
   int nlines = 1;
   for (unsigned int i = 0 ; i < s.length() ; i++) {
      char c = s[i];
      if (c == '\r' || c == '\n') {
         ++nlines;
         unsigned int i2 = i + 1;
         char c2 = (i2 < s.length())?s[i2]:'\0';
         if (c == '\r' && c2 == '\n') {
            ++i;
         }
      }
   }
   return nlines;
}



vector<string> SplitByNewLinesChomp(std::string s) {
   vector<string> lines;

   if (s.length() == 0) {
      lines.push_back("");
      return lines;
   }

   string line;
   for (unsigned int i = 0 ; i < s.length() ; ) {
      char c = s[i];
      if (c == '\r' || c == '\n') {

         lines.push_back(line);
         line = "";

         unsigned int i2 = i + 1;
         char c2 = (i2 < s.length())?s[i2]:'\0';
         if (c == '\r' && c2 == '\n') {
            ++i;
         }
      }
      else {
         line.push_back(c);
      }
      ++i;
      if (i == s.length() && line.length()) {
         lines.push_back(line);
      }
   }

   return lines;
}



std::vector<std::string> SplitByNewLinesNoChomp(std::string s) {
   std::vector<std::string> lines_chomp = SplitByNewLinesChomp(s);
   for (unsigned int i = 0 ; i < lines_chomp.size() ; ++i) {
      lines_chomp[i].push_back('\n');
   }
   return lines_chomp;
}




std::vector<std::string> SplitByDelimiterString(std::string string_to_split , const std::string token) {

   std::vector<std::string> tokens;
   size_t start_index = 0;
   size_t stop_index = 0;

   if (string_to_split.size() == 0) {
      return tokens;
   }

   const size_t split_string_size = string_to_split.size();

   while (stop_index <= split_string_size && start_index < split_string_size) {
      stop_index = string_to_split.find_first_of(token , start_index);
      if (stop_index == string::npos) {
         stop_index = string_to_split.size();
      }
      string sub = string_to_split.substr(start_index , stop_index - start_index);
      tokens.push_back(sub);
      start_index = stop_index + token.size();
   }
   return tokens;
}



std::string ReadStringUntil(const std::string& str , const char stop) {
   std::string result = "";
   const char* s = str.c_str();
   char duh[2] = {'\0' , '\0'};
   while (*s && *s != stop) {
      duh[0] = *s;
      result.append(duh);
      ++s;
   }
   return result;
}



/** Skips characters in expr string until openchar is reached, 
 *  then reads chars until closechar is reached or the end of the string.
 */
std::string GetArgumentList(const std::string& expr , const char openchar , const char closechar) {
   std::string result = "";
   int index = 0;
   const int end = expr.size();
   while (index < end && expr[index] != openchar) {++index;}
   ++index;/// skip openchar
   char duh[2] = {'\0' , '\0'};
   while (index < end && expr[index] != closechar) {
      duh[0] = expr[index];
      result.append(duh);
      ++index;
   }
   return result;
}



// returns pointer to one past end of quoted string, returns null on failure
const char* CopyQuotedString(const char* start_quote , string& s) {
   s = "";
   const char* c = start_quote + 1;

   if (*start_quote != '\"') {return 0;}// ensure string starts with double quote

   while (*c != '\"') {
      if (*c == '\0') {
         return 0;
         break;
      }
      else {
         s += *c;
         ++c;
      }
   }
   // c is now end quote and s holds quoted string
   c += 1;
   return c;
}



const char* SkipWhiteSpace(const char* str) {
/*
   while ( (*str != '\0') &&
             (*str == ' '  ||
              *str == '\n' ||
              *str == '\r' ||
              *str == '\t' ||
              *str == '\f') )
   {
      ++str;
   }
*/
   while (*str != '\0' && isspace(*str)) {++str;}

   return str;
}



void TrimTrailingWhiteSpace(char* str) {
   size_t index = strlen(str);
   if (index == 0) {return;}

   while(isspace(str[--index])) {str[index] = '\0';}
}



std::string Replace(std::string str , const char* exp , const char* rep) {
   std::string replace_str = str;
   int start = 0;
   int explen = strlen(exp);
   int replen = strlen(rep);
   while ((start = replace_str.find_first_of(exp , start)) != (int)std::string::npos) {
      replace_str.replace(start , explen , rep);
      start += replen;
   }
   return replace_str;
}



char* CStrDup(const char* str) {
   int sz = strlen(str);
   char* dup = (char*)malloc(sizeof(char)*(sz + 1));
   memcpy(dup , str , sz);
   dup[sz] = '\0';
   return dup;
}



std::string GetGuiText(std::string gui_text) {
   std::string stripped_text;
   const char amp = '&';
   for (unsigned int i = 0 ; i < gui_text.size() ; ++i) {

      if (gui_text[i] == amp) {
      // Leading ampersand
         if ((i + 1) < gui_text.size()) {
            if (gui_text[i+1] == amp) {
               // Two ampersands in a row
               stripped_text.push_back(amp);
               ++i;// skip ampersand
            }
            else {
               // Ampersand followed by character, indicates underline, add character to string
               stripped_text.push_back(gui_text[i + 1]);
               ++i;// advance past underlined character
            }
         }
         else {
            // i + 1 >= gui_text.size(), no character after ampersand

            // ignore for now
         }
      }
      else {
         // character is not ampersand, add normally
         stripped_text.push_back(gui_text[i]);
      }
   }
   return stripped_text;
}



std::string GetGuiUnderlineText(std::string gui_text) {
   std::string underline_text;
   const char amp = '&';
   const char underline = '_';
   const char space = ' ';
   for (unsigned int i = 0 ; i < gui_text.size() ; ++i) {
      if (gui_text[i] == amp) {
         // ampersand
         if ((i + 1) < gui_text.size()) {
            // character following ampersand
            if (gui_text[i+1] == amp) {
               // two ampersands - indicates a single ampersand
               underline_text.push_back(space);
               i++;
            }
            else {
               // ampersand followed by other character - indicates an underlined character
               underline_text.push_back(underline);
               i++;
            }
         }
         else {
            // no character following ampersand
            // ignore
         }
      }
      else {
         // normal text - add space to string
         underline_text.push_back(space);
      }
   }
   return underline_text;
}



string GetFileExtension(string& s) {
   string ext("");
   unsigned int index = s.find_last_of('.');
   if ((index != string::npos) && (index + 1 < s.size())) {
      ext = s.substr(index + 1);
   }
   return ext;
}



list<string> TokenizeString(const string& str , char delimiter) {
   list<string> tokens;
   string token;
   const char* s = str.c_str();
   unsigned int size = strlen(s);
   for (unsigned int i = 0 ; i < size ; ++i) {
      if (s[i] == delimiter) {
         tokens.push_back(token);
         token = "";
      } else {
         token.push_back(s[i]);
      }
   }
   tokens.push_back(token);
   return tokens;
}



string RemoveTrailingSlash(string str) {
   if (str.size() == 0) {return str;}
   char tail = str.at(str.size() - 1);
   if ((tail == '\\') || (tail == '/')) {
      str.erase(str.size() - 1 , 1);
   }
   return str;
}



string FixFilenameSlashes(const string& str) {
   char* copy = CStrDup(str.c_str());
   for (int i = 0 ; i < (int)strlen(copy) ; ++i) {
      if ((copy[i] == '\\') || (copy[i] == '/')) {
         copy[i] = EAGLE_NATIVE_PATH_SEP;
      }
   }
   string s = copy;
   free(copy);
   return s;
}



string GetShortFileName(const string& s) {
   if (s.size() == 0) {return string("");}
   unsigned int index = s.find_last_of(EAGLE_NATIVE_PATH_SEP);
   if (index == s.size() - 1) {return string("");}
   string short_name = s.substr(index + 1);
   return short_name;
}



std::string StripFileName(const std::string& s) {
   const char* str = s.c_str();
   int index = 0;
   for (index = strlen(str) ; index >= 0 ; --index) {
      if (str[index] == EAGLE_NATIVE_PATH_SEP || str[index] == EAGLE_OTHER_PATH_SEP) {break;}
   }
   if (index != -1) {
      return s.substr(0 , index);
   }
   return std::string();
}




int MatchingParenthesisIndex(const char* cstr) {
   EAGLE_ASSERT(cstr);
   EAGLE_ASSERT(cstr[0] == '(');
   int parenthesis = 0;
   for (size_t i = 0 ; i < strlen(cstr) ; ++i) {
      char c = cstr[i];
      if (c == '(') {
         ++parenthesis;
      } else if (c == ')') {
         --parenthesis;
         if (parenthesis == 0) {
            return i;
         }
      } else if (c == '\0') {
         return -1;
      }
   }
   return -1;
}



int ReverseStringSearch(const char* str , int start , char c) {
   int index = 0;
   for (int i = start ; i >= 0 ; --i , --index) {
      if (str[i] == c) {
         return index;
      }
   }
   return 1;
}



int StringSearch(const char* str , int start , char c) {
   int index = 0;
   for (int i = start ; i < (int)strlen(str) ; ++i , ++index) {
      if (str[i] == c) {
         return index;
      }
   }
   return -1;
}



int StringSearch(const char* str , int start , const char* find) {
   int index = 0;
   int length = strlen(find);
   for (int i = start ; i < (int)strlen(str) ; ++i , ++index) {
      if (strncmp(str + i , find , length) == 0) {
         return index;
      }
   }
   return -1;
}



string MakeDoubleString(bool is_negative , string numerals , int decimal_point , int exponent) {
   string output;
   char buffer[64];
   if (is_negative) {
      output += '-';
   }
   for (int i = 0 ; (i < decimal_point) && (i < (int)numerals.size()) ; ++i) {
      output += numerals[i];
   }
   if (decimal_point < (int)numerals.size()) {
      output += '.';
   }
   for (int i = decimal_point ; i < (int)numerals.size() ; ++i) {
      output += numerals[i];
   }
   if (exponent) {
      output += 'e';
      sprintf(buffer , "%i" , exponent);
      output += buffer;
   }
   return output;
//   printf("%s\n" , output.c_str());
}


std::string FormatDouble(double d) {
   return FormatDoubleString(StringPrintF("%.30lg" , d));
}



std::string FormatDoubleString(string str) {
   const char* cstr = str.c_str();
   string numerals;
   int exponent = 0;
   int decimal_point = -1;
   // 12345.67890000000e-5
   // 1.2345e10
   int i = 0;
   bool is_negative = false;
   bool exp_is_negative = false;

///   printf("Before formatting double, string is '%s'\n" , cstr);

   if (cstr[i] == '-') {
      is_negative = true;
      ++i;
   }

   for (; i < (int)strlen(cstr) ; ++i) {
      char c = cstr[i];
      if (isdigit(c)) {
         numerals.push_back(c);
      } else if (c == '.') {
         decimal_point = i;
      } else if ((c == 'e') || (c == 'E')) {
         ++i;
         if (cstr[i] == '-') {
            exp_is_negative = true;
            ++i;
         }
         if (cstr[i] == '+') {
            ++i;
         }
         while (cstr[i] == '0') {++i;}
         if (1 != sscanf(cstr + i , "%i" , &exponent)) {
            /// Hmm? No number following the 'e'.
            exponent = 0;
         }
         if (exp_is_negative) {exponent *= -1;}
         break;
      }
   }
   // 12345e2 12345. e2
   if (decimal_point == -1) {decimal_point = (int)strlen(numerals.c_str());}

///   printf("After reading string, formatted double is : '%s'\n" , MakeDoubleString(is_negative , numerals , decimal_point , exponent).c_str());

   // strip trailing zeros
   // 12300.00 -> 12300
   int index = -1;
   cstr = numerals.c_str();
   for (i = (int)strlen(cstr) - 1 ; i >= 0 ; --i) {
      if (cstr[i] == '0') {
         if (i < decimal_point) {
            --decimal_point;
            ++exponent;
         }
         index = i;
      } else {
         break;
      }
   }
   if (index != -1) {
      numerals.erase(index);
      cstr = numerals.c_str();
   }

///   printf("After stripping trailing zeros, formatted double is : '%s'\n" , MakeDoubleString(is_negative , numerals , decimal_point , exponent).c_str());

   // strip leading zeros 0.000123 -> 0000123 1 , 0 -> 000123 1 -1 -> 00123 1 -2 -> 0123 1 -3 -> 123 1 -4
   int zerocount = 0;
   for (i = 0 ; i < (int)strlen(cstr) ; ++i) {
      if (cstr[i] == '0') {
         ++zerocount;
         --exponent;
      } else {
         break;
      }
   }
   if (zerocount) {
      numerals.erase(0 , zerocount);
      cstr = numerals.c_str();
   }

///   printf("After stripping leading zeros , formatted double is : '%s'\n" , MakeDoubleString(is_negative , numerals , decimal_point , exponent).c_str());


   // 0.00000123 - 000000123

   // 123 2 -5 -> 123 1 -4
   // shift decimal point so that it is at index 1 - One numeral followed by decimal point followed by rest of numerals and then exponent
   if (decimal_point != -1) {
      if (decimal_point > 1) {
         exponent += decimal_point - 1;
         decimal_point = 1;
      }
      // 1.23 e3 1230 1.2345 e4 12345 1.23 e-5
      // if exponent is > 0 and < 3 then shift decimal point
      if ((exponent > 0) && (exponent < 3)) {
         decimal_point += exponent;
         exponent = 0;

         // add trailing zeros if necessary
         // 123. 3 0 -> 123 , 12 4 0 -> 1200
         int length = (int)strlen(numerals.c_str());
         if (decimal_point > length) {
            zerocount = decimal_point - length;
            numerals.insert(numerals.end() , zerocount , '0');
            cstr = numerals.c_str();
         }
      }
      // 1.23e-2 0.0123 , 1.23 e-3 0.00123
      if ((exponent > -3) && (exponent  < 0)) {
         zerocount = -exponent;
         exponent = 0;
         // add leading zeros if necessary
         numerals.insert(numerals.begin() , zerocount , '0');
         cstr = numerals.c_str();
      }
   }

   // render string
   string output = MakeDoubleString(is_negative , numerals , decimal_point , exponent);

///   printf("After formatting double, string is : '%s'\n" , output.c_str());

   return output;
}






