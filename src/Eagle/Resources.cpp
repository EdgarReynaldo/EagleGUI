
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
#include "Eagle/Lib.hpp"
#include "Eagle/Exception.hpp"
#include "Eagle/System.hpp"
#include "Eagle/FileSystem.hpp"
#include "Eagle/File.hpp"
#include "Eagle/ResourceLib.hpp"
#include "Eagle/GraphicsContext.hpp"



#include <atomic>



const RESOURCEID BADRESOURCEID = ~0UL;



unsigned int NextRid() {
   static std::atomic_int rid(-1);
   return (unsigned int)(++rid);
}



/// ---------------------      ResourceBase     ------------------------



ResourceBase::ResourceBase(const ResourceBase& r) :
   owner(0),
   rid(BADRESOURCEID),
   rtype(RT_UNKNOWN),
   loaded(false),
   filepath("")
{
   (void)r;
   throw EagleException("ResourceBase::ResourceBase - tried to copy a ResourceBase object.");
}



ResourceBase& ResourceBase::operator=(const ResourceBase& r) {
   (void)r;
   throw EagleException("Tried to assign ResourceBase object.");
   return *this;
}


ResourceBase::ResourceBase(RESOURCE_TYPE rt) :
      owner(Eagle::EagleLibrary::System("Any")->GetResourceLibrary()),
      rid(NextRid()),
      rtype(rt),
      loaded(false),
      filepath("")
{
   owner->RegisterRID(rid , this);
}




ResourceBase::~ResourceBase() {
   owner->UnregisterRID(rid);
   owner = 0;
}



bool ResourceBase::LoadFromFile(FilePath fp) {
   filepath = fp;
   std::vector<std::string> args;
   (void)args;
   return (loaded = LoadFromArgs(args));
}



bool ResourceBase::LoadFromFileWithArgs(std::vector<std::string> args) {
   if (args.size() < 1) {
      EagleWarn() << "No args passed to LoadFromFileWithArgs." << std::endl;
      return false;
   }
   filepath = FilePath(args[0]);
   args.erase(args.begin());
   return LoadFromArgs(args);
}



/// --------------------     Archive resource     -------------------------------



bool ArchiveResource::LoadFromArgs(std::vector<std::string> args) {
   (void)args;
   FreeContents();
   std::string file = filepath.File();
   size_t index = file.find_last_of('.');
   if (index == std::string::npos) {return false;}/// No extension
   std::string ext = file.substr(++index);
   std::vector<RESOURCEID> rids;
   
   if ((ext.compare("zip") == 0) || (ext.compare("7z") == 0) || (ext.compare("tar") == 0) || (ext.compare("gz") == 0)) {

      EagleSystem* sys = Eagle::EagleLibrary::System("Any");
      EAGLE_ASSERT(sys);
      FileSystem* fs = sys->GetFileSystem();
      EAGLE_ASSERT(fs);
      ResourceLibrary* reslib = sys->GetResourceLibrary();
      EAGLE_ASSERT(reslib);
      
      /// Recursively load contents here
      contents = fs->ReadArchive(filepath);/// Get the folders and files in the archive
      rids = reslib->LoadArchiveResource(contents);

      for (unsigned int i = 0 ; i < rids.size() ; ++i) {
         RESOURCEID rid1 = rids[i];
         ResourceBase* rb = reslib->LookupResourceByID(rid1);
         std::string path = rb->GetFilePath().Path();
         resfilemap[path] = rb;
      }
   }
   return rids.size();
}
      
      
/**
bool ArchiveResource::LoadFolder(std::shared_ptr<Folder> folder) {
   bool files = LoadFileResources(folder);
   bool folders = LoadSubFolders(folder);
   bool archives = LoadNestedArchives(folder);
   return files && folders && archives;
}



bool ArchiveResource::LoadFileResources(std::shared_ptr<Folder> folder) {
   
   Folder* fl = folder.get();
   EAGLE_ASSERT(fl);
   
   Folder::FILEMAP fmap = fl->Files();
   bool success = true;
   for (Folder::FILEMAP::iterator it = fmap.begin() ; it != fmap.end() ; ) {
      std::shared_ptr<File> fit = it->second;
      File* file = fit.get();
      EAGLE_ASSERT(file);
      FSInfo fsinfo = file->Info();
      if (!fsinfo.Exists()) {
         EagleError() << "File " << fsinfo.Path() << " does not exist." << std::endl;
      }
      else if (!fsinfo.Mode().CanRead()) {
         EagleError() << "File " << fsinfo.Path() << " can not be read." << std::endl;
      }
      else if (!fsinfo.Mode().IsFile()) {
         EagleError() << "FS Object " << fsinfo.Path() << " is not a file." << std::endl;
      }
      else {
         std::string stype = file->Ext();
         RESOURCE_TYPE restype = ResourceLibrary::GetResourceType(stype);
         ResourceBase* res = 0;
         if (restype > RT_UNKNOWN && restype < NUM_RT_TYPES) {
            switch(restype) {
            case RT_IMAGE :
               {
                  ImageResource* ires = new ImageResource();
                  ires->SetOurWindow(win);
                  res = ires;
               }
               break;
            case RT_FONT :
               {
                  FontResource* fres = new FontResource();
                  fres->SetOurWindow(win);
                  res = fres;
               }
               break;
            case RT_AUDIO :
               {
               res = new AudioResource();
               }
               break;
            case RT_VIDEO :
               {
               res = new VideoResource();
               }
               break;
            case RT_ARCHIVE :
               EAGLE_ASSERT(rtype != RT_ARCHIVE);/// We don't treat archives like regular files, they are dealt with separately
               break;
            case RT_BINFILE :
               {
               res = new BinaryResource();
               }
               break;
            case RT_TEXTFILE :
               {
               res = new TextResource();
               }
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
            }
            else {
               /// Save references to the File and ResourceBase* of the resource
               resfilemap.insert(std::pair<std::shared_ptr<File> , std::shared_ptr<ResourceBase> >(fit , res));
            }
         }
         else {
            EagleError() << "Failed to deduce resource type for " << stype << std::endl;
         }
      }
      ++it;
   }
   return success;
}



bool ArchiveResource::LoadSubFolders(std::shared_ptr<Folder> folder) {
   Folder* pfolder = folder.get();
   EAGLE_ASSERT(pfolder);
   if (!pfolder) {return false;}
   bool success = true;
   Folder::SUBFOLDERMAP fmap = pfolder->SubFolders();
   for (Folder::SUBFOLDERMAP::iterator it = fmap.begin() ; it != fmap.end() ; ++it) {
      std::shared_ptr<Folder> subfolder = it->second;
      bool folderloaded = LoadFolder(subfolder);
      success = success && folderloaded;
      if (!folderloaded) {
         EagleError() << "Failed to load sub folder " << subfolder->Path() << " from folder " << pfolder->Path() << std::endl;
      }
   }
   return success;
}



bool ArchiveResource::LoadNestedArchives(std::shared_ptr<Folder> folder) {
   Folder* pfolder = folder.get();
   EAGLE_ASSERT(pfolder);
   if (!pfolder) {return false;}
   bool success = true;
   Folder::ARCHIVEMAP amap = pfolder->Archives();
   for (Folder::ARCHIVEMAP::iterator it = amap.begin() ; it != amap.end() ; ++it) {
      std::shared_ptr<ArchiveFile> afile = it->second;
      ArchiveFile* arc = afile.get();
      EAGLE_ASSERT(arc);
      if (!arc) {
         success = false;
      }
      else {
         ArchiveResource* ares = new ArchiveResource(win);
         bool archive = ares->LoadFromFile(FilePath(arc->Path()));
         if (archive) {
            resfilemap.insert(std::pair<std::shared_ptr<File> , std::shared_ptr<ResourceBase> >(arc , ares));
         }
         else {
            delete ares;
         }
         success = success && archive;
      }
   }
   return success;
}

*/

ArchiveResource::ArchiveResource(EagleGraphicsContext* window) :
      ResourceBase(RT_ARCHIVE),
      win(window),
      contents(),
      resfilemap()
{}



ArchiveResource::~ArchiveResource()
{
///   FreeResources();///< Resources are owned by the resource library or the window, not us
   FreeContents();
   resfilemap.clear();
}



void ArchiveResource::FreeContents() {
   contents.reset();/// Free the shared pointer, we don't own this
}



/// --------------------    Binary resource     -----------------------



bool BinaryResource::LoadFromArgs(std::vector<std::string> args) {
   (void)args;
   return binstream.ReadDataFromFile(filepath);
}



/// -------------------    Text resource     -----------------------



bool TextResource::LoadFromArgs(std::vector<std::string> args) {
   (void)args;
   EagleSystem* sys = Eagle::EagleLibrary::System("Any");
   EAGLE_ASSERT(sys);
   FileSystem* fs = sys->GetFileSystem();
   EAGLE_ASSERT(fs);
   
   std::shared_ptr<File> fl = fs->ReadFile(filepath);
   if (!fl.get()->Info().Exists()) {
      EagleError() << "Failed to find file " << filepath.Path() << std::endl;
      return false;
   }
   if (!fl.get()->Info().Mode().CanRead()) {
      EagleError() << "Cannot read file " << filepath.Path() << std::endl;
   }
   FILE* f = fopen(filepath.Path().c_str() , "rb");
   EAGLE_ASSERT(f);
   filetext.resize(fl.get()->Info().Size() + 1);
   bool success = (fread(&filetext.at(0) , sizeof(char) , fl.get()->Info().Size() , f) == fl.get()->Info().Size());
   filetext[fl.get()->Info().Size()] = '\0';
   fclose(f);
   return success;
}






























