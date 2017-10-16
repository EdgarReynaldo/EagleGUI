
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
 *    Copyright 2009-2017+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#ifndef Logging_H
#define Logging_H



#include <string>
#include <iostream>
#include <fstream>
#include <unordered_set>

#include "Eagle/CXX11Mutexes.hpp"



bool SendOutputToFile(const std::string& filepath , const std::string& header , bool append = true);



enum EAGLE_LOGGING_LEVEL {
   EAGLE_LOG_INFO = 0,
   EAGLE_LOG_WARN = 1,
   EAGLE_LOG_ERROR = 2,
   EAGLE_LOG_CRITICAL = 3,
   EAGLE_LOG_NONE = 4
};



class EagleLogger {

friend class EagleLogGuard;

friend bool SendOutputToFile(const std::string& filepath , const std::string& header , bool append);

protected :
   
   CXX11Mutex mtx;
   
   EAGLE_LOGGING_LEVEL global_log_level;
   EAGLE_LOGGING_LEVEL old_global_log_level;
   EAGLE_LOGGING_LEVEL local_log_level;

   std::unordered_set<std::ostream*> outputs;


   EagleLogger& SetLocalLoggingLevel(EAGLE_LOGGING_LEVEL new_local_level);

   CXX11Mutex* Mutex();

   EagleLogger();
   ~EagleLogger() {}
   
   static EagleLogger& Instance();

   static EagleLogger& EagleInfoLog();
   static EagleLogger& EagleWarnLog();
   static EagleLogger& EagleErrorLog();
   static EagleLogger& EagleCriticalLog();

   static const char* LogPrefixStr(EAGLE_LOGGING_LEVEL l);
   
public :


   void SetGlobalLoggingLevel(EAGLE_LOGGING_LEVEL new_global_level);

   void TurnLogOff();
   void TurnLogOn();

   void AddOutput(std::ostream& output);
   void RemoveOutput(std::ostream& output);

   /* CREDITS : A kind shout out to relpatseht who helped me get std::endl to work with my logging class */

   typedef std::ostream&(*MANIP)(std::ostream&);

   EagleLogger& operator<<(MANIP manip);

   template<class Type>
   EagleLogger& operator<<(const Type& t);

   operator std::ostream&();
   
};




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




class EagleLogGuard {


private :

   EagleLogger& log;
   CXX11Mutex* pmutex;

   explicit EagleLogGuard(EagleLogger& logger , const char* prefix);
   
   EagleLogGuard(const EagleLogGuard& rhs);
   
   EagleLogGuard& operator=(const EagleLogGuard& rhs);
   
public :

   ~EagleLogGuard();

   typedef std::ostream&(*MANIP)(std::ostream&);

   EagleLogGuard& operator<<(MANIP manip);

   template<class Type>
   EagleLogGuard& operator<<(const Type& t);

   static EagleLogGuard EagleGuardLog();
   static EagleLogGuard EagleGuardInfo();
   static EagleLogGuard EagleGuardWarn();
   static EagleLogGuard EagleGuardError();
   static EagleLogGuard EagleGuardCritical();
   
   static EagleLogGuard EagleGuardPrefix(const char* prefix);

   friend EagleLogGuard EagleLog();
   friend EagleLogGuard EagleInfo();
   friend EagleLogGuard EagleWarn();
   friend EagleLogGuard EagleError();
   friend EagleLogGuard EagleCritical();

   friend EagleLogGuard EaglePrefix(const char* prefix);

   friend class EagleObject;
   friend EagleLogGuard& operator<<(EagleLogGuard& guard , const EagleObject& obj);
};


EagleLogGuard EagleLog();
EagleLogGuard EagleInfo();
EagleLogGuard EagleWarn();
EagleLogGuard EagleError();
EagleLogGuard EagleCritical();

EagleLogGuard EaglePrefix(const char* prefix);




template<class Type>
EagleLogGuard& EagleLogGuard::operator<<(const Type& t) {
   log << t;
   return *this;
}



EagleLogGuard& operator<<(EagleLogGuard& guard , const EagleObject& obj);




#endif // Logging_H





