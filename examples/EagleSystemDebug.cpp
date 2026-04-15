

#include <cstdio>


#include "Eagle/Backends/Allegro5Backend.hpp"
#include "Eagle/System.hpp"


/**
   bool InitializeSystem();
   bool FinalizeSystem();

   bool InitializeImages();
   bool InitializeFonts();
   bool InitializeTTFFonts();
   bool InitializeAudio();
   bool InitializeShaders();
   bool InitializePrimitives();
   bool InitializeDialog();

   virtual bool InitializeAll();// call this in overridden method calls

   bool InstallKeyboard();
   bool InstallMouse();
   bool InstallJoystick();
   bool InstallTouch();

   virtual bool InstallAll();// call this in overridden method calls
*/

int main(int argc , char** argv) {


   (void)argc;
   (void)argv;
   
   Allegro5System* a5sys = GetAllegro5System();
   
   bool sysup = a5sys->InitializeSystem();
   bool sysgood = a5sys->FinalizeSystem();
   
   std::cout << "Sys " << sysup << " Good " << sysgood << std::endl;




   return 0;
}

