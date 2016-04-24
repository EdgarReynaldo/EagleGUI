
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



#ifndef EagleConditionVar_HPP
#define EagleConditionVar_HPP


class EagleConditionVar {

private :
   
public :
   
   EagleConditionVar() {}
   virtual ~EagleConditionVar() {}
   
   virtual bool Create()=0;
   virtual void Destroy()=0;
   virtual bool Valid()=0;
   
   virtual void WaitForCondition()=0;
   virtual int WaitForConditionUntil(double wait_time)=0;// nonzero if timed out, zero if condition was signalled
   virtual void SignalCondition()=0;
   virtual void BroadcastCondition()=0;
   
   virtual void LockSharedMutex()=0;
   virtual void UnlockSharedMutex()=0;
};




#endif // EagleCondition_HPP




