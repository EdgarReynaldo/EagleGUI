



#include "Game.hpp"
#include "Player.hpp"
#include <cmath>
#include <deque>
#ifndef M_PI
   #define M_PI 3.14159265
#endif


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
      for (int i = 0 ; i < NumPlayers() ; ++i) {
         if (i + 1 == tile->owner) {continue;}
         TerritoryList& oturf = players[i+1]->our_turf;
         oturf.RemoveBorderTile(tile);
      }
   }
}



int HexGame::BattleTurn(HexTile* from , HexTile* to) {
   if (!from || !to) {
      EAGLE_ASSERT(from && to);
      return false;
   }
   if (from->owner == to->owner) {
      EAGLE_ASSERT(from->owner != to->owner);
      return false;
   }
   
   int nfrom = from->armies;
   int nto = to->armies;
   Dice dfrom(nfrom , 6);
   Dice dto(nto , 6);
   int result = DiceBattle(dfrom , dto);
   if (result < 0) {
      from->armies += result;
      if (from->armies < 0) {
         from->armies = 0;
      }
   }
   else if (result > 0) {
      to->armies -= result;
      if (to->armies < 0) {
         to->armies = 0;
      }
   }
   return result;
}



void HexGame::DrawArrow(double x1 , double y1 , double x2 , double y2 , EagleColor tint) {
   double dx = x2 - x1;
   double dy = y2 - y1;
   
   double dist = sqrt(dx*dx + dy*dy);
   if (dist == 0.0) {return;}
   
   double angle = atan2(dy,dx);
      
   Transform t = window->GetTransformer()->GetViewTransform();
   t.Translate(-arrowimg->W()/2 , 0 , 0);
   t.Scale(dist/100.0 , dist/100.0 , 1.0);
   t.Rotate(M_PI/2.0 + angle , 0.0 , 0.0 , 1.0);
   t.Translate(x2 , y2 , 0);
   window->GetTransformer()->PushViewTransform(t);
   window->DrawTinted(arrowimg , 0 , 0 , tint);
   window->GetTransformer()->PopViewTransform();
}



void HexGame::HandleFirstTurn(EagleEvent e) {
   if (turn > 0) {return;}
   if (e.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN && e.mouse.button == 1) {
      if (hover && hover->owner == 0) {
         /// On first turn, allow players to pick a territory
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
}



void HexGame::HandleRegularTurn(EagleEvent e) {
   if (e.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN && e.mouse.button == 1) {
      if (!drag) {
         if (hover && hover->owner == plturn + 1) {
            drag = true;
            dragtile = hover;
         }
      }
   }
   if (drag && e.type == EAGLE_EVENT_MOUSE_BUTTON_UP) {
      if (hover && hover->influence[plturn + 1] && hover->owner == 0) {
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
      drag = false;
   }
   else if (e.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN && e.mouse.button == 2) {
//      Claim(hover , 0);
   }
}



void HexGame::DisplayFirstTurn(EagleGraphicsContext* win , int x , int y) {
   if (turn > 0) {return;}
   
   if (hover && hover->owner == 0) {
      /// first turn allow players to select a starting territory
      hover->DrawFilled(win , x , y , players[plturn + 1]->our_color);
   }
}



void HexGame::DisplayRegularTurn(EagleGraphicsContext* win , int x , int y) {
   if (turn < 1) {return;}
   
   if (drag) {
      dragtile->DrawFilled(win , x , y , players[dragtile->owner]->our_color);
      dragtile->DrawStats(win , x , y , al_map_rgb(255,255,255));
      if (hover && hover != dragtile && hover->owner == 0) {
         std::vector<HexTile*> nbs = dragtile->neighbors;
         std::unordered_set<HexTile*> nbset(&nbs[0] , &nbs[nbs.size()]);
         if (nbset.find(hover) != nbset.end()) {
            /// normal turn, only highlight possible moves to a neighboring cell
            hover->DrawFilled(win , x , y , al_map_rgba(127,127,127,127));
            ALLEGRO_COLOR acol = players[plturn + 1]->our_color;
            EagleColor ecol = GetEagleColor(acol);
            ecol.SetAlpha(0.5f);
            DrawArrow(dragtile->mx + x , dragtile->my + y , hover->mx + x , hover->my + y , ecol);
         }
      }
   }
   else {
      if (hover && hover->owner == plturn + 1) {
         hover->DrawFilled(win , x , y , al_map_rgba(255,255,255,127));
      }
   }
}



HexGame::HexGame(EagleGraphicsContext* win) :
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
      hy(0),
      space(false),
      drag(false),
      dragtile(0),
      rdrag(false),
      rdragtile(0),
      arrowimg(0),
      window(0)
{
   EAGLE_ASSERT(win);
   window = win;
   
   arrowimg = window->LoadImageFromFile("Arrow.png");
   EAGLE_ASSERT(arrowimg && arrowimg->Valid());
   
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
   window->FreeImage(arrowimg);
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



void HexGame::HandleEvent(EagleEvent e) {
   if (e.type == EAGLE_EVENT_MOUSE_AXES) {
      mx = e.mouse.x;
      my = e.mouse.y;
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
   if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_SPACE) {
      space = true;
   }
   else if (e.type == EAGLE_EVENT_KEY_UP && e.keyboard.keycode == EAGLE_KEY_SPACE) {
      space = false;
   }
   if (turn == 0) {
      HandleFirstTurn(e);
   }
   else {
      HandleRegularTurn(e);
   }
   
   if (e.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN) {
      if (e.mouse.button == 2) {
         rdrag = true;
         rdragtile = hover;
      }
   }
   if (e.type == EAGLE_EVENT_MOUSE_BUTTON_UP) {
      if (e.mouse.button == 2) {
         rdrag = false;
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
   
   if (turn == 0) {
      DisplayFirstTurn(win , xpos + x , ypos + y);
   }
   else {
      DisplayRegularTurn(win , xpos + x , ypos + y);
   }
   
   if (rdrag) {
      PATH p = FindBestPath(rdragtile , hover);
      DrawPath(p , xpos + x , ypos + y);
   }
   
   for (int i = 0 ; i < NumPlayers() ; ++i) {
      std::string s = StringPrintF("%d" , players[i + 1]->TotalScore());
      win->DrawTextString(win->DefaultFont() , s , (i+1)*(1024/4) , 50 , GetEagleColor(players[i+1]->our_color) , HALIGN_CENTER , VALIGN_TOP);
      std::string s2 = StringPrintF("%d" , players[i + 1]->TotalIncome());
      win->DrawTextString(win->DefaultFont() , s2 , (i+1)*(1024/4) , 80 , GetEagleColor(players[i+1]->our_color) , HALIGN_CENTER , VALIGN_TOP);
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




PATH HexGame::FindBestPath(HexTile* start , HexTile* destination) {

   //EAGLE_ASSERT(start && destination);

   if (!start) {return PATH();}
   if (!destination) {return PATH();}
   if (start == destination) {return PATH(start);}
   
   std::unordered_set<HexTile*> visited;
   visited.insert(start);
   
   PATH pstart(start);

   PATHSET pathset;
   pathset.push_back(pstart);
   
   bool destreached = false;
   for (std::list<PATH>::iterator it = pathset.begin() ; it != pathset.end() ;) {
      PATH p = *it;
      it = pathset.erase(it);
      
      HexTile* from = p.path.back();
      HexTile* to = destination;
      double arad = atan2(to->my - from->my , to->mx - from->mx);
      
      const HEXDIRECTION d = AngleToHexDirection(arad);
      
      const std::vector<HexTile*> nb = p.path.back()->neighbors;
      const std::vector<HexTile*> nbs = {
         nb[d % 6],
         nb[(d+1)%6],
         nb[(d+5)%6],
         nb[(d+2)%6],
         nb[(d+4)%6],
         nb[(d+3)%6]
      };
      std::deque<PATH> npaths;
      for (unsigned int i = 0 ; i < NUM_HEX_DIRECTIONS ; ++i) {
         HexTile* next = nbs[i];
         if (!next) {continue;}
         if (visited.find(next) != visited.end()) {continue;}
         
         if (!hgrid.Safe(next)) {
//         if (next < &hgrid.grid[0][0] || next >= &hgrid.grid[hgrid.grid.size()][hgrid.grid[0].size()]) {
            throw EagleException("WTF");
         }
         
         
         PATH p2 = p;
         p2.PushBack(next);
         npaths.push_back(p2);
         visited.insert(next);
         if (next == destination) {
            return p2;
         }
      }
      pathset.insert(pathset.begin() , npaths.begin() , npaths.end());
      it = pathset.begin();
   }
   return PATH();
}



void HexGame::DrawPath(PATH p , int x , int y) {
   int i = 0;
   std::vector<HexTile*>::iterator it = p.path.begin();
   if (it == p.path.end()) {return;}
   std::vector<HexTile*>::iterator previt = p.path.begin();
   ++it;
   if (it == p.path.end()) {return;}
   for (; it != p.path.end() ; ++it) {
      ++i;
      HexTile* from = *previt;
      HexTile* to = *it;
      DrawArrow(x + from->mx , y + from->my , x + to->mx , y + to->my , EagleColor(255,255,0));
      previt = it;
   }
}





