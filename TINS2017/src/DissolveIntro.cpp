


#include "DissolveIntro.hpp"
#include "LousyGlobals.hpp"

#include "Eagle.hpp"

#include "Eagle/backends/Allegro5Backend.hpp"



EagleImage* our_image = 0;


Glob GetResult() {
   
   Glob g;
   g.spart.clear();
   Allegro5Image* img = dynamic_cast<Allegro5Image*>(our_image);
   for (int y = 0 ; y < img->H() ; y+=20) {
      g.spart.push_back(Particle(EagleColor(255,255,255,0) , 0.5 , y+ 0.5));
      g.spart.push_back(Particle(EagleColor(255,255,255,0) , img->W()-0.5 , y+10+0.5));
   }
   return g;
}


Glob GetResult2() {
   Glob g;
   g.spart.push_back(Particle(EagleColor(255,255,255,0) , our_win->Width()/4 , our_win->Height()/4));
   g.spart.push_back(Particle(EagleColor(255,255,255,0) , our_win->Width()*3/4 , our_win->Height()/4));
   g.spart.push_back(Particle(EagleColor(255,255,255,0) , our_win->Width()/4 , our_win->Height()*3/4));
   g.spart.push_back(Particle(EagleColor(255,255,255,0) , our_win->Width()*3/4 , our_win->Height()*3/4));
   return g;
}



Glob GetResult3() {
   Glob g;
   g.spart.clear();
   ALLEGRO_BITMAP* bmp = (dynamic_cast<Allegro5Image*>(our_image))->AllegroBitmap();
   al_lock_bitmap(bmp , ALLEGRO_PIXEL_FORMAT_ANY_32_WITH_ALPHA , ALLEGRO_LOCK_READONLY);
   for (int y = 15 ; y < our_win->Height() ; y += 30) {
      for (int x = 0 ; x < our_win->Width() ; ++x) {
         Particle p;
         p.c = EagleColor(255,255,255,255);
///         GetEagleColor(al_get_pixel(bmp , x , y));
         p.p = Pos2D(x + 0.5,y + 0.5);
         g.spart.push_back(p);
      }
   }
   al_unlock_bitmap(bmp);
   return g;
}



Glob GetResultOriginal() {
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
   
   while (!quit) {
      if (redraw) {
         our_win->DrawToBackBuffer();
         our_win->Clear(EagleColor(0,0,0));
         
         ct = (double)ProgramTime::Now() - st;
         pct = ct/8.0;
         
         if (pct < 1.0) {
            dissolver.SetAnimationPercent(pct);
         }
         else {
            quit = true;
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
   }
   
   
}


