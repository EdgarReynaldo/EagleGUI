



#include "Tiles.hpp"


#include <sstream>
#include <string>
using namespace std;


const int TILE_WIDTH = 64;
const int TILE_HEIGHT = 64;


EagleImage* tile_map_image;
EagleImage* tile_images[NUM_TILE_TYPES];

map<int , TileBase*> tile_type_map;




/// ----------------------     Global functions      --------------------------------



void SetTileFlags() {
   tile_type_map[SNOWY]->SetMoveFlags(MOVE_ALL);
   tile_type_map[GRASS]->SetMoveFlags(MOVE_ALL);
   tile_type_map[DIRT]->SetMoveFlags(MOVE_ALL);
   tile_type_map[STONE]->SetMoveFlags(MOVE_ALL);
   tile_type_map[FLOOR]->SetMoveFlags(MOVE_ALL);
   tile_type_map[BRICK]->SetMoveFlags(IMPASSABLE);
   tile_type_map[FROSTY_ROUND_TREE]->SetMoveFlags(MOVE_ALL);
   tile_type_map[GREEN_ROUND_TREE]->SetMoveFlags(MOVE_ALL);
   tile_type_map[BROWN_ROUND_TREE]->SetMoveFlags(MOVE_ALL);
   tile_type_map[GREEN_FIR_TREE]->SetMoveFlags(MOVE_ALL);
   tile_type_map[SNOWY_FIR_TREE]->SetMoveFlags(MOVE_ALL);
   tile_type_map[CARROTS]->SetMoveFlags(MOVE_ALL);
   tile_type_map[FENCE_CROSS]->SetMoveFlags(MOVE_BUNNY);
   tile_type_map[FENCE_VERT]->SetMoveFlags(MOVE_BUNNY);
   tile_type_map[FENCE_HORIZ]->SetMoveFlags(MOVE_BUNNY);
   tile_type_map[BAMBOO_GATE_HORIZ_CLOSED]->SetMoveFlags(ENTERABLE_HUMAN);
   tile_type_map[BAMBOO_GATE_VERT_CLOSED]->SetMoveFlags(ENTERABLE_HUMAN);
   tile_type_map[BAMBOO_FENCE_HORIZ]->SetMoveFlags(IMPASSABLE);
   tile_type_map[SWITCH_OFF]->SetMoveFlags(MOVE_ALL);
   tile_type_map[SWITCH_ON]->SetMoveFlags(MOVE_ALL);
   tile_type_map[SIGN_POST]->SetMoveFlags(MOVE_ALL);
   tile_type_map[BAMBOO_GATE_HORIZ_OPEN]->SetMoveFlags(MOVE_ALL);
   tile_type_map[BAMBOO_GATE_VERT_OPEN]->SetMoveFlags(MOVE_ALL);
   tile_type_map[BAMBOO_FENCE_VERT]->SetMoveFlags(IMPASSABLE);
   tile_type_map[COMPUTER]->SetMoveFlags(MOVE_ALL);
   tile_type_map[PCB_DIRTY]->SetMoveFlags(MOVE_ALL);
   tile_type_map[PCB_CLEAN]->SetMoveFlags(MOVE_ALL);
   tile_type_map[FLOOR_VENT]->SetMoveFlags(MOVE_ALL);
}



bool CreateTiles(EagleGraphicsContext* win) {
   FreeTiles(win);
   tile_map_image = win->LoadImageFromFile("data/tilesets/tile_map.png");
   if (!tile_map_image) {return false;}
   if (!tile_map_image->Valid()) {return false;}


   tile_type_map[TILE_EMPTY] = new TileBase(0 , TILE_EMPTY);

   bool success = true;
   int ypos = 0;
   for (int y = 0 ; y < TILE_MAP_NUM_TILES_TALL ; ++y) {
      int xpos = 0;
      for (int x = 0 ; x < TILE_MAP_NUM_TILES_WIDE ; ++x) {
         int type = (y*TILE_MAP_NUM_TILES_WIDE + x);
         if (type >= NUM_TILE_TYPES) {
            break;
         }
         EagleImage* sub_image = win->CreateSubImage(tile_map_image , xpos , ypos , TILE_WIDTH , TILE_HEIGHT);
         if (!sub_image || (sub_image && !sub_image->Valid())) {
            success = false;
         }
         TileBase* new_tile = new TileBase(sub_image , (TILE_TYPE)type);/// ALTER THIS TO USE NEW CLASSES
         tile_type_map[type] = new_tile;
         xpos += TILE_WIDTH;
      }
      ypos += TILE_HEIGHT;
   }
   SetTileFlags();
   return success;
}



void FreeTiles(EagleGraphicsContext* win) {
   for (map<int , TileBase*>::iterator it = tile_type_map.begin() ; it != tile_type_map.end() ; ++it) {
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
   
}



void TileBase::Draw(EagleGraphicsContext* win , int xpos , int ypos) {
   if (!tile_image) {
      EAGLE_ASSERT(this->tile_type == TILE_EMPTY);
      win->DrawFilledRectangle(xpos , ypos , TILE_WIDTH , TILE_HEIGHT , EagleColor(0,0,0));
   }
   else {
      win->Draw(tile_image , xpos , ypos);
   }
}



int TileBase::CX(int xpos) {return xpos + TILE_WIDTH/2;}



int TileBase::CY(int ypos) {return ypos + TILE_HEIGHT/2;}



void TileBase::SetMoveFlags(int flags) {
   move_flags = flags;
}



TILE_TYPE TileBase::GetTileType() {
   return tile_type;
}



/// ------------------------      StateToggler      --------------------------



void StateToggler::ToggleState() {
   state_on = !state_on;
   if (state_on) {
      Activate();
   }
   else {
      Deactivate();
   }
}



/// ------------------------     BambooGate      --------------------------




BambooGateTile::BambooGateTile(bool open , bool horizontal , TileBase* open_tile , TileBase* closed_tile) :
      TileBase(0 , TILE_EMPTY),
      gate_open(open),
      gate_horizontal(horizontal),
      gate_open_tile(*open_tile),
      gate_closed_tile(*closed_tile)
{
   
}



void BambooGateTile::Draw(EagleGraphicsContext* win , int xpos , int ypos) {
   if (gate_open) {
      gate_open_tile.Draw(win , xpos , ypos);
   }
   else {
      gate_closed_tile.Draw(win , xpos , ypos);
   }
}



TILE_TYPE BambooGateTile::GetTileType() {
   if (gate_horizontal) {
      if (gate_open) {
         return BAMBOO_GATE_HORIZ_OPEN;
      }
      else {
         return BAMBOO_GATE_HORIZ_CLOSED;
      }
   }
   else {
      if (gate_open) {
         return BAMBOO_GATE_VERT_OPEN;
      }
      else {
         return BAMBOO_GATE_VERT_CLOSED;
      }
   }
   return TILE_EMPTY;
}




/// -------------------------     Switch Tile    --------------------------------




SwitchTile::SwitchTile(bool on , TileBase* on_tile , TileBase* off_tile) :
   TileBase(0 , TILE_EMPTY),
   switch_on(on),
   switch_on_tile(*on_tile),
   switch_off_tile(*off_tile)
{}




void SwitchTile::Toggle() {
   switch_on = !switch_on;
   if (switch_on) {
      TurnOn();
   }
   else {
      TurnOff();
   }
}



void SwitchTile::Draw(EagleGraphicsContext* win , int xpos , int ypos) {
   if (switch_on) {
      switch_on_tile.Draw(win,xpos,ypos);
   }
   else {
      switch_off_tile.Draw(win,xpos,ypos);
   }
}



TILE_TYPE SwitchTile::GetTileType() {
   if (switch_on) {
      return SWITCH_ON;
   }
   else {
      return SWITCH_OFF;
   }
   return TILE_EMPTY;
}


/// -------------------------    TileLayerBase     -------------------------------





TileBaseLayer::TileBaseLayer() :
      tile_array(),
      ncols(0),
      nrows(0),
      mapx(0),
      mapy(0)
{}



void TileBaseLayer::Resize(int cols , int rows) {
   tile_array.resize(rows , vector<TileBase*>(cols , 0));
   ncols = cols;
   nrows = rows;
}



int TileBaseLayer::Read(const char* map_string) {
   stringstream ss;
   string s = map_string;
   s = Replace(s , "," , " ");
   ss.str(s);
   return Read(ss);
}



int TileBaseLayer::Read(istream& in) {
   int cols = 0;
   int rows = 0;
   if (!in.good()) {
      return -1;
   }
   in >> cols >> rows;
   if (!in.good()) {
      return -1;
   }

   Resize(cols,rows);
   for (int y = 0 ; y < rows ; ++y) {
      for (int x = 0 ; x < cols ; ++x) {
         int tile_num = -1;
         in >> tile_num;/// -1 marks empty tile
         if (!in.good()) {
            if (in.eof()) {
               EAGLE_ASSERT((y == (rows - 1) && (x == (cols - 1))));
            }
            else {
               return -1;
            }
         }
         EAGLE_ASSERT(tile_num >= TILE_EMPTY && tile_num <= BRICK);/// Terrain tiles only
         TileBase* tile = tile_type_map[tile_num];
         EAGLE_ASSERT(tile);
         tile_array[y][x] = tile;
      }
   }
   return cols*rows;
}



void TileBaseLayer::Draw(EagleGraphicsContext* win , int viewx , int viewy , int width , int height) {
   
   EagleImage* dest = win->GetDrawingTarget();
   
   dest->PushClippingRectangle(Rectangle(viewx , viewy , width , height));
   
   int ystart = (viewy - mapy)/TILE_HEIGHT - 1;
   int ystop = (viewy + height - mapy)/TILE_HEIGHT;
   
   int xstart = (viewx - mapx)/TILE_WIDTH - 1;
   int xstop = (viewx + width - mapx)/TILE_WIDTH;

   if (ystart < 0) {ystart = 0;}
   if (xstart < 0) {xstart = 0;}
   
   if (ystop > nrows) {ystop = nrows;}
   if (xstop > ncols) {xstop = ncols;}


   for (int y = ystart ; y < nrows && y < ystop ; ++y) {
      for (int x = xstart ; x < ncols && x < xstop ; ++x) {
///         if (((x + 1)*TILE_WIDTH + mapx) < viewx) {continue;}
///         if ((x*TILE_WIDTH) + mapx > (viewx + width)) {break;}
         TileBase* tile = tile_array[y][x];
         tile->Draw(win , x*TILE_WIDTH + mapx , y*TILE_HEIGHT + mapy);
      }
   }
   
   
   dest->PopClippingRectangle();
}



int TileBaseLayer::GetTileAt(int tx , int ty) {
   TileBase* t = tile_array[ty][tx];
   return t->GetTileType();
}




