



#ifndef Tiles_HPP
#define Tiles_HPP


extern const int TILE_WIDTH;
extern const int TILE_HEIGHT;



enum MOVE_FLAGS = {
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

enum TILE_TYPE = {
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

   TILE_TYPE type;
      
   int move_flags;
   
   
public :
   
   TileBase(EagleImage* image , TILE_TYPE type);
   
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
   
   TILE_TYPE gate_type1;
   TILE_TYPE gate_type2;
   
public :
   
   BambooGateTile(bool open , bool horizontal , TileBase* open_tile , TileBase* closed_tile);
   BambooGateTile(bool open , bool horizontal , TileBase* open_tile , TileBase* closed_tile) :
         gate_open(open),
         gate_horizontal(horizontal),
         gate_open_tile(*open_tile),
         gate_closed_tile(*closed_tile)
   {
      
   }
   
   
   virtual void Draw(EagleGraphicsContext* win , int xpos , int ypos) {
      if (gate_open) {
         gate_open_tile.Draw(win , xpos , ypos);
      }
      else {
         gate_closed_tile.Draw(win , xpos , ypos);
      }
   }
   
}



///void SetTileFlags();/// Not public



bool CreateTiles(EagleGraphicsContext* win);
void FreeTiles(EagleGraphicsContext* win);




class TileLayerBase {
   
   virtual int GetTileAt(int tx , int ty);
}

class TileTerrainLayer {

   vector<vector<Tile*> > tile_array;

public :
   
   void Resize(int cols , int rows);
   void Resize(int cols , int rows) {
      tile_array.resize(cols , vector<Tile*>(rows , 0));
   }
   void Read(istream& in);
   void Read(istream& in) {
      int cols = 0;
      int rows = 0;
      in >> cols >> rows;
      Resize(cols,rows);
      for (int y = 0 ; y < rows ; ++y) {
         for (int x = 0 ; x < cols ; ++x) {
            int tile_num = -1;
            in >> tile_num;/// -1 marks empty tile
            tile_array[y][x] = tile_type_map[tile_num];
         }
      }
   }
};



#endif // Tiles_HPP

