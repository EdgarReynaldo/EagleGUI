



#include "Hexagon.hpp"
#include "Player.hpp"


#include <string>
#include <cmath>
#ifndef M_PI
   #define M_PI 3.1415269
#endif


#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"


/// ---------------------------     Hexagon      -----------------------------




std::string HexDirectionToString(HEXDIRECTION d) {
   static const char* dirstrs[NUM_HEX_DIRECTIONS + 1] = {
      "HD_NORTH",
      "HD_NORTHEAST",
      "HD_SOUTHEAST",
      "HD_SOUTH",
      "HD_SOUTHWEST",
      "HD_NORTHWEST",
      "NUM_HEX_DIRECTIONS"
   };
   return dirstrs[d];
};



double HexDirectionToAngle(HEXDIRECTION d) {
   const double a = -M_PI/2.0 + (double)d*M_PI/3.0;
   return a;
}



HEXDIRECTION AngleToHexDirection(double arad) {
   
   /// North in Allegro is actually 270 degrees, so add a 1/4 turn clockwise to make North zero
   /// We want North to be from 240 degrees to 300 degrees (allegro degrees) so add 120 degrees
   arad += 2.0*M_PI/3.0;/// Adjust for the middle of the hex direction by turn cw 1/12

   /// Attempt to normalize the angle
   double ncircles = arad/(2.0*M_PI);
   if (ncircles < 0.0) {
      ncircles += ((int)-ncircles) + 1;
   }
   const double deg = fmod(ncircles , 1.0)*360.0;
   const int hex = int(deg/60.0) % 6;
   return HEXDIRECTION(hex);
}



void TestAngleToHexDirection() {
   for (unsigned int i = 0 ; i <= 24 ; ++i) {
      double a = i*M_PI/12.0;
      double deg = a*180.0/M_PI;
      HEXDIRECTION d = AngleToHexDirection(a);
      std::string dstr = HexDirectionToString(d);
      EagleInfo() << StringPrintF("Angle a (%3.1lf) = direction %s (%d)\n" , deg  , dstr.c_str() , d);
   }
}



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



void HexTile::CalcIncome() {
   income = 0;
   if (owner) {
      income += 4;
      income += 2*(int)neighbor_tty.size();
      income += border_tty.size();
   }
   else {
      if (maxinfluence.size()) {
         income = influence[*maxinfluence.begin()];
      }
   }
}



void HexTile::CollectIncome() {
   armies += income;
}



HexTile::HexTile() : 
      tx(-1),
      ty(-1),
      mx(-1.0),
      my(-1.0),
      owner(0),
      influence(),
      maxinfluence(),
      neighbor_tty(),
      border_tty(),
      neighbors(6 , NULL),
      income(0),
      armies(0)
{}



void HexTile::SetRadius(double r) {
   proto.SetRadius(fabs(r));
}



void HexTile::CalcBorders() {
   border_tty.clear();
   neighbor_tty.clear();
   maxinfluence.clear();
   int maxcount = 0;
   influence.clear();
//   std::map<int , int> infmap;/// <owner , count>
   for (unsigned int i = 0 ; i < NUM_HEX_DIRECTIONS ; ++i) {
      HexTile* n = neighbors[i];
      if (!n) {continue;}

      if (n->owner != 0) {
         if (n->owner == owner) {
            /// We connect to this territory
            neighbor_tty.insert((HEXDIRECTION)i);
         }
         influence[n->owner]++;

         if (influence[n->owner] > maxcount) {
            maxcount = influence[n->owner];
            maxinfluence.clear();
            maxinfluence.insert(n->owner);
         }
         else if (influence[n->owner] == maxcount) {
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



void HexTile::DrawStats(EagleGraphicsContext* win , double xpos , double ypos , ALLEGRO_COLOR color) {
//   std::string s1 = StringPrintF("%d" , income);
//   std::string s2 = StringPrintF("%d" , armies);
   std::string s3 = StringPrintF("%d+%d" , armies , income);
//   win->DrawTextString(win->DefaultFont() , s1 , xpos , ypos - 5.0 , GetEagleColor(color) , HALIGN_CENTER , VALIGN_BOTTOM);
//   win->DrawTextString(win->DefaultFont() , s2 , xpos , ypos + 5.0 , GetEagleColor(color) , HALIGN_CENTER , VALIGN_TOP);
   win->DrawTextString(win->DefaultFont() , s3 , mx + xpos , my + ypos , GetEagleColor(color) , HALIGN_CENTER , VALIGN_CENTER);
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



bool HexGrid::CheckGrid() {
   bool safe = true;
   unsigned int height = grid.size();
   unsigned int width = grid[0].size();
   for (unsigned int row = 0 ; row < height ; ++row) {
      for (unsigned int col = 0 ; col < width ; ++col) {
         HexTile* hex = &grid[row][col];
         for (unsigned int i = 0 ; i < NUM_HEX_DIRECTIONS ; ++i) {
            HexTile* nb = hex->neighbors[i];
            if (!Safe(nb)) {
               safe = false;
               std::string dstr = HexDirectionToString((HEXDIRECTION)i);
               EagleError() << StringPrintF("Unsafe tile at row %u , col %u , dir %s (%u) , %p\n" , row , col , dstr.c_str() , i , nb);
            }
         }
      }
   }
   return safe;
}



HexGrid::HexGrid() :
      w(0),
      h(0),
      rad(0),
      grid()
{}



void HexGrid::Resize(unsigned int width , unsigned int height , double radius) {
   
   w = width;
   h = height;
   rad = radius;
   HexTile::proto.SetRadius(rad);
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

   EAGLE_DEBUG(
      if (!CheckGrid()) {throw EagleException("Failed to pass grid check");}
   );
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
         win->DrawTextString(win->DefaultFont() , label , xpos + tile->mx , ypos + tile->my ,
                             EagleColor(255,255,255) , HALIGN_CENTER , VALIGN_CENTER);
         ++tit;
      }
      ++it;
   }
}



void HexGrid::DrawGrid2(EagleGraphicsContext* win , int xpos , int ypos , std::map<int , Player*> players) {

   /// Top left
   const double ty = ypos;
   const double lx = xpos;

   /// Draw team colors
   for (unsigned int row = 0 ; row < h ; ++row) {
      for (unsigned int col = 0 ; col < w ; ++col) {
         HexTile* tile = &grid[row][col];
         tile->DrawFilled(win , lx , ty , players[tile->owner]->our_color);
         if (tile->owner) {
            tile->DrawStats(win , lx , ty , al_map_rgb(255,255,255));
         }
      }
   }
   /// Draw team influence
   for (unsigned int row = 0 ; row < h ; ++row) {
      for (unsigned int col = 0 ; col < w ; ++col) {
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
         tile->DrawFilled(win , lx , ty , blended);
         tile->DrawStats(win , lx , ty  , al_map_rgb(255,255,255));
      }
   }

   /// Outline in white
   for (unsigned int row = 0 ; row < h ; ++row) {
      for (unsigned int col = 0 ; col < w ; ++col) {
         grid[row][col].DrawOutline(win , lx , ty , al_map_rgb(255,255,0));
      }
   }
}



bool HexGrid::Safe(HexTile* tile) {
   if (!tile) {return true;}
   for (unsigned int i = 0 ; i < grid.size() ; ++i) {
      for (unsigned int j = 0 ; j < grid[i].size() ; ++j) {
         if (tile == &grid[i][j]) {
            return true;
         }
      }
   }
   return false;
}

