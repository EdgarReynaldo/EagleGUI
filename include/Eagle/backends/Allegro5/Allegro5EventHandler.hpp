


#ifndef Allegro5EventHandler_HPP
#define Allegro5EventHandler_HPP


#include "Eagle/Events.hpp"
#include "Eagle/Threads.hpp"


#include "Eagle/backends/Allegro5/Allegro5Conditions.hpp"
#include "Eagle/backends/Allegro5/Allegro5Threads.hpp"

#include "allegro5/allegro.h"


enum EVENT_THREAD_MESSAGES {
   CLOSE_EVENT_THREAD = 0
};

EagleEvent GetEagleEvent(ALLEGRO_EVENT ev);


void* Allegro5EventThreadProcess(EagleThread* thread , void* event_handler);


class Allegro5EventHandler : public EagleEventHandler {
   
private :

   friend void* Allegro5EventThreadProcess(EagleThread* thread , void* data);

   ALLEGRO_EVENT_QUEUE* event_queue;/// main event queue
   ALLEGRO_EVENT_SOURCE main_source;// for messages to the event thread
   
   Allegro5Thread* event_thread;

   bool Running();

public :

   Allegro5EventHandler(std::string objname = "A5EH" , bool delay_emitted_events = false);
   ~Allegro5EventHandler();

   virtual bool Create();
   virtual void Destroy();
   virtual bool Valid();

   virtual void RespondToEvent(EagleEvent e , EagleThread* thread);

   ALLEGRO_EVENT_QUEUE* AllegroQueue() {return event_queue;}

};




#endif // Allegro5EventHandler_HPP




