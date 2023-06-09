
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
   typemap.insert(std::pair<RESOURCE_TYPE , std::set<std::string> >(RT_TEXTFILE, {"txt"}));
   
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



bool ResourceLibrary::LoadFileResource(std::shared_ptr<File> file) {
   File* f = file.get();
   EAGLE_ASSERT(f);
   if (!f) {
      EagleError() << "NULL file in LoadFileResource" << std::endl;
      return false;
   }
   FSInfo fsinfo = f->Info();
   if (!fsinfo.Exists()) {
      EagleError() << "File " << fsinfo.Path() << " does not exist." << std::endl;
      return false;
   }
   else if (!fsinfo.Mode().CanRead()) {
      EagleError() << "File " << fsinfo.Path() << " can not be read." << std::endl;
      return false;
   }
   else if (!fsinfo.Mode().IsFile()) {
      EagleError() << "FS Object " << fsinfo.Path() << " is not a file." << std::endl;
      return false;
   }
   std::string ext = f->Ext();
   RESOURCE_TYPE restype = GetResourceType(ext);
   ResourceBase* res = 0;
   bool success = true;
   if (restype > RT_UNKNOWN && restype < NUM_RT_TYPES) {
      switch(restype) {
      case RT_IMAGE :
         {
            ImageResource* ires = new ImageResource();
            ires->SetOurWindow(window);
            res = ires;
         }
         break;
      case RT_FONT :
         {
            FontResource* fres = new FontResource();
            fres->SetOurWindow(window);
            res = fres;
         }
         break;
      case RT_AUDIO :
         res = new AudioResource();
         break;
      case RT_VIDEO :
         res = new VideoResource();
         break;
      case RT_ARCHIVE :
         EAGLE_ASSERT(restype != RT_ARCHIVE);/// We don't treat archives like regular files, they are dealt with separately
         break;
      case RT_BINFILE :
         res = new BinaryResource();
         break;
      case RT_TEXTFILE :
         res = new TextResource();
         break;
      default :
         EAGLE_ASSERT(0);
         break;
      }
      bool fileloaded = res->LoadFromFile((FilePath)file->Path());
      success = success && fileloaded;
      if (!fileloaded) {
         delete res;
         EagleError() << "Failed to load resource " << file->Path() << std::endl;
         return false;
      }
      else {
         /// Save references to the File and ResourceBase* of the resource
         resmap[res->RID()] = res;
         lookupmap.insert(std::pair<std::string , RESOURCEID>(file->Path() , res->RID()));
      }
   }
   else {
      EagleError() << "Failed to deduce resource type for " << restype << std::endl;
      return false;
   }
   return success;
}



bool ResourceLibrary::LoadFolderResource(std::shared_ptr<Folder> folder , bool descend) {
   Folder* fl = folder.get();

   EAGLE_ASSERT(fl);
   if (!fl) {
      EagleError() << "NULL folder in LoadFolderResource" << std::endl;
      return false;
   }
   
   FSInfo fsinfo = fl->Info();
   
   if (!fsinfo.Exists()) {
      EagleError() << "Folder " << fsinfo.Path() << " does not exist." << std::endl;
      return false;
   }
   else if (!fsinfo.Mode().CanRead()) {
      EagleError() << "Folder " << fsinfo.Path() << " can not be read." << std::endl;
      return false;
   }
   else if (fsinfo.Mode().IsFile()) {
      EagleError() << "FS Object " << fsinfo.Path() << " is not a folder." << std::endl;
      return false;
   }

   bool success = true;

   Folder::FILEMAP fmap = fl->Files();

   for (Folder::FILEMAP::iterator it = fmap.begin() ; it != fmap.end() ; it++) {
      success = success && LoadFileResource(it->second);
   }

   if (descend) {
      Folder::SUBFOLDERMAP flmap = fl->SubFolders();
      for (Folder::SUBFOLDERMAP::iterator it = flmap.begin() ; it != flmap.end() ; ++it) {
         success = success && LoadFolderResource(it->second , descend);
      }
   }
   
   Folder::ARCHIVEMAP arcmap = fl->Archives();
   for (Folder::ARCHIVEMAP::iterator it = arcmap.begin() ; it != arcmap.end() ; ++it) {
      success = success && LoadArchiveResource(it->second);
   }
   
   return success;
}



bool ResourceLibrary::LoadArchiveResource(std::shared_ptr<ArchiveFile> archive) {
   ArchiveFile* arc = archive.get();
   EAGLE_ASSERT(arc);
   if (!arc) {
      EagleError() << "NULL archive in LoadFileResource" << std::endl;
      return false;
   }
   
   FSInfo fsinfo = arc->File::Info();
   
   if (!fsinfo.Exists()) {
      EagleError() << "Archive " << fsinfo.Path() << " does not exist." << std::endl;
      return false;
   }
   else if (!fsinfo.Mode().CanRead()) {
      EagleError() << "Archive " << fsinfo.Path() << " can not be read." << std::endl;
      return false;
   }
   else if (!fsinfo.Mode().IsFile()) {
      EagleError() << "FS Object " << fsinfo.Path() << " is not an archive file." << std::endl;
      return false;
   }
   EagleSystem* sys = (window?window->GetSystem():0);
   if (!sys) {
      sys = Eagle::EagleLibrary::System("Any");
   }
   EAGLE_ASSERT(sys);
   FileSystem* fs = sys->GetFileSystem();
   EAGLE_ASSERT(fs);
   

   bool mount = fs->MountArchive(FilePath(fsinfo.Path()));
   std::shared_ptr<Folder> root = fs->ReadFolder(FilePath("./") , true);
   bool success = mount && LoadFolderResource(root);
   if (mount) {
      fs->UnmountArchive();
   }
   return success;
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



