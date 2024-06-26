



#ifndef Hexagon_HPP
#define Hexagon_HPP


#include <unordered_set>
#include <vector>
#include <map>


#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"


class EagleGraphicsContext;

enum HEXCORNER {
   HC_EAST         = 0,
   HC_SOUTHEAST    = 1,
   HC_SOUTHWEST    = 2,
   HC_WEST         = 3,
   HC_NORTHWEST    = 4,
   HC_NORTHEAST    = 5,
   NUM_HEX_CORNERS = 6
};

enum HEXDIRECTION {
   HD_NORTH     = 0,
   HD_NORTHEAST = 1,
   HD_SOUTHEAST = 2,
   HD_SOUTH     = 3,
   HD_SOUTHWEST = 4,
   HD_NORTHWEST = 5,
   NUM_HEX_DIRECTIONS = 6
};


std::string HexDirectionToString(HEXDIRECTION d);

double HexDirectionToAngle(HEXDIRECTION d);
HEXDIRECTION AngleToHexDirection(double arad);

void TestAngleToHexDirection();



class Hexagon {
   double radius;
   std::vector<ALLEGRO_VERTEX> verts;
   
   void GenPoints();


public :
   Hexagon() : radius(0.0) , verts(NUM_HEX_DIRECTIONS , ALLEGRO_VERTEX()) {}
   Hexagon(double width);
   
   void SetRadius(double r);
   void DrawFilled(EagleGraphicsContext* win , double x , double y , ALLEGRO_COLOR color);
   void DrawOutline(EagleGraphicsContext* win , double x , double y , ALLEGRO_COLOR color);
};



class HexTile {
   static Hexagon proto;

   unsigned int tx;/// Tile x
   unsigned int ty;/// Tile y

   double mx;/// Map x
   double my;/// Map y

   int owner;
   std::map<int , int> influence;
   std::unordered_set<int> maxinfluence;/// 1 or more players influence this tile

   std::unordered_set<HEXDIRECTION> neighbor_tty;/// Neighbor territories we own
   std::unordered_set<HEXDIRECTION> border_tty;/// Border territories we do not own

   std::vector<HexTile*> neighbors;
   
   int income;
   int armies;
   
   
   friend class HexGrid;
   friend class HexGame;
   friend class Territory;
   friend class TerritoryList;
   friend std::unordered_set<HexTile*>& GetAdjoiningTiles(std::unordered_set<HexTile*>& , HexTile*);
   friend std::unordered_set<HexTile*> GetBorders(std::unordered_set<HexTile*>);
   
   /// unsigned int TID
   
   void CalcIncome();
   
   void CollectIncome();
   
   
   void CalcBorders();
   
public :
   HexTile();
   
   void SetRadius(double r);
   
   void DrawFilled(EagleGraphicsContext* win , double xpos , double ypos , ALLEGRO_COLOR color);
   void DrawOutline(EagleGraphicsContext* win , double xpos , double ypos ,  ALLEGRO_COLOR color);
   void DrawStats(EagleGraphicsContext* win , double xpos , double ypos , ALLEGRO_COLOR color);
   int TotalIncome() {return income;}
};



std::unordered_set<HexTile*> GetAdjoiningTiles(HexTile* start);/// O(N)

std::unordered_set<HexTile*> GetBorders(std::unordered_set<HexTile*> tty);/// O(N)



class Player;

class HexGrid {
   unsigned int w,h,rad;
   
   std::vector<std::vector<HexTile> > grid;
   
   friend class HexGame;
   
   
   bool CheckGrid();
   
public :
   HexGrid();
   
   void Resize(unsigned int width , unsigned int height , double radius);

   void DrawGrid(EagleGraphicsContext* win , int xpos , int ypos);
   void DrawPlayer(EagleGraphicsContext* win , int xpos , int ypos , Player* player);

   void DrawGrid2(EagleGraphicsContext* win , int xpos , int ypos , std::map<int , Player*> players);
   
   unsigned int Width() {return w;}
   unsigned int Height() {return h;}
   
   bool Safe(HexTile* tile);
};


#endif // Hexagon_HPP
