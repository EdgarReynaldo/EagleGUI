



#ifndef Game_HPP
#define Game_HPP

#include "Hexagon.hpp"

#include <map>


#include "Eagle.hpp"


class HexGame {
   static const double root3;
   
   
   HexGrid hgrid;
   double rad;
   double xpos,ypos;
   
   std::map<int , Player*> players;

   int turn;
   int nplayers;
   
   int mx,my;
   
   HexTile* hover;
   int hx,hy;

      
   void Claim(HexTile* tile , int owner);
   
   
public :
   
   HexGame();
   ~HexGame();

   void Resize(unsigned int width , unsigned int height , double radius);
   void Position(double x , double y);

   void HandleEvent(EagleEvent ee);

   void DisplayOn(EagleGraphicsContext* win , int xpos , int ypos);

   void Run(EagleSystem* sys , EagleGraphicsContext* win);
   
   int NumPlayers() {return nplayers;}
};




#endif // Game_HPP
