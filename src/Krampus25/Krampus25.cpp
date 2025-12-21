/**
Dear Mr. Krampus,

My wishes are as follows. Pick some, one, or none!

Controls: I’ve been playing a lot of classic arcade games at the local arcade - Joust, Centipede, Rampage, Donkey Kong, to name a few. Compared to modern games, each of these seems to rely on somewhat non-ideal or less responsive controls, which results in a unique type of gameplay you don’t often find today. I’d like a game that takes this into account, a spectrum of controls that creates interesting gameplay.

A Secret Room: It would be nice to have an unexpected surprise, a reward, or a hidden passageway.

Ranged Attack: I like the idea of attacking from a distance rather than committing to a risky close range hit. This could be in FPS games, top down bullet hell styles, or anywhere else, doesn’t need to be a main mechanic.

ASCII Tiles: Visually, I enjoy a good ASCII style glyph font for game boards. Incorporating that in some way would be cool.

As always, feel free to ditch any or all of these if something else inspires you. :) Thanks!

//*/



#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"


#include "Games.hpp"


int main(int argc , char** argv) {

   (void)argc;
   (void)argv;
   
   Allegro5System* sys = GetAllegro5System();
   int ret = EAGLE_STANDARD_SETUP;
   ret = sys->Initialize(ret);
   if (!(ret & EAGLE_STANDARD_SETUP)) {
      EagleLog() << "Failed to setup standard Allegro 5 system" << std::endl;
   }
   
   EagleGraphicsContext* win = sys->CreateGraphicsContext("Window" , 1024,768,EAGLE_WINDOWED | EAGLE_OPENGL);
   
   /// Menu, 0 frogger 1 red baron 2 artillery 3 ascii frogger
   int menu_choice = -1;
   if (argc > 1) {
      
   }
   Scene* pgame[4] = {0,0,0,0};
//   KGame* cgame = 0;
   
   
//   game[0] = new FroggerGame;
//   game[1] = new RedBaronGame;
   pgame[0] = agame = new ArtilleryGame(win);
   Scene* cgame = pgame[0];
//   cgame = game[0];
//   game[3] = new AsciiFroggerGame;

   bool quit = false;
   bool redraw = true;
   sys->GetSystemTimer()->Start();

   while (!quit) {
      if (redraw) {
         win->Clear();
         cgame->DisplayOn(win);
         win->FlipDisplay();
         redraw = false;
      }
      do {
         EagleEvent e = sys->WaitForSystemEventAndUpdateState();
         if (e.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_ESCAPE) {
            quit = true;
         }
         if (e.type == EAGLE_EVENT_TIMER) {
            redraw = true;
            ret = cgame->Update(e.timer.eagle_timer_source->SPT());
            if (ret & DIALOG_CLOSE) {
               quit = true;
            }
         }
         ret = cgame->HandleEvent(e);
         if (ret & DIALOG_CLOSE) {
            quit = true;
         }
      }
      while (!sys->UpToDate());
   }
   return 0;
}
