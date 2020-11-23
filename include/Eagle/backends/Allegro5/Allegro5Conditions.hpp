
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
 * @file Allegro5Conditions.hpp
 * @brief The interface for an Allegro 5 condition variable
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
   
   Allegro5ConditionVar(std::string objname = "Nemo");///< Default constructor
   ~Allegro5ConditionVar();///< Destructor

   bool Create();///< Create an Allegro 5 condition variable

   void Destroy();///< Destroy the underlying condition variable

   bool Valid();///< True if valid and ready

   ///< Wait indefinitely for the condition to be signalled
   void WaitForCondition();

   /**! @fn WaitForConditionUntil
    *   @brief Wait up to 'wait_time' for the condition to be signalled
    *   @retval zero if condition was signalled, non-zero if wait time expired
    */
   int WaitForConditionUntil(double wait_time);

   ///< Signal anyone waiting on this condition
   void SignalCondition();

   ///< Signal everyone waiting on this condition
   void BroadcastCondition();
   
   void LockSharedMutex();///< Lock our shared mutex
   void UnlockSharedMutex();///< Unlock our shared mutex
};



#endif // Allegro5ConditionVar_HPP



