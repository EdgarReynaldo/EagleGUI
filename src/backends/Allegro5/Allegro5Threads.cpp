

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

   EagleInfo() << StringPrintF("Starting  Process %p on EagleThread %p." , thread_process , ethread) << std::endl;

   ethread->running = true;
   void* process_result = thread_process(ethread , data);
   ethread->running = false;

   al_lock_mutex(ethread->finish_mutex);

   ethread->finished_bool = true;

   al_signal_cond(ethread->finish_condition_var);

   al_unlock_mutex(ethread->finish_mutex);

   EagleInfo() << StringPrintF("Finishing Process %p on EagleThread %p." , thread_process , ethread) << std::endl;


   return process_result;

}



Allegro5Thread::Allegro5Thread(std::string objname) :
      EagleThread("Allegro5Thread" , objname),
      a5thread(0),
      data(0),
      process(0),
      return_value(0),
      running(false),
      finish_condition_var(0),
      finish_mutex(0)
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
   finish_condition_var = al_create_cond();
   finish_mutex = al_create_mutex();
   finished_bool = false;

   if (!a5thread || !finish_condition_var || !finish_mutex) {
      if (!a5thread) {
         throw EagleException("Allegro5Thread::Create - failed to create a5thread.");
      }
      if (!finish_condition_var) {
         throw EagleException("Allegro5Thread::Create - failed to create finish_condition_var.");
      }
      if (!finish_mutex) {
         throw EagleException("Allegro5Thread::Create - failed to create finish_mutex.");
      }
   }

   return a5thread && finish_condition_var && finish_mutex;
}



void Allegro5Thread::Destroy() {

   FinishThread();

   if (a5thread) {
      al_destroy_thread(a5thread);
      a5thread = 0;
   }
   if (finish_condition_var) {
      al_destroy_cond(finish_condition_var);
      finish_condition_var = 0;
   }
   if (finish_mutex) {
      al_destroy_mutex(finish_mutex);
      finish_mutex = 0;
   }
   finished_bool = false;
}



void Allegro5Thread::Start() {
   EAGLE_ASSERT(a5thread);
   if (!Running()) {
      al_start_thread(a5thread);
   }
}



void Allegro5Thread::SignalToStop() {
   if (!a5thread) {return;}
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
      return (void*)-1;
   }
   EAGLE_ASSERT(finish_condition_var);
   EAGLE_ASSERT(finish_mutex);

   return_value = 0;
   if (Running()) {

      al_lock_mutex(finish_mutex);

      while (!finished_bool) {
         al_wait_cond(finish_condition_var , finish_mutex);
      }

      al_unlock_mutex(finish_mutex);

      al_join_thread(a5thread , &return_value);
   }

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





