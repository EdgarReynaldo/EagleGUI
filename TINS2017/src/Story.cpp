


#include "Story.hpp"



std::string IntroStory() {
   std::string s;
   s.append("Dr. Von Chicken Pox welcomes you!\n");
   s.append("\n");
   s.append("He needs your help curing his rather\n");
   s.append("embarassing case of chicken pox. You\n");
   s.append("see he knows the cure but he forgot the\n");
   s.append("recipe, and the only remaining vial is in\n");
   s.append("his safe. But the combination to the safe\n");
   s.append("is the recipe for the cure!\n");
   s.append("\n");
   s.append("Help Dr. Pox cure himself!\n");
   s.append("He codes his recipes in hexadecimal, with\n");
   s.append("each digit corresponding to a certain pill.\n");
   s.append("He left clues scattered about elsewhere.\n");
   s.append("\n");
   s.append("Help Dr. Pox recover his recipes and decode\n");
   s.append("his lock to the safe!\n");
   s.append("\n");
   s.append("\n");
   s.append("\n");
   
   return s;
}



Pos2D Story::GetPos(double percent) {
   int h = story_font->Height(story);
   int w = story_font->Width(story);
   return Pos2D(our_win->Width()/2 - w/2 , our_win->Height() - percent*(h + our_win->Height()));
}



void Story::Draw(EagleGraphicsContext* win , int x , int y) {
   win->DrawToBackBuffer();
   win->Clear(EagleColor(0,0,0,0));
   win->DrawMultiLineTextString(story_font , story , pos.X() , pos.Y() , EagleColor(255,255,255) , story_font->Height()/4 , HALIGN_CENTER , VALIGN_TOP);
   win->FlipDisplay();
}



int Story::HandleEvent(EagleEvent e) {
   int ret = 0;
   if (e.type == EAGLE_EVENT_KEY_DOWN) {
      if (e.keyboard.keycode == EAGLE_KEY_UP) {
         key_up = true;
      }
      else if (e.keyboard.keycode == EAGLE_KEY_DOWN) {
         key_down = true;
      }
      else {
         /// user skipped with a key
         redraw = -1;
      }
   }
   else if (e.type == EAGLE_EVENT_KEY_UP) {
      if (e.keyboard.keycode == EAGLE_KEY_UP) {
         key_up = false;
      }
      else if (e.keyboard.keycode == EAGLE_KEY_DOWN) {
         key_down = false;
      }
   }
   if (e.type == EAGLE_EVENT_TIMER) {
      if (key_up || key_down) {
         if (key_up) {
            t += a5sys->GetSystemTimer()->SecondsPerTick();
         }
         if (key_down) {
            t -= a5sys->GetSystemTimer()->SecondsPerTick();
         }
      }
      else {
         t += a5sys->GetSystemTimer()->SecondsPerTick();
         ret = 1;
      }
   }
   return ret;
}



void Story::Play() {
   int redraw = 1;
   while (redraw != -1) {
      if (redraw) {
         Draw();
         redraw = 0;
      }
      do {
         EagleEvent e = a5sys->GetSystemQueue()->WaitForEvent(0);
         redraw = HandleEvent(e);
         if (redraw == -1) {
            break;
         }
      } while (a5sys->GetSystemQueue()->HasEvent(0));
   }
}





