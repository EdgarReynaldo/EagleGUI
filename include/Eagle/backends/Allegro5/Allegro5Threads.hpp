
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
 * @file Allegro5Threads.hpp
 * @brief The interface for threading with Allegro 5
 */

#ifndef Allegro5Threads_HPP
#define Allegro5Threads_HPP

#include <atomic>
#include "Eagle/Threads.hpp"



struct ALLEGRO_THREAD;
struct ALLEGRO_COND;
struct ALLEGRO_MUTEX;
struct ALLEGRO_EVENT_SOURCE;
struct ALLEGRO_EVENT_QUEUE;


/**! @fn A5ThreadWrapperProcess <ALLEGRO_THREAD* , void*>
 *   @brief The main process for an Allegro 5 thread
 */

void* A5ThreadWrapperProcess(ALLEGRO_THREAD* allegro_thread , void* argument);




/**! @class Allegro5Thread
 *   @brief The Allegro 5 implementation of an EagleThread
 */

class Allegro5Thread : public EagleThread {

private :
   ALLEGRO_THREAD* a5thread;
   void* data;
   void* (*process)(EagleThread* , void*);
   void* return_value;
   std::atomic<bool> running;
   std::atomic<bool> complete;
   
   ALLEGRO_EVENT_SOURCE* finish_event_source;
   ALLEGRO_EVENT_QUEUE* finish_queue;
   


   friend void* A5ThreadWrapperProcess(ALLEGRO_THREAD* , void*);///< Friend function for the thread wrapper process
   
   EAGLE_THREAD_PROCESS Process() {return process;}///< Returns the process to be run
   void* Data() {return data;}///< Get the data associated with this process

public :
   Allegro5Thread(std::string objname = "Nemo");
   ~Allegro5Thread();
   
   bool Create(void* (*process_to_run)(EagleThread* , void*) , void* arg);///< Create a new thread with the given process and argument
   void Destroy();///< Destroy this thread
   void Start();///< Start the thread, does not start automatically
   void SignalToStop();///< Signal the thread to stop
   
   void* Join();/// Implicitly signal stop
   void* FinishThread();/// Wait for thread to complete

   bool ShouldStop();///< True if we should stop
   bool Running();///< True if we're running
   bool Complete();///< True if we ran to completion

   bool Valid();///< True if we are valid and ready to run
};



#endif // Allegro5Threads_HPP



