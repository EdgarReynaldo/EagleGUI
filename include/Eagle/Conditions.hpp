
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
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Conditions.hpp
 * @brief An interface to work with condition variables and synchronization
 * 
 * EagleConditionVar is a pure virtual base class for signaling and receiving conditions
 *
 * See @ref Allegro5ConditionVar for an Allegro 5 implementation
 */

#ifndef EagleConditionVar_HPP
#define EagleConditionVar_HPP



#include "Eagle/Object.hpp"



/**! @class EagleConditionVar
 *   @brief A simple pure virtual base class for working with conditions and signals
 */

class EagleConditionVar : public EagleObject {

protected :
   
   ///< Lock the shared mutex
   virtual void LockSharedMutex()=0;
   ///< Unlock the shared mutex
   virtual void UnlockSharedMutex()=0;

public :
   
   ///< Create an EagleConditionVar base with the specified object class and name
   EagleConditionVar(std::string objclass = "EagleConditionVar" , std::string objname = "Nemo");

   virtual ~EagleConditionVar() {}///< Empty virtual destructor
   
   virtual bool Create()=0;///< Base creation function, override in your sub classes
   virtual void Destroy()=0;///< Base destruction function, override in your sub classes
   virtual bool Valid()=0;///< Base validity check function, override in your sub classes
   
   ///< Base class conditional wait function. Will wait for this condition to be signaled.
   virtual void WaitForCondition()=0;
   
   ///< Base class conditional wait function. Will wait for this condition to be signaled or until the timeout is reached.
   ///< Returns non-zero if timed out, zero if condition was signalled
   virtual int WaitForConditionUntil(double wait_time)=0;

   ///< Signal this condition, will wake the next thread waiting for this condition to be signalled
   virtual void SignalCondition()=0;
   
   ///< Broadcast the signal for this condition to all those waiting
   virtual void BroadcastCondition()=0;
   
};




#endif // EagleCondition_HPP




