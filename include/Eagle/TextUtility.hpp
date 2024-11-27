
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file TextUtility.hpp
 * @brief Some line wrapping algorithms. WIP.
 * 
 * 
 * 
 */

#ifndef TextUtility_HPP
#define TextUtility_HPP


#include <string>
#include <vector>

class EagleFont;
std::vector<std::string> TextLinesCharWrap(std::string text , EagleImage* font , int textpixelwidth);
std::vector<std::string> TextLinesWordWrap(std::string text , EagleImage* font , int textpixelwidth);

std::vector<std::string> TextLinesCharWrap(std::string text , EagleImage* font , int textpixelwidth) {
   const int TPW = textpixelwidth;
   
   EAGLE_ASSERT(font);
   
   std::vector<std::string> lines;
   std::string line;
   for (unsigned int i = 0 ; i < text.size() ; ++i) {
      char c = text[i];
      char c2 = '\0';
      if (c == '\r' || c == '\n') {
         lines.push_back(line);
         if (i + 1 < text.size()) {
            c2 = text[i+1];
         }
         if (c2 == '\n') {
            i++;
         }
         continue;
      }
      line.push_back(c);
      if (font->Width(line) > TPW) {
         line.pop_back();
         lines.push_back(line);
         line.clear();
         --i;
         continue;
      }
      if ((i + 1) == text.size()) {
         lines.push_back(line);
         break;
      }
   }
   return lines;
   
}
std::vector<std::string> TextLinesWordWrap(std::string text , EagleImage* font , int textpixelwidth) {
   const int TPW = textpixelwidth;
   
   EAGLE_ASSERT(font);
   
   std::vector<std::string> lines;
   std::string line;
   std::string word;
   for (unsigned int i = 0 ; i < text.size() ; ++i) {
      char c = text[i];
      char c2 = '\0';
      if (c == '\r' || c == '\n') {
         lines.push_back(line);
         if (i + 1 < text.size()) {
            c2 = text[i+1];
         }
         if (c2 == '\n') {
            i++;
         }
         continue;
      }
      word.push_back(c);
      std::string current = line;
      current.insert(current.end() , word.begin() , word.end());
      if (font->Width(current) > TPW) {
         lines.push_back(line);
         line.clear();
         line.insert(line.end() , word.begin() , word.end());
         word.clear();
         continue;
      }
      if ((i + 1) == text.size()) {
         line.insert(line.begin() , word.begin() , word.end());
         lines.push_back(line);
         break;
      }
   }
   return lines;
   
}





#endif // TextUtility_HPP








