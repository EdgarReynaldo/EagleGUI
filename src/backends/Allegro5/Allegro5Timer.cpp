


#include "Eagle/StringWork.hpp"

#include "Eagle/backends/Allegro5/Allegro5EventHandler.hpp"
#include "Eagle/backends/Allegro5/Allegro5Timer.hpp"
#include "Eagle/backends/Allegro5/Allegro5Threads.hpp"




enum EAGLE_TIMER_MESSAGE_TYPE {
   EAGLE_MESSAGE_CREATE_TIMER = 0,
   EAGLE_MESSAGE_START_TIMER = 1,
   EAGLE_MESSAGE_STOP_TIMER = 2,
   EAGLE_MESSAGE_CLOSE_TIMER = 3
};



void* TimerProcess(EagleThread* ethread , void* etimer) {
   EagleTimer* eagle_timer = (EagleTimer*)etimer;
   Allegro5Timer* eagle_a5_timer = dynamic_cast<Allegro5Timer*>(eagle_timer);
   EAGLE_ASSERT(eagle_a5_timer);

   ALLEGRO_TIMER* timer = eagle_a5_timer->AllegroTimer();
   ALLEGRO_EVENT_QUEUE* queue = eagle_a5_timer->AllegroEventQueue();
   ALLEGRO_EVENT_SOURCE* event_source = eagle_a5_timer->AllegroEventSource();

   EAGLE_ASSERT(timer);
   EAGLE_ASSERT(queue);
   EAGLE_ASSERT(event_source);

   int counter = 0;
   bool close = false;
   while (!ethread->ShouldStop() && !close) {
      ALLEGRO_EVENT ev;
      al_wait_for_event(queue , &ev);
//      EagleInfo() << "Event " << ev.type << std::endl;
      if (ev.type == ALLEGRO_EVENT_TIMER && ev.timer.source == (ALLEGRO_TIMER*)eagle_a5_timer->Source()) {
//         EagleInfo() << "TIMER_EVENT received" << std::endl;
         ++counter;
         eagle_a5_timer->Tick(ev.any.timestamp , ethread);///al_get_time());
      }
      else if (ev.type == EAGLE_EVENT_USER_START) {
         switch (ev.user.data1) {
            case EAGLE_MESSAGE_CREATE_TIMER :
//               EagleInfo() << "EAGLE_MESSAGE_CREATE_TIMER received." << std::endl;
               break;
            case EAGLE_MESSAGE_START_TIMER :
               al_start_timer(timer);
//               EagleInfo() << "EAGLE_MESSAGE_START_TIMER received." << std::endl;
               break;
            case EAGLE_MESSAGE_STOP_TIMER :
               al_stop_timer(timer);
//               EagleInfo() << "EAGLE_MESSAGE_STOP_TIMER received." << std::endl;
               break;
            case EAGLE_MESSAGE_CLOSE_TIMER :
               close = true;
//               EagleInfo() << "EAGLE_MESSAGE_CLOSE_TIMER received." << std::endl;
               break;
            default : EAGLE_ASSERT(0);
               break;
         }
         al_emit_user_event(event_source , &ev , 0);// bounce message back to synchronize states
      }
   }
   return (void*)(long)counter;
}



void Allegro5Timer::SendTimerProcessMessage(int message) {
   ALLEGRO_EVENT ev;
   ev.type = EAGLE_EVENT_USER_START;
   ev.user.data1 = message;
   al_emit_user_event(&timer_event_source , &ev , NULL);
}



Allegro5Timer::Allegro5Timer() :
      EagleTimer(StringPrintF("Allegro5Timer at %p" , this)),
      timer(0),
      timer_queue(0),
      process_queue(0),
///      queue_lock(0),
      timer_event_source(),
      process_event_source(),
      ethread(0)
{
   al_init_user_event_source(&timer_event_source);
   al_init_user_event_source(&process_event_source);
//   queue_lock = al_create_mutex();
}



Allegro5Timer::~Allegro5Timer() {
   Destroy();
//   al_destroy_mutex(queue_lock);
//   queue_lock = 0;
}



bool Allegro5Timer::Create(double seconds_per_tick) {
   EAGLE_ASSERT(seconds_per_tick > 0.0);

   Destroy();

   EagleInfo() << "Allegro5Timer::Create this=" << this << std::endl;

   timer = al_create_timer(seconds_per_tick);
   timer_queue = al_create_event_queue();
   process_queue = al_create_event_queue();

   ethread = new Allegro5Thread();
   ethread->SetName(StringPrintF("%s Thread (EID = %d)" , GetNameCStr() , ethread->GetEagleId()));
   
   EAGLE_ASSERT(timer);
   EAGLE_ASSERT(timer_queue);
   EAGLE_ASSERT(process_queue);

   if (timer && timer_queue && process_queue) {
      spt = seconds_per_tick;
      previous_ticks = current_ticks = al_get_timer_count(timer);
      al_register_event_source(process_queue , al_get_timer_event_source(timer));
      al_register_event_source(process_queue , &timer_event_source);
      al_register_event_source(timer_queue , &process_event_source);
      /// MUST create TimerProcess thread AFTER registering event sources or it will wait forever
      ethread->Create(TimerProcess , this);

      // wait for thread to synchronize

      if (ethread->Valid()) {
         ethread->Start();

         // send out create message to thread process
         ALLEGRO_EVENT ev;
         ev.type = EAGLE_EVENT_USER_START;
         ev.user.data1 = EAGLE_MESSAGE_CREATE_TIMER;
         al_emit_user_event(&timer_event_source , &ev , 0);

         // wait for return message
         do {
            al_wait_for_event(timer_queue , &ev);
         } while (!((ev.type == EAGLE_EVENT_USER_START) && (ev.user.data1 == EAGLE_MESSAGE_CREATE_TIMER)));

         return true;
      }
   }

   if (!timer_queue) {
      EagleError() << "Allegro5Timer::Create - Could not create an Allegro 5 Timer - Couldn't create timer_queue." << std::endl;
   }
   if (!process_queue) {
      EagleError() << "Allegro5Timer::Create - Could not create an Allegro 5 Timer - Couldn't create process_queue." << std::endl;
   }
   if (!timer) {
      EagleError() << "Allegro5Timer::Create - Could not create an Allegro 5 Timer - Couldn't create an ALLEGRO_TIMER." << std::endl;
   }
   if (!ethread->Valid()) {
      EagleError() << "Allegro5Timer::Create - ethread is not valid." << std::endl;
   }

   // The queue or the timer failed to be created
   Destroy();
   return false;
}



void Allegro5Timer::Destroy() {
   EagleInfo() << "Allegro5Timer::Destroy this=" << this << std::endl;

   Close();

   if (timer_queue) {
      al_unregister_event_source(timer_queue , &process_event_source);
      al_unregister_event_source(timer_queue , al_get_timer_event_source(timer));
      al_destroy_event_queue(timer_queue);
      timer_queue = 0;
   }
   if (process_queue) {
      al_unregister_event_source(process_queue , &timer_event_source);
      al_destroy_event_queue(process_queue);
      process_queue = 0;
   }
   if (timer) {
      al_destroy_timer(timer);
      timer = 0;
   }
   if (ethread) {
      delete ethread;
      ethread = 0;
   }
   spt = 0.0;
}



void Allegro5Timer::Start() {
   SendTimerProcessMessage(EAGLE_MESSAGE_START_TIMER);

   ALLEGRO_EVENT ev;
   do {
      al_wait_for_event(timer_queue , &ev);
   } while (!((ev.type == EAGLE_EVENT_USER_START) && (ev.user.data1 == EAGLE_MESSAGE_START_TIMER)));
}



void Allegro5Timer::Stop() {
   SendTimerProcessMessage(EAGLE_MESSAGE_STOP_TIMER);

   ALLEGRO_EVENT ev;
   do {
      al_wait_for_event(timer_queue , &ev);
   } while (!((ev.type == EAGLE_EVENT_USER_START) && (ev.user.data1 == EAGLE_MESSAGE_STOP_TIMER)));
}



void Allegro5Timer::Close() {
   if (!ethread || (ethread && !ethread->Running())) {return;}

   SendTimerProcessMessage(EAGLE_MESSAGE_CLOSE_TIMER);

   // wait for message to be bounced back
   ALLEGRO_EVENT ev;
   do {
      al_wait_for_event(timer_queue , &ev);
   } while (!((ev.type == EAGLE_EVENT_USER_START) && (ev.user.data1 == EAGLE_MESSAGE_CLOSE_TIMER)));

   ethread->FinishThread();
}



void Allegro5Timer::WaitForTick(EagleThread* thread) {
   if (timer_queue && timer) {
      do {
         ALLEGRO_EVENT e;
         al_wait_for_event(timer_queue , &e);
         if (e.type == ALLEGRO_EVENT_TIMER) {
            Tick(al_get_time() , thread);
            break;
         }
      } while (true);
   }
   return;
}



void* Allegro5Timer::Source() {
   return timer;
}



void Allegro5Timer::RefreshTimer(EagleThread* thread) {
   if (timer_queue && timer) {
      ALLEGRO_EVENT ev;
      while (al_get_next_event(timer_queue , &ev)) {
         if (ev.type == ALLEGRO_EVENT_TIMER) {
            Tick(al_get_time() , thread);
         }
      }
   }
}



bool Allegro5Timer::Valid() {
   return timer && timer_queue && process_queue && ethread->Valid();
}



long long int Allegro5Timer::Count() {
   if (timer) {return al_get_timer_count(timer);}
   return -1;
}



void Allegro5Timer::RegisterTimerInput(EagleEventHandler* event_handler) {
/*
   EAGLE_ASSERT(event_handler);
   Allegro5EventHandler* a5_event_handler = dynamic_cast<Allegro5EventHandler*>(event_handler);
   EAGLE_ASSERT(allegro_handler);

   ALLEGRO_EVENT_QUEUE* allegro_queue = a5_event_handler->AllegroQueue();
   EAGLE_ASSERT(allegro_queue);
   al_register_event_source(allegro_queue , al_get_timer_event_source(timer));
*/
   // so we don't have timers registered to queues and eagle timers registered as event sources at the same time
   SubscribeListener(event_handler);/// TODO Convert to an event source
}

