
/**
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
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#ifndef EagleTimer_HPP
#define EagleTimer_HPP


#include "Eagle/Events.hpp"
#include "Eagle/Object.hpp"


class EagleThread;



class EagleTimer : public EagleObject , public EagleEventSource {

private :
   int id;// unique id

protected :
   double spt;// seconds per tick
   unsigned long long previous_ticks;
   unsigned long long current_ticks;
   bool running;
   
   static int NextId() {
      static int i = 0;
      return i++;
   }
   
   void Tick(double timestamp , EagleThread* thread);
   
public :

   EagleTimer(std::string objclass = "EagleTimer" , std::string objname = "Nemo");
   virtual ~EagleTimer() {}

   /// EagleEventSource
   void ReadEvents() {RefreshTimer(0);}

   virtual bool Create(double seconds_per_tick)=0;
   virtual void Destroy()=0;
   virtual void Start()=0;
   virtual void Stop()=0;
   virtual void Close()=0;
   virtual void WaitForTick(EagleThread* thread)=0;
   bool SetSecondsPerTick(double secs) {return Create(secs);}
   
   
   int ID() {return id;}
   double SPT() {return spt;}
   double SecondsPerTick() {return spt;}
   unsigned long long Count();
   
   virtual void* Source()=0;

   virtual void RefreshTimer(EagleThread* thread)=0;
   
   // time passed since the last TakeAllTime was called
   unsigned long long TicksPassed();
   double TimePassed();
   int    TakeAllTicks();
   double TakeAllTime();
   
   void ClearTicks() {(void)TakeAllTicks();}
   
   virtual bool Valid()=0;
   virtual bool Running() {return running;}
   
   virtual void RegisterTimerInput(EagleEventHandler* queue)=0;
   
};


#endif // EagleTimer_HPP
