
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
 * @file Allegro5ResourceLibrary.hpp
 * @brief The Allegro 5 implementation of the ResourceLibrary
 */

#ifndef Allegro5ResourceLib_HPP
#define Allegro5ResourceLib_HPP


#include "Eagle/ResourceLib.hpp"


#include <set>
#include <vector>



/**! @class Allegro5ResourceLibrary
 *   @brief The Allegro 5 implementation of the ResourceLibrary
 */

class Allegro5ResourceLibrary : public ResourceLibrary {

protected :
   
   static Allegro5ResourceLibrary* me;
   
public :

   Allegro5ResourceLibrary();



   virtual std::set<std::string> GetSupportedTypes(RESOURCE_TYPE rt);

   
   static Allegro5ResourceLibrary* Instance() {return me;}
};



#endif // Allegro5ResourceLib_HPP



