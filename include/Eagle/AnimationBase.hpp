
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
 * @file AnimationBase.hpp
 * @brief Animation interface
 *
 * Header file for the AnimationBase class and related functions.
 */



#ifndef AnimationBase_HPP
#define AnimationBase_HPP

#include "Eagle/Events.hpp"

/*! @enum EAGLE_ANIMATION_TYPE
 *  @brief Animation attribute values
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



bool AnimationRepeats(EAGLE_ANIMATION_TYPE t);///< True if the animation type repeats
bool AnimationForward(EAGLE_ANIMATION_TYPE t);///< True if the animation type plays forward
bool AnimationBackward(EAGLE_ANIMATION_TYPE t);///< True if the animation type plays backward
bool AnimationPingPong(EAGLE_ANIMATION_TYPE t);///< True if the animation type plays back and forth


/*! \class AnimationBase
 *  \brief This class provides functionality common to all animations.
 * 
 * The AnimationBase class makes animation easy. All you do is tell it the
 * number of loops, the duration of each loop, and the number of frames per loop
 * and then you're ready to animate. Call the @ref AnimationBase::Init function to get setup and then
 * @ref AnimationBase::AdvanceAnimationTime, @ref AnimationBase::SetAnimationTime, or 
 * @ref AnimationBase::SetAnimationPercent to alter the playback time.
 * 
 * When you need to find out the normalized animation percent or frame number, call
 * @ref AnimationBase::GetFrameNum or @ref AnimationBase::GetNormalizedPercent. The frame number 
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
      
   virtual void OnSetAnimationPercent(){};///< Virtual function that is called when the animation percent changes.
   virtual void OnLoopComplete() {}///< Override this virtual function to respond to loop completion events.
   virtual void OnComplete() {}///< Override this virtual function to respond to animation complete events.

   void LoopComplete();///< This will be called for you when the loop is complete.
   void Complete();///< This will be called for you when the animation is complete.
   

public :
   
   AnimationBase();///< Base constructor
   
   AnimationBase(const AnimationBase& ab);///< Copy constructor performs deep copy
   
   virtual ~AnimationBase();///< Nothing to destroy, virtual for descendants use
   
   AnimationBase& operator=(const AnimationBase& a);///< Assignment operator, deep copy
   
   void Init(int num_frames , int num_loops , double loop_time , EAGLE_ANIMATION_TYPE play_type = ANIMATION_ONCE_FORWARDS);///< Basic initialization with # frames,loops, loop time and animation type
   
   void ResetAnimation();///< Resets the animation playback to the beginning

   void SetAnimationTime(double t);///< Lets you set the playback time of the animation
   void AdvanceAnimationTime(double dt);///< Advances or retreats the animation time by dt

   void SetAnimationPercent(double percent);///< Sets the percentage of completion [0.0,1.0] or higher
   
   void AdvanceFrame();

   double GetAnimationPercent();///< Gets the percent of playback time passed, may be higher than one for looping animations
   double GetNormalizedPercent();///< Gets the normalized percentage of the playback time from [0.0 , 1.0)
   double GetTotalPercent();///< Gets the total percent of time played over duration (from [0.0 , 1.0]);
   double GetAnimationTime();///< Returns how many seconds the animation has been playing for (the time index)
   double GetDuration();///< Returns the duration in seconds of a single loop
   int    GetNumLoops();///< Returns the number of loops specified
   int    GetLoopNum();///< Returns the current loop number
   int    GetNumFrames();///< Returns the number of frames per loop
   int    GetFrameNum();///< Returns the current frame number
   
   EAGLE_ANIMATION_TYPE GetAnimationType();///< Returns the type of animation

   bool Repeats()   {return AnimationRepeats(GetAnimationType());}///< Whether this animation repeats
   bool Forwards()  {return AnimationForward(GetAnimationType());}///< Whether this animation plays forward
   bool Backwards() {return AnimationBackward(GetAnimationType());}///< Whether this animation plays backward
   bool PingPong()  {return AnimationPingPong(GetAnimationType());}///< Whether this animation plays back and forth
   
};




#endif // AnimationBase_HPP





