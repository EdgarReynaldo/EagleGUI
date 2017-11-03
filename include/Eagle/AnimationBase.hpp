



#ifndef Animations_HPP
#define Animations_HPP

#include "Eagle/Gui/WidgetMessage.hpp"
#include "Eagle/Events.hpp"



enum EAGLE_ANIMATION_TYPE {
   ANIMATION_ONCE_FORWARDS = 0,
   ANIMATION_ONCE_BACKWARDS = 1,
   ANIMATION_ONCE_FORWARDS_AND_BACKWARDS = 2,
   ANIMATION_REPEAT_FORWARDS = 3,
   ANIMATION_REPEAT_BACKWARDS = 4,
   ANIMATION_REPEAT_FORWARDS_AND_BACKWARDS = 5
};



bool AnimationRepeats(EAGLE_ANIMATION_TYPE t);
bool AnimationForward(EAGLE_ANIMATION_TYPE t);
bool AnimationBackward(EAGLE_ANIMATION_TYPE t);
bool AnimationPingPong(EAGLE_ANIMATION_TYPE t);



class AnimationBase : public EagleEventSource {
   
protected :
   double animation_percent;
   double normal_percent;
   double animation_time;
   double loop_duration;
   
   EAGLE_ANIMATION_TYPE animation_type;

   int nloops;
   int loop_num;
   
   int nframes;
   int frame_num;
   
   EagleThread* our_thread;



   ANIMATION_EVENT_DATA GetEventData();
      
   virtual void OnSetAnimationPercent()=0;
   virtual void OnLoopComplete() {}
   virtual void OnComplete() {}

   void LoopComplete();
   void Complete();
   

public :
   
   AnimationBase();
   virtual ~AnimationBase() {}
   
   void Init(double loop_time , int num_loops = 1 , EAGLE_ANIMATION_TYPE play_type = ANIMATION_REPEAT_FORWARDS , int num_frames = 1);
   
   void ResetAnimation();

   void SetAnimationTime(double t);
   void AdvanceAnimationTime(double dt);

   void SetAnimationPercent(double percent);
   

   double GetAnimationPercent();
   double GetNormalizedPercent();/// [0.0 , 1.0)
   double GetAnimationTime();
   double GetDuration();
   int    GetNumLoops();
   int    GetLoopNum();
   
   EAGLE_ANIMATION_TYPE GetAnimationType();

   bool Repeats() {return AnimationRepeats(GetAnimationType());}
   bool Forwards() {return AnimationForward(GetAnimationType());}
   bool Backwards() {return AnimationBackward(GetAnimationType());}
   bool PingPong() {return AnimationPingPong(GetAnimationType());}
   
};




#endif // Animations_HPP

