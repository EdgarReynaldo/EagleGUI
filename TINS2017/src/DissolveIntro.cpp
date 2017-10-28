


#include "DissolveIntro.hpp"
#include "LousyGlobals.hpp"

#include "Eagle.hpp"

#include "Eagle/backends/Allegro5Backend.hpp"



EagleImage* our_image = 0;



Glob GetResult() {
   Glob g;
   g.spart.clear();
   ALLEGRO_BITMAP* bmp = (dynamic_cast<Allegro5Image*>(our_image))->AllegroBitmap();
   al_lock_bitmap(bmp , ALLEGRO_PIXEL_FORMAT_ANY_32_WITH_ALPHA , ALLEGRO_LOCK_READONLY);
   for (int y = 1 ; y < our_win->Height() ; y += 3) {
      for (int x = 0 ; x < our_win->Width() ; ++x) {
         Particle p;
         p.c = GetEagleColor(al_get_pixel(bmp , x , y));
         p.p = Pos2D(x + 0.5,y + 0.5);
         g.spart.push_back(p);
      }
   }
   al_unlock_bitmap(bmp);
   return g;
}



void DissolveIntro(EagleGraphicsContext* win , EagleImage* intro) {
   
   our_win = win;
   our_image = intro;
   
   bool quit = false;
   bool redraw = true;
   
   Dissolver dissolver;
   
   Glob r = GetResult();
   dissolver.ConstructDissolveMapFromSources(our_image , r);
   
   double st = ProgramTime::Now();
   double ct = st;
   
   double pct = 0.0;
   
   do {
      if (redraw) {
         our_win->DrawToBackBuffer();
         our_win->Clear(EagleColor(0,0,0));
         
         ct = (double)ProgramTime::Now() - st;
         pct = ct/8.0;
         
         if (pct < 1.0) {
            dissolver.SetAnimationPercent(pct);
         }
         else if (pct < 1.5) {
            dissolver.SetAnimationPercent(1);
         }
         dissolver.Draw(our_win , 0 , 0);
         our_win->FlipDisplay();
         
         redraw = false;
      }
      
      do {
         EagleEvent e = a5sys->GetSystemQueue()->WaitForEvent(0);
         
         if (e.type == EAGLE_EVENT_TIMER) {
            redraw = true;
         }
         if (e.type == EAGLE_EVENT_DISPLAY_CLOSE || (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_ESCAPE)) {
            quit = true;
         }
         
      } while (a5sys->GetSystemQueue()->HasEvent(0));
      
      
   } while (!quit);
   
   
}


