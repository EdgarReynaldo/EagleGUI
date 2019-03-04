

#include "Eagle/AnimationBase.hpp"
#include "Eagle/Exception.hpp"

#include <cmath>



bool AnimationRepeats(EAGLE_ANIMATION_TYPE t) {
   return t >= ANIMATION_REPEAT_FORWARDS;
}



bool AnimationForward(EAGLE_ANIMATION_TYPE t) {
   return ((t == ANIMATION_ONCE_FORWARDS) || (t == ANIMATION_REPEAT_FORWARDS));
}



bool AnimationBackward(EAGLE_ANIMATION_TYPE t) {
   return ((t == ANIMATION_ONCE_BACKWARDS) || (t == ANIMATION_REPEAT_BACKWARDS));
}



bool AnimationPingPong(EAGLE_ANIMATION_TYPE t) {
   return ((t == ANIMATION_ONCE_FORWARDS_AND_BACKWARDS) || (t == ANIMATION_REPEAT_FORWARDS_AND_BACKWARDS));
}



ANIMATION_EVENT_DATA AnimationBase::GetEventData() {
   ANIMATION_EVENT_DATA d;
   d.source = this;
   d.loops_complete = (int)animation_percent;
   d.animation_complete = (d.loops_complete >= nloops);
   return d;
}



void AnimationBase::LoopComplete() {
   OnLoopComplete();
   EagleEvent e;
   e.type = EAGLE_EVENT_ANIMATION_LOOP_COMPLETE;
   e.source = this;
   e.window = 0;
   e.animation = GetEventData();
   EmitEvent(e , our_thread);
}



void AnimationBase::Complete() {
   OnComplete();
   EagleEvent e;
   e.type = EAGLE_EVENT_ANIMATION_COMPLETE;
   e.source = this;
   e.window = 0;
   e.animation = GetEventData();
   EmitEvent(e , our_thread);
}



AnimationBase::AnimationBase() :
      EagleEventSource(),
      animation_percent(0.0),
      normal_percent(0.0),
      animation_time(0.0),
      loop_duration(1.0),
      animation_type(ANIMATION_ONCE_FORWARDS),
      nloops(1),
      loop_num(0),
      nframes(1),
      frame_num(0),
      our_thread(0)
{
   
}



void AnimationBase::Init(double loop_time , int num_loops , EAGLE_ANIMATION_TYPE play_type , int num_frames) {
   if (loop_time <= 0.0) {loop_time = 1.0;}
   if (num_loops < 1) {num_loops = 1;}
   if (num_frames < 1) {num_frames = 1;}
   animation_type = play_type;
   loop_duration = loop_time;
   nloops = num_loops;
   nframes = num_frames;
   ResetAnimation();
}



void AnimationBase::ResetAnimation() {
   SetAnimationTime(0.0);
}



void AnimationBase::SetAnimationTime(double t) {
   animation_time = t;
   SetAnimationPercent(animation_time / loop_duration);
}



void AnimationBase::AdvanceAnimationTime(double dt) {
   SetAnimationTime(animation_time + dt);
}



void AnimationBase::SetAnimationPercent(double percent) {

   double old_animation_percent = animation_percent;
   animation_percent = percent;
   normal_percent = GetNormalizedPercent();

   int old_loop_num = (int)old_animation_percent;
   loop_num = (int)animation_percent;
   frame_num = (int)(nframes*(normal_percent));

   OnSetAnimationPercent();
   
   for (int i = old_loop_num ; i < loop_num ; ++i) {
      if (PingPong()) {
         if (i % 2 == 0) {
            LoopComplete();
         }
      }
      else {
         LoopComplete();
      }
   }
   if (animation_percent >= nloops && old_animation_percent < nloops) {
      Complete();
   }
}



double AnimationBase::GetAnimationPercent() {
   return animation_percent;
}



double AnimationBase::GetNormalizedPercent() {
   double pct = GetAnimationPercent();
   bool forward = true;
   if (PingPong()) {
      pct = fmod(pct , 2.0);
      if (pct >= 1.0) {
         pct -= 1.0;
         forward = false;
      }
   }
   else if (Backwards()) {
      forward = false;
   }
   if (!forward) {
      return 1.0 - pct;
   }
   return pct;
}



double AnimationBase::GetAnimationTime() {
   return animation_time;
}



double AnimationBase::GetDuration() {
   return loop_duration;
}



int AnimationBase::GetNumLoops() {
   return nloops;
}



int AnimationBase::GetLoopNum() {
   return (int)animation_percent;
}



EAGLE_ANIMATION_TYPE AnimationBase::GetAnimationType() {
   return animation_type;
}




