



#ifndef Player_HPP
#define Player_HPP


#include "allegro5/allegro.h"

#include "Territory.hpp"


class Player {
   ALLEGRO_COLOR our_color;
   TerritoryList our_turf;
   
   int points;
   
   int pid;/// Our player id
   
public :
//   Player(ALLEGRO_COLOR teamcolor);
   Player(ALLEGRO_COLOR teamcolor) :
         our_color(teamcolor),
         our_turf(),
         points(0),
         pid(-1)
   {}
   
   friend class HexGrid;
   friend class HexGame;
};



#endif // Player_HPP
