
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
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */

 
 
#include "Eagle/ResourceLib.hpp"
#include "Eagle/Logging.hpp"



RESOURCEID NextRid() {
   static RESOURCEID rid = 0;
   return rid++;
}



/// ---------------------      ResourceLibrary     ------------------------



ResourceLibrary::~ResourceLibrary() {
   FreeResources();
   typemap.clear();
}



void ResourceLibrary::FreeResources() {
   resmap.clear();
}



std::set<std::string> ResourceLibrary::GetSupportedTypes(RESOURCE_TYPE rt) {
   if (typemap.find(rt) != typemap.end()) {
      return typemap[rt];
   }
   return std::set<std::string>();
}



RESOURCE_TYPE ResourceLibrary::DeduceResourceType(std::string ext) {
   for (int i = RT_IMAGE ; i < NUM_RT_TYPES ; ++i) {
      std::set<std::string> rset = GetSupportedTypes((RESOURCE_TYPE)i);
      if (rset.find(ext) != rset.end()) {
         return (RESOURCE_TYPE)i;
      }
   }
   return RT_UNKNOWN;
}



SHAREDOBJECT<ResourceBase> ResourceLibrary::GetResource(RESOURCEID rid) {
   if (resmap.find(rid) != resmap.end()) {
      return resmap[rid];
   }
   return SHAREDOBJECT<ResourceBase>();
}





