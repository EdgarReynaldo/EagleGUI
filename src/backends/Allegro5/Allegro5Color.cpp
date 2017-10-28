


#include "Eagle/backends/Allegro5/Allegro5Color.hpp"



ALLEGRO_COLOR GetAllegroColor(EagleColor c) {
   return al_map_rgba_f(c.fr , c.fg , c.fb , c.fa);
///   return al_map_rgba(c.r , c.g , c.b , c.a);
}



EagleColor GetEagleColor(ALLEGRO_COLOR c) {
   unsigned char r,g,b,a;
   al_unmap_rgba(c , &r , &g , &b , &a);
   return EagleColor(r,g,b,a);
}
