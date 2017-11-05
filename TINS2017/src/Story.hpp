



#ifndef Story_HPP
#define Story_HPP


#include "Eagle/AnimationBase.hpp"
#include "Eagle/Events.hpp"
#include "Eagle/Position.hpp"

std::string IntroStory();

void PlayStory();



class EagleFont;

class Story : public AnimationBase {
   
   std::string story;
   EagleFont* story_font;
   
   bool key_up;
   bool key_down;
   
   Pos2D pos;
   
   bool complete;
   double resting;
   
   Pos2D GetPos(double percent);

protected :
   void OnSetAnimationPercent();

   void OnComplete();

public :
   Story(std::string story_str , EagleFont* font);

   virtual void Draw(EagleGraphicsContext* win , int x , int y);

   int HandleEvent(EagleEvent e);

   void Play();

};

#endif // Story_HPP
