



#include "Tiles.hpp"


const int TILE_WIDTH = 64;
const int TILE_HEIGHT = 64;


EagleImage* tile_map_image;
EagleImage* tile_images[NUM_TILE_TYPES];

map<int , TileBase*> tile_type_map;




/// ----------------------     Global functions      --------------------------------



void SetTileFlags() {
   tile_type_map[SNOWY].SetMoveFlags(MOVE_ALL);
   tile_type_map[GRASS].SetMoveFlags(MOVE_ALL);
   tile_type_map[DIRT].SetMoveFlags(MOVE_ALL);
   tile_type_map[STONE].SetMoveFlags(MOVE_ALL);
   tile_type_map[FLOOR].SetMoveFlags(MOVE_ALL);
   tile_type_map[BRICK].SetMoveFlags(IMPASSABLE);
   tile_type_map[FROSTY_ROUND_TREE].SetMoveFlags(MOVE_ALL);
   tile_type_map[GREEN_ROUND_TREE].SetMoveFlags(MOVE_ALL);
   tile_type_map[BROWN_ROUND_TREE].SetMoveFlags(MOVE_ALL);
   tile_type_map[GREEN_FIR_TREE].SetMoveFlags(MOVE_ALL);
   tile_type_map[SNOWY_FIR_TREE].SetMoveFlags(MOVE_ALL);
   tile_type_map[CARROTS].SetMoveFlags(MOVE_ALL);
   tile_type_map[FENCE_CROSS].SetMoveFlags(MOVE_BUNNY);
   tile_type_map[FENCE_VERT].SetMoveFlags(MOVE_BUNNY);
   tile_type_map[FENCE_HORIZ].SetMoveFlags(MOVE_BUNNY);
   tile_type_map[BAMBOO_GATE_HORIZ_CLOSED].SetMoveFlags(ENTERABLE_HUMAN);
   tile_type_map[BAMBOO_GATE_VERT_CLOSED].SetMoveFlags(ENTERABLE_HUMAN);
   tile_type_map[BAMBOO_FENCE_HORIZ].SetMoveFlags(IMPASSABLE);
   tile_type_map[SWITCH_OFF].SetMoveFlags(MOVE_ALL);
   tile_type_map[SWITCH_ON].SetMoveFlags(MOVE_ALL);
   tile_type_map[SIGN_POST].SetMoveFlags(MOVE_ALL);
   tile_type_map[BAMBOO_GATE_HORIZ_OPEN].SetMoveFlags(MOVE_ALL);
   tile_type_map[BAMBOO_GATE_VERT_OPEN].SetMoveFlags(MOVE_ALL);
   tile_type_map[BAMBOO_FENCE_VERT].SetMoveFlags(IMPASSABLE);
   tile_type_map[COMPUTER].SetMoveFlags(MOVE_ALL);
   tile_type_map[PCB_DIRTY].SetMoveFlags(MOVE_ALL);
   tile_type_map[PCB_CLEAN].SetMoveFlags(MOVE_ALL);
   tile_type_map[FLOOR_VENT].SetMoveFlags(MOVE_ALL);
}



bool CreateTiles(EagleGraphicsContext* win) {
   FreeTiles();
   tile_map_image = win->LoadImageFromFile("data/tilesets/tile_map.png");
   EAGLE_ASSERT(tile_map_image && tile_map_image->Valid());
   int ypos = 0;
   for (int y = 0 ; y < TILE_MAP_NUM_TILES_TALL ; ++y) {
      int xpos = 0;
      for (int x = 0 ; x < TILE_MAP_NUM_TILES_WIDE ; ++x) {
         int type = (y*TILE_MAP_NUM_TILES_WIDE + x);
         if (type >= NUM_TILE_TYPES) {
            break;
         }
         EagleImage* sub_image = win->CreateSubImage(tile_map_image , xpos , ypos , TILE_WIDTH , TILE_HEIGHT);
         TileBase* new_tile = new TileBase(sub_image , (TILE_TYPE)type);/// ALTER THIS TO USE NEW CLASSES
         tile_type_map[type] = new_tile;
         xpos += TILE_WIDTH;
      }
      ypos += TILE_HEIGHT;
   }
   SetTileFlags();
}



void FreeTiles(EagleGraphicsContext* win) {
   for (map<int , Tile*>::iterator it = tile_type_map.begin() ; it != tile_type_map.end() ; ++it) {
      delete it->second;
   }
   tile_type_map.clear();
   for (int i = 0 ; i < NUM_TILE_TYPES ; ++i) {
      win->FreeImage(tile_images[i]);
      tile_images[i] = 0;
   }
   win->FreeImage(tile_map_image);
   tile_map_image = 0;
}




/// -------------------------------      TileBase      ------------------------------




TileBase::TileBase(EagleImage* image , TILE_TYPE type) :
      tile_image(image),
      tile_type(type)
{
   EAGLE_ASSERT(tile_image && tile_image->Valid());
}



void TileBase::Draw(EagleGraphicsContext* win , int xpos , int ypos) {
   win->Draw(tile_image , xpos , ypos);
}



int TileBase::CX(int xpos) {return xpos + (ntw*TILE_WIDTH)/2;}



int TileBase::CY(int ypos) {return ypos + (ntt*TILE_HEIGHT)/2;}



void TileBase::SetMoveFlags(int flags) {
   move_flags = flags;
}



TILE_TYPE TileBase::GetTileType() {
   return type;
}



/// -------------------------    TileLayerBase     -------------------------------


int TileLayerBase::GetTileAt(int tx , int ty) {
   
}
