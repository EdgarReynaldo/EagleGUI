
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
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file AnimationBase.hpp
 * @brief Animation interface
 *
 * Header file for the AnimationBase class and related functions.
 */



#ifndef AnimationBase_HPP
#define AnimationBase_HPP

#include "Eagle/Events.hpp"

/*! @brief Animation attribute values
 *  
 *  EAGLE_ANIMATION_TYPE describes the playback attributes of an animation.
 */
enum EAGLE_ANIMATION_TYPE {
   ANIMATION_ONCE_FORWARDS                 = 0,
   ANIMATION_ONCE_BACKWARDS                = 1,
   ANIMATION_ONCE_FORWARDS_AND_BACKWARDS   = 2,
   ANIMATION_REPEAT_FORWARDS               = 3,
   ANIMATION_REPEAT_BACKWARDS              = 4,
   ANIMATION_REPEAT_FORWARDS_AND_BACKWARDS = 5
};



bool AnimationRepeats(EAGLE_ANIMATION_TYPE t);
bool AnimationForward(EAGLE_ANIMATION_TYPE t);
bool AnimationBackward(EAGLE_ANIMATION_TYPE t);
bool AnimationPingPong(EAGLE_ANIMATION_TYPE t);


/*! \class AnimationBase
 *  \brief This class provides functionality common to all animations.
 * 
 * The AnimationBase class makes animation easy. All you do is tell it the
 * number of loops, the duration of each loop, and the number of frames per loop
 * and then you're ready to animate. Call the AnimationBase::Init function to get setup and then
 * AnimationBase::AdvanceAnimationTime, AnimationBase::SetAnimationTime, or 
 * AnimationBase::SetAnimationPercent to alter the playback time.
 * 
 * When you need to find out the normalized animation percent or frame number, call
 * AnimationBase::GetFrameNum or AnimationBase::GetNormalizedPercent. The frame number 
 * is equivalent to :
 * 
 *     (int)(GetNormalizedPercent()*GetNumFrames())
 * 
 */

class AnimationBase : public EagleEventSource {
   
protected :
   double animation_percent;///< Floating point loop number
   double normal_percent;   ///< Normalized percentage of progress through the animation from [0,1.0)
   double animation_time;   ///< Current playback time of the animation
   double loop_duration;    ///< How long each loop is
   
   EAGLE_ANIMATION_TYPE animation_type;///< The type of animation

   int nloops;  ///< The number of loops to play
   int loop_num;///< The current loop number
   
   int nframes;  ///< The number of frames per loop
   int frame_num;///< The current frame number
   
///   EagleThread* our_thread;///< Currently unused



   ANIMATION_EVENT_DATA GetEventData();
      
   virtual void OnSetAnimationPercent()=0;///< Pure virtual function that is called when the animation percent changes.
   virtual void OnLoopComplete() {}///< Override this virtual function to respond to loop completion events.
   virtual void OnComplete() {}///< Override this virtual function to respond to animation complete events.

   void LoopComplete();///< This will be called for you when the loop is complete.
   void Complete();///< This will be called for you when the animation is complete.
   

public :
   
   AnimationBase();
   virtual ~AnimationBase();
   
   void Init(int num_frames , int num_loops , double loop_time , EAGLE_ANIMATION_TYPE play_type = ANIMATION_ONCE_FORWARDS);
   
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
   int    GetNumFrames();
   int    GetFrameNum();
   
   EAGLE_ANIMATION_TYPE GetAnimationType();

   bool Repeats()   {return AnimationRepeats(GetAnimationType());}
   bool Forwards()  {return AnimationForward(GetAnimationType());}
   bool Backwards() {return AnimationBackward(GetAnimationType());}
   bool PingPong()  {return AnimationPingPong(GetAnimationType());}
   
};




#endif // AnimationBase_HPP

