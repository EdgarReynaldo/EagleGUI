
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
 * @file FileWork.hpp
 * @brief Some helper functions for working with files and FILE*s
 */

#ifndef FileWork_HPP
#define FileWork_HPP



#include <string>
#include <cstdio>
#include <iostream>



/**! @fn GetLinePN
 *   @brief Platform neutral getline - reads up unti EOF, \r (OSX), \r\n (WIN), or \n (NIX)
 */

///< Get a line from an istream. May only work in BINARY MODE.
std::istream& GetLinePN(std::istream& is , std::string& s);

///< Get a line from a FILE*. ONLY works in BINARY MODE!
FILE* GetLinePN(FILE* f , std::string& s);


///< Skip all white space for the given FILE*
void SkipWhiteSpace(FILE* f);

///< Read an integer string from the FILE* and store it
bool ReadTextInteger(FILE* f , int* store);

/// ONLY works in BINARY MODE!
int fpeek(FILE* f);

///< Skip all forward and backward slashes from the current point forward
///< Returns the index of the first non-slash character
unsigned int SkipSeparators(const std::string& p , unsigned int index);




#endif // FileWork_HPP



