

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



const ProgramTime program_start = ProgramTime::Now();



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
    EAGLE_ASSERT(0 == clock_gettime(CLOCK_REALTIME , &ts));
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


