
/*
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
 *    EAGLE
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2018+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#ifndef FileWork_H
#define FileWork_H


#include <string>
#include <cstdio>
#include <iostream>



/// Platform neutral getline - reads up unti EOF, \r (OSX), \r\n (WIN), or \n (NIX)

/// May only work in BINARY MODE.
std::istream& GetLinePN(std::istream& is , std::string& s);

/// Same as above, but ONLY works in BINARY MODE!
FILE* GetLinePN(FILE* f , std::string& s);



void SkipWhiteSpace(FILE* f);
bool ReadTextInteger(FILE* f , int* store);


/// ONLY works in BINARY MODE!
int fpeek(FILE* f);




#endif // FileWork_H




