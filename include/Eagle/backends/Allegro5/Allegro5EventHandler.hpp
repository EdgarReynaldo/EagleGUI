
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
 * @file Allegro5EventHandler.hpp
 * @brief The interface for an Allegro 5 event handler
 */

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



EagleEvent GetEagleEvent(ALLEGRO_EVENT ev);///< Get an EagleEvent from an ALLEGRO_EVENT



void* Allegro5EventThreadProcess(EagleThread* thread , void* event_handler);///< The main thread process for an event handler



/**! @class Allegro5EventHandler
 *   @brief The Allegro 5 implementation of an EagleEventHandler
 */

class Allegro5EventHandler : public EagleEventHandler {
   
private :

   friend void* Allegro5EventThreadProcess(EagleThread* thread , void* data);///< Friend function for the event thread process

   ALLEGRO_EVENT_QUEUE* event_queue;///< Main event queue
   bool destroy_queue;
   ALLEGRO_EVENT_SOURCE main_source;///< For messages to the event thread
   
   Allegro5Thread* event_thread;///< The thread our event process runs on

   bool Running();///< True if running

public :

   Allegro5EventHandler(ALLEGRO_EVENT_QUEUE* adopt = 0 , std::string objname = "A5EH" , bool delay_emitted_events = false);
   ~Allegro5EventHandler();

   virtual bool Create(ALLEGRO_EVENT_QUEUE* q);
   virtual void Destroy();
   virtual bool Valid();

   virtual void RespondToEvent(EagleEvent e , EagleThread* thread);

   ALLEGRO_EVENT_QUEUE* AllegroQueue() {return event_queue;}

};




#endif // Allegro5EventHandler_HPP




