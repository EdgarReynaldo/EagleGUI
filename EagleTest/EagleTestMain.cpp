



#include "Eagle/backends/Allegro5Backend.hpp"






int main(int argc , char** argv) {

   (void)argc;
   (void)argv;
   
///   EAGLE_ASSERT(ALLEGRO5_REGISTERED);
   
   Allegro5System* sys = GetAllegro5System();
///   EagleSystem* sys = Eagle::EagleLibrary::System("Allegro5");
   
   if (EAGLE_FULL_SETUP != sys->Initialize(EAGLE_FULL_SETUP)) {
      EagleWarn() << "Failed to install some components." << std::endl;
   }
   EagleGraphicsContext* win = sys->GetWindowManager()->CreateWindow("win" , 800 , 600 , EAGLE_OPENGL | EAGLE_WINDOWED);
   
   win->Clear(EagleColor(0,255,255));
   win->FlipDisplay();
   
   
   WidgetHandler gui(win);
   
   
   
   
   sys->Rest(3.0);
   
   return 0;
}

