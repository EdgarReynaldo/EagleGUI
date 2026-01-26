



#include "ArtilleryGame.hpp"
#include "Projectile.hpp"
#include "ArtilleryPlayer.hpp"
#include "RNG.hpp"
#include "Games.hpp"

#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/GraphicsContext.hpp"
#include "Eagle/InputHandler.hpp"
#include "Eagle/Math.hpp"


ArtilleryGame::ArtilleryGame(EagleGraphicsContext* win) :
      Scene(),
      window(win),
      terrain(),
      level(0),
      turn(0),
      windvel(0.0),
      windangled(0.0),
      game_over(false),
      players(),
      live_rounds()
{
   Reset();
   players[0]->aim = true;
   players[0]->turn = true;
}



ArtilleryGame::~ArtilleryGame() {
   Clear();
}



void ArtilleryGame::Clear() {
   terrain.Clear();
   turn = 0;
   windvel = 0.0;
   windangled = 90.0;
   for (unsigned int i = 0 ; i < players.size() ; ++i) {
      delete players[i];
   }
   players.clear();
   for (unsigned int i = 0 ; i < live_rounds.size() ; ++i) {
      delete live_rounds[i];
   }
   live_rounds.clear();
}



void ArtilleryGame::Reset() {
   Clear();
   ++level;
   // ++turn;
   for (int i = 0 ; i < (int)players.size() ; ++i) {
      ArtilleryPlayer* p = players[i];
      if (p->score >= 5) {
         p->tourney++;
         p->score = 0;
         for (int j = 0 ; j < (int)players.size() ; ++j) {
            if (j != i) {
               ArtilleryPlayer* p2 = players[j];
               p2->score = 0;
            }
         }
      }
   }
   terrain.Generate(scrw , scrh);
   int lx = rng.Rand0toNM1(scrw/2);
   int rx = rng.Rand0toNM1(scrw/2) + scrw/2;
   players.push_back(new HumanPlayer());
   players[0]->Setup(lx , scrh - terrain.spans[lx].h , EagleColor(0,0,255) , EagleColor(255,255,0) , 45.0*M_PI/180.0);
//   players[0]->Setup(50 , window->Height() - (terrain.spans[50].h + 25) , EagleColor(0,0,255) , EagleColor(127,255,0) , 45.0);
   players.push_back(new HumanPlayer());
//   players[1]->Setup(750 , window->Height() - (terrain.spans[window->Width() - 50].h + 25) , EagleColor(255,0,0) , EagleColor(127,255,0) , 135.0);
   players[1]->Setup(rx , scrh - terrain.spans[rx].h , EagleColor(255,0,0) , EagleColor(255,255,0) , 135.0*M_PI/180.0);
//   players[1]->Setup(750 , window->Height() - (terrain.spans[window->Width() - 50].h + 25) , EagleColor(255,0,0) , EagleColor(127,255,0) , 135.0);
}

int ArtilleryGame::HandleEvent(EagleEvent e) {
   if (e.type == EAGLE_EVENT_DISPLAY_CLOSE) {
      return DIALOG_CLOSE;
   }
   if (game_over) {
      return DIALOG_REMOVE_ME;
   }
   if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_SPACE) {
      Reset();
   }
   if (e.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN && e.mouse.button == 1) {
      players[turn]->aim = !players[turn]->aim;
   }
   for (unsigned int i = 0 ; i < players.size() ; ++i) {
      int ret = players[i]->HandleEvent(e);
      if (ret & DIALOG_REMOVE_ME) {
         
      }
   }
   if (e.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN && e.mouse.button == 2) {
      players[turn]->Launch();
      players[turn]->aim = false;
      players[turn]->turn = false;
      turn = (turn + 1) % players.size();
      players[turn]->turn = true;
      players[turn]->aim = true;
   }
   
   return DIALOG_OKAY;
}



void ArtilleryGame::DisplayOn(EagleGraphicsContext* win) {
   terrain.DisplayOn(win);
   for (unsigned int i = 0 ; i < players.size() ; ++i) {
      players[i]->DisplayOn(win);
   }
   for (unsigned int i = 0 ; i < live_rounds.size() ; ++i) {
      live_rounds[i]->DisplayOn(win);
   }
}



int ArtilleryGame::Update(double dt) {
   for (int i = 0 ; i < (int)players.size() ; ++i) {
      int ret = players[i]->Update(dt);
      if (ret & DIALOG_DISABLED) {
         game_over = true;
         for (int j = 0 ; j < (int)players.size() ; ++j) {
            if (j != i) {
               players[j]->score++;
            }
         }
         Reset();
         return DIALOG_OKAY;
      }
   }
   for (int i = 0 ; i < (int)live_rounds.size() ; ++i) {
      double xpos = live_rounds[i]->xpos;
      double ypos = live_rounds[i]->ypos;
      int ret = live_rounds[i]->Update(dt);
      Projectile* p = live_rounds[i];
      double xpos2 = p->xpos;
      double ypos2 = p->ypos;
      int dx = (int)(ceil(xpos2 - xpos));
      int dy = (int)(ceil(ypos2 - ypos));
      
      if (dx >= 0) {
         for (int x = 0 ; x <= dx ; ++x) {
            int htatx = terrain.HeightAtX(x + (int)xpos);
            if (scrh - htatx <= ypos + (double)x*dy/dx) {
               p->Explode(buffer);
               break;
            }
            for (int k = 0 ; k < (int)agame->players.size() ; ++k) {
               
               if (agame->players[k]->base.area->Contains(x + xpos , ypos + (double)x*dy/dx)) {
                  p->Explode(buffer);
                  for (int j = 0 ; j < (int)agame->players.size() ; ++j) {
                     if (j != k) {
                        agame->players[j]->score++;
                     }
                  }
                  break;
               }
            }
         }
      }
      else {
         EAGLE_ASSERT(dx <= 0.0);
         for (int x = 0 ; x >= dx ; --x) {
            int htatx = terrain.HeightAtX(x + (int)xpos);
            EAGLE_ASSERT(htatx >= 0);
            if (scrh - htatx <= ypos + (double)x*dy/dx) {
               p->Explode(buffer);
               break;
            }
            for (int k = 0 ; k < (int)agame->players.size() ; ++k) {
               
               if (agame->players[k]->base.area->Contains(x + xpos , ypos + (double)x*dy/dx)) {
                  p->Explode(buffer);
                  for (int j = 0 ; j < (int)agame->players.size() ; ++j) {
                     if (j != k) {
                        agame->players[j]->score++;
                     }
                  }
                  break;
               }
            }
         }
      }
      if (ret & DIALOG_REMOVE_ME) {
         if (live_rounds.size() > 1) {
            live_rounds[i] = live_rounds.back();
            delete p;
         }
         if (live_rounds.size()) {
            live_rounds.pop_back();
         }
         --i;
      }
   }
   return DIALOG_OKAY;
}

void ArtilleryGame::AddPlayer(ArtilleryPlayer* newp) {
   players.push_back(newp);
}



void ArtilleryGame::AddProjectile(Projectile* newp) {
   live_rounds.push_back(newp);
}



