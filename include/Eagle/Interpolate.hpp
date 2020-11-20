
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
 * @file Interpolate.hpp
 * @brief Simple linear interpolation function in template form
 * 
 */

#ifndef Interpolate_HPP
#define Interpolate_HPP



/**! @brief Perform linear interpolation between v1 and v2 */

template <class Value>
inline Value Interpolate(const Value& v1 , const Value& v2 , double frac) {
   return v1 + (v2 - v1)*frac;
}



#endif // Interpolate_HPP



