
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


#include "Eagle/Platform.hpp"
#include "Eagle/Time.hpp"
#include "Eagle/Exception.hpp"


#if defined EAGLE_WIN32
   #include "windows.h"
#elif defined EAGLE_LINUX
    #include <ctime>
    #include <cmath>
#else
    #error "Eagle/Time.hpp does not support this platform."
#endif



ProgramTime program_start = ProgramTime::Now();



ProgramTime::ProgramTime() :
      program_time(Now())
{}



ProgramTime::ProgramTime(double t) :
      program_time(t)
{}



ProgramTime::ProgramTime(const ProgramTime& t) :
      program_time(t.program_time)
{}



ProgramTime& ProgramTime::operator=(const ProgramTime& t) {
   program_time = t.program_time;
   return *this;
}



double ProgramTime::Diff(const ProgramTime& t2) const {
   return program_time - t2.program_time;
}



double ProgramTime::operator-(const ProgramTime& t2) const {return Diff(t2);}



ProgramTime::operator double() const {
   return program_time;
}



ProgramTime ProgramTime::Now() {
#if defined EAGLE_WIN32
   LARGE_INTEGER qpf;
   LARGE_INTEGER qpc;
   QueryPerformanceFrequency(&qpf);
   QueryPerformanceCounter(&qpc);
   return ProgramTime((double)qpc.QuadPart/(double)qpf.QuadPart);
#elif defined EAGLE_LINUX
    timespec ts;
    clockid_t CLOCK;
    #if defined _POSIX_TIMERS and defined _POSIX_MONOTONIC_CLOCK
      CLOCK = CLOCK_MONOTONIC;
    #else
      CLOCK = CLOCK_REALTIME;
    #endif ///defined _POSIX_TIMERS and defined _POSIX_MONOTONIC_CLOCK
    int ret = clock_gettime(CLOCK_MONOTONIC , &ts);
    (void)ret;
    EAGLE_ASSERT(ret == 0);
    double rt = (double)ts.tv_sec + (double)ts.tv_nsec / pow(10 , 9);
    return ProgramTime(rt);
#else
    #error "ProgramTime::Now() is not implemented on this platform"
#endif
}



ProgramTime ProgramTime::ProgramStart() {
   return program_start;
}



ProgramTime ProgramTime::Elapsed() {
   return Now() - ProgramStart();
}


ProgramTime ProgramTime::SyncTime(double t) {
   return program_start = t - ProgramTime::Now();
}

