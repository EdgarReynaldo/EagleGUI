
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

