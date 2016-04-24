
/*
 *
 *     _______       ___       ____      __       _______
 *    /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *    \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *     \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *      \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *       \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *        \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    EAGLE
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2013+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#ifndef Allegro5Timer_HPP
#define Allegro5Timer_HPP



#include "Eagle.hpp"

#include "allegro5/allegro.h"


/// TODO Rename to Allegro5TimerProcess
void* TimerProcess(EagleThread* ethread , void* );



class Allegro5Timer : public EagleTimer {

private :


   ALLEGRO_TIMER* timer;
   
   ALLEGRO_EVENT_QUEUE* timer_queue;
   ALLEGRO_EVENT_QUEUE* process_queue;
   
///   ALLEGRO_MUTEX* queue_lock;
   ALLEGRO_EVENT_SOURCE timer_event_source;
   ALLEGRO_EVENT_SOURCE process_event_source;
   
   EagleThread* ethread;
   
   friend void* TimerProcess(EagleThread* ethread , void* etimer);
   void TickRelay(double timestamp) {Tick(timestamp);}
   
   ALLEGRO_TIMER* AllegroTimer() {return timer;}
   ALLEGRO_EVENT_QUEUE* AllegroEventQueue() {return process_queue;}
   ALLEGRO_EVENT_SOURCE* AllegroEventSource() {return &process_event_source;}
   
   void SendTimerProcessMessage(int message);

protected :
   

public :
   Allegro5Timer();
   ~Allegro5Timer();
   
///   void ReadEvents();
   
   
   virtual bool Create(double seconds_per_tick);
   virtual void Destroy();
   virtual void Start();
   virtual void Stop();
   virtual void Close();
   virtual void WaitForTick();
   
   virtual void* Source();

   void RefreshTimer();
   
   bool Valid();
   
   void RegisterTimerInput(EagleEventHandler* eagle_handler);
   
   long long int Count();
};


#endif // Allegro5Timer_HPP

