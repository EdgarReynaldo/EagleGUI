
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
 * @file Timer.hpp
 * @brief The interface for timer routines in Eagle
 */

#ifndef EagleTimer_HPP
#define EagleTimer_HPP



#include "Eagle/Events.hpp"
#include "Eagle/Object.hpp"



class EagleThread;



/**! @class EagleTimer
 *   @brief The base class used to represent a tickable timer
 */

class EagleTimer : public EagleObject , public EagleEventSource {

private :
   int id;/// unique id

protected :
   double spt;///< Seconds per tick
   unsigned long long previous_ticks;///< The previous count of timer ticks
   unsigned long long current_ticks;///< The current count of timer ticks
   bool running;///< True if running
   
   /**! @fn NextId
    *   @brief Returns the next id starting at zero
    */
   static int NextId() {
      static int i = 0;
      return i++;
   }
   
   void Tick(double timestamp , EagleThread* thread);///< Our 'ticker' callback
   
public :

   EagleTimer(std::string objclass = "EagleTimer" , std::string objname = "Nemo");///< Default constructor with class and object name
   virtual ~EagleTimer() {}///< Virtual destructor

   /// EagleEventSource
   void ReadEvents() {RefreshTimer(0);}///< Not normally needed

   virtual bool Create(double seconds_per_tick)=0;///< Create this timer with the specified rate @retval True on success
   virtual void Destroy()=0;///< Destroy this timer
   virtual void Start()=0;///< Start this timer
   virtual void Stop()=0;///< Stop this timer
   virtual void Close()=0;///< Shutdown the timer
   
   virtual void WaitForTick(EagleThread* thread)=0;///< Wait for this timer to tick
   bool SetSecondsPerTick(double secs) {return Create(secs);}///< Reset the timer rate @retval True on success
   
   
   int ID() {return id;}///< Get the id of this timer
   double SPT() {return spt;}///< Return the tick rate in seconds per tick
   double SecondsPerTick() {return spt;}///< Same as @ref SPT
   unsigned long long Count();///< Timer count
   
   virtual void* Source()=0;///< The raw timer source

   virtual void RefreshTimer(EagleThread* thread)=0;///< Refresh the timer (not needed)
   
   /// time passed since the last TakeAllTime was called


   unsigned long long TicksPassed();///< Number of ticks passed since last call to @ref TakeAllTicks
   double TimePassed();///< Amount of time passed since last call to @ref TakeAllTime
   int    TakeAllTicks();///< Take all the accrued ticks
   double TakeAllTime();///< Take all the accrued time
   
   void ClearTicks() {(void)TakeAllTicks();}///< Clear the tick count
   
   virtual bool Valid()=0;///< Pure virtual function to check object validity @retval true if valid and ready
   virtual bool Running() {return running;}///< @retval true if running
   
///   virtual void RegisterTimerInput(EagleEventHandler* queue)=0;///< Register a timer's output with a queue
};




#endif // EagleTimer_HPP



