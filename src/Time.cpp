

#include "Eagle/Time.hpp"



#ifdef EAGLE_WIN32
   #include "windows.h"
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
#ifdef EAGLE_WIN32
   LARGE_INTEGER qpf;
   LARGE_INTEGER qpc;
   QueryPerformanceFrequency(&qpf);
   QueryPerformanceCounter(&qpc);
   return ProgramTime((double)qpc.QuadPart/(double)qpf.QuadPart);
#endif
}



ProgramTime ProgramTime::ProgramStart() {
   return program_start;
}





