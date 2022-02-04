

#include "Eagle/backends/Allegro5/Allegro5Threads.hpp"
#include "Eagle.hpp"

#include "allegro5/allegro.h"



void* A5ThreadWrapperProcess(ALLEGRO_THREAD* allegro_thread , void* argument) {
   (void)allegro_thread;
   EAGLE_ASSERT(allegro_thread);
   EAGLE_ASSERT(argument);
   Allegro5Thread* ethread = (Allegro5Thread*)argument;

   void* (*thread_process)(EagleThread* , void*) = ethread->Process();
   EAGLE_ASSERT(thread_process);
   void* data = ethread->Data();

   EagleInfo() << StringPrintF("A5ThreadWrapperProcess : Starting process %p on %s." , (void*)thread_process , ethread->FullName()) << std::endl;

   ethread->running = true;
   void* process_result = thread_process(ethread , data);
   ethread->running = false;
   ethread->complete = true;

   EagleInfo() << StringPrintF("A5ThreadWrapperProcess : Finished process %p on %s." , (void*)thread_process , ethread->FullName()) << std::endl;

   ALLEGRO_EVENT ev;
   ev.type = EAGLE_EVENT_USER_START;
   ev.user.source = ethread->finish_event_source;
   ev.user.data1 = (intptr_t)process_result;
   
   al_emit_user_event(ethread->finish_event_source , &ev , 0);

   return process_result;

}



Allegro5Thread::Allegro5Thread(std::string objname) :
      EagleThread("Allegro5Thread" , objname),
      a5thread(0),
      data(0),
      process(0),
      return_value(0),
      running(false),
      complete(false),
      finish_event_source(0),
      finish_queue(0)
{
}



Allegro5Thread::~Allegro5Thread() {
   Destroy();
}



bool Allegro5Thread::Create(void* (*process_to_run)(EagleThread* , void*) , void* arg) {
   Destroy();

   EAGLE_ASSERT(process_to_run);
   process = process_to_run;

   // data may be null
   data = arg;

   a5thread = al_create_thread(A5ThreadWrapperProcess , this);

   finish_queue = al_create_event_queue();
   
   finish_event_source = new ALLEGRO_EVENT_SOURCE();
   
   if (!a5thread || !finish_queue) {
      if (!a5thread) {
         throw EagleException("Allegro5Thread::Create - failed to create a5thread.");
      }
      if (!finish_queue) {
         throw EagleException("Allegro5Thread::Create - failed to create finish_queue.");
      }
   }

   al_init_user_event_source(finish_event_source);

   al_register_event_source(finish_queue , finish_event_source);

   return true;
}



void Allegro5Thread::Destroy() {

///   FinishThread();

   if (a5thread && running) {
      Join();
      al_destroy_thread(a5thread);
      a5thread = 0;
   }

   if (finish_queue) {
      al_unregister_event_source(finish_queue , finish_event_source);
      al_destroy_event_queue(finish_queue);
      finish_queue = 0;
   }
   if (finish_event_source) {
      al_destroy_user_event_source(finish_event_source);
      delete finish_event_source;
      finish_event_source = 0;
   }
}



void Allegro5Thread::Start() {
   EAGLE_ASSERT(a5thread);
   if (Running()) {
      EagleWarn() << StringPrintF("Allegro5Thread::Start() : Already running!") << std::endl;
   }
   else {
      EagleInfo() << StringPrintF("Allegro5Thread::Start() : Starting thread.") << std::endl;
      complete = false;
      al_start_thread(a5thread);
      while (!Running()) {}
   }
   
}



void Allegro5Thread::SignalToStop() {
   if (!a5thread) {return;}
   if (Complete()) {return;}
   if (Running()) {
      al_set_thread_should_stop(a5thread);
   }
}



void* Allegro5Thread::Join() {
   SignalToStop();
   return FinishThread();
}



void* Allegro5Thread::FinishThread() {
   if (!a5thread) {
      return THREAD_NOT_VALID;
   }
   if (!complete) {
      EAGLE_ASSERT(running);
   }

   EagleInfo() << StringPrintF("Allegro5Thread::FinishThread() : waiting for thread %s to finish." , FullName()) << std::endl;
   
   ALLEGRO_EVENT finish_event;
   al_wait_for_event(finish_queue , &finish_event);
   
   EAGLE_ASSERT(finish_event.type == EAGLE_EVENT_USER_START);
   EAGLE_ASSERT(finish_event.user.source == finish_event_source);
   
   return (void*)finish_event.user.data1;
}



bool Allegro5Thread::ShouldStop() {
   EAGLE_ASSERT(a5thread);

   return al_get_thread_should_stop(a5thread);
}



bool Allegro5Thread::Running() {
   return running;
}



bool Allegro5Thread::Complete() {
   return complete;
}



bool Allegro5Thread::Valid() {
   return a5thread;
}





