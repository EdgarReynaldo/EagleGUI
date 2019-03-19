
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



#ifndef Indenter_HPP
#define Indenter_HPP


#include <string>
#include <iostream>


class Indenter {
private :
   unsigned int indent_level;
   unsigned int num_spaces;
   std::string indent;

   void ResetSpaces();

public :
   Indenter();
   Indenter(int level , int spaces);

   void SetLevel(unsigned int indentation_level);
   void SetSpaces(unsigned int number_of_spaces);


   /// Prefix increment and decrement operators for changing the indentation level.
   Indenter& operator++();
   Indenter& operator--();
   /// Postfix - returns copy and increments indenter
   Indenter operator++(int);
   Indenter operator--(int);

   friend std::ostream& operator<<(std::ostream& os , const Indenter& i);
};





#endif // Indenter_HPP

