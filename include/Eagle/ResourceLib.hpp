
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
 *    Copyright 2009-2025+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file ResourceLib.hpp
 * @brief The main interface for working with resources and resource files in Eagle
 * 
 * A lookup map between resource name, type, id, and resource pointer
 */

#ifndef ResourceLib_HPP
#define ResourceLib_HPP



#include <set>
#include <vector>
#include <string>
#include <memory>

#include "Eagle/Resources.hpp"
#include "Eagle/SharedMemory.hpp"
#include "Eagle/ConfigFile.hpp"




class ResourceLibrary {
public :
   typedef std::map<RESOURCEID , ResourceBase* > RESMAP;/// Resource ids are unique and we can track our memory with thme
   typedef RESMAP::iterator RMIT;

   typedef std::map<std::string , RESOURCEID> LOOKUPMAP;/// This is for getting a resource from a path, name, or string
   typedef LOOKUPMAP::iterator LMIT;

   typedef std::map<RESOURCE_TYPE , std::set<std::string> > TYPEMAP;

   
protected :
   RESMAP resmap;
   LOOKUPMAP lookupmap;
   static TYPEMAP typemap;

   EagleGraphicsContext* window;// optional, if null, memory bitmaps and fonts will be created
   
   void RegisterRID(RESOURCEID rid , ResourceBase* resource);
   void UnregisterRID(RESOURCEID rid);
   
   friend class ResourceBase;
   
public :
   
   ResourceLibrary(EagleGraphicsContext* win = 0);

   virtual ~ResourceLibrary();

   void SetWindow(EagleGraphicsContext* win);
   
   virtual std::vector<RESOURCEID> LoadResourcesFromConfig(const ConfigFile& cfg);
   
   virtual RESOURCEID LoadFileResource(File* file)=0;
   virtual std::vector<RESOURCEID> LoadFolderResource(Folder* folder , bool descend = false)=0;
   virtual std::vector<RESOURCEID> LoadArchiveResource(ArchiveFile* archive)=0;

   
   void FreeResource(RESOURCEID rid);
   
   void FreeResources();
   
   static std::set<std::string> GetSupportedTypes(RESOURCE_TYPE rt);
   
   static RESOURCE_TYPE GetResourceType(std::string ext);

   RESOURCEID LookupResourceID(std::string path);
   ResourceBase* LookupResourceByID(RESOURCEID rid);
   
   ResourceBase* LookupResourceByPath(std::string path);
};






///< Trick to force variant return type
template <class TYPE>
TYPE* GetTypedResource(ResourceBase* resbase , TYPE** store) {
   EAGLE_ASSERT(store);
   *store = dynamic_cast<TYPE*>(resbase);
   return *store;
}






#endif // ResourceLib_HPP



