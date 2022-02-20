



#include "Eagle/backends/Allegro5Backend.hpp"




int main(int argc , char** argv) {
   
   EagleSystem* sys = GetAllegro5System();
   
   EAGLE_ASSERT(sys);
   
   int ret = sys->Initialize(EAGLE_FULL_SETUP);
   if (ret != EAGLE_FULL_SETUP) {
      EagleLog() << "Failed to initialized subsystem. Continuing." << std::endl;
   }
   
   int sw = 1024;
   int sh = 768;
   
   EagleGraphicsContext* win = sys->CreateGraphicsContext(sw , sh , EAGLE_OPENGL | EAGLE_WINDOWED | EAGLE_NOFRAME);
   EAGLE_ASSERT(win && win->Valid());
   
   WidgetHandler gui(win , "GUI" , "Main GUI");
   
   gui.SetWidgetArea(Rectangle(0 , 0 , sw , sh) , false);
   
   gui.SetupBuffer(win , sw , sh);
   
   
   
   
   
   
   bool quit = false;
   bool redraw = true;
   
   sys->GetSystemTimer()->Start();
   
   while (!quit) {
      if (redraw) {
         win->SetDrawingTarget(win->GetBackBuffer());
         win->Clear();
         gui.Display(win , 0 , 0);
         win->FlipDisplay();
         redraw = false;
      }
      do {
         EagleEvent e = sys->WaitForSystemEventAndUpdateState();
      } while (!sys->UpToDate());
         
   };
   
   
   return 0;
}
