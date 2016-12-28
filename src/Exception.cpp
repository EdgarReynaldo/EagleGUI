
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
 *    Copyright 2009-2016+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#include "Eagle/Exception.hpp"

#include <cassert>



void LogFailedAssert(const char* exp , const char* file , int line , const char* func) {
   EagleError() << "Assert(" << exp << ") failed at line " << line << " of " << file << " in function " << func << "." << std::endl;
}



void LogFailedAssertAndFail(const char* exp , const char* file , int line , const char* func) {
   LogFailedAssert(exp , file , line , func);
   ASSERT_EXCEPTION();
}



void EpicFail() {
   int i = 1/int(0);
   (void)i;
   assert(0);
}



/// -------------------------      EagleException      ----------------------------



EagleException::EagleException(const std::exception e) : error(e.what()) {
   EagleError() << e.what() << std::endl;
   ASSERT_EXCEPTION();
}



EagleException::EagleException(const std::string& e) : error(e) {
   EagleError() << e << std::endl;
   ASSERT_EXCEPTION();
}



EagleException::EagleException(const char* e) : error(e) {
   EagleError() << e << std::endl;
   ASSERT_EXCEPTION();
}


