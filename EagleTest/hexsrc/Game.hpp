



#ifndef Game_HPP
#define Game_HPP

#include "Hexagon.hpp"

#include <map>
#include "PRNG.hpp"

#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"

/**
 *    Points are based on territorial advantages.
 *    When fighting for a contested territory, each player gets as many d6 as they have neighbor tiles to the territory in question.
 */


struct PATH {
   std::vector<HexTile*> path;
   PATH() : path() {}
   PATH(HexTile* start) : path() {PushBack(start);}
   PATH(HexTile* start , HexTile* next) : path() {PushBack(start);PushBack(next);}
   
//   void PushBack(HexTile* next);
   void PushBack(HexTile* next) {
      path.push_back(next);
   }
};

typedef struct std::list<PATH> PATHSET;



enum GAMESTATE {
   PLACE_TERRITORY = 0,
   PLAY_GAME = 1,
   BATTLE_TURN = 2,
   GAME_OVER = 3
};

class HexGame {
   static const double root3;
   
   GAMESTATE gstate;
   
   HexGrid hgrid;
   double rad;
   double xpos,ypos;
   
   std::map<int , Player*> players;

   int turn;
   int plturn;
   int nplayers;
   
   int mx,my;
   
   HexTile* hover;
   int hx,hy;
   bool space;
   bool drag;
   HexTile* dragtile;
   bool rdrag;
   HexTile* rdragtile;
   
   EagleImage* arrowimg;
   EagleGraphicsContext* window;
   
   DiceBattle dbattle;
   
   
   
   void Claim(HexTile* tile , int owner);
   
   int BattleTurn(HexTile* from , HexTile* to);
   
   void DrawArrow(double x1 , double y1 , double x2 , double y2 , EagleColor tint);
   
   /// Event handling
   void HandleFirstTurn(EagleEvent e);
   void HandleRegularTurn(EagleEvent e);
   void HandleBattleTurn(EagleEvent e);
   
   /// Display
   void DisplayFirstTurn(EagleGraphicsContext* win , int x , int y);
   void DisplayRegularTurn(EagleGraphicsContext* win , int x , int y);
   
   
public :
   
   HexGame(EagleGraphicsContext* win);
   ~HexGame();

   void Resize(unsigned int width , unsigned int height , double radius);
   void Position(double x , double y);

   void HandleEvent(EagleEvent e);

   void DisplayOn(EagleGraphicsContext* win , int xpos , int ypos);

   void Run(EagleSystem* sys , EagleGraphicsContext* win);
   
   int NumPlayers() {return nplayers;}

   PATH FindBestPath(HexTile* start , HexTile* destination);
   PATH FindConnectedPath(HexTile* start , HexTile* destination);

   void DrawPath(PATH p , int x , int y);
};




#endif // Game_HPP
