
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
 * @file Time.hpp
 * @brief The interface for tracking high precision time in Eagle
 */

#ifndef EagleProgramTime_HPP
#define EagleProgramTime_HPP



#include <ctime>



/**! @class ProgramTime
 *   @brief A class to represent clock time in high precision (microseconds or better)
 */

class ProgramTime {

protected :

   double program_time;///< The actual time value
   
public :

   ProgramTime();///< Default constructor for a ProgramTime
   ProgramTime(double t);///< Double constructor for initializing the time
   ProgramTime(const ProgramTime& t);///< Copy constructor

   ProgramTime& operator=(const ProgramTime& t);///< Assignment operator

   double Diff(const ProgramTime& t2) const ;///< Get the difference between t2 and *this in seconds

   double operator-(const ProgramTime& t2) const;///< Subtract t2 from *this and return a double
   operator double() const;///< Cast to a double

   static ProgramTime Now();///< The current time
   static ProgramTime ProgramStart();///< The start time of the program
   static ProgramTime Elapsed();///< The time passed since program start

};



extern const ProgramTime program_start;///< Const access to program start time



#endif // EagleProgramTime_HPP



