



#include "Game.hpp"
#include "Player.hpp"



/// ------------------------     HexGame     -----------------------------



const double HexGame::root3 = sqrt(3.0);


void HexGame::Claim(HexTile* tile , int owner) {
   if (!tile) {return;}
   
   /// Remove before updating, so everything is current
   if (tile->owner) {
      players[tile->owner]->our_turf.RemoveTile(tile);
   }
   
   tile->owner = owner;
   std::vector<HexTile*>& nb = tile->neighbors;
   for (unsigned int i = 0 ; i < NUM_HEX_DIRECTIONS ; ++i) {
      HexTile* neighbor = nb[i];
      if (!neighbor) {continue;}
      neighbor->CalcBorders();
   }
   tile->CalcBorders();
   
   /// Now the tile is updated, add it to the appropriate territory
   if (tile->owner) {
      players[tile->owner]->our_turf.AddTile(tile);
   }
}



HexGame::HexGame() :
      hgrid(),
      rad(0.0),
      xpos(0.0),
      ypos(0.0),
      players(),
      turn(0),
      nplayers(3),
      mx(0),
      my(0),
      hover(0),
      hx(0),
      hy(0)
{
   ALLEGRO_COLOR teamcolors[4] = {
      al_map_rgb(0,0,0),
      al_map_rgb(255,0,0),
      al_map_rgb(0,255,0),
      al_map_rgb(0,0,255)
   };
   for (int i = 0 ; i < nplayers + 1 ; ++i) {
      Player* p = new Player(teamcolors[i]);
      p->pid = i;
      players[i] = p;
   }
}
   


HexGame::~HexGame() {
   for (std::map<int , Player*>::iterator it = players.begin() ; it != players.end() ; ++it) {
      delete it->second;
   }
   players.clear();
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
   hgrid.DrawGrid(win , xpos + x , ypos + y);
   for (int i = 0 ; i < nplayers + 1 ; ++i) {
//      hgrid.DrawPlayer(win , xpos + x , ypos + y , players[i]);
   }
   if (hover) {
      if (hover->owner == 0) {
         hover->DrawFilled(win , 0.0 , 0.0 , players[turn + 1]->our_color);
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


