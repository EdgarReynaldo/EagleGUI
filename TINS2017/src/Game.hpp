

#ifndef Game_HPP
#define Game_HPP

#include "LousyGlobals.hpp"

#include <vector>
#include <string>
#include <utility>

typedef std::vector<std::string> HINTLIST;

HINTLIST MakeHintList(int hcount , const char* hints[]);

typedef std::pair<std::string , HINTLIST> HINTPAIR;

typedef std::vector<HINTPAIR> HINTMAP;

HINTMAP LevelOne();
HINTMAP LevelTwo();
HINTMAP LevelThree();

typedef HINTMAP (*LEVEL_FUNC)();

extern const int NLEVELS;

class Questions {
   int level;
   int question;
   int hintnum;
   HINTMAP hintmap;
   HINTLIST hints;
   
   
public :
   
   bool SetLevel(int lnum);
   bool NextLevel();

   bool SetQuestion(int qnum);
   bool NextQuestion();
   
   bool SetHint(int hnum);
   bool NextHint();
   
   std::string Solution();
   HINTLIST GetHints();


   Questions();


};

#include "Eagle/Events.hpp"
#include "Animation.hpp"
#include "CombinationLock.hpp"


class EagleFont;
class EagleImage;

class Game {
   
   Questions questions;
   
   Safe safe;
   
   std::vector<char> input;
   int caret;
   
   std::string guess;
   std::string info;
   std::string message;
   HINTLIST hints;
   
   TextAnimation message_fader;
   
   bool question_up;
   bool level_up;
   bool finish;
   double duration;
   double msgtime;
   
   EagleFont* guess_font;
   EagleFont* info_font;
   EagleFont* pill_font;

   EagleImage* pill_image;

   void SetMessage(const char* msg , EagleColor color);

   void RebuildInfo();
   void RefreshGuess();

public :
   
   Game();
   
   bool Init();
   
   void Draw();
   int HandleEvent(EagleEvent e);
};

#endif // Game_HPP

