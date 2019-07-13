
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
 * @file Logging.hpp
 * @brief The main interface for all logging in Eagle
 */

#ifndef Logging_H
#define Logging_H



#include <string>
#include <iostream>
#include <fstream>
#include <unordered_set>



#include "Eagle/CXX11Mutexes.hpp"


///< Sends the log output to a file
bool SendOutputToFile(const std::string& filepath , const std::string& header , bool append = true);


/**! @enum EAGLE_LOGGING_LEVEL
 *   @brief The level of logging to use
 */

enum EAGLE_LOGGING_LEVEL {
   EAGLE_LOG_INFO     = 0,///< This is merely informative
   EAGLE_LOG_WARN     = 1,///< Warning
   EAGLE_LOG_ERROR    = 2,///< Error
   EAGLE_LOG_CRITICAL = 3,///< Critical error
   EAGLE_LOG_NONE     = 4 ///< Don't log anything;
};

/**! @class EagleLogger
 *   @brief The EagleLogger class is responsible for sending output to the log.
 *
 *
 */

class EagleLogger {

friend class EagleLogGuard;

friend bool SendOutputToFile(const std::string& filepath , const std::string& header , bool append);

protected :
   
   CXX11Mutex mtx;///< The log is protected by a mutex to prevent garbled messages and data races
   
   EAGLE_LOGGING_LEVEL global_log_level;    ///< The global logging level
   EAGLE_LOGGING_LEVEL old_global_log_level;///< The previous global logging level
   EAGLE_LOGGING_LEVEL local_log_level;     ///< The local logging level

   std::unordered_set<std::ostream*> outputs;///< The set of outputs to stream log entries to


   EagleLogger& SetLocalLoggingLevel(EAGLE_LOGGING_LEVEL new_local_level);///< Set the local logging level

   CXX11Mutex* Mutex();///< Get our mutex

   EagleLogger();///< Constructor is private! You can not instantiate this class!
   ~EagleLogger() {}///< Destructor is private! You can not destroy this class!
   
   static EagleLogger& Instance();///< Get the instance of the EagleLogger

   static EagleLogger& EagleInfoLog();///< Set the local logging level to info and return the logger
   static EagleLogger& EagleWarnLog();///< Set the local logging level to warning and return the logger
   static EagleLogger& EagleErrorLog();///< Set the local logging level to error and return the logger
   static EagleLogger& EagleCriticalLog();///< Set the local logging level to critical and return the logger

   static const char* LogPrefixStr(EAGLE_LOGGING_LEVEL l);///< Helper function to get the prefix for this logging level
   
public :


   /**! @fn SetGlobalLoggingLevel <EAGLE_LOGGING_LEVEL>
    *   @brief Set the global logging level
    *
    *   This will affect all logging calls, and prevents messages with a lower level from being output.
    *   Think of it as a filter. If you specify EAGLE_LOG_ERROR, warnings and info will be discarded.
    */
   
   void SetGlobalLoggingLevel(EAGLE_LOGGING_LEVEL new_global_level);

   friend void EnableLog();///< Friend function to enable the log
   friend void DisableLog();///< Friend function to disable the log

   void TurnLogOff();///< Member function to turn thge log off
   void TurnLogOn();///< Member function to turn the log back on

   /**! @fn AddOutput <std::ostream&>
    *   @brief Add an output stream to the list of outputs
    *   @fn RemoveOutput <std::ostream&>
    *   @brief Remove an output stream from the list of outputs
    *
    *   The EagleLogger has multi logging capabilities. Each registered output will receive any messages sent to the log.
    */

   void AddOutput(std::ostream& output);
   void RemoveOutput(std::ostream& output);///< Remove an output stream from the list of outputs

   static void AddOutputStream(std::ostream& output) {
      Instance().AddOutput(output);
   }
   static void RemoveOutputStream(std::ostream& output) {
      Instance().RemoveOutput(output);
   }
   
   
   /**! @typedef MANIP
    *   @brief A typedef for working with ostream manipulator objects
    */

   typedef std::ostream&(*MANIP)(std::ostream&);

   /**! @fn operator<< <MANIP>
    *   @brief Outputs the ostream manipulator to the stream, for working with things like std::endl and such
    *
    *   CREDITS : A kind shout out to relpatseht who helped me get std::endl to work with my logging class 
    */

   EagleLogger& operator<<(MANIP manip);

   /**! @fn operator<< <Type&>
    *   @brief Template method to output an object to the stream. As long as operator<<(std::ostream& , Type&) has been overloaded
    *          for the object in question, it will work normally.
    *   @param t The object to output to the stream
    */
   
   template<class Type>
   EagleLogger& operator<<(const Type& t);

   ///< Act as if we are an ostream&
   operator std::ostream&();
};


/**! @fn EnableLog <>
 *   @brief Enables the eagle log
 */

void EnableLog();

/**! @fn DisableLog <>
 *   @brief Disables the eagle log
 */

void DisableLog();



template<class Type>
EagleLogger& EagleLogger::operator<<(const Type& t) {
   if (local_log_level >= global_log_level) {
      for (std::unordered_set<std::ostream*>::iterator it = outputs.begin() ; it != outputs.end() ; ++it) {
         std::ostream& os = *(*it);
         os << t;
      }
   }
   return *this;
}


/**! @class EagleLogGuard
 *   @brief The EagleLogGuard class acts as a sentry for logging. It handles the mundane task of locking the mutex, performing output,
 *          and unlocking the mutex when done.
 */

class EagleLogGuard {


private :

   EagleLogger& log;///< A reference to the underlying log
   CXX11Mutex* pmutex;///< A pointer to the mutex used

   /**! @fn EagleLogGuard::EagleLogGuard <EagleLogger& , const char*>
    *   @brief The only constructor for this class.
    *   @param logger Required parameter, represents the underlying log we're using
    */

   explicit EagleLogGuard(EagleLogger& logger , const char* prefix);
   
   /**! @fn EagleLogGuard::EagleLogGuard <const EagleLogGuard&>
    *   @brief EagleLogGuard copy constructor
    */
   
   EagleLogGuard(const EagleLogGuard& rhs);
   
   /**! @fn EagleLogGuard::operator= <EagleLogGuard&>
    *   @brief The assignment operator, copies the @param rhs explicitly
    */
   
   EagleLogGuard& operator=(const EagleLogGuard& rhs);
   
public :

   ~EagleLogGuard();///< Public destructor

   /**! @typedef MANIP
    *   @brief A typedef to work with ostream manipulators
    */
   
   typedef std::ostream&(*MANIP)(std::ostream&);

   ///< Output an ostream manipulator to the underlying stream
   EagleLogGuard& operator<<(MANIP manip);

   ///< Output an object to the underlying stream
   template<class Type>
   EagleLogGuard& operator<<(const Type& t);

   /**! @fn EagleGuardLog @fn EagleGuardInfo @fn EagleGuardWarn @fn EagleGuardError @fn EagleGuardCritical
    *   @brief Gets an EagleLogGuard object in the specified logging state
    */
   
   static EagleLogGuard EagleGuardLog();
   static EagleLogGuard EagleGuardInfo();
   static EagleLogGuard EagleGuardWarn();
   static EagleLogGuard EagleGuardError();
   static EagleLogGuard EagleGuardCritical();
   
   /**! @fn EagleGuardPrefix
    *   @brief Gets an EagleLogGuard, using the specified log prefix
    */
   
   static EagleLogGuard EagleGuardPrefix(const char* prefix);

   friend EagleLogGuard EagleLog();     ///< Friend function to get access to the regular log
   friend EagleLogGuard EagleInfo();    ///< Friend function to get access to an info log
   friend EagleLogGuard EagleWarn();    ///< Friend function to get access to a warning log
   friend EagleLogGuard EagleError();   ///< Friend fucntion to get access to an error log
   friend EagleLogGuard EagleCritical();///< Friend function to get access to the critical log

   friend EagleLogGuard EaglePrefix(const char* prefix);///< Friend function to get access to a log using the specified @param prefix

   friend class EagleObject;
   friend EagleLogGuard& operator<<(EagleLogGuard& guard , const EagleObject& obj);///< Global friend function to log EagleObject objects
};


EagleLogGuard EagleLog();     ///< Get access to the log
EagleLogGuard EagleInfo();    ///< Get access to the info log
EagleLogGuard EagleWarn();    ///< Get access to the warning log
EagleLogGuard EagleError();   ///< Get access to the error log
EagleLogGuard EagleCritical();///< Get access to the critical error log

EagleLogGuard EaglePrefix(const char* prefix);///< Get access to the log using the specified logging prefix




template<class Type>
EagleLogGuard& EagleLogGuard::operator<<(const Type& t) {
   log << t;
   return *this;
}



EagleLogGuard& operator<<(EagleLogGuard& guard , const EagleObject& obj);




#endif // Logging_H




