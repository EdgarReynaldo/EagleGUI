

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
   
   ethread->running = true;
   void* process_result = thread_process(ethread , data);
   ethread->running = false;
   return process_result;
   
}



Allegro5Thread::Allegro5Thread() :
      EagleThread(),
      a5thread(0),
      data(0),
      process(0),
      return_value(0),
      running(false)
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
   
   return (bool)a5thread;
}



void Allegro5Thread::Destroy() {
   if (a5thread && running) {
      // TODO Join thread? Kill thread?
      al_join_thread(a5thread , 0);
   }
   if (a5thread) {
      al_destroy_thread(a5thread);
      a5thread = 0;
   }
}



void Allegro5Thread::Start() {
   EAGLE_ASSERT(a5thread);
   al_start_thread(a5thread);
}



void Allegro5Thread::SignalToStop() {
   EAGLE_ASSERT(a5thread);
   al_set_thread_should_stop(a5thread);
}



void* Allegro5Thread::Join() {
   EAGLE_ASSERT(a5thread);
   return_value = 0;
   al_join_thread(a5thread , &return_value);
   return return_value;
}



bool Allegro5Thread::ShouldStop() {
   EAGLE_ASSERT(a5thread);
   return al_get_thread_should_stop(a5thread);
}



bool Allegro5Thread::Running() {
   return running;
}



bool Allegro5Thread::Valid() {
   return a5thread;
}





