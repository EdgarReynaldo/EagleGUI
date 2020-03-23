
#include <vector>
#include <unordered_set>


#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"
#include "hexsrc/Game.hpp"
#include "hexsrc/Math.hpp"
#include "hexsrc/PRNG.hpp"

int main2(int argc , char** argv) {

   (void)argc;
   (void)argv;
   
   Allegro5System* a5sys = GetAllegro5System();
   
   int ret = 0;
   if ((ret = a5sys->Initialize(EAGLE_FULL_SETUP)) != EAGLE_FULL_SETUP) {
      if (!(ret & (EAGLE_MOUSE | EAGLE_KEYBOARD))) {
         EagleCritical() << "Failed to install keyboard and mouse." << std::endl;
      }
   }
   
   int sw = 1024;
   int sh = 768;
   EagleGraphicsContext* win = a5sys->CreateGraphicsContext("HexGame window" , sw , sh , EAGLE_OPENGL);
   
   EAGLE_ASSERT(win && win->Valid());
   
   a5sys->GetSystemTimer()->Start();
   
   DiceBattle battle;

   bool redraw = true;
   bool quit = false;

   EagleImage* const d6 = win->LoadImageFromFile("d6_50x50_2.png");
   
   EAGLE_ASSERT(d6 && d6->Valid());
   
   EagleImage* const d6sides[6] = {
      win->CreateSubImage(d6 , 0   , 0  , 50 , 50 , "d6_1"),
      win->CreateSubImage(d6 , 50  , 0  , 50 , 50 , "d6_2"),
      win->CreateSubImage(d6 , 100 , 0  , 50 , 50 , "d6_3"),
      win->CreateSubImage(d6 , 0   , 50 , 50 , 50 , "d6_4"),
      win->CreateSubImage(d6 , 50  , 50 , 50 , 50 , "d6_5"),
      win->CreateSubImage(d6 , 100 , 50 , 50 , 50 , "d6_6")
   };
   
//   EagleImage* const images[7] = {d6 , d6sides[0] , d6sides[1] , d6sides[2] , d6sides[3] , d6sides[4] , d6sides[5]};
   
   
   while (!quit) {
      if (redraw) {
         
         win->Clear();
         
         std::vector<unsigned int> drolls = battle.GetDefRolls();
         std::vector<unsigned int> orolls = battle.GetOffRolls();

         unsigned int n = 0;
         for (; n < MIN(drolls.size() , orolls.size()) ; ++n) {
            unsigned int d = drolls[n];
            unsigned int o = orolls[n];
            
            win->DrawTinted(d6sides[(int)o - 1] , (n+1)*55 , 50 , EagleColor(255,0,0));
            win->DrawTinted(d6sides[(int)d - 1] , (n+1)*55 , 110 , EagleColor(0,255,0));
            win->DrawTinted(d6sides[(int)(MAX(o,d)) - 1] , (n+1)*55 , 170 , (o>d)?EagleColor(255,0,0):EagleColor(0,255,0));
         }
         std::vector<unsigned int>* lrolls = (drolls.size() > orolls.size())?&drolls:&orolls;
         for (; n < MAX(drolls.size() , orolls.size()) ; ++n) {
            win->DrawTinted(d6sides[(*lrolls)[n] - 1] , (n+1)*55 , (lrolls == &orolls)?50:110 , (lrolls == &orolls)?EagleColor(255,0,0):EagleColor(0,255,0));
            win->DrawTinted(d6sides[(*lrolls)[n] - 1] , (n+1)*55 , 170 , (lrolls == &orolls)?EagleColor(255,0,0):EagleColor(0,255,0));
         }
         
         std::string rstr = StringPrintF("%i %s casualties" , abs(battle.GetResult()) , (battle.GetResult()>0)?"Defender":"Attacker");
         win->DrawTextString(win->DefaultFont() , rstr , 50 , 220 , EagleColor(255,255,255));
         
         win->FlipDisplay();
         
         redraw = false;
      }
      do {
         EagleEvent e = a5sys->WaitForSystemEventAndUpdateState();
         if (e.type == EAGLE_EVENT_TIMER) {redraw = true;}
         if ((input_key_held(EAGLE_KEY_SPACE) && key_held_duration[EAGLE_KEY_SPACE] > 1.0) || input_key_press(EAGLE_KEY_SPACE)) {
            battle.SetDice(Dice(10 , 6) , Dice(12 , 6));
         }
         if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_ESCAPE) {
            quit = true;
         }
         if (e.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
      } while (!a5sys->UpToDate());
   }
   
   return 0;
}




int main(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   
   Allegro5System* a5sys = GetAllegro5System();
   
   int ret = 0;
   if ((ret = a5sys->Initialize(EAGLE_FULL_SETUP)) != EAGLE_FULL_SETUP) {
      if (!(ret & (EAGLE_MOUSE | EAGLE_KEYBOARD))) {
         EagleCritical() << "Failed to install keyboard and mouse." << std::endl;
      }
   }
   
   int sw = 1024;
   int sh = 768;
   EagleGraphicsContext* win = a5sys->CreateGraphicsContext("HexGame window" , sw , sh , EAGLE_OPENGL);
   
   EAGLE_ASSERT(win && win->Valid());
   
   HexGame game(win);
   
   game.Resize(14,12,25);
   
   game.Position(sw/4 , sh/4);
   
   a5sys->GetSystemTimer()->Start();

   game.Run(a5sys , win);
   
//   win->FlipDisplay();
   
///   a5sys->GetSystemQueue()->WaitForEvent(EAGLE_EVENT_KEY_DOWN , 0);
   
   
   return 0;
}




