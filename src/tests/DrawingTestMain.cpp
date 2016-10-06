



#include "DrawingTestMain.hpp"

#include "Eagle.hpp"
#include "Eagle/backends/Allegro5backend.hpp"



DEFINE_TEST(DrawingTestMain , "Drawing Test (incomplete)");


int DrawingTestMain(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   Allegro5GraphicsContext a5window(800,600,EAGLE_WINDOWED);
   EagleGraphicsContext* win = &a5window;
   if (!win->Valid()) {
      return 2;
   }
   
   Allegro5Font a5font("Data/fonts/verdana.ttf" , 20 , 0);

   win->Clear(EagleColor(0,127,255));
   
   win->DrawTextString(&a5font , "Drawing test not implemented." , 100 , 250 , EagleColor(255,0,0));
   win->DrawTextString(&a5font , "Press any key to continue." , 100 , 290 , EagleColor(255,0,0));
   
   win->FlipDisplay();
   
   sys->GetSystemQueue()->WaitForEvent(EAGLE_EVENT_KEY_DOWN);
   
/**   
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
*/   
   return 0;
}
