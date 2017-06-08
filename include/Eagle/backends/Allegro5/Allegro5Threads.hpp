


#ifndef Allegro5Threads_HPP
#define Allegro5Threads_HPP


#include "Eagle/Threads.hpp"



struct ALLEGRO_THREAD;
struct ALLEGRO_COND;
struct ALLEGRO_MUTEX;


void* A5ThreadWrapperProcess(ALLEGRO_THREAD* allegro_thread , void* argument);




class Allegro5Thread : public EagleThread {

private :
   ALLEGRO_THREAD* a5thread;
   void* data;
   void* (*process)(EagleThread* , void*);
   void* return_value;
   bool running;
   
   ALLEGRO_COND* finish_condition_var;
   ALLEGRO_MUTEX* finish_mutex;
   
   
   
   friend void* A5ThreadWrapperProcess(ALLEGRO_THREAD* , void*);
   
//   void* (*)(EagleThread* , void*) Process() {return process;}
   EAGLE_THREAD_PROCESS Process() {return process;}
   void* Data() {return data;}

public :
   Allegro5Thread();
   ~Allegro5Thread();
   
   bool Create(void* (*process_to_run)(EagleThread* , void*) , void* arg);
   void Destroy();
   void Start();
   void SignalToStop();
   
   void* Join();/// Implicitly signal stop
   void* FinishThread();/// Wait for thread to complete

   bool ShouldStop();
   bool Running();
   bool Valid();
};




#endif // Allegro5Threads_HPP
