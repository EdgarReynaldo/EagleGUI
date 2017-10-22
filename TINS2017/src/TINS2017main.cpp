


#include "Intro.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"

int main(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   SendOutputToFile("TINS2017mainLOG.txt" , "" , false);

   Allegro5System* a5sys = GetAllegro5System();
   
   a5sys->Initialize(EAGLE_FULL_SETUP);
   
   a5sys->GetSystemTimer()->Start();
   
   al_save_bitmap("Intro.png" , dynamic_cast<Allegro5Image*>(RunIntro())->AllegroBitmap());
   
   Eagle::EagleLibrary::ShutdownEagle();
   
   return 0;
}





