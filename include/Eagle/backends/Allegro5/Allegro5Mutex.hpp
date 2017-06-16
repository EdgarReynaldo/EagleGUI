
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


#ifndef Allegro5Mutex_HPP
#define Allegro5Mutex_HPP



#include "Eagle/Mutexes.hpp"

#include "allegro5/allegro.h"



class Allegro5Mutex : public EagleMutex {

private :

   ALLEGRO_MUTEX* mutex;

   virtual void PrivateLock();
   virtual void PrivateUnlock();

public :

   Allegro5Mutex() : EagleMutex() , mutex(0) {}
   ~Allegro5Mutex() {Destroy();}

   virtual bool Create(bool recursive);
   virtual void Destroy();
   virtual bool Valid();

   /// void Lock()
   /// void Unlock()
};



#endif // Allegro5Mutex_HPP



