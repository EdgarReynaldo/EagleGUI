

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
 *    Copyright 2009-2014+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */

#include "Eagle/Error.hpp"
#include "Eagle/Logging.hpp"

#include "Eagle/backends/Allegro5/Allegro5Conditions.hpp"

#include "allegro5/allegro5.h"



Allegro5ConditionVar::Allegro5ConditionVar() :
      EagleConditionVar(),
      mutex(0),
      cond(0)
{
   
}



bool Allegro5ConditionVar::Create() {
   Destroy();
   
   mutex = al_create_mutex();
   cond = al_create_cond();
   
   if (!mutex || !cond) {
      if (!mutex) {
         EagleLog() << "Failed to create allegro 5 mutex." << std::endl;
      }
      if (!cond) {
         EagleLog() << "Failed to create allegro 5 condition variable." << std::endl;
      }
      Destroy();
      return false;
   }
   
   return true;
}



void Allegro5ConditionVar::Destroy() {
   if (mutex) {
      al_destroy_mutex(mutex);
      mutex = 0;
   }
   if (cond) {
      al_destroy_cond(cond);
      cond = 0;
   }
}



bool Allegro5ConditionVar::Valid() {
   return mutex && cond;
}


void Allegro5ConditionVar::WaitForCondition() {
   
   EAGLE_ASSERT(Valid());
   
   al_lock_mutex(mutex);
   al_wait_cond(cond , mutex);
   al_unlock_mutex(mutex);
}



int Allegro5ConditionVar::WaitForConditionUntil(double wait_time) {
   EAGLE_ASSERT(Valid());
   
   ALLEGRO_TIMEOUT timeout;
   al_init_timeout(&timeout , wait_time);
   int ret = 0;
   
   al_lock_mutex(mutex);
   ret = al_wait_cond_until(cond , mutex , &timeout);
   al_unlock_mutex(mutex);
   if (ret) {
      // timed out
   }
   else {
      // signalled
   }
   return ret;
}



void Allegro5ConditionVar::SignalCondition() {
   EAGLE_ASSERT(cond);
   
   al_signal_cond(cond);
}



void Allegro5ConditionVar::BroadcastCondition() {
   EAGLE_ASSERT(cond);
   
   al_broadcast_cond(cond);
}



void Allegro5ConditionVar::LockSharedMutex() {
   EAGLE_ASSERT(mutex);
   al_lock_mutex(mutex);
}



void Allegro5ConditionVar::UnlockSharedMutex() {
   EAGLE_ASSERT(mutex);
   al_unlock_mutex(mutex);
}




