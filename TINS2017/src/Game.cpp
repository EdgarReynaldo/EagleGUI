


#include "Game.hpp"

#include "Eagle/backends/Allegro5Backend.hpp"
#include "Eagle.hpp"



Pos2D TopCenter(double percent) {
   (void)percent;
   return Pos2D(our_win->Width()/2 , our_win->Height()/2);
}


EagleColor fc;

EagleColor Fader(double percent) {
   percent = 1.0 - percent;
   EagleColor fade((float)percent*fc.fr , percent*fc.fg , percent*fc.fb , percent);
   return fade;
}



Transform Shrinker(double percent) {
   if (percent > 1.0) {percent = 1.0;}
   if (percent < 0.0) {percent = 0.0;}
   
   double factor = 2.0;
   percent = factor*(1.0 - percent);
   Transform t = our_win->GetTransformer()->GetIdentityTransform();
   Pos2D p = TopCenter(0.0);
   t.Translate(-p.X() , -p.Y());
   t.Scale(percent , percent);
   t.Translate(p.X() , p.Y());
   return t;
}



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
   hmap.push_back(HINTPAIR("baadf00d" , HINTLIST({"I think I ate something raunchy..."})));
   hmap.push_back(HINTPAIR("deadbeef" , HINTLIST({"Moo!"})));
   hmap.push_back(HINTPAIR("cdcdcdcd" , HINTLIST({"Damn, triggered a breakpoint" , "This place is super seedy!"})));
   return hmap;
}



std::string Questions::Solution() {
   return hintmap[question].first;
}



bool Questions::SetLevel(int lnum) {
   if (lnum < 0) {return false;}
   if (lnum >= NLEVELS) {return false;}

   level = lnum;
   hintmap = (*level_funcs[level])();

   SetQuestion(0);
   SetHint(0);
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
   hints = hintmap[question].second;
   return true;
}



bool Questions::NextQuestion() {
   return SetQuestion(question + 1);
}



bool Questions::SetHint(int hnum) {
   if (hnum < 0) {return false;}
   if (hnum >= (int)hints.size()) {return false;}
   hintnum = hnum;
   return true;
}



bool Questions::NextHint() {
   return SetHint(hintnum + 1);
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



HINTLIST Questions::GetHints() {
   HINTLIST show;
   for (int i = 0 ; i <= hintnum ; ++i) {
      show.push_back(hints[i]);
   }
   return show;
}



/// -------------------------     Game       --------------------------



void Game::SetMessage(const char* msg , EagleColor color) {
   msgtime = duration;
   message = msg;
   fc = color;
   message_fader.Create(guess_font , message , Shrinker , Fader , TopCenter);
}



void Game::RebuildInfo() {
   info = "";
   for (int i = 0 ; i < (int)hints.size() ; ++i) {
      info.append(hints[i]);
      info.append("\n");
   }
}



void Game::RefreshGuess() {
   guess = "";
   int i = 0;
   while (i < 8) {
      char str[2] = {input[i] , '\0'};
      guess.append(str);
      ++i;
   }
}

/**
   Questions questions;
   
   std::vector<char> input;
   int caret;
   
   std::string guess;
   std::string info;
   HINTLIST hints;
   
   bool question_up;
   bool level_up;
   bool finish;
   double duration;
   double msg_time;
   
   EagleFont* guess_font;
   EagleFont* info_font;
*/
Game::Game() :
      questions(),
      input(),
      caret(0),
      guess(""),
      info(""),
      message(""),
      hints(),
      message_fader(),
      question_up(false),
      level_up(false),
      finish(false),
      duration(0.0),
      msgtime(-1),
      guess_font(0),
      info_font(0)
{
   duration = 3;
}



bool Game::Init() {
   questions.SetLevel(0);
   input = {' ',' ',' ',' ',' ',' ',' ',' '};
   RefreshGuess();
   caret = 0;
   hints = questions.GetHints();
   RebuildInfo();

   if (!guess_font) {
      guess_font = our_win->LoadFont("Consola.ttf" , -96);
   }
   if (!info_font) {
      info_font = our_win->LoadFont("Verdana.ttf" , -36);
   }

   SetMessage("Start!!!" , EagleColor(0,255,0));

   return guess_font && info_font;
}



void Game::Draw() {
   
   our_win->DrawToBackBuffer();
   our_win->Clear(EagleColor(0,0,0));
   int w = guess_font->Width(guess.c_str());
   int h = guess_font->Height();
   int x = our_win->Width()/2 - w/2 + caret*w/8;
   int y = our_win->Height()*3/4 - h/2;
   our_win->DrawFilledRectangle(x , y , w/8 , h , EagleColor(0,0,255));
   our_win->DrawTextString(guess_font , guess , our_win->Width()/2 , our_win->Height()*3/4 ,
                           EagleColor(255,255,255) , HALIGN_CENTER , VALIGN_CENTER);
///   our_win->DrawTextString(info_font , questions.Solution() , 10 , 10 , EagleColor(255,255,255));
   
   our_win->DrawMultiLineTextString(info_font , info , 10 , 10 , EagleColor(255,255,255) , info_font->Height()/2);
   
   if (msgtime > 0.0) {
      message_fader.SetAnimationPercent((duration - msgtime)/duration);
      message_fader.Draw(our_win , 0 , 0);
///      our_win->DrawTextString(guess_font , message , our_win->Width()/2 , our_win->Height()/4 , EagleColor(0,255,0) , HALIGN_CENTER , VALIGN_CENTER);
   }
   
   our_win->FlipDisplay();
}



int Game::HandleEvent(EagleEvent e) {
   int ret = 0;
   if (e.type == EAGLE_EVENT_DISPLAY_CLOSE || (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_ESCAPE)) {
      return -1;
   }
   if (e.type == EAGLE_EVENT_TIMER) {
      double oldmsgtime = msgtime;
      msgtime -= a5sys->GetSystemTimer()->SecondsPerTick();
      if (oldmsgtime > 0.0 && msgtime <= 0.0) {
         question_up = false;
         level_up = false;
         if (finish) {
            return -1;
         }
      }
      ret = 1;
   }
   if (e.type == EAGLE_EVENT_KEY_CHAR) {
      int ascii = e.keyboard.unicode;
      if (isalpha(ascii)) {ascii = tolower(ascii);}
      if (isdigit(ascii) || (ascii >= 'a' && ascii <= 'f')) {
         ///
         if (caret < 8) {
            input[caret++] = ascii;
            if (caret == 8) {caret = 8;}
         }
      }

      int kc = e.keyboard.keycode;
      if (kc == EAGLE_KEY_BACKSPACE) {
         if (caret > 0) {
            input[--caret] = ' ';
         }
      }
      else if (kc == EAGLE_KEY_DELETE) {
         input[caret] = ' ';
      }
      else if (kc == EAGLE_KEY_LEFT) {
         --caret;
         if (caret < 0) {caret = 0;}
      }
      else if (kc == EAGLE_KEY_RIGHT) {
         ++caret;
         if (caret > 7) {caret = 7;}
      }
      else if (kc == EAGLE_KEY_H) {
         if (questions.NextHint()) {
            hints = questions.GetHints();
            RebuildInfo();
         }
         else {
            SetMessage("No more hints!" , EagleColor(255,0,0));
         }
      }
      else if (kc == EAGLE_KEY_ENTER) {
         if (questions.Solution().compare(guess) == 0) {
            if (questions.NextQuestion()) {
               question_up = true;
               input = {' ',' ',' ',' ',' ',' ',' ',' '};
               caret = 0;
               SetMessage("Question Up!" , EagleColor(127,127,255));
               hints = questions.GetHints();
               RebuildInfo();
            }
            else if (questions.NextLevel()) {
               level_up = true;
               input = {' ',' ',' ',' ',' ',' ',' ',' '};
               caret = 0;
               SetMessage("Level Up!" , EagleColor(0,255,255));
               hints = questions.GetHints();
               RebuildInfo();
            }
            else {
               duration = 10;
               finish = true;
               SetMessage("You won!" , EagleColor(0,255,0));
            }
         }
         else {
            SetMessage("Incorrect!" , EagleColor(255,0,0));
         }
      }
      RefreshGuess();
   }
   return ret;
}




