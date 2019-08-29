
#include <vector>
#include <unordered_set>
#include <cmath>
#ifndef M_PI
   #define M_PI 3.1415269
#endif

#include <map>

#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"
#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"

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
   HD_NORTHEAST = 0,
   HD_SOUTHEAST = 1,
   HD_SOUTH     = 2,
   HD_SOUTHWEST = 3,
   HD_NORTHWEST = 4,
   HD_NORTH     = 5,
   NUM_HEX_DIRECTIONS = 6
};

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


typedef std::pair<int , unsigned int> OWNER_TTY_PAIR;
class HexTile {
   static Hexagon proto;

   int owner;
   std::unordered_set<int> maxinfluence;/// 1 or more players influence this tile
   std::unordered_set<OWNER_TTY_PAIR> border_ttys;/// Border territories
   std::vector<HexTile*> neighbors;
   
   unsigned int tid;/// Territory id
   

   friend class HexGrid;
   friend class HexGame;
   friend class Territory;
   
   void CalcBorders();
   
public :
   HexTile() : owner(0) , maxinfluence() , border_ttys() , neighbors(6 , NULL) , tid((unsigned int)-1) {}
   
//   void SetRadius(double r);
   void SetRadius(double r) {
      proto.SetRadius(fabs(r));
   }
   
   void DrawFilled(EagleGraphicsContext* win , double x , double y , ALLEGRO_COLOR color);
   void DrawOutline(EagleGraphicsContext* win , double x , double y , ALLEGRO_COLOR color);
};



std::unordered_set<HexTile*> GetAdjoiningTiles(HexTile* start);/// O(N)

std::unordered_set<HexTile*> GetBorders(std::unordered_set<HexTile*> tty);/// O(N)



class HexGrid {
   unsigned int w,h,rad;
   
   std::vector<std::vector<HexTile> > grid;
   
   friend class HexGame;
   
   
public :
   HexGrid();
   
   void Resize(unsigned int width , unsigned int height);

   void SetRadius(unsigned int radius);

   void DrawGrid(EagleGraphicsContext* win , int xpos , int ypos , std::map<int , ALLEGRO_COLOR> colors);

   unsigned int Width() {return w;}
   unsigned int Height() {return h;}
};



class Territory {
   int owner;
   unsigned int tid;/// Territory id ('a' + tid)
   
   std::unordered_set<HexTile*> tlist;/// Territory list
   std::unordered_set<HexTile*> blist;/// Border list
   
   
   
public :

//   Territory(HexTile* tile);
   Territory(HexTile* tile) :
         owner(0),
         tid(BADTID),
         tlist(),
         blist()
   {}

   std::map<HexTile* , unsigned int> AddTile(HexTile* tile);
   std::map<HexTile* , unsigned int> RemoveTile(HexTile* tile);
   
   bool Contains(HexTile* tile);
   bool Borders(HexTile* tile);
   
   void Absorb(Territory* ot);
   void Absorb(Territory* ot) {
      if (!ot) {return;}
      tlist.insert(ot->tlist.begin() , ot->tlist.end());
      ot->tlist.clear();
   }
   void SetTerritory(std::unordered_set<HexTile*> tset);
   void SetTerritory(std::unordered_set<HexTile*> tset) {
      tlist = tset;
      CalcBorderSet();
   }
};


class TerritoryList {
   int owner;
   std::map<unsigned int , Territory> tlist;/// <territory id , Territory>



   unsigned int GetNewTerritoryId();///< Return first free tid

   friend class Player;
   friend class HexGame;
   
public :
      
   static const unsigned int BADTID;

   TerritoryList();
   TerritoryList() :
         owner(-1),
         tlist()
   {}
      
   unsigned int AddTile(HexTile* tile);/// Returns territory id
   void RemoveTile(HexTile* tile);

   bool Contains(HexTile* tile);
   bool Borders(HexTile* tile);

   
};

const unsigned int TerritoryList::BADTID = (unsigned int)-1;

class TerritoryMap {

   std::map<int , TerritoryList> tmap;/// < owner , tlist>

public :
   

};



class Player {
   
   TerritoryList* our_turf;
public :
      
};



class HexGame {
   static const double root3;
   
   
   HexGrid hgrid;
   double rad;
   double xpos,ypos;
   
   std::map<int , ALLEGRO_COLOR> teamcolors;

   int turn;
   int nplayers;
   
   int mx,my;
   
   HexTile* hover;
   int hx,hy;
   
   std::map<int , Player> players;
   
   
   
   
   
   
   void Claim(HexTile* tile , int owner);
   
   
public :
   
   HexGame();

   void Resize(unsigned int width , unsigned int height , double radius);
   void Position(double x , double y);

   void HandleEvent(EagleEvent ee);

   void DisplayOn(EagleGraphicsContext* win , int xpos , int ypos);

   void Run(EagleSystem* sys , EagleGraphicsContext* win);
   
   int NumPlayers() {return nplayers;}
};



int main(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   
   Allegro5System* a5sys = GetAllegro5System();
   
   int ret = 0;
   if ((ret = a5sys->Initialize(EAGLE_FULL_SETUP)) != EAGLE_FULL_SETUP) {
      if (!(ret & (EAGLE_MOUSE | EAGLE_KEYBOARD))) {
         EagleCritical() << "Failed to install keyboard and mouse." << std::endl;
      }
   }
   
   int sw = 1024;
   int sh = 768;
   EagleGraphicsContext* win = a5sys->CreateGraphicsContext("HexGame window" , sw , sh , EAGLE_OPENGL);
   
   EAGLE_ASSERT(win && win->Valid());
   
   HexGame game;
   
   game.Resize(12,9,25);
   
   game.Position(sw/4 , sh/4);
   
   a5sys->GetSystemTimer()->Start();

   game.Run(a5sys , win);
   
   win->FlipDisplay();
   
///   a5sys->GetSystemQueue()->WaitForEvent(EAGLE_EVENT_KEY_DOWN , 0);
   
   
   return 0;
}



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



void Hexagon::DrawFilled(EagleGraphicsContext* win , double x , double y , ALLEGRO_COLOR color) { 
   (void)win;
   std::vector<float> vtx(2*verts.size() , 0.0);
   for (unsigned int i = 0 ; i < NUM_HEX_CORNERS ; ++i) {
      vtx[2*i + 0] = x + verts[i].x;
      vtx[2*i + 1] = y + verts[i].y;
   }
   al_draw_filled_polygon(&vtx[0] , 6 , color);
}



void Hexagon::DrawOutline(EagleGraphicsContext* win , double x , double y , ALLEGRO_COLOR color) { 
   (void)win;
   std::vector<float> vtx(2*verts.size() , 0.0);
   for (unsigned int i = 0 ; i < NUM_HEX_CORNERS ; ++i) {
      vtx[2*i + 0] = x + verts[i].x;
      vtx[2*i + 1] = y + verts[i].y;
   }
   al_draw_polygon(&vtx[0] , 6 , ALLEGRO_LINE_JOIN_MITER , color , 2.0 , 10.0);
}



/// -------------------------     HexTile     -----------------------



Hexagon HexTile::proto;


void HexTile::CalcBorders() {
   border.clear();
   maxinfluence.clear();
   int maxcount = 0;
   std::map<int , int> infmap;/// <owner , count>
   for (unsigned int i = 0 ; i < NUM_HEX_DIRECTIONS ; ++i) {
      HexTile* n = neighbors[i];
      if (!n) {continue;}

      if (n->owner != 0) {
         border_ttys.insert(OWNER_TTY_PAIR(owner,n->tid));
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
   }
}



void HexTile::DrawFilled(EagleGraphicsContext* win , double x , double y , ALLEGRO_COLOR color) {
   proto.DrawFilled(win , x , y , color);
}



void HexTile::DrawOutline(EagleGraphicsContext* win , double x , double y , ALLEGRO_COLOR color) {
   proto.DrawOutline(win , x , y , color);
}



std::unordered_set<HexTile*>& GetAdjoiningTiles(std::unordered_set<HexTile*>& tset , HexTile* prev);



std::unordered_set<HexTile*> GetAdjoiningTiles(HexTile* start) {
   std::unordered_set<HexTile*> tset;
   tset.insert(start);
   GetAdjoiningTiles(tset , start);
   return tset;
}



std::unordered_set<HexTile*>& GetAdjoiningTiles(std::unordered_set<HexTile*>& tset , HexTile* prev) {
   unsigned int owner = prev->owner;
   std::vector<HexTile*>& nb = prev->neighbors;
   
   /// For (every neighbor of every tile in the territory) = O(6N)
   for (unsigned int i = 0 ; i < NUM_HEX_DIRECTIONS ; ++i) {
      HexTile* nt = nb[i];
      if (!nt) {continue;}
      if (nt->owner == owner) {
         if (tset.find(nt) == tset.end()) {
            tset.insert(nt);
            GetAdjoiningTiles(tset , nt);
         }
      }
   }
   return tset;
}



std::unordered_set<HexTile*> GetBordersN(std::unordered_set<HexTile*> tty);
std::unordered_set<HexTile*> GetBorders1(std::unordered_set<HexTile*> tty);

std::unordered_set<HexTile*> GetBorders(std::unordered_set<HexTile*> tty);
std::unordered_set<HexTile*> GetBorders(std::unordered_set<HexTile*> tty) {
   
}

std::unordered_set<HexTile*> GetBordersN(std::unordered_set<HexTile*> tty) {
   /// Brute force network traversal , O(N)
   
   std::unordered_set<HexTile*> borders;
   if (tty.empty()) {
      return borders;
   }

   std::unordered_set<HexTile*>::iterator it = tty.begin();
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

std::unordered_set<HexTile*> GetBorders1(std::unordered_set<HexTile*> tty) {
   
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
   for (unsigned int y = 0 ; y < height ; ++y) {
      for (unsigned int x = 0 ; x < width ; ++x) {
         grid[y][x].neighbors.clear();
         grid[y][x].neighbors.resize(6 , NULL);
         
         /// In a hex grid, n sometimes changes the y value
         /// In a hex grid, s doesn't always change the y value
         const int N = (x%2==0)?y:y-1;
         const int S = (x%2==0)?y+1:y;
         
         /// SOUTH
         if (y < height - 1) {
            grid[y][x].neighbors[HD_SOUTH] = &grid[y + 1][x];
            /// SOUTHEAST
            if (x < width - 1) {
               grid[y][x].neighbors[HD_SOUTHEAST] = &grid[S][x + 1];
            }
            /// SOUTHWEST
            if (x > 0) {
               grid[y][x].neighbors[HD_SOUTHWEST] = &grid[S][x - 1];
            }
         }
         else if (y < height) {
            if (x % 2 == 1) {
               /// SOUTHEAST
               if (x < width - 1) {
                  grid[y][x].neighbors[HD_SOUTHEAST] = &grid[S][x + 1];
               }
               /// SOUTHWEST
               if (x > 0) {
                  grid[y][x].neighbors[HD_SOUTHWEST] = &grid[S][x - 1];
               }
            }
         }
         /// NORTH
         if (y > 0) {
            grid[y][x].neighbors[HD_NORTH] = &grid[y - 1][x];
            /// NORTHEAST
            if (x < width - 1) {
               grid[y][x].neighbors[HD_NORTHEAST] = &grid[N][x + 1];
            }
            /// NORTHWEST
            if (x > 0) {
               grid[y][x].neighbors[HD_NORTHWEST] = &grid[N][x - 1];
            }
         }
         else if (y == 0) {
            if (x % 2 == 0) {
               /// NORTHEAST
               if (x < width - 1) {
                  grid[y][x].neighbors[HD_NORTHEAST] = &grid[N][x + 1];
               }
               /// NORTHWEST
               if (x > 0) {
                  grid[y][x].neighbors[HD_NORTHWEST] = &grid[N][x - 1];
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



void HexGrid::DrawGrid(EagleGraphicsContext* win , int xpos , int ypos , std::map<int , ALLEGRO_COLOR> colors) {
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
         tile->DrawFilled(win , x , ly , colors[tile->owner]);
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
            al_unmap_rgb(colors[*it] , &r1 , &g1 , &b1);
            r += r1;
            g += g1;
            b += b1;
         }
         int factor = 2*infset.size();
         
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



/// ------------------------     Territory     -----------------------------



std::map<HexTile* , unsigned int> Territory::AddTile(HexTile* tile) {
   std::map<HexTile* , unsigned int> tilemap;
   /// BAD tile
   if (!tile) {
      return tilemap;/// Empty map, no adjoining territories owned by us
   }
   
   /// An owned tile can't be part of a border
   std::unordered_set<HexTile*>::iterator it = blist.find(tile);
   if (it != blist.end()) {
      blist.erase(tile);
   }

   /// Add the tile to our territory list
   tlist.insert(tile);

   /// Add the neighbors to our border if they are unowned
   std::vector<HexTile*>& nb = tile->neighbors;
   for (unsigned int i = 0 ; i < NUM_HEX_DIRECTIONS ; ++i) {
      HexTile* t = nb[i];
      if (!t) {continue;}
      if (t->owner == 0) {
         blist.insert(t);
      }
   }
}



std::map<HexTile* , unsigned int> Territory::RemoveTile(HexTile* tile) {
   std::map<HexTile* , unsigned int> tilemap;/// Nothing removed, no connections
   /// BAD tile
   if (!tile) {
      return tilemap;
   }
   
   /// Remove the tile from the territory
   std::unordered_set<HexTile*>::iterator it1 = tlist.find(tile);
   if (it1 != tlist.end()) {
      tlist.erase(it1);
   }
   
   /// If we are bordered by any tiles owned by us, we are now part of the border
   std::vector<HexTile*>& nb = tile->neighbors;
   for (unsigned int i = 0 ; i < NUM_HEX_DIRECTIONS ; ++i) {
      HexTile* t = nb[i];
      if (!t) {continue;}
      if (t->owner == tile->owner) {
         tilemap[t] = t->tid;
         blist.insert(tile);
         break;
      }
   }
   return tilemap;
}



bool Territory::Contains(HexTile* tile) {
   return tlist.find(tile) != tlist.end();
}



bool Territory::Borders(HexTile* tile) {
   return blist.find(tile) != blist.end();
}



/// ------------------------     TerritoryList     -----------------------------



unsigned int TerritoryList::GetNewTerritoryId() {
   unsigned int id = 0;
   if (tlist.find(id) != tlist.end()) {
      /// Already on list, skip id
      ++id;
   }
   return id;
}



unsigned int TerritoryList::AddTile(HexTile* tile) {
   if (!tile) {return BADTID;}
   
   unsigned int towner = tile->owner;
   
   if (Contains(tile) != BADTID) {
      /// Already on our territory list
      return BADTID;
   }
   std::unordered_set<unsigned int> borderset = Borders(tile);
   unsigned int newid = BADTID;
   Territory newtty;
   newtty.AddTile(tile);
   if (borderset.empty()) {
      /// Isolated territory, spawn new
      newid = GetNewTerritoryId();
   }
   else {
      /// This is on the border of at least one of our territories
      newid = *borderset.begin();
   }
   for (std::unordered_set<unsigned int>::iterator it = borderset.begin() ; it != borderset.end() ; ++it) {
      Territory* territory = &tlist[*it];
      newtty.Absorb(territory);
      tlist.erase(tlist.find(*it));
   }
   newtty.SetTerritoryId(newid);
   tlist[newid] = newtty;
   return newid;
}
   


void TerritoryList::RemoveTile(HexTile* tile) {
   if (!tile) {return;}
   
   unsigned int TTYID = Contains(tile);
   if (TTYID == BADTID) {
      /// None of our territories contain this tile
      EAGLE_ASSERT(TTYID != BADTID);
      return;
   }
   
   std::unordered_set<HexTile*> nbset;
   std::vector<HexTile*>& nb = tile->neighbors;
   for (unsigned int i = 0 ; i < NUM_HEX_DIRECTIONS ; ++i) {
      HexTile* btile = nb[i];
      if (!btile) {
         continue;
      }
      if (tile->owner != btile->owner) {
         nb[i] = 0;
      }
      else {
         nbset.insert(btile);
      }
   }
   /// nbmap now holds a list of neighboring territories owned by us
   /// Each of those territories may be linked to a unique territory,
   /// And removing this tile may create up to 3 unique regions
   

   /// Find connected territories
   std::vector<std::unordered_set<HexTile*> > tset;
   for (unsigned int i = 0 ; i < NUM_HEX_DIRECTIONS ; ++i) {
      if (nb[i]) {
         tset.push_back(GetAdjoiningTiles(nb[i]));/// Expensive traversal of grid network
      }
   }
   
   std::unordered_set< std::unordered_set<HexTile*> > territoryset;
   
   /// We now have several sets of tiles, if any are equivalent, you know they belong to the same territory
   for (unsigned int i = 0 i < tset.size() ; ) {
      /// Filter unique territories
      if (territoryset.find(tset[i]) == territoryset.end()) {
         territoryset.insert(tset[i]);
      }
   }
   
   /// Erase the old territory
   tlist.erase(tlist.find(tile->tid));
   
   /// Create a new territory for each unique set
   std::unordered_set< std::unordered_set<HexTile*> >::iterator it = territoryset.begin();
   while (it != territoryset.end()) {
      Territory tty;
      tty.tid = GetNewTerritoryId();
      tty.owner = tile->owner;
      tty.SetTerritory(*it);
      tlist[tty.tid] = tty;
      ++it;
   }
}



unsigned int TerritoryList::Contains(HexTile* tile) {
   std::map<unsigned int , Territory>::iterator it= tlist.begin();
   while (it != tlist.end()) {
      if (it->second.Contains(tile)) {
         return it->first;
      }
      ++it;
   }
   return BADTID;
}



std::unordered_set<unsigned int> TerritoryList::Borders(HexTile* tile) {
   std::unordered_set<unsigned int> bset;
   std::map<unsigned int , Territory>::iterator it= tlist.begin();
   while (it != tlist.end()) {
      if (it->second.Borders(tile)) {
         bset->insert(it->first);
      }
      ++it;
   }
   return bset;
}



/// ------------------------     HexGame     -----------------------------



const double HexGame::root3 = sqrt(3.0);


void HexGame::Claim(HexTile* tile , int owner) {
   if (!tile) {return;}
   tile->owner = owner;
   std::vector<HexTile*>& nb = tile->neighbors;
   for (unsigned int i = 0 ; i < NUM_HEX_DIRECTIONS ; ++i) {
      HexTile* neighbor = nb[i];
      if (!neighbor) {continue;}
      neighbor->CalcBorders();
   }
}



HexGame::HexGame() :
      hgrid(),
      rad(0.0),
      teamcolors(),
      turn(0),
      nplayers(3),
      mx(0),
      my(0),
      hover(0),
      hx(0.0),
      hy(0.0)
{
   teamcolors[0] = al_map_rgb(0,0,0);
   teamcolors[1] = al_map_rgb(255,0,0);
   teamcolors[2] = al_map_rgb(0,255,0);
   teamcolors[3] = al_map_rgb(0,0,255);
}
   


void HexGame::Resize(unsigned int width , unsigned int height , double radius) {
   rad = radius;
   if (radius < 10.0) {radius = 10.0;}
   hgrid.Resize(width,height);
   hgrid.SetRadius(radius);
}
   
   

void HexGame::Position(double x , double y) {
   xpos = x;
   ypos = y;
}



void HexGame::HandleEvent(EagleEvent ee) {
   if (ee.type == EAGLE_EVENT_MOUSE_AXES) {
      mx = ee.mouse.x;
      my = ee.mouse.y;
      HexTile* oldhover = hover;
      hover = 0;
      const double lx = xpos - (3.0/4.0)*rad;
      int col = (int)(((double)mx + 0.5 - lx)/((3.0/2.0)*rad));
      double ly = ypos - rad*0.5*root3;
      if (col % 2 == 1) {
         ly -= rad*0.5*root3;
      }
      int row = (int)((((double)my + 0.5) - ly)/(rad*root3));
      if (col >= 0 && col < (int)hgrid.Width()) {
         if (row >= 0 && row < (int)hgrid.Height()) {
            hover = &(hgrid.grid[row][col]);
            hx = col;
            hy = row;
         }
      }
   }
   if (ee.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN) {
      if (hover) {
         if (ee.mouse.button == 1) {
            if (hover->owner == 0) {
               Claim(hover , turn + 1);
               turn = (turn + 1)%NumPlayers();
            }
         }
         else if (ee.mouse.button == 2) {
            Claim(hover , 0);
         }
      }
   }
}



void HexGame::DisplayOn(EagleGraphicsContext* win , int x , int y) {
   hgrid.DrawGrid(win , xpos + x , ypos + y , teamcolors);
   if (hover) {
      double x2 = x + xpos + 1.5*rad*hx;
      double y2 = y + ypos + hy*rad*root3 + ((hx % 2 == 0)?0.0:-rad*0.5*root3);
      if (hover->owner == 0) {
         hover->DrawFilled(win,x2,y2,teamcolors[turn + 1]);
      }
   }
}



void HexGame::Run(EagleSystem* sys , EagleGraphicsContext* win) {
   EagleEventHandler* q = sys->GetSystemQueue();
   
   bool quit = false;
   bool redraw= true;
   do {
      if (redraw) {
         win->DrawToBackBuffer();
         win->Clear();
         DisplayOn(win , 0 , 0);
         
         if (hover) {
            std::string hstr = StringPrintF("%d,%d" , hx , hy);
            win->DrawTextString(win->DefaultFont() , hstr , 10 , 10 , EagleColor(255,255,255));
         }
         
         
         win->FlipDisplay();
         redraw = false;
      }
      do {
         EagleEvent ev = sys->WaitForSystemEventAndUpdateState();
      
         if (ev.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         if (ev.type == EAGLE_EVENT_KEY_DOWN && ev.keyboard.keycode == EAGLE_KEY_ESCAPE) {
            quit = true;
         }
         
         HandleEvent(ev);
         
         if (ev.type == EAGLE_EVENT_TIMER) {
            redraw = true;
         }
         
      } while (q->HasEvent(0));
   } while (!quit);
}



