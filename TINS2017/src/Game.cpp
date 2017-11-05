


#include "Game.hpp"
#include "PillFont.hpp"
#include "Story.hpp"



#include "Eagle/backends/Allegro5Backend.hpp"
#include "Eagle.hpp"



Pos2D TopCenter(double percent) {
   (void)percent;
   return Pos2D(our_win->Width()/2 , our_win->Height()/4);
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
   hmap.push_back(HINTPAIR("00000000" , HINTLIST({"What did you see here? Nada, El Capitan!" , 
                                                  "All work and no play makes Dr. Pox a null boy!"})));
   hmap.push_back(HINTPAIR("ffffffff" , HINTLIST({"Don't be so negative" , "I^2"})));
   hmap.push_back(HINTPAIR("feeefeee" , HINTLIST({"Fi Fo Fum" , "I don't like paying these" , "Be free"})));
   return hmap;
}



HINTMAP LevelTwo() {
   HINTMAP hmap;
   hmap.push_back(HINTPAIR("abababab" , HINTLIST({"Why are there 4?" , "Killer abdomen!" , "Nice abs!"})));
   hmap.push_back(HINTPAIR("abcdabcd" , HINTLIST({"Alphabet soup!" , "Takes me back to grade school."})));
   hmap.push_back(HINTPAIR("f000f000" , HINTLIST({"Is that perfume?" , "-BAR!"})));
   return hmap;
}



HINTMAP LevelThree() {
   HINTMAP hmap; 
   hmap.push_back(HINTPAIR("baadf00d" , HINTLIST({"I think I ate something raunchy..."})));
   hmap.push_back(HINTPAIR("deadbeef" , HINTLIST({"Moo!" , "Cow!"})));
   hmap.push_back(HINTPAIR("1 babb1e" , HINTLIST({"I do not!" , "No, you babble!" , "There's only one!"})));
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



double Game::GetTimeLeft() {
   double dt = (double)ProgramTime::Now() - time_start;
   return stop_time_counter - dt;
}



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
      safe(),
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
      info_font(0),
      pill_font(0),
      pill_image(0),
      time_start(0.0),
      stop_time_counter(0.0),
      time_left(0.0),
      finish_time(-1.0),
      yay(0),
      oops(0),
      applause(0)
{
   duration = 3;

   stop_time_counter = 24.0*60;/// 24 hours (minutes) to cure his chicken pox!
///   stop_time_counter = 16;

///   time_counter_rate = 60;/// One minute per second, that gives you 24*60 seconds, or 1440, or 24 minutes to solve the game
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
///      info_font = our_win->LoadFont("Verdana.ttf" , -36);
      info_font = our_win->LoadFont("Akashi.ttf" , -28);
   }
//   if (!pill_image) {
//      pill_image = CreatePillFontImage(64,64);
//      EAGLE_ASSERT(pill_image->Save("PillImage" , "png"));
//   }
   if (!pill_font) {
      pill_font = CreatePillFont(64,64);
      EAGLE_ASSERT(pill_font && pill_font->Valid());
   }
   
   if (!yay) {
      yay = a5soundman->CreateSound("Yay.ogg");
   }
   if (!oops) {
      oops = a5soundman->CreateSound("Oops.ogg");
   }
   if (!applause) {
      applause = a5soundman->CreateSound("Applause3.ogg");
   }
   
   SetMessage("Start!!!" , EagleColor(0,255,0));


   time_start = ProgramTime::Now();
   
   
   
   return guess_font && info_font;
}



void Game::Draw() {
   
   our_win->DrawToBackBuffer();
   our_win->Clear(EagleColor(0,0,0));

   int gx = our_win->Width()/2;
   int gy = our_win->Height()/2;
   
   /// x,y,w,h are text dimensions of guess
   int w = guess_font->Width(guess.c_str());
   int h = guess_font->Height();
   int x = gx - w/2 + caret*w/8;
   int y = gy - h/2;
   
   /// cursor
   our_win->DrawFilledRectangle(x , y , w/8 , h , EagleColor(0,0,255));

   /// current guess
   our_win->DrawTextString(guess_font , std::string("0x").append(guess) , gx + w/2 , gy ,
                           EagleColor(255,255,255) , HALIGN_RIGHT , VALIGN_CENTER);

   /// solution in pill encoding
   our_win->DrawTextString(pill_font , safe.GetPillEncoding(questions.Solution()) , gx , gy + our_win->Height()/8 ,
                           EagleColor(255,255,255) , HALIGN_CENTER , VALIGN_CENTER);

   /// hints?
   our_win->DrawMultiLineTextString(info_font , info , 10 , our_win->Height() - 10 , EagleColor(255,255,255) , info_font->Height()/4 , HALIGN_LEFT , VALIGN_BOTTOM);
   
   /// message overlay
   if (msgtime > 0.0) {
      message_fader.SetAnimationPercent((duration - msgtime)/duration);
      message_fader.Draw(our_win , 0 , 0);
///      our_win->DrawTextString(guess_font , message , our_win->Width()/2 , our_win->Height()/4 , EagleColor(0,255,0) , HALIGN_CENTER , VALIGN_CENTER);
   }

   /// time left
   double t = time_left;
   double pct = time_left/stop_time_counter;
   int m = t/60.0;
   int s = (t - 60.0*m);
   if (t <= 0.0) {
      m = 0;
      s = 0;
   }
   string tstr = StringPrintF("%02dH:%02dM left!" , m , s);
   EagleColor tcolor(0,255,0,255);
   if (pct < 0.25) {
      tcolor = EagleColor(255,0,0,255);
   }
   else if (pct < 0.5) {
      tcolor = EagleColor(255,127,0,255);
   }
   our_win->DrawTextString(info_font , tstr , our_win->Width() - 10 , 10 , tcolor , HALIGN_RIGHT , VALIGN_TOP);
   
   /// display
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
      double old_time_left = time_left;
      time_left = GetTimeLeft();
      if (old_time_left >= 0.0 && time_left < 0.0) {
         SetMessage("YOU FAILED!" , EagleColor(255,0,0));
         finish = true;
      }
      ret = 1;
   }
   if (e.type == EAGLE_EVENT_KEY_CHAR) {
      int ascii = e.keyboard.unicode;
      if (isalpha(ascii)) {ascii = tolower(ascii);}
      if (isdigit(ascii) || (ascii >= 'a' && ascii <= 'f') || (ascii == ' ')) {
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
      else if (kc == EAGLE_KEY_S) {
         PlayStory();
      }
      else if (kc == EAGLE_KEY_ENTER) {
         if (questions.Solution().compare(guess) == 0) {
            if (questions.NextQuestion()) {
               question_up = true;
               input = {' ',' ',' ',' ',' ',' ',' ',' '};
               caret = 0;
               SetMessage("Question Up!" , EagleColor(127,127,255));
               yay->Play();
               hints = questions.GetHints();
               RebuildInfo();
            }
            else if (questions.NextLevel()) {
               level_up = true;
               input = {' ',' ',' ',' ',' ',' ',' ',' '};
               caret = 0;
               SetMessage("Level Up!" , EagleColor(0,255,255));
               yay->Play();
               hints = questions.GetHints();
               RebuildInfo();
            }
            else {
               finish_time = GetTimeLeft();
               finish = true;
               duration = 10;
               applause->Play();
               SetMessage("You won!" , EagleColor(0,255,0));
            }
         }
         else {
            SetMessage("Incorrect!" , EagleColor(255,0,0));
            oops->Play();
         }
      }
      RefreshGuess();
   }
   return ret;
}




