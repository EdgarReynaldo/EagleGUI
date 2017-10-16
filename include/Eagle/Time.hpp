



#ifndef EagleProgramTime_HPP
#define EagleProgramTime_HPP

#include <ctime>

class ProgramTime {

protected :

   double program_time;
   
public :

   ProgramTime();
   ProgramTime(double t);
   ProgramTime(const ProgramTime& t);

   ProgramTime& operator=(const ProgramTime& t);

   double Diff(const ProgramTime& t2) const ;

   double operator-(const ProgramTime& t2) const;
   operator double() const;

   static ProgramTime Now();
   static ProgramTime ProgramStart();
   static ProgramTime Elapsed();

};

extern const ProgramTime program_start;

#endif // EagleProgramTime_HPP

