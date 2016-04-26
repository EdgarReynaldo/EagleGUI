
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


ofstream logfile;

ostream* ostr = &cout;

FakeOstream fakelog;



ostream& OutputLog() {return *ostr;}

FakeOstream& FakeLog() {return fakelog;}




bool SendOutputToFile(const std::string& filepath , const std::string& header , bool append) {
   logfile.close();
   logfile.clear();
   ios::openmode mode = ios::out;
   if (append) {mode |= ios::app;}
   logfile.open(filepath.c_str() , mode);
   bool success = logfile.good();
   if (success) {
      SendOutputTo(logfile);
      logfile << header.c_str() << endl;
   }
   return success;
}



void SendOutputTo(std::ostream& output_stream) {
   ostr = &output_stream;
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



/** ##############         Logger class methods           ################# */

/** TODO : FIXME?
   Not working correctly, see the header.

void Logger::SetMaxLoggingLevel(LogLevel m) {
   if (m < LOG_RESTORE) {
      max = m;
      enabled = (current <= max);
   }
}



void Logger::SetLoggingLevel(LogLevel ll) {
   if (ll == LOG_RESTORE) {
      SetLoggingLevel(previous);
   } else {
//   if (ll < LOG_RESTORE) {
      previous = current;
      current = ll;
      enabled = (current <= max);
   }
}



bool Logger::SendOutputToFile(const std::string& filepath , const std::string& header , bool append) {
   logfile.close();
   logfile.clear();
   ios::openmode mode = ios::out;
   if (append) {mode |= ios::app;}
   logfile.open(filepath.c_str() , mode);
   bool success = logfile.good();
   if (success) {
      SendOutputTo(logfile);
      logfile << header.c_str() << endl;
   }
   return success;
}



Logger OutputLog;

*/



