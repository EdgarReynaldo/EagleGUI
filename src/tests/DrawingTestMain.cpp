



#include "DrawingTestMain.hpp"

#include "Eagle.hpp"
#include "Eagle/backends/Allegro5backend.hpp"



int DrawingTestMain(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   EagleLog() << "DrawingTestMain not implemented." << std::endl;
   return 0;
   
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
         
         
         
         win->FlipDisplay();
         redraw = false;
      }
      
      do {
         EagleEvent e = sys->WaitForSystemEventAndUpdateState();
         
         if (e.type == EAGLE_EVENT_TIMER) {
            
            redraw = true;
         }
         
         
      } while (!sys->UpToDate());
         
   } while (!close);
   
   
   
   
   
   
   
   delete sys;
   
   al_uninstall_system();
   
   return 0;
}
