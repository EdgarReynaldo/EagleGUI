


#include "LayoutTestMain.hpp"

#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"

#include "TestRegistry.hpp"

int LayoutTestMain(int argc , char** argv) {

   (void)argc;
   (void)argv;

   EagleGraphicsContext* win = sys->CreateGraphicsContext(800,600,0);
   if (!win) {
      return 1;
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

   return 0;
}
