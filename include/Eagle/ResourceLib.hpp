/**
 *
 *     _______       ___       ____      __       _______       _______
 *    /\  ____\    /|   \     /  __\    /\ \     /\  ____\     /\  ____\
 *    \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_    \ \ \_____
 *     \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\    \ \_____ \
 *      \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_    \/ ____\ \
 *       \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\     /\______\
 *        \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/     \/______/
 *
 *
 *    EAGLE 5
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2018+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */


#include <set>
#include <vector>
#include <string>
#include <memory>

#include "Eagle/Resources.hpp"

#include <memory>

class ResourceLibrary {
public :
   typedef std::map<RESOURCEID , std::shared_ptr<ResourceBase> > RESMAP;
   typedef RESMAP::iterator RMIT;
protected :
   RESMAP resmap;

public :
   
///   ResourceLibrary();
   ResourceLibrary() :
         resmap()
   {}
   
   virtual ~ResourceLibrary() {}
   
///   virtual RESOURCEID LoadResourceFromFile(FilePath fp , RESOURCE_TYPE rt = RT_UNKNOWN)=0;
   
   virtual std::set<std::string> GetSupportedTypes(RESOURCE_TYPE rt)=0;
   
   RESOURCE_TYPE DeduceResourceType(std::string ext);

   std::shared_ptr<ResourceBase> GetResource(RESOURCEID rid) {return resmap[rid];}
};

