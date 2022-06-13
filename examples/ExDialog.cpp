



#include "Eagle/backends/Allegro5Backend.hpp"
#include "Eagle.hpp"

#include <iostream>



int main(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   Allegro5System* sys = GetAllegro5System();
   
   int ret = sys->Initialize(EAGLE_FULL_SETUP);
   if (EAGLE_FULL_SETUP != ret) {
      EagleWarn() << "Failed to initialize some subsystem. Continuing anyway." << std::endl;
   }
   
   PopupText ptext(-1 , -1 , 0 , "Hello popup" , "Data/Fonts/Verdana.ttf" , -18 );
   
   ptext.Show();
   
   sys->Rest(4.0);

   ptext.Hide();
   
   sys->Rest(2.0);
   
   return 0;
}
