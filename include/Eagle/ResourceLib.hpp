
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

#include "Eagle/SharedMemory.hpp"



class ResourceLibrary {
public :
   typedef std::map<RESOURCEID , SHAREDOBJECT<ResourceBase> > RESMAP;
   typedef RESMAP::iterator RMIT;
   typedef std::map<RESOURCE_TYPE , std::set<std::string> > TYPEMAP;

protected :
   RESMAP resmap;
   TYPEMAP typemap;

public :
   
///   ResourceLibrary();
   ResourceLibrary() :
         resmap(),
         typemap()
   {}
   virtual ~ResourceLibrary();
   
   void FreeResources();
   
   std::set<std::string> GetSupportedTypes(RESOURCE_TYPE rt);
   
   RESOURCE_TYPE DeduceResourceType(std::string ext);

   SHAREDOBJECT<ResourceBase> GetResource(RESOURCEID rid);

///   virtual RESOURCEID LoadResourceFromFile(FilePath fp)=0;
///   virtual RESOURCEID LoadResourceFromMemory(const MemFile* memfile)=0;
};

