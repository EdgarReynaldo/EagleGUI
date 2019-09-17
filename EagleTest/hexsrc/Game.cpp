



#include "Game.hpp"
#include "Player.hpp"



/// ------------------------     HexGame     -----------------------------



const double HexGame::root3 = sqrt(3.0);


void HexGame::Claim(HexTile* tile , int owner) {
   if (!tile) {return;}
   
   
   /// Remove after updating, so everything is current
   if (tile->owner) {
      TerritoryList& turf = players[tile->owner]->our_turf;
      turf.RemoveTile(tile);
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
      TerritoryList& turf = players[tile->owner]->our_turf;
      turf.AddTile(tile);
   }
}



HexGame::HexGame() :
      hgrid(),
      rad(0.0),
      xpos(0.0),
      ypos(0.0),
      players(),
      turn(0),
      plturn(0),
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
   if (rad < 10.0) {rad = 10.0;}
   hgrid.Resize(width,height,rad);
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
            if (turn == 0) {
               /// On first turn, allow players to pick a territory
               if (hover->owner == 0) {
                  Claim(hover , plturn + 1);
                  plturn = (plturn + 1)%NumPlayers();
                  for (int i = 0 ; i < NumPlayers() ; ++i) {
                     Player* pl = players[i + 1];
                     pl->CalcIncome();
                     if (plturn == 0) {
                        pl->EndTurn();
                     }
                  }
                  if (plturn == 0) {
                     ++turn;
                  }
               }
            }
            else {
               if (hover->influence[plturn + 1] && hover->owner == 0) {
                  Claim(hover , plturn + 1);
                  plturn = (plturn + 1) % NumPlayers();
                  for (int i = 0 ; i < NumPlayers() ; ++i) {
                     Player* pl = players[i + 1];
                     pl->CalcIncome();
                     if (plturn == 0) {
                        pl->EndTurn();
                     }
                  }
                  if (plturn == 0) {
                     ++turn;
                  }
               }
            }
         }
         else if (ee.mouse.button == 2) {
            Claim(hover , 0);
         }
      }
   }
}



void HexGame::DisplayOn(EagleGraphicsContext* win , int x , int y) {
/*
   for (int i = 0 ; i < nplayers + 1 ; ++i) {
      hgrid.DrawPlayer(win , xpos + x , ypos + y , players[i]);
   }
   if (hover) {
      if (hover->owner == 0) {
         hover->DrawFilled(win , xpos + x , ypos + y , players[turn + 1]->our_color);
      }
   }
   hgrid.DrawGrid(win , xpos + x , ypos + y);

*/
   hgrid.DrawGrid2(win , xpos + x , ypos + y , players);
   if (hover) {
      if (hover->owner == 0) {
         if (turn == 0) {
            /// first turn allow players to select a starting territory
            hover->DrawFilled(win , xpos + x , ypos + y , players[plturn + 1]->our_color);
         }
         else {
            /// normal turn, only highlight possible moves
            if (hover->influence[plturn + 1]) {
               hover->DrawFilled(win , xpos + x , ypos + y , players[plturn + 1]->our_color);
            }
         }
      }
   }
   
   for (int i = 0 ; i < NumPlayers() ; ++i) {
      std::string s = StringPrintF("%d" , players[i + 1]->TotalScore());
      win->DrawTextString(win->DefaultFont() , s , (i+1)*(1024/5) , 50 , GetEagleColor(players[i+1]->our_color) , HALIGN_CENTER , VALIGN_TOP);
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


