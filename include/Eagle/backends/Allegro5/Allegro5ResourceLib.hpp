
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
 *    Copyright 2009-2024+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Allegro5ResourceLib.hpp
 * @brief The Allegro 5 implementation of the ResourceLibrary
 */

#ifndef Allegro5ResourceLib_HPP
#define Allegro5ResourceLib_HPP


#include "Eagle/ResourceLib.hpp"


#include <set>
#include <vector>



/**! @class Allegro5ResourceLibrary
 *   @brief The Allegro 5 implementation of the ResourceLibrary. To use, call System::GetResourceLibrary or Allegro5ResourceLibrary::Instance().
 */

class Allegro5ResourceLibrary : public ResourceLibrary {

protected :
   
   Allegro5ResourceLibrary();
   
public :
   ~Allegro5ResourceLibrary() {}

   static Allegro5ResourceLibrary* Instance();


   virtual std::set<std::string> GetSupportedTypes(RESOURCE_TYPE rt);

   virtual RESOURCEID LoadFileResource(File* file);
   virtual std::vector<RESOURCEID> LoadFolderResource(Folder* folder , bool descend = false);
   virtual std::vector<RESOURCEID> LoadArchiveResource(ArchiveFile* archive);
};



#endif // Allegro5ResourceLib_HPP



