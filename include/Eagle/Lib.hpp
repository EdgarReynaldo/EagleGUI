



#ifndef EAGLE_LIB_HPP
#define EAGLE_LIB_HPP


#include "Eagle/System.hpp"


namespace Eagle {

class EagleLibrary {
   
protected :

   typedef EagleSystem*(*SYS_CREATION_FUNC)(void);
   typedef std::map<std::string , SYS_CREATION_FUNC> SYS_CREATION_MAP;
   typedef SYS_CREATION_MAP::iterator SCMIT;
   
   typedef std::map<std::string , EagleSystem*> SYS_MAP;
   typedef SYS_MAP::iterator SMIT;
   
   SYS_CREATION_MAP sys_creation_map;
   SYS_MAP sys_map;
   
   static EagleLibrary* eagle_lib;
   static int create;
   static int destroy;
   
   EagleLibrary();
   
public :
   
   ~EagleLibrary();
   
   static EagleLibrary* Eagle();

   void Shutdown();
   
   static void ShutdownEagle();
   
   static int RegisterSystemCreator(std::string driver , SYS_CREATION_FUNC sys_create_func);

   
   static EagleSystem* System(std::string driver);

};

}/// namespace Eagle


#endif // EAGLE_LIB_HPP

