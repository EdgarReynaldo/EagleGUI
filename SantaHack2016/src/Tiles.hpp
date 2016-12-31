



#ifndef Tiles_HPP
#define Tiles_HPP



#include "Eagle.hpp"

#include <vector>
#include <list>
using namespace std;


extern const int TILE_WIDTH;
extern const int TILE_HEIGHT;



enum MOVE_FLAGS {
   IMPASSABLE = 0,
   PASSABLE_HUMAN = 1,
   PASSABLE_BUNNY = 2,
   ENTERABLE_HUMAN = 4,
   ENTERABLE_BUNNY = 8,
   MOVE_HUMAN = 5,
   MOVE_BUNNY = 10,
   MOVE_ALL = 15,
};


#define NUM_TILE_TYPES 30
#define TILE_MAP_NUM_TILES_WIDE 6
#define TILE_MAP_NUM_TILES_TALL 5

enum TILE_TYPE  {
   TILE_EMPTY = -1,
   SNOWY = 0,
   GRASS = 1,
   DIRT = 2,
   STONE = 3,
   FLOOR = 4,
   BRICK = 5,
   FROSTY_ROUND_TREE = 6,
   GREEN_ROUND_TREE = 7,
   BROWN_ROUND_TREE = 8,
   GREEN_FIR_TREE = 9,
   SNOWY_FIR_TREE = 10,
   CARROTS = 11,
   FENCE_CROSS = 12,
   FENCE_VERT = 13,
   FENCE_HORIZ = 14,
   BAMBOO_GATE_HORIZ_CLOSED = 15,
   BAMBOO_GATE_VERT_CLOSED = 16,
   BAMBOO_FENCE_HORIZ = 17,
   SWITCH_OFF = 18,
   SWITCH_ON = 19,
   SIGN_POST = 20,
   BAMBOO_GATE_HORIZ_OPEN = 21,
   BAMBOO_GATE_VERT_OPEN = 22,
   BAMBOO_FENCE_VERT = 23,
   COMPUTER = 24,
   PCB_DIRTY = 25,
   PCB_CLEAN = 26,
   FLOOR_VENT = 27,
   DOOR_CLOSED = 28,
   DOOR_OPEN = 29
   
};

class TileBase {

protected :
   
   EagleImage* tile_image;

   TILE_TYPE tile_type;
      
   int move_flags;
   
   
public :
   
   TileBase(EagleImage* image , TILE_TYPE type);
   virtual ~TileBase() {}
   
   
   virtual void Draw(EagleGraphicsContext* win , int xpos , int ypos);
   
   int CX(int xpos);
   int CY(int ypos);
   
   void SetMoveFlags(int flags);
   
   virtual TILE_TYPE GetTileType();
};



class BambooGateTile : public TileBase {
   
   bool gate_open;
   bool gate_horizontal;
   
   TileBase gate_open_tile;
   TileBase gate_closed_tile;

public :
   
   virtual ~BambooGateTile() {}
   
   BambooGateTile(bool open , bool horizontal , TileBase* open_tile , TileBase* closed_tile);
   virtual void Draw(EagleGraphicsContext* win , int xpos , int ypos);
   virtual TILE_TYPE GetTileType();


};



class SwitchTile : public TileBase {
   
protected :
   bool switch_on;

   TileBase switch_on_tile;
   TileBase switch_off_tile;
   
public :
   
   SwitchTile(bool on , TileBase* on_tile , TileBase* off_tile);
   void Toggle();

   virtual void TurnOn()=0;
   virtual void TurnOff()=0;
   
   virtual void Draw(EagleGraphicsContext* win , int xpos , int ypos);
   virtual TILE_TYPE GetTileType();
   
};


///void SetTileFlags();/// Not public

bool CreateTiles(EagleGraphicsContext* win);
void FreeTiles(EagleGraphicsContext* win);





/// ------------------------------      TileLayer       -----------------------------




class TileBaseLayer {
   vector<vector<TileBase*> > tile_array;
   
   int ncols;
   int nrows;
   
   /// placement on screen
   int mapx;
   int mapy;

public :
   void Resize(int cols , int rows);
   
   int Read(const char* map_string);
   
   int Read(istream& in);

   virtual void Draw(EagleGraphicsContext* win , int viewx , int viewy , int width , int height);
   
   int GetTileAt(int tx , int ty);
   
   TileBase* CreateTile(TILE_TYPE tt);
   TileBase* CreateTile(TILE_TYPE tt) {
      switch (tt) {
/**
   BAMBOO_GATE_HORIZ_CLOSED = 15,
   BAMBOO_GATE_VERT_CLOSED = 16,
   BAMBOO_GATE_HORIZ_OPEN = 21,
   BAMBOO_GATE_VERT_OPEN = 22,
   SWITCH_OFF = 18,
   SWITCH_ON = 19,
   DOOR_CLOSED = 28,
   DOOR_OPEN = 29
   PCB_DIRTY = 25,
   PCB_CLEAN = 26,
*/
      case BAMBOO_GATE_HORIZ_CLOSED :
         return new BambooGateTile(false , true , tile_type_map[BAMBOO_GATE_HORIZ_OPEN] , tile_type_map[BAMBOO_GATE_HORIZ_CLOSED]);
         break;
      case BAMBOO_GATE_HORIZ_OPEN :
         return new BambooGateTile(true , true , tile_type_map[BAMBOO_GATE_HORIZ_OPEN] , tile_type_map[BAMBOO_GATE_HORIZ_CLOSED]);
         break;
      case BAMBOO_GATE_VERT_CLOSED :
         return new BambooGateTile(false , false , tile_type_map[BAMBOO_GATE_VERT_OPEN] , tile_type_map[BAMBOO_GATE_VERT_CLOSED]);
         break;
      case BAMBOO_GATE_VERT_OPEN :
         return new BambooGateTile(true , false , tile_type_map[BAMBOO_GATE_VERT_OPEN] , tile_type_map[BAMBOO_GATE_VERT_CLOSED]);
         break;
      case :
         
         break;
      case :
         
         break;
      case :
         
         break;
      case :
         
         break;
         
      }
   }
};


class TileTerrainLayer : public TileBaseLayer {

public :
   
   TileTerrainLayer();
   
   virtual void Draw(EagleGraphicsContext* win , int viewx , int viewy , int width , int height);
   
   int GetTileAt(int tx , int ty);
};


class TileObjectLayer {

   vector<vector<TileBase* > > object_array;
   
   list<Item*> item_list;

public :
      
   TileObjectLayer();
   


};









#endif // Tiles_HPP

