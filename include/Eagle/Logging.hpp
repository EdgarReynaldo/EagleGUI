
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



#ifndef Logging_H
#define Logging_H



#include <string>
#include <iostream>
#include <fstream>
#include <unordered_set>





bool SendOutputToFile(const std::string& filepath , const std::string& header , bool append = true);



enum EAGLE_LOGGING_LEVEL {
   EAGLE_LOG_INFO = 0,
   EAGLE_LOG_WARN = 1,
   EAGLE_LOG_ERROR = 2,
   EAGLE_LOG_CRITICAL = 3,
   EAGLE_LOG_NONE = 4
};


class EagleLogger {
   
friend EagleLogger& EagleLog();
friend EagleLogger& EagleInfo();
friend EagleLogger& EagleWarn();
friend EagleLogger& EagleError();
friend EagleLogger& EagleCritical();

protected :
   EAGLE_LOGGING_LEVEL global_log_level;
   EAGLE_LOGGING_LEVEL old_global_log_level;
   EAGLE_LOGGING_LEVEL local_log_level;

   std::unordered_set<std::ostream*> outputs;
   

   EagleLogger& SetLocalLoggingLevel(EAGLE_LOGGING_LEVEL new_local_level);


   EagleLogger();
   

public :   

   static EagleLogger& Instance();

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



EagleLogger& EagleLog();
EagleLogger& EagleInfo();
EagleLogger& EagleWarn();
EagleLogger& EagleError();
EagleLogger& EagleCritical();





/// ------------------------      Indenter class      --------------------------------




class Indenter {
private :
   unsigned int indent_level;
   unsigned int num_spaces;
   std::string indent;

   void ResetSpaces();
   
public :
   Indenter();
   Indenter(int level , int spaces);

   void SetLevel(unsigned int indentation_level);
   void SetSpaces(unsigned int number_of_spaces);

   
   /// Prefix increment and decrement operators for changing the indentation level.
   Indenter& operator++();
   Indenter& operator--();
   /// Postfix - returns copy and increments indenter
   Indenter operator++(int);
   Indenter operator--(int);

   friend std::ostream& operator<<(std::ostream& os , const Indenter& i);
};



#endif // Logging_H





