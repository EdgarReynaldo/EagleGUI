
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
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */

/**
#include "Eagle/Exception.hpp"
#include "Eagle/backends/Allegro5/Allegro5Mutex.hpp"



void Allegro5Mutex::PrivateLock() {
   EAGLE_ASSERT(mutex);
   al_lock_mutex(mutex);
}



void Allegro5Mutex::PrivateUnlock() {
   EAGLE_ASSERT(mutex);
   al_unlock_mutex(mutex);
}



bool Allegro5Mutex::Create(bool multi_lockable) {
   Destroy();

   if (multi_lockable) {
      mutex = al_create_mutex_recursive();
   }
   else {
      mutex = al_create_mutex();
   }
   this->recursive = multi_lockable && (bool)mutex;
   return (bool)mutex;
}



void Allegro5Mutex::Destroy() {
   if (mutex) {
      al_destroy_mutex(mutex);
      mutex = 0;
      recursive = false;
   }
}



bool Allegro5Mutex::Valid() {
   return mutex;
}

//*/



