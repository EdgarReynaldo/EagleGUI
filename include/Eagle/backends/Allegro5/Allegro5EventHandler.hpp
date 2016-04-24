


#ifndef Allegro5EventHandler_HPP
#define Allegro5EventHandler_HPP


#include "Eagle/Events.hpp"
#include "Eagle/Threads.hpp"


#include "Eagle/backends/Allegro5/Allegro5Conditions.hpp"
#include "Eagle/backends/Allegro5/Allegro5Threads.hpp"

#include "allegro5/allegro.h"


EagleEvent GetEagleEvent(ALLEGRO_EVENT ev);

enum EVENT_THREAD_MESSAGES {
   CLOSE_EVENT_THREAD = 0
};

void* Allegro5EventThreadProcess(EagleThread* thread , void* event_handler);


class Allegro5EventHandler : public EagleEventHandler {
   
private :

   friend void* Allegro5EventThreadProcess(EagleThread* thread , void* data);

   ALLEGRO_EVENT_QUEUE* event_queue;/// main event queue
   ALLEGRO_EVENT_SOURCE main_source;// for messages to the event thread
   
   Allegro5Thread* event_thread;
   
   Allegro5ConditionVar* cond_var;

   bool emitter_delay;


   bool Running();

   virtual EagleEvent PrivateWaitForEvent();
   virtual EagleEvent PrivateWaitForEvent(double timeout);
   
   virtual void PrivateRefreshQueue();

public :

   Allegro5EventHandler(bool delay_emitted_events);
   ~Allegro5EventHandler();

   virtual bool Create();
   virtual void Destroy();
   virtual bool Valid();

   virtual void RespondToEvent(EagleEvent e);

   ALLEGRO_EVENT_QUEUE* AllegroQueue() {return event_queue;}

};




#endif // Allegro5EventHandler_HPP




