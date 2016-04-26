





#ifndef Alignment_HPP
#define Alignment_HPP


#include <string>


enum HALIGNMENT {
   HALIGN_LEFT = 0,
   HALIGN_CENTER = 1,
   HALIGN_RIGHT = 2
};

enum VALIGNMENT {
   VALIGN_TOP = 0,
   VALIGN_CENTER = 1,
   VALIGN_BOTTOM = 2
};

std::string PrintAlignment(HALIGNMENT halign , VALIGNMENT valign);



#endif // Alignment_HPP

