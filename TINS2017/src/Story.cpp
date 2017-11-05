


#include "Story.hpp"

#include "Eagle/backends/Allegro5Backend.hpp"


#include "LousyGlobals.hpp"

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
   s.append("Help Dr. Pox cure himself before it's too late!\n");
   s.append("\n");
   s.append("He codes his recipes in hexadecimal, with\n");
   s.append("each digit corresponding to a certain pill.\n");
   s.append("But he doesn't remember which pill is which!\n");
   s.append("\n");
   s.append("He left clues scattered about. Press H for.\n");
   s.append("a hint or two. Left and right move the cursor.\n");
   s.append("Enter an 8 digit hexadecimal code and press\n");
   s.append("enter to see if its the correct solution!\n");
   s.append("\n");
   s.append("\n");
   s.append("\n");
   s.append("Help Dr. Pox recover his recipes and decode\n");
   s.append("the lock to the safe!\n");
   s.append("\n");
   
   return s;
}



void PlayStory() {
   EagleFont* story_font = our_win->LoadFont("Akashi.ttf" , -32);
   
   EAGLE_ASSERT(story_font && story_font->Valid());
   
   Story story(IntroStory() , story_font);
   
   story.Init(30.0);
   
   story.Play(); 
   
   our_win->FreeFont(story_font);
   
   return;
}



Pos2D Story::GetPos(double percent) {
   int h = story_font->Height(story , story_font->Height()/4);
   int w = story_font->Width(story);
   return Pos2D(our_win->Width()/2 - w/2 , our_win->Height() - percent*(h + our_win->Height()));
}



void Story::OnSetAnimationPercent() {
   pos = GetPos(GetAnimationPercent());
}



void Story::OnComplete() {
   complete = true;
}



Story::Story(std::string story_str , EagleFont* font) :
      AnimationBase(),
      story(story_str),
      story_font(font),
      key_up(false),
      key_down(false),
      pos(),
      complete(false),
      resting(0.0)
{}

void Story::Draw(EagleGraphicsContext* win , int x , int y) {
   win->DrawToBackBuffer();
   win->Clear(EagleColor(0,0,0,0));
   win->DrawMultiLineTextString(story_font , story , pos.X() + x , pos.Y() + y , EagleColor(255,255,255) , story_font->Height()/4 , HALIGN_LEFT , VALIGN_TOP);
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
      else if (e.keyboard.keycode == EAGLE_KEY_ESCAPE) {
         /// user skipped with a key
         ret = -1;
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
         resting = 3.0;
         if (key_up) {
            AdvanceAnimationTime(a5sys->GetSystemTimer()->SecondsPerTick());
         }
         if (key_down) {
            AdvanceAnimationTime(-a5sys->GetSystemTimer()->SecondsPerTick());
         }
      }
      else {
         resting -= e.timer.eagle_timer_source->SecondsPerTick();
         if (resting <= 0.0) {
            AdvanceAnimationTime(a5sys->GetSystemTimer()->SecondsPerTick());
         }
      }
      ret = 1;
   }
   return ret;
}



void Story::Play() {
   int redraw = 1;
   while (redraw != -1) {
      if (redraw) {
         Draw(our_win , 0 , 0);
         redraw = 0;
      }
      do {
         EagleEvent e = a5sys->GetSystemQueue()->WaitForEvent(0);
         redraw = HandleEvent(e);
         if (redraw == -1) {
            break;
         }
      } while (a5sys->GetSystemQueue()->HasEvent(0));
      if (complete) {
         break;
      }
   }
}





