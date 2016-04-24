


#include "LayoutTestMain.hpp"

#include "Eagle.hpp"
#include "Eagle/backends/Allegro5backend.hpp"



int LayoutTestMain(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   Allegro5System* sys = new Allegro5System();
   if (sys->Initialize(EAGLE_FULL_SETUP) != EAGLE_FULL_SETUP) {
      delete sys;
      return 1;
   }
   
   EagleGraphicsContext* win = sys->CreateGraphicsContext(800,600,0);
   if (!win) {
      delete sys;
      return 2;
   }
   
   win->Clear(EagleColor(0,127,255));
   win->FlipDisplay();
   
   sys->Rest(2.0);
   
   
   
   
   
   
   
   
   
   
   
   bool close = false;
   bool redraw = true;
   
   sys->GetSystemTimer()->Start();
   
   do {
      
      if (redraw) {
         
         win->Clear(EagleColor(0,255,127));
         
         // gui->Draw();
         
         win->FlipDisplay();
         
         redraw = false;
      }
      
      do {
         EagleEvent e = sys->WaitForSystemEventAndUpdateState();
         
         if (e.type == EAGLE_EVENT_TIMER) {
            
            redraw = true;
         }
         if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_ESCAPE) {
            close = true;
            break;
         }
         if (e.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            close = true;
            break;
         }
         
      } while (!sys->UpToDate());
         
   } while (!close);
   
   
   
   
   
   
   
   delete sys;
   
   al_uninstall_system();
   
   return 0;
}
