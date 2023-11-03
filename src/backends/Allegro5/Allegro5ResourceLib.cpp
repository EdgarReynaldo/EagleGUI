



#include "Eagle/Exception.hpp"



#include "Eagle/backends/Allegro5/Allegro5ResourceLib.hpp"




Allegro5ResourceLibrary* Allegro5ResourceLibrary::me = 0;



Allegro5ResourceLibrary::Allegro5ResourceLibrary() :
      ResourceLibrary()
{
   if (me) {
      throw EagleException("Allegro5ResourceLibrary reports multiple creation");
   }
   me = this;
}



std::set<std::string> Allegro5ResourceLibrary::GetSupportedTypes(RESOURCE_TYPE rt) {
   static std::vector<std::string> stypes[NUM_RT_TYPES];
   static int init = 1;
   if (init) {
      init = 0;
      stypes[RT_IMAGE].push_back("bmp");
      stypes[RT_IMAGE].push_back("png");
      stypes[RT_IMAGE].push_back("jpg");
      stypes[RT_IMAGE].push_back("jpeg");
      stypes[RT_IMAGE].push_back("tga");

      stypes[RT_FONT].push_back("ttf");
      stypes[RT_FONT].push_back("bmp");

      stypes[RT_AUDIO].push_back("ogg");
      stypes[RT_AUDIO].push_back("wav");

      stypes[RT_VIDEO].push_back("ogv");

      stypes[RT_ARCHIVE].push_back("zip");
      stypes[RT_ARCHIVE].push_back("7z");

      stypes[RT_BINFILE].push_back("dat");
      stypes[RT_BINFILE].push_back("bin");

      stypes[RT_TEXTFILE].push_back("txt");
      stypes[RT_TEXTFILE].push_back("cfg");
      stypes[RT_TEXTFILE].push_back("ini");

      stypes[RT_UNKNOWN].insert(stypes[RT_UNKNOWN].end() , stypes[RT_IMAGE   ].begin() , stypes[RT_IMAGE   ].end());
      stypes[RT_UNKNOWN].insert(stypes[RT_UNKNOWN].end() , stypes[RT_FONT    ].begin() , stypes[RT_FONT    ].end());
      stypes[RT_UNKNOWN].insert(stypes[RT_UNKNOWN].end() , stypes[RT_AUDIO   ].begin() , stypes[RT_AUDIO   ].end());
      stypes[RT_UNKNOWN].insert(stypes[RT_UNKNOWN].end() , stypes[RT_VIDEO   ].begin() , stypes[RT_VIDEO   ].end());
      stypes[RT_UNKNOWN].insert(stypes[RT_UNKNOWN].end() , stypes[RT_ARCHIVE ].begin() , stypes[RT_ARCHIVE ].end());
      stypes[RT_UNKNOWN].insert(stypes[RT_UNKNOWN].end() , stypes[RT_BINFILE ].begin() , stypes[RT_BINFILE ].end());
      stypes[RT_UNKNOWN].insert(stypes[RT_UNKNOWN].end() , stypes[RT_TEXTFILE].begin() , stypes[RT_TEXTFILE].end());
   }
   
   

   std::set<std::string> types;
   types.insert(stypes[rt].begin() , stypes[rt].end());
   return types;
}



bool Allegro5ResourceLibrary::LoadFileResource(std::shared_ptr<File> file) {
   
}



bool Allegro5ResourceLibrary::LoadFolderResource(std::shared_ptr<Folder> folder , bool descend = false) {
   
}



bool Allegro5ResourceLibrary::LoadArchiveResource(std::shared_ptr<ArchiveFile> archive) {
   
}




bool ResourceLibrary::LoadFileResource(std::shared_ptr<File> file) {
   File* f = file.get();
   if (!f) {
      EagleError() << "NULL file in LoadFileResource" << std::endl;
      EAGLE_ASSERT(f);
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
         res = new ImageResource(window);
         break;
      case RT_FONT :
         res = new FontResource(window);
         break;
      case RT_AUDIO :
         res = new AudioResource();
         break;
      case RT_VIDEO :
         res = new VideoResource();
         break;
      case RT_ARCHIVE :
         res = new ArchiveResource(window);
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








