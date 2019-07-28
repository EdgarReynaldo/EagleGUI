
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
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Indenter.hpp
 * @brief The interface for consistent indentation in logging
 */

#ifndef Indenter_HPP
#define Indenter_HPP



#include <string>
#include <iostream>


/**! @class Indenter
 *   @brief A simple class to track indentation level and spacing and make logging pretty again
 */

class Indenter {
protected :
   unsigned int indent_level;///< The number of levels of indentation to use
   unsigned int num_spaces;///< The number of spaces to use per indentation level
   std::string indent;///< A string holding the current indent as spaces

   void ResetSpaces();///< Reset the indent string

public :
   Indenter();///< Default empty constructor, uses 0 levels of indentation with 3 spaces per level
   Indenter(int level , int spaces);///< Constructor for specifying indentation level and space count

   void SetLevel(unsigned int indentation_level);///< Set the indentation level
   void SetSpaces(unsigned int number_of_spaces);///< Set the space count per indentation level

   /**! @fn operator++ <> and @fn operator-- <>
    *   @brief Prefix increment and decrement operators for changing the indentation level.
    */
   Indenter& operator++();///< Directly increments the indentation level
   Indenter& operator--();///< Directly decrements the indentation level

   /**! @fn operator++ <int> and @fn operator-- <int>
    *   @brief Postfix increment and decrement operators. Returns copy and then alters object.
    */

   Indenter operator++(int);///< Indirectly increments the indentation level
   Indenter operator--(int);///< Indirectly decrements the indentation level

   friend std::ostream& operator<<(std::ostream& os , const Indenter& i);///< For outputting an indent object to a stream
};



#endif // Indenter_HPP



