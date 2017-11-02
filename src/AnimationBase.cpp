

#include "Eagle/AnimationBase.hpp"
#include "Eagle/Exception.hpp"



const int TOPIC_ANIMATION = NextFreeTopicId();


AnimationBase::AnimationBase() :
      EagleEventSource(),
      animation_percent(0.0),
      animation_time(0.0),
      total_duration(1.0),
      total_num_loops(1),
      animation_type(ANIMATION_ONCE_FORWARDS)
{
   
}



void AnimationBase::SetAnimationAttributes(double duration , EAGLE_ANIMATION_TYPE type ,  int num_loops) {
   EAGLE_ASSERT(duration != 0.0);
   total_duration = duration;
   animation_type = type;
   total_num_loops = num_loops;
   animation_time = 0.0;
   animation_percent = 0.0;
}



void AnimationBase::ResetAnimation() {
   SetAnimationTime(0.0);
}



void AnimationBase::SetAnimationTime(double t) {
   animation_time = t;
   SetAnimationPercent(animation_time / total_duration);
}



void AnimationBase::AdvanceAnimationTime(double dt) {
   SetAnimationTime(animation_time + dt);
}



void AnimationBase::SetAnimationPercent(double percent) {

   double old_animation_percent = animation_percent;
   animation_percent = percent;

   OnSetAnimationPercent();
   
   int delta_loops = (int)animation_percent - (int)old_animation_percent;
   
   for (int i = 0 ; i < delta_loops ; ++i) {
      OnLoopComplete();
   }
   if (animation_percent >= total_num_loops) {
      OnComplete();
   }
   
}



double AnimationBase::GetAnimationPercent() {
   return animation_percent;
}



double AnimationBase::GetAnimationTime() {
   return animation_time;
}



double AnimationBase::GetDuration() {
   return total_duration;
}



int AnimationBase::GetNumLoops() {
   return total_num_loops;
}



int AnimationBase::GetLoopNum() {
   return (int)animation_percent;
}



EAGLE_ANIMATION_TYPE AnimationBase::GetAnimationType() {
   return animation_type;
}




