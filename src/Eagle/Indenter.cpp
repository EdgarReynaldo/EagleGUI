
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




#include "Eagle/Indenter.hpp"



void Indenter::ResetSpaces() {
   indent.clear();
   indent.append(indent_level*num_spaces , ' ');
}



Indenter::Indenter() :
      indent_level(0) , num_spaces(3) , indent("")
   {}



Indenter::Indenter(int level , int spaces) :
      indent_level(0) , num_spaces(3) , indent() {
   if (level < 0) {level = 0;}
   if (spaces < 1) {spaces = 1;}
   indent_level = level;
   num_spaces = spaces;
   ResetSpaces();
}



void Indenter::SetLevel(unsigned int indentation_level) {
   indent_level = indentation_level;
   ResetSpaces();
}



void Indenter::SetSpaces(unsigned int number_of_spaces) {
   num_spaces = number_of_spaces;
   ResetSpaces();
}


/// Prefix
Indenter& Indenter::operator++() {
   ++indent_level;
   ResetSpaces();
   return *this;
}



Indenter& Indenter::operator--() {
   if (indent_level) {
      --indent_level;
      ResetSpaces();
   }
   return *this;
}


/// Postfix
Indenter Indenter::operator++(int) {
   Indenter copy = *this;
   ++indent_level;
   ResetSpaces();
   return copy;
}



Indenter Indenter::operator--(int) {
   Indenter copy = *this;
   if (indent_level) {
      --indent_level;
      ResetSpaces();
   }
   return copy;
}



std::ostream& operator<<(std::ostream& os , const Indenter& i) {
   os << i.indent.c_str();
   return os;
}



