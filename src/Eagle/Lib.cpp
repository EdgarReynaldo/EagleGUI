
/**
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
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#include <cstdlib>

#include "Eagle/Lib.hpp"
#include "Eagle/StringWork.hpp"



using namespace Eagle;




EagleLibrary* EagleLibrary::eagle_lib = 0;



EagleLibrary::EagleLibrary() : 
      sys_creation_map(),
      sys_map()
{
///   atexit(ShutdownEagle);
}



EagleLibrary::~EagleLibrary() {
///   Shutdown();
}



void EagleLibrary::Shutdown() {
   if (eagle_lib) {
      for (SMIT it = sys_map.begin() ; it != sys_map.end() ; ++it) {
         delete it->second;
      }
      sys_map.clear();

      delete eagle_lib;
      eagle_lib = 0;
   }
}



void EagleLibrary::ShutdownEagle() {
   printf("**** SHUTDOWN EAGLE ******\n");
///   EagleInfo() << "Shutting down Eagle in atexit" << std::endl;
   Eagle()->Shutdown();
}



EagleLibrary* EagleLibrary::Eagle() {
   if (!eagle_lib) {
      eagle_lib = new EagleLibrary();
   }
   return eagle_lib;
}



int EagleLibrary::RegisterSystemCreator(std::string driver , SYS_CREATION_FUNC sys_create_func) {
   Eagle()->sys_creation_map[driver] = sys_create_func;
   return 1;
}



EagleSystem* EagleLibrary::System(std::string driver) {
   
   if (driver.compare("Any") == 0) {
      for (SMIT it = Eagle()->sys_map.begin() ; it != Eagle()->sys_map.end() ; ++it) {
         EagleSystem* s = it->second;
         return s;
      }
      return 0;
   }
   
   SMIT it = Eagle()->sys_map.find(driver);
   
   if (it != Eagle()->sys_map.end()) {
      return it->second;
   }
   else {
      EagleInfo() << "Failed to find " << driver << " driver in library system map. Attempting creation." << std::endl;
   }
   
   SCMIT scmit = Eagle()->sys_creation_map.find(driver);
   
   EagleSystem* new_sys = 0;
   
   if (scmit != Eagle()->sys_creation_map.end()) {
      SYS_CREATION_FUNC syscreator = scmit->second;
      new_sys = syscreator();
      Eagle()->sys_map[driver] = new_sys;
   }
   else {
      throw EagleException(StringPrintF("Failed to create %s driver in EagleLibrary::System. No creation function registered." ,
                                         driver.c_str()));
   }
   
   return new_sys;
}
