
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
 *    Copyright 2009-2020+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file 
 * @brief
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
   int width = 0 , cwidth = 0 , estwidth = 0;
   double avewidth = 0.0;
}
std::vector<std::string> TextLinesWordWrap(std::string text , EagleImage* font , int textpixelwidth) {
   
}

#endif // TextUtility_HPP
