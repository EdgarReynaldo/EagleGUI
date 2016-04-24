

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


#ifndef Allegro5ConditionVar_HPP
#define Allegro5ConditionVar_HPP


#include "Eagle/Conditions.hpp"

#include "allegro5/allegro.h"


class Allegro5ConditionVar : public EagleConditionVar {

private :
   ALLEGRO_MUTEX* mutex;
   ALLEGRO_COND* cond;

public :
   
   Allegro5ConditionVar();
   ~Allegro5ConditionVar() {Destroy();}

   bool Create();
   void Destroy();
   bool Valid();

   void WaitForCondition();
   int WaitForConditionUntil(double wait_time);
   void SignalCondition();
   void BroadcastCondition();
   
   void LockSharedMutex();
   void UnlockSharedMutex();
};


#endif // Allegro5ConditionVar_HPP
