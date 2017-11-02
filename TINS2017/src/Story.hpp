



#ifndef Story_HPP
#define Story_HPP


#include "Eagle/AnimationBase.hpp"
#include "Eagle/Events.hpp"



std::string IntroStory();



class Story : public AnimationBase {
   
   std::string story;
   EagleFont* story_font;
   
   bool key_up;
   bool key_down;
   
   Pos2D pos;
   
   
   
   Pos2D GetPos(double percent);

protected :
   virtual void OnSetAnimationPercent();
   virtual void OnLoopComplete();
   virtual void OnComplete();
void OnSetAnimationPercent() }
   pos = GetPos(GetAnimationPercent());
}
void OnLoopComplete() {
   
}
void OnComplete() {
   
}

public :
   virtual void Draw(EagleGraphicsContext* win , int x , int y);
   int HandleEvent(EagleEvent e);
   void Play();

};

#endif // Story_HPP
