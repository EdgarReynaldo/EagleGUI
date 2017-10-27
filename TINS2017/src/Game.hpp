

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
   
   std::string Solution();
   HINTLIST GetHints();


   Questions();


};

#include "Eagle/Events.hpp"

class EagleFont;

class Game {
   
   Questions questions;
   
   std::string guess;
   std::string info;
   HINTLIST hints;
   
   EagleFont* guess_font;
   EagleFont* info_font;
   
public :
   
   Game();
   
   bool Init();
   
   void Draw();
   int HandleEvent(EagleEvent e);
};

#endif // Game_HPP

