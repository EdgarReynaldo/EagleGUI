
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
 * @file Lib.hpp
 * @brief The main interface for the Eagle Library
 */




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
   
   /**! @var sys_creation_map
    *   @brief A map of system driver creation functions
    */
   
   SYS_CREATION_MAP sys_creation_map;

   /**! @var sys_map
    *   @brief A map of the system drivers
    */
   
   SYS_MAP sys_map;
   
   static EagleLibrary* eagle_lib;///< The real eagle library
   
   EagleLibrary();///< Private constructor
   
public :
   
   ~EagleLibrary();///< Destructor
   
   ///< Get a pointer to the EagleLibrary
   static EagleLibrary* Eagle();

   ///< Member function to shutdown eagle
   void Shutdown();
   
   ///< Shutdown eagle completely, shortcut to @ref EagleLibrary::Shutdown
   static void ShutdownEagle();
   
   ///< For system drivers to register themself with the eagle library
   static int RegisterSystemCreator(std::string driver , SYS_CREATION_FUNC sys_create_func);

   
   /**! @fn System <std::string>
    *   @brief Get the system for the specified driver string
    *    
    *   Driver strings supported are : "Allegro5"
    */
   
   static EagleSystem* System(std::string driver);

};

}/// namespace Eagle



#endif // EAGLE_LIB_HPP



