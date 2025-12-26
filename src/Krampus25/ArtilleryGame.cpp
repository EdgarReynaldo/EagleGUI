



#include "ArtilleryGame.hpp"
#include "Projectile.hpp"
#include "ArtilleryPlayer.hpp"



#include "Eagle/Gui/WidgetBase.hpp"
#include "Eagle/GraphicsContext.hpp"
#include "Eagle/InputHandler.hpp"



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
   terrain.Generate(win->Width() , win->Height());
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
   turn = 0;
   players.push_back(new HumanPlayer());
   players[0]->Setup();
   players.push_back(new HumanPlayer());
   players[1]->Setup();
   
   terrain.Generate(window->Width() , window->Height());
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
   for (unsigned int i = 0 ; i < players.size() ; ++i) {
      int ret = players[i]->Update(dt);
      if (ret & DIALOG_DISABLED) {
         game_over = true;
         return DIALOG_REMOVE_ME;
      }
   }
   for (int i = 0 ; i < (int)live_rounds.size() ; ++i) {
      int ret = live_rounds[i]->Update(dt);
      if (ret & DIALOG_REMOVE_ME) {
         Projectile* p = live_rounds[i];
         delete p;
         if (live_rounds.size()) {
            live_rounds[i] = live_rounds.back();
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



