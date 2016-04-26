
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


class FakeOstream {
   
public :
   template <class Type>
   FakeOstream& operator<<(Type t) {(void)t;return *this;}
   
   /* CREDITS : A kind shout out to relpatseht who helped me get std::endl to work with the FakeOstream class */
   FakeOstream& operator<<(std::ostream& (*)(std::ostream&)) { return *this; }   
};


std::ostream& OutputLog();/// Returns a single global output destination for logging

FakeOstream& FakeLog();/// Dummy ostream to turn the log off.

#ifdef LOG
   #define EagleLog() OutputLog()
#else
   #define EagleLog() FakeLog()
#endif



/// These alter the destination of the output log returned by OutputLog()
bool SendOutputToFile(const std::string& filepath , const std::string& header , bool append = true);
void SendOutputTo(std::ostream& output_stream);



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





