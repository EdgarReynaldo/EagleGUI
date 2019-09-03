



#include "Hexagon.hpp"
#include "Player.hpp"


#include <string>
#include <cmath>
#ifndef M_PI
   #define M_PI 3.1415269
#endif


#include "Eagle.hpp"
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"


/// ---------------------------     Hexagon      -----------------------------




void Hexagon::GenPoints() {
   double theta = 0.0;
   verts.clear();
   verts.resize(NUM_HEX_CORNERS , ALLEGRO_VERTEX());
   for (unsigned int i = 0 ; i < NUM_HEX_CORNERS ; ++i) {
      ALLEGRO_VERTEX* v = &verts[i];
      v->x = radius*cos(theta);
      v->y = radius*sin(theta);
      v->z = 0.0;
      v->u = 0;
      v->v = 0;
      v->color = al_map_rgb(0,0,0);
      theta -= M_PI/3.0;
   }
}



Hexagon::Hexagon(double width) :
      radius(fabs(width/2.0)),
      verts()
{
   SetRadius(fabs(width/2.0));
}



void Hexagon::SetRadius(double r) {
   radius = r;
   GenPoints();
}



void Hexagon::DrawFilled(EagleGraphicsContext* win , double xpos , double ypos , ALLEGRO_COLOR color) { 
   (void)win;
   std::vector<float> vtx(2*verts.size() , 0.0);
   for (unsigned int i = 0 ; i < NUM_HEX_CORNERS ; ++i) {
      vtx[2*i + 0] = xpos + verts[i].x;
      vtx[2*i + 1] = ypos + verts[i].y;
   }
   al_draw_filled_polygon(&vtx[0] , 6 , color);
}



void Hexagon::DrawOutline(EagleGraphicsContext* win , double xpos , double ypos , ALLEGRO_COLOR color) { 
   (void)win;
   std::vector<float> vtx(2*verts.size() , 0.0);
   for (unsigned int i = 0 ; i < NUM_HEX_CORNERS ; ++i) {
      vtx[2*i + 0] = xpos + verts[i].x;
      vtx[2*i + 1] = ypos + verts[i].y;
   }
   al_draw_polygon(&vtx[0] , 6 , ALLEGRO_LINE_JOIN_MITER , color , 2.0 , 10.0);
}



/// -------------------------     HexTile     -----------------------



Hexagon HexTile::proto;



HexTile::HexTile() : 
      tx(-1),
      ty(-1),
      mx(-1.0),
      my(-1.0),
      owner(0),
      maxinfluence(),
      neighbor_tty(),
      border_tty(),
      neighbors(6 , NULL)
{}



void HexTile::SetRadius(double r) {
   proto.SetRadius(fabs(r));
}



void HexTile::CalcBorders() {
   border_tty.clear();
   neighbor_tty.clear();
   maxinfluence.clear();
   int maxcount = 0;
   std::map<int , int> infmap;/// <owner , count>
   for (unsigned int i = 0 ; i < NUM_HEX_DIRECTIONS ; ++i) {
      HexTile* n = neighbors[i];
      if (!n) {continue;}

      if (n->owner != 0) {
         if (n->owner == owner) {
            /// We connect to this territory
            neighbor_tty.insert((HEXDIRECTION)i);
         }
         infmap[n->owner]++;

         if (infmap[n->owner] > maxcount) {
            maxcount = infmap[n->owner];
            maxinfluence.clear();
            maxinfluence.insert(n->owner);
         }
         else if (infmap[n->owner] == maxcount) {
            maxinfluence.insert(n->owner);
         }
      }
      else {
         /// Owner is 0
         border_tty.insert((HEXDIRECTION)i);
      }
   }
}



void HexTile::DrawFilled(EagleGraphicsContext* win , double xpos , double ypos , ALLEGRO_COLOR color) {
   proto.DrawFilled(win , mx + xpos , my + ypos , color);
}



void HexTile::DrawOutline(EagleGraphicsContext* win , double xpos , double ypos , ALLEGRO_COLOR color) {
   proto.DrawOutline(win , mx + xpos , my + ypos , color);
}



std::unordered_set<HexTile*>& GetAdjoiningTiles(std::unordered_set<HexTile*>& tset , HexTile* prev);



std::unordered_set<HexTile*> GetAdjoiningTiles(HexTile* start) {
   std::unordered_set<HexTile*> tset;
   tset.insert(start);
   GetAdjoiningTiles(tset , start);
   return tset;
}



std::unordered_set<HexTile*>& GetAdjoiningTiles(std::unordered_set<HexTile*>& tset , HexTile* prev) {
   
   if (!prev) {return tset;}
   
   /// O(n) territory traversal by neighbor
   
   /// For every neighbor territory, add to list and get adjoining tiles if not on list
   const std::unordered_set<HEXDIRECTION>& nbset = prev->neighbor_tty;
   std::unordered_set<HEXDIRECTION>::const_iterator it = nbset.begin();
   while (it != nbset.end()) {
      HexTile* nb = prev->neighbors[*it];
      if (tset.find(nb) == tset.end()) {
         tset.insert(nb);
         tset = GetAdjoiningTiles(tset , nb);
      }
      ++it;
   }
   return tset;
}



//std::unordered_set<HexTile*> GetBordersN(std::unordered_set<HexTile*> tty);
//std::unordered_set<HexTile*> GetBorders1(std::unordered_set<HexTile*> tty);

std::unordered_set<HexTile*> GetBorders(std::unordered_set<HexTile*> tty) {


   /// We already have the border territories stored in our hex tile, just extract them
   /// O(N) on number of territory / border connections
   std::unordered_set<HexTile*> newborders;
   
   std::unordered_set<HexTile*>::iterator it = tty.begin();
   while (it != tty.end()) {
      HexTile* current = *it;
      std::unordered_set<HEXDIRECTION>::const_iterator bit = current->border_tty.begin();
      while (bit != current->border_tty.end()) {
         HexTile* newbordertile = current->neighbors[*bit];
         newborders.insert(newbordertile);
         ++bit;
      }
      ++it;
   }
   return newborders;

   /// OLD, inefficient
   
   /// Brute force network traversal , O(N)
   
   std::unordered_set<HexTile*> borders;
   if (tty.empty()) {
      return borders;
   }

   it = tty.begin();
   while (it != tty.end()) {

      /// BAD Algorithm - For (every neighbor of every tile) = O(6N);
      std::vector<HexTile*>& nb = (*it)->neighbors;
      for (unsigned int i = 0 ; i < NUM_HEX_DIRECTIONS ; ++i) {
         HexTile* tile = nb[i];
         if (!tile) {continue;}
         borders.insert(tile);
      }
      ++it;

   }
   /// Remove the territory from the borders O(N)
   it = tty.begin();
   while (it != tty.end()) {
      std::unordered_set<HexTile*>::iterator bit = borders.find(*it);
      if (bit != borders.end()) {
         borders.erase(bit);
      }
      ++it;
   }
   
   return borders;
}



/// ------------------------    HexGrid     -------------------------



HexGrid::HexGrid() :
      w(0),
      h(0),
      rad(0),
      grid()
{}



void HexGrid::Resize(unsigned int width , unsigned int height) {
   
   w = width;
   h = height;
   grid.clear();
   grid.resize(height , std::vector<HexTile>(width , HexTile()));

   static const double root3 = sqrt(3);
   
   /// Width and height of grid
   const double dx = 1.5*rad;
   const double dy = root3*rad;

   /// Top left
   const double ty = 0.0;
   const double lx = 0.0;

   for (unsigned int row = 0 ; row < height ; ++row) {
      double y = ty + dy*row;
      for (unsigned int col = 0 ; col < width ; ++col) {
         double ly = (col%2==0)?y:(y-dy/2.0);/// Offset odd columns up by dy
         double x = lx + dx*col;
         
         HexTile* hex = &grid[row][col];
         
         hex->mx = x;
         hex->my = ly;
         
         hex->tx = col;
         hex->ty = row;

         hex->neighbors.clear();
         hex->neighbors.resize(6 , NULL);
         
         /// In a hex grid, n sometimes changes the y value
         /// In a hex grid, s doesn't always change the y value
         const int N = (col%2==0)?row:row-1;
         const int S = (col%2==0)?row+1:row;
         
         /// SOUTH
         if (row < height - 1) {
            hex->neighbors[HD_SOUTH] = &grid[row + 1][col];
            /// SOUTHEAST
            if (col < width - 1) {
               hex->neighbors[HD_SOUTHEAST] = &grid[S][col + 1];
            }
            /// SOUTHWEST
            if (col > 0) {
               hex->neighbors[HD_SOUTHWEST] = &grid[S][col - 1];
            }
         }
         else if (row < height) {
            if (col % 2 == 1) {
               /// SOUTHEAST
               if (col < width - 1) {
                  hex->neighbors[HD_SOUTHEAST] = &grid[S][col + 1];
               }
               /// SOUTHWEST
               if (col > 0) {
                  hex->neighbors[HD_SOUTHWEST] = &grid[S][col - 1];
               }
            }
         }
         /// NORTH
         if (row > 0) {
            hex->neighbors[HD_NORTH] = &grid[row - 1][col];
            /// NORTHEAST
            if (col < width - 1) {
               hex->neighbors[HD_NORTHEAST] = &grid[N][col + 1];
            }
            /// NORTHWEST
            if (col > 0) {
               hex->neighbors[HD_NORTHWEST] = &grid[N][col - 1];
            }
         }
         else if (row == 0) {
            if (col % 2 == 0) {
               /// NORTHEAST
               if (col < width - 1) {
                  hex->neighbors[HD_NORTHEAST] = &grid[N][col + 1];
               }
               /// NORTHWEST
               if (col > 0) {
                  hex->neighbors[HD_NORTHWEST] = &grid[N][col - 1];
               }
            }
         }
      }
   }
}



void HexGrid::SetRadius(unsigned int radius) {
   rad = radius;
   if (grid.size()) {
      if (grid[0].size()) {
         grid[0][0].SetRadius(radius);
      }
   }
}


void HexGrid::DrawGrid(EagleGraphicsContext* win , int xpos , int ypos) {
   /// Outline in white
   for (unsigned int row = 0 ; row < h ; ++row) {
      for (unsigned int col = 0 ; col < w ; ++col) {
         grid[row][col].DrawOutline(win , xpos , ypos , al_map_rgb(255,255,255));
      }
   }
}



void HexGrid::DrawPlayer(EagleGraphicsContext* win , int xpos , int ypos , Player* player) {
   EAGLE_ASSERT(player);
   if (!player) {return;}
   
   const int pid = player->pid;
   
   TerritoryList* tlist = &player->our_turf;
   std::map<unsigned int , Territory*>& turf_map = tlist->tlist;
   
   std::map<unsigned int , Territory*>::iterator it = turf_map.begin();
   while (it != turf_map.end()) {
      unsigned int tid = it->first;
      Territory* tty = it->second;
   
      std::string label = StringPrintF("%d%c" , pid , 'a' + (char)tid);
      
      std::unordered_set<HexTile*>& ttylist = tty->tlist;
   
      std::unordered_set<HexTile*>::iterator tit = ttylist.begin();
      while (tit != ttylist.end()) {
         HexTile* tile = *tit;
         tile->DrawFilled(win , xpos , ypos , player->our_color);
         ++tit;
      }
      ++it;
   }
}


/**
void HexGrid::DrawGrid2(EagleGraphicsContext* win , int xpos , int ypos , std::map<int , Player*> players) {
   static const double root3 = sqrt(3);
   
   /// Width and height of grid
   const double dx = 1.5*rad;
   const double dy = root3*rad;

   /// Top left
   const double ty = ypos;
   const double lx = xpos;

   /// Draw team colors
   for (unsigned int row = 0 ; row < h ; ++row) {
      double y = ty + dy*row;
      for (unsigned int col = 0 ; col < w ; ++col) {
         double ly = (col%2==0)?y:(y-dy/2.0);/// Offset odd columns up by dy
         double x = lx + dx*col;
         HexTile* tile = &grid[row][col];
         tile->DrawFilled(win , x , ly , players[tile->owner]->our_color);
         if (tile->owner) {
            std::string s = StringPrintF("%d" , tile->owner);
            win->DrawTextString(win->DefaultFont() , s , x , ly , EagleColor(255,255,255) , HALIGN_CENTER , VALIGN_CENTER);
         }
      }
   }
   /// Draw team influence
   for (unsigned int row = 0 ; row < h ; ++row) {
      double y = ty + dy*row;
      for (unsigned int col = 0 ; col < w ; ++col) {
         double ly = (col%2==0)?y:(y-dy/2.0);/// Offset odd columns up by dy
         double x = lx + dx*col;
         HexTile* tile = &grid[row][col];
         if (tile->owner != 0) {continue;}
         const std::unordered_set<int>& infset = tile->maxinfluence;
         if (infset.empty()) {continue;}
         unsigned char r = 0;
         unsigned char g = 0;
         unsigned char b = 0;
         for (std::unordered_set<int>::const_iterator it = infset.begin() ; it != infset.end() ; ++it) {
            unsigned char r1 = 0;
            unsigned char g1 = 0;
            unsigned char b1 = 0;
            al_unmap_rgb(players[*it]->our_color , &r1 , &g1 , &b1);
            r += r1;
            g += g1;
            b += b1;
         }
//         int factor = 2*infset.size();
         
         ALLEGRO_COLOR blended = al_map_rgba(r/2 , g/2 , b/2 , 127);
         tile->DrawFilled(win , x , ly , blended);
      }
   }
   /// Outline in white
   for (unsigned int row = 0 ; row < h ; ++row) {
      double y = ty + dy*row;
      for (unsigned int col = 0 ; col < w ; ++col) {
         double ly = (col%2==0)?y:(y-dy/2.0);/// Offset odd columns up by dy
         double x = lx + dx*col;
         grid[row][col].DrawOutline(win , x , ly , al_map_rgb(255,255,255));
      }
   }
}

*/


