
#include <vector>
#include <unordered_set>


#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"
#include "hexsrc/Game.hpp"


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



