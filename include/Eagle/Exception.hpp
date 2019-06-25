
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
 * @file Exception.hpp
 * @brief The interface for EagleException objects and debugging exceptions
 */

#ifndef Error_H
#define Error_H



#include <string>
#include <cassert>



#include "Eagle/Platform.hpp"
#include "Eagle/Logging.hpp"


///< The main eagle trace function
std::string EagleTraceStr(const char* exp , const char* file , int line , const char* func);

///< Send a failed assert info to the log
void LogFailedAssert(std::string info);

///< Send a failed assert info to the log and crash intentionally
void LogFailedAssertAndFail(std::string info);

///< Add a line to the log
void Trace(std::string info);

///< The main eagle assert handler, which logs for you
void EagleAssertHandler(const char* exp , const char* file , int line , const char* func);

///< Our intentional divide by zero function, to make the program SIGFPE and stop
void EpicFail();

/**! @def EAGLE__TRACE(exp)
 *   @brief Helper macro to log where you are in a source file
 */

#define EAGLE__TRACE(exp) EagleTraceStr(#exp , __FILE__ , __LINE__ , EAGLE__FUNC)


/**! @def ASSERT_EXCEPTION
 *   @brief When EAGLE_ASSERT_EXCEPTION is defined, asserts will crash and fail. Otherwise they will silently continue.
 */
#ifdef EAGLE_ASSERT_EXCEPTION
   #define ASSERT_EXCEPTION() EpicFail()
#else
   #define ASSERT_EXCEPTION() (void)0
#endif

/**! @def DEBUG
 *   @brief Enables EAGLE_ASSERT, EAGLE_TRACE, and EAGLE_DEBUG expressions
 */

/**! @def EAGLE_ASSERT @def EAGLE_TRACE @def EAGLE_DEBUG
 *
 *   The EAGLE_ASSERT macro is used just like the C assert function, except that it will log the file, line, function, and expression
 *   used. Does nothing if @ref DEBUG is not defined.
 *
 *   The EAGLE_TRACE macro is similar, but only does logging. Does nothing if @ref DEBUG is not defined.
 *
 *   The EAGLE_DEBUG macro will execute code in DEBUG mode only. Be careful with this one, as it can lead
 *   to bizarre differences between release and debug code.
 *
 *   CREDITS : Thanks to Per Larsson on allegro.cc for this do while trick in the macros
 */

#ifdef DEBUG
   #define EAGLE_ASSERT(exp) do {                                                              \
                                if (!(exp)) {                                                  \
                                   LogFailedAssert(EAGLE__TRACE(exp));                         \
                                   ASSERT_EXCEPTION();                                         \
                                }                                                              \
                              } while (false)
   #define EAGLE_TRACE(exp) do {                        \
                              Trace(EAGLE__TRACE(exp)); \
                            } while (false)

   #define EAGLE_DEBUG(exp) do {                \
                               exp              \
                            } while (false)

#else
   #define EAGLE_ASSERT(exp) do { } while (false)
   #define EAGLE_TRACE(exp)  do { } while (false)
   #define EAGLE_DEBUG(exp)  do { } while (false)
#endif



/**! @class EagleException
 *   @brief Base exception class for Eagle
 *
 *   This class is the basic class for all Eagle exceptions. It automatically logs errors to EagleError
 *   and will intentionally crash on a failed assert given DEBUG and EAGLE_ASSERT_EXCEPTION are defined so you
 *   can get a backtrace.
 *
 *   ```
 *       #define DEBUG
 *       #define EAGLE_ASSERT_EXCEPTION
 *       #include "Eagle/Exception.hpp"
 *       EAGLE_ASSERT(0);///< Halt and log in debug mode
 *       throw EagleException("Halt in debug or release mode with an exception");
 *   ```
 */

class EagleException : public std::exception {

protected :
   std::string error;
   
public :
   
   EagleException(const std::exception e);///< For catching a std::exception and forwarding it on
   EagleException(const std::string& e);///< String constructor, e will be logged
   EagleException(const char* e);///< Cstring constructor

   ~EagleException() throw () {}///< The destructor is guaranteed not to throw
   
   const char* what() const throw() {return error.c_str();}///< Get a c string reference to the error
};



#endif // Error_H



