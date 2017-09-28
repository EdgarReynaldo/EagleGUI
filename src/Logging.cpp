
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
   local_log_level = new_local_level;
   return *this;
}



EagleLogger::EagleLogger() :
      global_log_level(EAGLE_LOG_INFO),
      old_global_log_level(EAGLE_LOG_INFO),
      local_log_level(EAGLE_LOG_INFO),
      outputs()
{

   AddOutput(cout);

#ifndef EAGLE_LOG
   TurnLogOff();
#endif
}



EagleLogger& EagleLogger::Instance() {
   static EagleLogger logger;
   return logger;
}



void EagleLogger::SetGlobalLoggingLevel(EAGLE_LOGGING_LEVEL new_global_level) {
   global_log_level = new_global_level;
}



void EagleLogger::TurnLogOff() {
   old_global_log_level = global_log_level;
   SetGlobalLoggingLevel(EAGLE_LOG_NONE);
}



void EagleLogger::TurnLogOn() {
   SetGlobalLoggingLevel(old_global_log_level);
}



void EagleLogger::AddOutput(ostream& output) {
   outputs.insert(&output);
}



void EagleLogger::RemoveOutput(ostream& output) {
   outputs.erase(&output);
}



EagleLogger& EagleLogger::operator<<(MANIP manip) {

   if (manip == std::endl) {
      cout << "Detected std::endl;" << std::endl;
   }

   if (local_log_level >= global_log_level) {
      for (std::unordered_set<std::ostream*>::iterator it = outputs.begin() ; it != outputs.end() ; ++it) {
         std::ostream& os = *(*it);
         os << manip;
      }
   }
   return *this;
}



/// ------------------------     Global Eagle Log functions     -----------------------




EagleLogger& EagleLog() {
   return EagleInfo();
}



EagleLogger& EagleInfo() {
   return EagleLogger::Instance().SetLocalLoggingLevel(EAGLE_LOG_INFO);
}



EagleLogger& EagleWarn() {
   return EagleLogger::Instance().SetLocalLoggingLevel(EAGLE_LOG_WARN);
}



EagleLogger& EagleError() {
   return EagleLogger::Instance().SetLocalLoggingLevel(EAGLE_LOG_ERROR);
}



EagleLogger& EagleCritical() {
   return EagleLogger::Instance().SetLocalLoggingLevel(EAGLE_LOG_CRITICAL);
}



EagleLogger::operator ostream&() {
   return log_file.log_file;
}



/** #################            Indenter class methods               ################# */



void Indenter::ResetSpaces() {
   indent.clear();
   indent.append(indent_level*num_spaces , ' ');
}



Indenter::Indenter() :
      indent_level(0) , num_spaces(3) , indent("")
   {}



Indenter::Indenter(int level , int spaces) :
      indent_level(0) , num_spaces(3) , indent() {
   if (level < 0) {level = 0;}
   if (spaces < 1) {spaces = 1;}
   indent_level = level;
   num_spaces = spaces;
   ResetSpaces();
}



void Indenter::SetLevel(unsigned int indentation_level) {
   indent_level = indentation_level;
   ResetSpaces();
}



void Indenter::SetSpaces(unsigned int number_of_spaces) {
   num_spaces = number_of_spaces;
   ResetSpaces();
}


/// Prefix
Indenter& Indenter::operator++() {
   ++indent_level;
   ResetSpaces();
   return *this;
}



Indenter& Indenter::operator--() {
   if (indent_level) {
      --indent_level;
      ResetSpaces();
   }
   return *this;
}


/// Postfix
Indenter Indenter::operator++(int) {
   Indenter copy = *this;
   ++indent_level;
   ResetSpaces();
   return copy;
}



Indenter Indenter::operator--(int) {
   Indenter copy = *this;
   if (indent_level) {
      --indent_level;
      ResetSpaces();
   }
   return copy;
}



ostream& operator<<(ostream& os , const Indenter& i) {
   os << i.indent.c_str();
   return os;
}




