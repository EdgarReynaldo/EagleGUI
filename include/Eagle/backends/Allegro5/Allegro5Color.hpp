
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
 * @file Allegro5Color.hpp
 * @brief The interface for working with Eagle and Allegro colors
 */

#ifndef Allegro5Color_HPP
#define Allegro5Color_HPP



#include "Eagle/Color.hpp"

#include "allegro5/allegro.h"



ALLEGRO_COLOR GetAllegroColor(EagleColor c);///< Get an ALLEGRO_COLOR from an EagleColor

EagleColor GetEagleColor(ALLEGRO_COLOR c);///< Get an EagleColor from an ALLEGRO_COLOR




#endif // Allegro5Color_HPP



