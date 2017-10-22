


#include "Intro.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"

int main(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   SendOutputToFile("TINS2017mainLOG.txt" , "" , false);

   Allegro5System* a5sys = GetAllegro5System();
   
   a5sys->Initialize(EAGLE_FULL_SETUP);
   
   a5sys->GetSystemTimer()->Start();
   
   RunIntro();
   
   Eagle::EagleLibrary::ShutdownEagle();
   
   return 0;
}





