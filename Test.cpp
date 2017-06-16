
#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"
#include "allegro5/allegro.h"

int main(int argc , char** argv) {

///   EagleSystem* sys = Eagle::EagleLibrary::System("Allegro5");
///   sys->Initialize(EAGLE_FULL_SETUP);

///   EagleGraphicsContext* win = sys->CreateGraphicsContext(800 , 600 , EAGLE_WINDOWED);

   if (!al_init()) {return 1;}

   ALLEGRO_DISPLAY* d = al_create_display(800,600);
   if (!d) {return 2;}

   al_clear_to_color(al_map_rgb(0,255,0));

   al_flip_display();

///   win->Clear(EagleColor(0,255,0));
   al_clear_to_color(al_map_rgb(0,255,0));

///   win->FlipDisplay();
   al_flip_display();


///   sys->Rest(3.0);
   al_rest(1.0);

   return 0;
}

