


#include "Game.hpp"

#include "Eagle.hpp"


const int NLEVELS = 3;



LEVEL_FUNC level_funcs[3] = {LevelOne , LevelTwo , LevelThree};



HINTLIST MakeHintList(int hcount , const char* hints[]) {
      HINTLIST hlist(hcount , std::string(""));
      for (int i = 0 ; i < hcount ; ++i) {
         hlist[i] = hints[i];
      }
      return hlist;
}



HINTMAP LevelOne() {

   HINTMAP hmap;

   const char* q1hint[2] = {"What did you see here? Nada, El Capitan!" ,
                            "All work and no play makes Dr. Pox a null boy"};
   HINTPAIR q1(std::string("00000000") , MakeHintList(2 , q1hint));
   
   const char* q2hint[2] = {"Don't be so negative" , "I^2"};
   HINTPAIR q2(std::string("ffffffff") , MakeHintList(2 , q2hint));
   
   const char* q3hint[3] = {"Fi Fo Fum" , "I don't like paying these" , "Be free"};
   HINTLIST hlist = {"Fi Fo Fum" , "I don't like paying these" , "Be free"};
   HINTPAIR q3(std::string("feeefeee") , MakeHintList(3 , q3hint));
   
   hmap.push_back(q1);
   hmap.push_back(q2);
   hmap.push_back(q3);

   return hmap;
}



HINTMAP LevelTwo() {
   HINTMAP hmap;
   hmap.push_back(HINTPAIR("abababab" , HINTLIST({"Quit babbling!" , "Killer abdomen!" , "Nice abs!"})));
   hmap.push_back(HINTPAIR("abcdabcd" , HINTLIST({"Alphabet soup!" , "Takes me back to grade school."})));
   hmap.push_back(HINTPAIR("f000f000" , HINTLIST({"Is that perfume?"})));
   return hmap;
}



HINTMAP LevelThree() {
   HINTMAP hmap;
   hmap.push_back(HINTPAIR("baadfood" , HINTLIST({"I think I ate something raunchy..."})));
   hmap.push_back(HINTPAIR("deadbeef" , HINTLIST({"Moo!"})));
   hmap.push_back(HINTPAIR("cdcdcdcd" , HINTLIST({"Damn, triggered a breakpoint"})));
   return hmap;
}



std::string Questions::Solution() {
   return hintmap[question].first;
}



HINTLIST Questions::GetHints() {
   return hintmap[question].second;
}



bool Questions::SetLevel(int lnum) {
   if (lnum < 0) {return false;}
   if (lnum >= NLEVELS) {return false;}
   level = lnum;
   question = 0;
   hintnum = 0;
   hintmap = (*level_funcs[level])();
   hints.clear();
   return true;
}



bool Questions::NextLevel() {
   return SetLevel(level + 1);
}



bool Questions::SetQuestion(int qnum) {
   if (qnum < 0) {return false;}
   if (qnum >= (int)hintmap.size()) {return false;}
   question = qnum;
   hintnum = 0;
   
   return true;
}



bool Questions::NextQuestion() {
   return SetQuestion(question + 1);
}



Questions::Questions() :
      level(0),
      question(0),
      hintnum(0),
      hintmap(),
      hints()
{
   SetLevel(0);
}



/// -------------------------     Game       --------------------------



Game::Game() :
      questions(),
      guess(""),
      info(""),
      hints(),
      guess_font(0),
      info_font(0)
{}



bool Game::Init() {
   questions.SetLevel(0);
   guess = "";
   hints = questions.GetHints();
   info = hints[0];
   if (!guess_font) {
      guess_font = our_win->LoadFont("Verdana.ttf" , -96);
   }
   if (!info_font) {
      info_font = our_win->LoadFont("Verdana.ttf" , -20);
   }
   return guess_font && info_font;
}



void Game::Draw() {
   
   our_win->DrawToBackBuffer();
   our_win->Clear(EagleColor(0,0,0));
   our_win->DrawTextString(guess_font , guess , our_win->Width()/2 , our_win->Height()/2 ,
                           EagleColor(255,255,255) , HALIGN_CENTER , VALIGN_CENTER);
   our_win->DrawTextString(info_font , questions.Solution() , 10 , 10 , EagleColor(255,255,255));
   our_win->FlipDisplay();
}



int Game::HandleEvent(EagleEvent e) {
   if (e.type == EAGLE_EVENT_DISPLAY_CLOSE || (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_ESCAPE)) {
      return -1;
   }
   if (e.type == EAGLE_EVENT_KEY_CHAR) {
      int ascii = e.keyboard.unicode;
      int kc = e.keyboard.keycode;
      if (kc == EAGLE_KEY_BACKSPACE) {
            
      }
      else if (kc == EAGLE_KEY_DELETE) {
         
      }
      else if (kc == EAGLE_KEY_ENTER) {
            
            
      }
   }
   return 0;
}




