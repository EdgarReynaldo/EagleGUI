



#include "GameMap.hpp"




const char* const default_game_map = 
"12 , 14\n"
"-1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1\n"
"-1 ,  0 ,  0 ,  0 ,  0 ,  0 ,  1 ,  1 ,  1 ,  1 ,  1 , -1\n" 
"-1 ,  0 ,  0 ,  0 ,  0 ,  0 ,  1 ,  1 ,  1 ,  1 ,  1 , -1\n"
"-1 ,  0 ,  0 ,  0 ,  0 ,  0 ,  1 ,  2 ,  2 ,  2 ,  1 , -1\n"
"-1 ,  0 ,  0 ,  0 ,  0 ,  0 ,  5 ,  5 ,  5 ,  5 ,  5 , -1\n"
"-1 ,  0 ,  0 ,  0 ,  0 ,  0 ,  5 ,  4 ,  4 ,  4 ,  5 , -1\n"
"-1 ,  3 ,  3 ,  3 ,  3 ,  3 ,  4 ,  4 ,  4 ,  4 ,  5 , -1\n"
"-1 ,  0 ,  0 ,  0 ,  0 ,  0 ,  5 ,  4 ,  4 ,  4 ,  5 , -1\n"
"-1 ,  0 ,  0 ,  0 ,  0 ,  0 ,  5 ,  4 ,  4 ,  4 ,  5 , -1\n"
"-1 ,  0 ,  0 ,  0 ,  0 ,  0 ,  5 ,  5 ,  4 ,  5 ,  5 , -1\n"
"-1 ,  0 ,  0 ,  0 ,  0 ,  0 ,  1 ,  1 ,  3 ,  1 ,  1 , -1\n"
"-1 ,  0 ,  0 ,  0 ,  0 ,  0 ,  1 ,  1 ,  3 ,  1 ,  1 , -1\n"
"-1 ,  0 ,  0 ,  0 ,  0 ,  0 ,  1 ,  1 ,  3 ,  1 ,  1 , -1\n"
"-1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1 , -1\n"
;






void GameMap::Draw(EagleGraphicsContext* win , int xpos , int ypos , int width , int height) {
   terrain_layer.Draw(win , xpos , ypos , width , height);
}



void GameMap::Load(const char* terrain_map) {
   terrain_layer.Read(terrain_map);
}


