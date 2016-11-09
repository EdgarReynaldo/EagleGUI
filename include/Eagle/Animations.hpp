



#ifndef Animations_HPP
#define Animations_HPP

#include "Eagle/Gui/WidgetMessage.hpp"


enum EAGLE_ANIMATION_TYPE {
   ANIMATION_ONCE_FORWARDS = 0,
   ANIMATION_ONCE_BACKWARDS = 1,
   ANIMATION_ONCE_FORWARDS_AND_BACKWARDS = 2,
   ANIMATION_REPEAT_FORWARDS = 3,
   ANIMATION_REPEAT_BACKWARDS = 4,
   ANIMATION_REPEAT_FORWARDS_AND_BACKWARDS = 5
};

extern const int TOPIC_ANIMATION;

enum ANIMATION_MSGS {
   ANIMATION_LOOP_COMPLETE = 0,
   ANIMATION_COMPLETE = 1
};

REGISTER_WIDGET_MESSAGE(TOPIC_ANIMATION , ANIMATION_LOOP_COMPLETE);
REGISTER_WIDGET_MESSAGE(TOPIC_ANIMATION , ANIMATION_COMPLETE);


class AnimationBase {
   
protected :
   double animation_percent;
   double animation_time;
   double total_duration;
   int total_num_loops;
   EAGLE_ANIMATION_TYPE animation_type;
   
   
   void SetAnimationPercent(double percent);

   
   virtual void OnSetAnimationPercent()=0;

   virtual void OnLoopComplete()=0;
   virtual void OnComplete()=0;

public :
   
   AnimationBase();
   virtual ~AnimationBase() {}
   
   
   void SetAnimationAttributes(double duration , EAGLE_ANIMATION_TYPE type ,  int num_loops = 1);
   
   void AdvanceAnimationTime(double dt);
   void SetAnimationTime(double t);
   void Reset();

   double GetAnimationPercent();
   double GetAnimationTime();
   double GetDuration();
   int    GetNumLoops();
   int    GetLoopNum();
   EAGLE_ANIMATION_TYPE GetAnimationType();

};




#endif // Animations_HPP

