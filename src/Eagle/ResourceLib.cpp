
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
 *    Copyright 2009-2023+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */

 
 
#include "Eagle/Resources.hpp"
#include "Eagle/ResourceLib.hpp"
#include "Eagle/File.hpp"

#include "Eagle/Lib.hpp"
#include "Eagle/System.hpp"
#include "Eagle/FileSystem.hpp"
#include "Eagle/Logging.hpp"

#include <memory>



/// ---------------------      ResourceLibrary     ------------------------



ResourceLibrary::TYPEMAP ResourceLibrary::typemap;



ResourceLibrary* ResourceLibrary::ResLib() {
   static ResourceLibrary reslib((EagleGraphicsContext*)0);
   return &reslib;
}



void ResourceLibrary::RegisterRID(RESOURCEID rid , ResourceBase* resource) {
   EAGLE_ASSERT(rid != BADRESOURCEID);
   EAGLE_ASSERT(resource);
   RMIT it = resmap.find(rid);
   if (it != resmap.end()) {
      /// rid on the resource map
      EAGLE_ASSERT(it->second == resource);/// double registration
   }
   else {
      /// Not on map, add it in
      resmap[rid] = resource;
   }
   
}



void ResourceLibrary::UnregisterRID(RESOURCEID rid) {
   RMIT it = resmap.find(rid);
   if (it != resmap.end()) {
      resmap[rid] = 0;
   }
}




ResourceLibrary::ResourceLibrary(EagleGraphicsContext* win) :
      resmap(),
      lookupmap(),
      window(0)
{
   
   typemap.insert(std::pair<RESOURCE_TYPE , std::set<std::string> >(RT_IMAGE   , {"bmp" , "png" , "jpg" , "tga"}));
   typemap.insert(std::pair<RESOURCE_TYPE , std::set<std::string> >(RT_FONT    , {"ttf"}));
   typemap.insert(std::pair<RESOURCE_TYPE , std::set<std::string> >(RT_AUDIO   , {"wav" , "ogg"}));
   typemap.insert(std::pair<RESOURCE_TYPE , std::set<std::string> >(RT_VIDEO   , {"ogv"}));
   typemap.insert(std::pair<RESOURCE_TYPE , std::set<std::string> >(RT_ARCHIVE , {"zip" , "7z" , "tar"}));
   typemap.insert(std::pair<RESOURCE_TYPE , std::set<std::string> >(RT_BINFILE , {"dat" , "bin"}));
   typemap.insert(std::pair<RESOURCE_TYPE , std::set<std::string> >(RT_TEXTFILE, {"txt" , "log"}));
   
   SetWindow(win);
}



ResourceLibrary::~ResourceLibrary() {
   FreeResources();
}



void ResourceLibrary::SetWindow(EagleGraphicsContext* win) {
   if (win) {
      if (window) {
         win->AdoptBitmaps(window);/// TODO : Make the new window adopt the bitmaps so they are compatible with the newly specified display
      }
      window = win;
   }
}



bool ResourceLibrary::LoadResourcesFromConfig(const ConfigFile& cfg) {

   EagleSystem* sys = Eagle::EagleLibrary::System("Any");
   EAGLE_ASSERT(sys);
   FileSystem* fs = sys->GetFileSystem();
   EAGLE_ASSERT(fs);
   
   const ConfigSection* cs = 0;
   try {
      cs = &cfg["Resources"];
   }
   catch(...) {
      EagleWarn() << "Resources section not found in config file. No resources to load." << std::endl;
      return false;
   }
   
   std::vector<std::string> keys = cs->GetKeys();
   bool success = true;

   for (unsigned int i = 0 ; i < keys.size() ; ++i) {
      std::string key = keys[i];
      const ConfigLine* cl = cs->FindConfigConst(key);
      if (!cl) {
         EagleWarn() << "No config line found for key '" << key << "'" << std::endl;
         continue;
      }
      if (cl->IsKeyValuePair()) {
         std::string resource = cl->Value();
         std::shared_ptr<File> pfile = fs->ReadFile(FilePath(resource));
         if (!pfile.get()) {
            EagleError() << "Could not get resource file from the filesystem. (" << resource << ")" << std::endl;
            continue;
         }
         std::shared_ptr<ArchiveFile> arcfile(pfile.get()->IsArchive()?new ArchiveFile(pfile.get()->Info()):0);
         bool floaded = true;
         if (arcfile.get()) {
            floaded = floaded && LoadArchiveResource(arcfile);
         }
         else {
            floaded = floaded && LoadFileResource(pfile);
         }
         success = success && floaded;
         if (!floaded) {
            EagleError() << "Failed to load resource for path '" << resource << "'" << std::endl;
            continue;
         }
         ResourceBase* rbase = LookupResourceByPath(FilePath(resource).Path());
         EAGLE_ASSERT(rbase);
         if (!rbase) {
            EagleError() << "Failed to look up resource by path in resource library for path'" << FilePath(resource).Path() << "'" << std::endl;
            continue;
         }
         lookupmap[key] = rbase->RID();
      }
   }
   return success;
}



void ResourceLibrary::FreeResource(RESOURCEID rid) {
   if (resmap[rid]) {
      delete resmap[rid];
      resmap[rid] = 0;
   }
}



void ResourceLibrary::FreeResources() {
   
   RMIT it = resmap.begin();
   while (it != resmap.end()) {
      ResourceBase* rbase = it->second;
      if (rbase) {
         delete rbase;/// Resourcebase destructor unregisters itself with us
      }
///      it->second = 0;
      ++it;
   }
   resmap.clear();
   lookupmap.clear();
}



std::set<std::string> ResourceLibrary::GetSupportedTypes(RESOURCE_TYPE rt) {
   if (typemap.find(rt) != typemap.end()) {
      return typemap[rt];
   }
   return std::set<std::string>();
}



RESOURCE_TYPE ResourceLibrary::GetResourceType(std::string ext) {
   
   /// Case insensitive
   for (size_t i = 0 ; i < ext.size() ; i++) {
      ext[i] = tolower(ext[i]);
   }
   for (int i = RT_IMAGE ; i < NUM_RT_TYPES ; ++i) {
      std::set<std::string> rset = GetSupportedTypes((RESOURCE_TYPE)i);
      if (rset.find(ext) != rset.end()) {
         return (RESOURCE_TYPE)i;
      }
   }
   return RT_UNKNOWN;
}



RESOURCEID ResourceLibrary::LookupResourceID(std::string path) {
   LMIT lit = lookupmap.find(path);
   if (lit != lookupmap.end()) {
      return lit->second;
   }
   return BADRESOURCEID;
}



ResourceBase* ResourceLibrary::LookupResourceByID(RESOURCEID rid) {
   RMIT rit = resmap.find(rid);
   if (rit != resmap.end()) {
      return rit->second;
   }
   return 0;
}



ResourceBase* ResourceLibrary::LookupResourceByPath(std::string path) {
   return LookupResourceByID(LookupResourceID(path));
}



