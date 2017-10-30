


#include "Intro.hpp"
#include "DissolveIntro.hpp"
#include "LousyGlobals.hpp"
#include "Game.hpp"

#include "Eagle/backends/Allegro5Backend.hpp"



int main(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   SendOutputToFile("TINS2017mainLOG.txt" , "" , false);

   a5sys = GetAllegro5System();
   
   a5sys->Initialize(EAGLE_FULL_SETUP);
   
   double t = 0.0;
   
   our_win = a5sys->GetWindowManager()->CreateWindow("TINS 2017 main" , 1024 , 768 , EAGLE_OPENGL | EAGLE_WINDOWED);
   
   
   EagleImage* rsq = our_win->CreateImage(50,50);
   EagleImage* gsq = our_win->CreateImage(400,400);
   our_win->SetDrawingTarget(rsq);
   our_win->Clear(EagleColor(255,0,0));
   our_win->SetDrawingTarget(gsq);
   our_win->Clear(EagleColor(0,255,0));
   
   int rx = 100;
   int ry = 125;
   int gx = our_win->Width()/2;
   int gy = 100;
   Dissolver d;
   d.ConstructDissolveMapFromImages(rsq , rx , ry , gsq , gx , gy);
   d.SetAnimationPercent(0.0);
   
   EagleInfo() << "Dissolver has " << d.NParticles() << std::endl;
   
   a5sys->GetSystemTimer()->Start();

/**
   bool quit = false;
   bool redraw = true;
   bool sub = false;
   bool add = false;
   bool dir = false;
   
   double duration = 4;
   
   while (!quit) {
      if (redraw) {
         d.SetAnimationPercent(t/duration);

         our_win->DrawToBackBuffer();
         our_win->Clear(EagleColor(0,0,0));
         
         d.Draw(our_win , 0 , 0);
         
         our_win->DrawTextString(our_win->DefaultFont() , StringPrintF("%lf" , t) , our_win->Width()/2 , our_win->Height() - 20 , EagleColor(255,255,255) , HALIGN_CENTER , VALIGN_BOTTOM);
         
         our_win->FlipDisplay();
      }
      do {
         EagleEvent e = a5sys->GetSystemQueue()->WaitForEvent(0);
         if (e.type == EAGLE_EVENT_TIMER) {
///            t += e.timer.eagle_timer_source->SecondsPerTick()
               if (sub) {
                  t -= a5sys->GetSystemTimer()->SecondsPerTick();
               }
               if (add) {
                  t += a5sys->GetSystemTimer()->SecondsPerTick();
               }
               if (dir) {
                  t += a5sys->GetSystemTimer()->SecondsPerTick();
               }
               else {
                  t -= a5sys->GetSystemTimer()->SecondsPerTick();
               }
               if (t >= duration) {
                  t = duration;
                  dir = false;
               }
               if (t < 0.0) {
                  t = 0.0;
                  dir = true;
               }
            redraw = true;
         }
         if (e.type == EAGLE_EVENT_DISPLAY_CLOSE || (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_ESCAPE)) {
            quit = true;
            break;
         }
         if (e.type == EAGLE_EVENT_KEY_DOWN) {
            if (e.keyboard.keycode == EAGLE_KEY_LEFT) {
               sub = true;
               t -= a5sys->GetSystemTimer()->SecondsPerTick();
            }
            if (e.keyboard.keycode == EAGLE_KEY_RIGHT) {
               add = true;
               t += a5sys->GetSystemTimer()->SecondsPerTick();
            }
            redraw = true;
         }
         if (e.type == EAGLE_EVENT_KEY_UP) {
            if (e.keyboard.keycode == EAGLE_KEY_LEFT) {
               sub = false;
            }
            if (e.keyboard.keycode == EAGLE_KEY_RIGHT) {
               add = false;
            }
            redraw = true;
         }
      } while (a5sys->GetSystemQueue()->HasEvent(0));
   }
*/

   
///   Allegro5Image* intro_img = dynamic_cast<Allegro5Image*>(RunIntro());
///   al_save_bitmap("Intro.png" , intro_img->AllegroBitmap());



   Allegro5Image* intro_img = dynamic_cast<Allegro5Image*>(RunIntro());

   our_win->DrawToBackBuffer();
   
   DissolveIntro(our_win , intro_img);

   Game g;
   g.Init();
   
   int redraw = 1;
   
   while (redraw != -1) {
      if (redraw) {
         g.Draw();
         redraw = false;
      }
      do {
            
         EagleEvent e = a5sys->GetSystemQueue()->WaitForEvent(0);
         
         redraw = g.HandleEvent(e);
         
      } while (a5sys->GetSystemQueue()->HasEvent(0));
   }
   
   Eagle::EagleLibrary::ShutdownEagle();
   
   return 0;
}





