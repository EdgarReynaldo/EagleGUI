
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
 * @file Alignment.hpp
 * @brief Simple alignment control
 * 
 */

#ifndef Alignment_HPP
#define Alignment_HPP



#include <string>



/**! @enum HALIGNMENT
 *   @brief Controls the horizontal alignment
 */
enum HALIGNMENT {
   HALIGN_LEFT   = 0,///< Align with the left side
   HALIGN_CENTER = 1,///< Align with the center horizontally
   HALIGN_RIGHT  = 2 ///< Align with the right side
};

/**! @enum VALIGNMENT
 *   @brief Controls the vertical alignment
 */
enum VALIGNMENT {
   VALIGN_TOP    = 0,///< Align with the top
   VALIGN_CENTER = 1,///< Align with the center vertically
   VALIGN_BOTTOM = 2 ///< Align with the bottom
};


///< Helper function to print alignment values
std::string PrintAlignment(HALIGNMENT halign , VALIGNMENT valign);



#endif // Alignment_HPP



