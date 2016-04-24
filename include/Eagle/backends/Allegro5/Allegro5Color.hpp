

#ifndef Allegro5Color_HPP
#define Allegro5Color_HPP

#include "Eagle/Color.hpp"

#include "allegro5/allegro.h"

ALLEGRO_COLOR GetAllegroColor(EagleColor c);
EagleColor GetEagleColor(ALLEGRO_COLOR c);



#endif // Allegro5Color_HPP

