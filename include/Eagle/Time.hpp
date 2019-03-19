
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

