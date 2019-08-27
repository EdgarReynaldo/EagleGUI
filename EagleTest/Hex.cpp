
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
   HC_SOUTHWEST     = 2,
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
   HD_NORTH = 5,
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



class HexTile {
   static Hexagon proto;

   int owner;
   std::unordered_set<int> maxinfluence;
   std::unordered_set<int> border;
   std::vector<HexTile*> neighbors;

   friend class HexGrid;
   friend class HexGame;
   
   void CalcBorders();
   
public :
   HexTile() : owner(0) , maxinfluence() , border() , neighbors(6 , NULL) {}
   
//   void SetRadius(double r);
   void SetRadius(double r) {
      proto.SetRadius(fabs(r));
   }
   
   void DrawFilled(EagleGraphicsContext* win , double x , double y , ALLEGRO_COLOR color);
   void DrawOutline(EagleGraphicsContext* win , double x , double y , ALLEGRO_COLOR color);
};



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
         border.insert(owner);
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



