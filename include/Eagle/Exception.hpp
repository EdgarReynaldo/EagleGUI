
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



#ifndef Error_H
#define Error_H

#include <string>
#include <cassert>

#include "Eagle/Logging.hpp"
//#include "Eagle/StringWork.hpp"



void LogFailedAssert(const char* exp , const char* file , int line , const char* func);
void LogFailedAssertAndFail(const char* exp , const char* file , int line , const char* func);

void EpicFail();


#ifdef EAGLE_ASSERT_EXCEPTION
   #define ASSERT_EXCEPTION() EpicFail()
#else
   #define ASSERT_EXCEPTION() (void)0
#endif



/** CREDITS : Thanks to Per Larsson on allegro.cc for this do while trick in the macros */
#ifdef DEBUG
   #define EAGLE_ASSERT(exp) do {                                                           \
                                if (!(exp)) {                                               \
                                   LogFailedAssert(# exp , __FILE__ , __LINE__ , __func__); \
                                   ASSERT_EXCEPTION();                                      \
                                }                                                           \
                              } while (false)

   #define EAGLE_DEBUG(exp) do {                \
                                 exp            \
                               } while (false)

#else
   #define EAGLE_ASSERT(exp) do { } while (false)
   #define EAGLE_DEBUG(exp) do { } while (false)
   
#endif




class EagleException : public std::exception {

protected :
   std::string error;
   
public :
   
   EagleException(const std::exception e);
   EagleException(const std::string& e);
   EagleException(const char* e);

   ~EagleException() throw () {}
   
   const char* what() const throw() {return error.c_str();}
};



#endif // Error_H
