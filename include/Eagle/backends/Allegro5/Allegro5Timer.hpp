
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
 * @file Allegro5Timer.hpp
 * @brief The interface for timers using Allegro 5
 */

#ifndef Allegro5Timer_HPP
#define Allegro5Timer_HPP



#include "Eagle/Threads.hpp"
#include "Eagle/Timer.hpp"

#include "allegro5/allegro.h"



/// TODO Rename to Allegro5TimerProcess
void* Allegro5TimerProcess(EagleThread* ethread , void* );



class Allegro5Timer : public EagleTimer {

private :
   ALLEGRO_TIMER* timer;
   
   ALLEGRO_EVENT_QUEUE* timer_queue;
   ALLEGRO_EVENT_QUEUE* process_queue;
   
///   ALLEGRO_MUTEX* queue_lock;
   ALLEGRO_EVENT_SOURCE timer_event_source;
   ALLEGRO_EVENT_SOURCE process_event_source;
   
   EagleThread* ethread;
   


   friend void* Allegro5TimerProcess(EagleThread* ethread , void* etimer);
   
   ALLEGRO_TIMER* AllegroTimer() {return timer;}
   ALLEGRO_EVENT_QUEUE* AllegroEventQueue() {return process_queue;}
   ALLEGRO_EVENT_SOURCE* AllegroEventSource() {return &process_event_source;}
   
   void SendTimerProcessMessage(int message);

public :
   Allegro5Timer(std::string objname = "Nemo");///< Default constructor takes a name
   ~Allegro5Timer();///< Destructor
   
   virtual bool Create(double seconds_per_tick);///< Create a new timer at the specified rate
   virtual void Destroy();///< Destroy this timer
   virtual void Start();///< Start our timer
   virtual void Stop();///< Stop our timer
   virtual void Close();///< Close the timer (will be called on @ref Destroy )

   virtual void WaitForTick(EagleThread* thread);///< Yield until we get a tick (CPU FREE)
   
   virtual void* Source();///< The raw source of the timer

   void RefreshTimer(EagleThread* thread);///< Called automatically
   
   bool Valid();///< True if we are valid and ready
};




#endif // Allegro5Timer_HPP



