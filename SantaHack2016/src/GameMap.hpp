



#ifndef GameMap_HPP
#define GameMap_HPP




#include "Eagle.hpp"


#include "Tiles.hpp"



extern const char* const default_game_map;


class GameMap {
   
   TileTerrainLayer terrain_layer;
   
public :
   
///   GameMap();
   GameMap() :
         terrain_layer()
   {}
   
   void Draw(EagleGraphicsContext* win , int xpos , int ypos , int width , int height);
   
   void Load(const char* terrain_map);
   
};



#endif // GameMap_HPP

