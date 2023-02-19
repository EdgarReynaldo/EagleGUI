
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file AnimationBase.cpp
 * @brief Animation implementation
 * 
 * Implementation file for the AnimationBase class and related functions.
 */


#include "Eagle/AnimationBase.hpp"

#include <cmath>


/*! \brief Returns whether or not the animation type repeats. */

bool AnimationRepeats(EAGLE_ANIMATION_TYPE t) {
   return t >= ANIMATION_REPEAT_FORWARDS;
}



/*! \brief Returns whether or not the animation type plays forwards. */

bool AnimationForward(EAGLE_ANIMATION_TYPE t) {
   return ((t == ANIMATION_ONCE_FORWARDS) || (t == ANIMATION_REPEAT_FORWARDS));
}



/*! \brief Returns whether or not the animation type plays backwards. */

bool AnimationBackward(EAGLE_ANIMATION_TYPE t) {
   return ((t == ANIMATION_ONCE_BACKWARDS) || (t == ANIMATION_REPEAT_BACKWARDS));
}



/*! \brief Returns whether or not the animation type plays forwards and backwards. */

bool AnimationPingPong(EAGLE_ANIMATION_TYPE t) {
   return ((t == ANIMATION_ONCE_FORWARDS_AND_BACKWARDS) || (t == ANIMATION_REPEAT_FORWARDS_AND_BACKWARDS));
}




/*! \brief Fills in the ANIMATION_EVENT_DATA structure with appropriate values. */

ANIMATION_EVENT_DATA AnimationBase::GetEventData() {
   ANIMATION_EVENT_DATA d;
   d.source = this;
   d.loops_complete = (int)animation_percent;
   d.animation_complete = (d.loops_complete >= nloops);
   return d;
}


/*! 
 *  Called when a loop is complete.
 *  Also calls the protected virtual callback AnimationBase::OnLoopComplete().
 */

void AnimationBase::LoopComplete() {
   OnLoopComplete();
   EagleEvent e;
   e.type = EAGLE_EVENT_ANIMATION_LOOP_COMPLETE;
   e.source = this;
   e.window = 0;
   e.animation = GetEventData();
   EmitEvent(e , 0);/// TODO : thread???
}



/*!
 *  Called when the animationis complete.
 *  Also calls the protected virtual callback AnimationBase::OnComplete().
 */

void AnimationBase::Complete() {
   OnComplete();
   EagleEvent e;
   e.type = EAGLE_EVENT_ANIMATION_COMPLETE;
   e.source = this;
   e.window = 0;
   e.animation = GetEventData();
   EmitEvent(e , 0);/// TODO : thread?
}


/*! \brief Sets up an empty animation object.
 *  
 *  Call AnimationBase::Init(int,int,double,EAGLE_ANIMATION_TYPE) to initialize the object with data.
 *  
 */

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
      frame_num(0)
{
   
}


/*! \brief Empty destructor.
 * 
 */

AnimationBase::~AnimationBase() 
{
   
}



/*! \brief Init sets up the animation's required information.
 *  
 *  @param num_frames The number of frames per loop.
 *  @param num_loops The number of times to loop.
 *  @param loop_time The duration of each loop.
 *  @param play_type What kind of animation to setup.
 *  
 *  The loop time is always positive, the number of loops is
 *  always at least 1 and the number of frames is always at
 *  least 1.
 *  
 *  @retval None The function is always successful.
 *  
 */

void AnimationBase::Init(int num_frames , int num_loops , double loop_time , EAGLE_ANIMATION_TYPE play_type) {
   if (loop_time <= 0.0) {loop_time = 1.0;}
   if (num_loops < 1) {num_loops = 1;}
   if (num_frames < 1) {num_frames = 1;}
   animation_type = play_type;
   loop_duration = loop_time;
   nloops = num_loops;
   nframes = num_frames;
   ResetAnimation();
}


/*! \brief Resets the animation. Shortcut for `SetAnimationTime(0.0);`. 
 * 
 * See @ref SetAnimationTime
 */

void AnimationBase::ResetAnimation() {
   SetAnimationTime(0.0);
}



/*! \brief Sets the animation time which directly corresponds to a number of seconds. */

void AnimationBase::SetAnimationTime(double t) {
   animation_time = t;
   SetAnimationPercent(animation_time / loop_duration);
}



/*! \brief Advances the animation time by dt.
 *  @param dt The delta time to advance the animation by.
 *  
 *  The delta time may be negative to 'rewind'.
 */

void AnimationBase::AdvanceAnimationTime(double dt) {
   SetAnimationTime(animation_time + dt);
}



/*! \brief Sets the elapsed percentage of the animation.
 *
 * @param percent Equal to animation time divided by loop duration.
 *  
 *  AnimationBase::LoopComplete will be called for each loop completed
 *  when advancing the animation time.
 *  AnimationBase::Complete will be called when the animation is complete.
 *  
 *  Each function will emit an event. Use an EagleEventQueue or
 *  EagleEventListener to listen for events from this object.
 *  
 *      event_queue.ListenTo(&animation_object);
 *  
 *  @retval None This function cannot fail.
 */

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
      if (!Repeats()) {
         Complete();
      }
   }
}



void AnimationBase::AdvanceFrame() {
   EAGLE_ASSERT(nframes > 1);
   if (PingPong() && (loop_num % 2 == 1)) {// ping pong and loop num is odd, going backward
      --frame_num;
      if (frame_num < 0) {
         LoopComplete();
         ++loop_num;
         if (loop_num == nloops) {
            Complete();
         }
      }
   }
   else {// not ping pong or loop num is even, advancing forward
      ++frame_num;
      if (frame_num == nframes) {
         if (PingPong() && loop_num % 2 == 0) {
            ++loop_num;
            LoopComplete();
         }
         else if (!PingPong()) {
            ++loop_num;
            LoopComplete();
         }
         if (loop_num == nloops) {
            Complete();
         }
      }
   }
   int fnum = frame_num;
   if (fnum < 0) {
      fnum = 0;
   }
   if (fnum >= nframes) {
      fnum = nframes - 1;
   }
   animation_percent = (double)loop_num + (double)fnum/(nframes - 1);
   animation_time = (loop_num*loop_duration) + (double)fnum/(nframes - 1)*loop_duration;
   normal_percent = animation_time / (double)(nloops*loop_duration);
}



/*! \brief Returns the animation percentage. */

double AnimationBase::GetAnimationPercent() {
   return animation_percent;
}



/*! \brief Returns the normalized animation percent from [0.0 to 1.0). */

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
   else {
      pct = fmod(pct , 1.0);
   }
   if (!forward) {
      return 1.0 - pct;
   }
   return pct;
}



/*! \brief Returns the animation time divided by the total duration.
 *  
 */

double AnimationBase::GetTotalPercent() {
   return animation_time / (nloops*loop_duration);
}



/*! \brief Returns the animation time in loops.
 *  
 */

double AnimationBase::GetAnimationTime() {
   return animation_time;
}



/*! \brief Returns the duration of each loop in seconds.
 *  
 */

double AnimationBase::GetDuration() {
   return loop_duration;
}



/*! \brief Returns the number of loops specified in AnimationBase::Init(int,int,double,EAGLE_ANIMATION_TYPE).
 *  
 */

int AnimationBase::GetNumLoops() {
   return nloops;
}



/*! \brief Returns the current loop number.
 *  
 */

int AnimationBase::GetLoopNum() {
   return (int)animation_percent;
}



/*! \brief Returns the number of frames specified in AnimationBase::Init(int,int,double,EAGLE_ANIMATION_TYPE).
 *  
 */

int AnimationBase::GetNumFrames() {
   return nframes;
}



/*! \brief Returns the current frame number.
 *  
 */

int AnimationBase::GetFrameNum() {
   return frame_num;
}



/*! \brief Returns the current animation type specified in AnimationBase::Init(int,int,double,EAGLE_ANIMATION_TYPE)
 *  
 */

EAGLE_ANIMATION_TYPE AnimationBase::GetAnimationType() {
   return animation_type;
}




