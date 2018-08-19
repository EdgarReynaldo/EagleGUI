
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



#include "Eagle/Logging.hpp"
#include "Eagle/Exception.hpp"


#include <fstream>


using std::string;
using std::ostream;
using std::ofstream;
using std::ios;
using std::cout;
using std::endl;


class LogFile {
public :
   ofstream log_file;
   ~LogFile();

};

LogFile::~LogFile() {
   log_file.close();
}

LogFile log_file;

bool SendOutputToFile(const std::string& filepath , const std::string& header , bool append) {
   EagleLogger::Instance().RemoveOutput(log_file.log_file);
   log_file.log_file.close();
   log_file.log_file.clear();
   ios::openmode mode = ios::out;
   if (append) {mode |= ios::app;}
   log_file.log_file.open(filepath.c_str() , mode);
   bool success = log_file.log_file.good();
   if (success) {
      EagleLogger::Instance().AddOutput(log_file.log_file);
      log_file.log_file << header.c_str() << endl;
   }
   return success;
}



/// -----------------------     EagleLogger class     ---------------------------------



EagleLogger& EagleLogger::SetLocalLoggingLevel(EAGLE_LOGGING_LEVEL new_local_level) {
   if (new_local_level == EAGLE_LOG_NONE) {
      new_local_level = EAGLE_LOG_CRITICAL;
   }
   LockMutex(&mtx);
   local_log_level = new_local_level;
   UnLockMutex(&mtx);
   return *this;
}



CXX11Mutex* EagleLogger::Mutex() {
   return &mtx;
}



EagleLogger::EagleLogger() :
      mtx("EagleLogMutex" , false),
      global_log_level(EAGLE_LOG_INFO),
      old_global_log_level(EAGLE_LOG_INFO),
      local_log_level(EAGLE_LOG_INFO),
      outputs()
{
   if (!mtx.Create(true , false)) {
      throw EagleException("Failed to create EagleLogger::mtx mutex");
   }

   AddOutput(cout);

#ifndef EAGLE_LOG
   TurnLogOff();
#endif
}



EagleLogger& EagleLogger::Instance() {
   static EagleLogger logger;
   return logger;
}



EagleLogger& EagleLogger::EagleInfoLog() {
   return EagleLogger::Instance().SetLocalLoggingLevel(EAGLE_LOG_INFO);
}



EagleLogger& EagleLogger::EagleWarnLog() {
   return EagleLogger::Instance().SetLocalLoggingLevel(EAGLE_LOG_WARN);
}



EagleLogger& EagleLogger::EagleErrorLog() {
   return EagleLogger::Instance().SetLocalLoggingLevel(EAGLE_LOG_ERROR);
}



EagleLogger& EagleLogger::EagleCriticalLog() {
   return EagleLogger::Instance().SetLocalLoggingLevel(EAGLE_LOG_CRITICAL);
}



const char* EagleLogger::LogPrefixStr(EAGLE_LOGGING_LEVEL l) {
   static const char* loglevelstrs[5] = {
      "",
      "EAGLE WARN     : ",
      "EAGLE ERROR    : ",
      "EAGLE CRITICAL : ",
      "EAGLE LOG NONE : "
   };
   return loglevelstrs[l];
}



void EagleLogger::SetGlobalLoggingLevel(EAGLE_LOGGING_LEVEL new_global_level) {
   LockMutex(&mtx);
   global_log_level = new_global_level;
   UnLockMutex(&mtx);
}



void EagleLogger::TurnLogOff() {
   LockMutex(&mtx);
   old_global_log_level = global_log_level;
   UnLockMutex(&mtx);
   SetGlobalLoggingLevel(EAGLE_LOG_NONE);
}



void EagleLogger::TurnLogOn() {
   LockMutex(&mtx);
   SetGlobalLoggingLevel(old_global_log_level);
   UnLockMutex(&mtx);
}



void EagleLogger::AddOutput(ostream& output) {
   LockMutex(&mtx);
   outputs.insert(&output);
   UnLockMutex(&mtx);
}



void EagleLogger::RemoveOutput(ostream& output) {
   LockMutex(&mtx);
   outputs.erase(&output);
   UnLockMutex(&mtx);
}



void EnableLog() {
   EagleLogger::Instance().TurnLogOn();
}



void DisableLog() {
   EagleLogger::Instance().TurnLogOff();
}



EagleLogger& EagleLogger::operator<<(MANIP manip) {
/*
   if (manip == std::endl) {
      cout << "Detected std::endl;" << std::endl;
   }
*/
   if (local_log_level >= global_log_level) {
      for (std::unordered_set<std::ostream*>::iterator it = outputs.begin() ; it != outputs.end() ; ++it) {
         std::ostream& os = *(*it);
         os << manip;
      }
   }
   return *this;
}



/// ------------------------     EagleLogGuard     -------------------------------




EagleLogGuard::EagleLogGuard(EagleLogger& logger , const char* prefix) :
      log(logger),
      pmutex(log.Mutex())
{
   EAGLE_ASSERT(pmutex);
   LockMutex(pmutex);
   log << prefix;
}



EagleLogGuard::EagleLogGuard(const EagleLogGuard& rhs) :
      log(rhs.log),
      pmutex(0)
{
///   static_assert(false , "EagleLogGuard's are not copyable!");
   EAGLE_ASSERT(false);
}



EagleLogGuard& EagleLogGuard::operator=(const EagleLogGuard& rhs) {
///   static_assert(false , "EagleLogGuard's are not assignable!");
   (void)rhs;
   EAGLE_ASSERT(false);
   return *this;
}



EagleLogGuard::~EagleLogGuard() {
   UnLockMutex(pmutex);
}




EagleLogGuard& EagleLogGuard::operator<<(MANIP manip) {
   log << manip;
   return *this;
}



EagleLogGuard EagleLogGuard::EagleGuardLog() {
   return EagleLogGuard(EagleLogger::EagleInfoLog() , EagleLogger::LogPrefixStr(EAGLE_LOG_INFO));
}



EagleLogGuard EagleLogGuard::EagleGuardInfo() {
   return EagleLogGuard(EagleLogger::EagleInfoLog() , EagleLogger::LogPrefixStr(EAGLE_LOG_INFO));
}



EagleLogGuard EagleLogGuard::EagleGuardWarn() {
   return EagleLogGuard(EagleLogger::EagleWarnLog() , EagleLogger::LogPrefixStr(EAGLE_LOG_WARN));
}



EagleLogGuard EagleLogGuard::EagleGuardError() {
   return EagleLogGuard(EagleLogger::EagleErrorLog() , EagleLogger::LogPrefixStr(EAGLE_LOG_ERROR));
}



EagleLogGuard EagleLogGuard::EagleGuardCritical() {
   return EagleLogGuard(EagleLogger::EagleCriticalLog() , EagleLogger::LogPrefixStr(EAGLE_LOG_CRITICAL));
}



EagleLogGuard EagleLogGuard::EagleGuardPrefix(const char* prefix) {
   return EagleLogGuard(EagleLogger::EagleCriticalLog() , prefix);
}


/// ----------------------     Global functions     ---------------------------------




EagleLogGuard EagleLog() {
   return EagleLogGuard::EagleGuardLog();
}



EagleLogGuard EagleInfo() {
   return EagleLogGuard::EagleGuardInfo();
}


EagleLogGuard EagleWarn() {
   return EagleLogGuard::EagleGuardWarn();
}



EagleLogGuard EagleError() {
   return EagleLogGuard::EagleGuardError();
}



EagleLogGuard EagleCritical() {
   return EagleLogGuard::EagleGuardCritical();
}



EagleLogGuard EaglePrefix(const char* prefix) {
   return EagleLogGuard::EagleGuardPrefix(prefix);
}





/// ------------------------     Global Eagle Log functions     -----------------------



EagleLogger::operator ostream&() {
   return log_file.log_file;
}




EagleLogGuard& operator<<(EagleLogGuard& guard , const EagleObject& obj) {
   std::ostream& os = (std::ostream&)(guard.log);
   os << obj;
   return guard;
}


