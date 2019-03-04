
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
 *    Copyright 2009-2013+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */


#include "Eagle/Threads.hpp"
#include "Eagle/Timer.hpp"
#include "Eagle/StringWork.hpp"


void EagleTimer::Tick(double timestamp , EagleThread* thread) {
   ++current_ticks;
   EagleEvent ev;
   TIMER_EVENT_DATA d;
   d.eagle_timer_source = this;
   d.raw_source = Source();
   d.count = current_ticks;
   ev.timer = d;
   ev.source = this;
   ev.type = EAGLE_EVENT_TIMER;
   ev.timestamp = timestamp;
   EmitEvent(ev , thread);
}



EagleTimer::EagleTimer(std::string objclass , std::string objname) :
      EagleObject(objclass , objname),
      EagleEventSource(),
      id(NextId()),
      spt(0.0),
      previous_ticks(0UL),
      current_ticks(0UL),
      running(false)
{}



unsigned long long EagleTimer::Count() {
   RefreshTimer(0);
   return current_ticks;
}



unsigned long long EagleTimer::TicksPassed() {
   RefreshTimer(0);
   return current_ticks - previous_ticks;
}



double EagleTimer::TimePassed() {
   RefreshTimer(0);
   return (double)(current_ticks - previous_ticks)*spt;
}



int EagleTimer::TakeAllTicks() {
   RefreshTimer(0);
   int delta_ticks = (int)(current_ticks - previous_ticks);
   previous_ticks = current_ticks;
   return delta_ticks;
}



double EagleTimer::TakeAllTime() {
   RefreshTimer(0);
   double t = (double)(current_ticks - previous_ticks)*spt;
   previous_ticks = current_ticks;
   return t;
}

