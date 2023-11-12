
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\____/     \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2023+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Allegro5ResourceLib.cpp
 * @brief Implementation of the Allegro 5 resource library for Eagle.
 * 
 *
 */

#include "Eagle/backends/Allegro5/Allegro5ResourceLib.hpp"
#include "Eagle/backends/Allegro5/Allegro5Sound.hpp"

#include "Eagle/Exception.hpp"
#include "Eagle/Font.hpp"
#include "Eagle/Image.hpp"
#include "Eagle/GraphicsContext.hpp"
#include "Eagle/Lib.hpp"
#include "Eagle/Sound.hpp"
#include "Eagle/System.hpp"
#include "Eagle/FileSystem.hpp"



Allegro5ResourceLibrary::Allegro5ResourceLibrary() :
      ResourceLibrary()
{
   static int init = 0;
   if (init) {
      throw EagleException("Allegro5ResourceLibrary reports multiple creation");
   }
   init = 1;
}



Allegro5ResourceLibrary* Allegro5ResourceLibrary::Instance() {
   static Allegro5ResourceLibrary us;
   return &us;
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

      stypes[RT_AUDIO_SAMPLE].push_back("ogg");
      stypes[RT_AUDIO_SAMPLE].push_back("wav");
      stypes[RT_AUDIO_SAMPLE].push_back("mp3");
      
      stypes[RT_AUDIO_STREAM].push_back("ogg");
      stypes[RT_AUDIO_STREAM].push_back("wav");
      stypes[RT_AUDIO_STREAM].push_back("mp3");

      stypes[RT_VIDEO].push_back("ogv");

      stypes[RT_ARCHIVE].push_back("zip");
      stypes[RT_ARCHIVE].push_back("7z");

      stypes[RT_BINFILE].push_back("dat");
      stypes[RT_BINFILE].push_back("bin");

      stypes[RT_TEXTFILE].push_back("txt");
      stypes[RT_TEXTFILE].push_back("cfg");
      stypes[RT_TEXTFILE].push_back("ini");

      stypes[RT_UNKNOWN].insert(stypes[RT_UNKNOWN].end() , stypes[RT_IMAGE       ].begin() , stypes[RT_IMAGE       ].end());
      stypes[RT_UNKNOWN].insert(stypes[RT_UNKNOWN].end() , stypes[RT_FONT        ].begin() , stypes[RT_FONT        ].end());
      stypes[RT_UNKNOWN].insert(stypes[RT_UNKNOWN].end() , stypes[RT_AUDIO_SAMPLE].begin() , stypes[RT_AUDIO_SAMPLE].end());
      stypes[RT_UNKNOWN].insert(stypes[RT_UNKNOWN].end() , stypes[RT_AUDIO_STREAM].begin() , stypes[RT_AUDIO_STREAM].end());
      stypes[RT_UNKNOWN].insert(stypes[RT_UNKNOWN].end() , stypes[RT_VIDEO       ].begin() , stypes[RT_VIDEO       ].end());
      stypes[RT_UNKNOWN].insert(stypes[RT_UNKNOWN].end() , stypes[RT_ARCHIVE     ].begin() , stypes[RT_ARCHIVE     ].end());
      stypes[RT_UNKNOWN].insert(stypes[RT_UNKNOWN].end() , stypes[RT_BINFILE     ].begin() , stypes[RT_BINFILE     ].end());
      stypes[RT_UNKNOWN].insert(stypes[RT_UNKNOWN].end() , stypes[RT_TEXTFILE    ].begin() , stypes[RT_TEXTFILE    ].end());
   }
   
   

   std::set<std::string> types;
   types.insert(stypes[rt].begin() , stypes[rt].end());
   return types;
}



RESOURCEID Allegro5ResourceLibrary::LoadFileResource(std::shared_ptr<File> file) {
   File* f = file.get();
   if (!f) {
      EagleError() << "NULL file in LoadFileResource" << std::endl;
      EAGLE_ASSERT(f);
      return BADRESOURCEID;
   }
   FSInfo fsinfo = f->Info();
   if (!fsinfo.Exists()) {
      EagleError() << "File " << fsinfo.Path() << " does not exist." << std::endl;
      return BADRESOURCEID;
   }
   else if (!fsinfo.Mode().CanRead()) {
      EagleError() << "File " << fsinfo.Path() << " can not be read." << std::endl;
      return BADRESOURCEID;
   }
   else if (!fsinfo.Mode().IsFile()) {
      EagleError() << "FS Object " << fsinfo.Path() << " is not a file." << std::endl;
      return BADRESOURCEID;
   }
   std::string ext = f->Ext();
   RESOURCE_TYPE restype = GetResourceType(ext);
   ResourceBase* res = 0;
   bool success = true;
   if (restype > RT_UNKNOWN && restype < NUM_RT_TYPES) {
      switch(restype) {
      case RT_IMAGE :
         res = window->LoadImageFromFile(f->Path());
         break;
      case RT_FONT :
         res = window->GetFont(f->Path() , default_font_size , default_font_flags , VIDEO_IMAGE);
         break;
      case RT_AUDIO_SAMPLE :
         res = new Allegro5SoundSample();
         break;
      case RT_AUDIO_STREAM :
         res = new Allegro5SoundStream();
         break;
      case RT_VIDEO :
///< TODO : DO ME         res = new VideoResource();
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
         return BADRESOURCEID;
      }
      else {
         /// Save references to the File and ResourceBase* of the resource
         resmap[res->RID()] = res;
         lookupmap.insert(std::pair<std::string , RESOURCEID>(file->Path() , res->RID()));
      }
   }
   else {
      EagleError() << "Failed to deduce resource type for " << restype << std::endl;
      return BADRESOURCEID;
   }
   return res->RID();
}



std::vector<RESOURCEID> Allegro5ResourceLibrary::LoadFolderResource(std::shared_ptr<Folder> folder , bool descend) {
   Folder* fl = folder.get();

   std::vector<RESOURCEID> resids;
   
   EAGLE_ASSERT(fl);
   if (!fl) {
      EagleError() << "NULL folder in LoadFolderResource" << std::endl;
      return resids;
   }
   
   FSInfo fsinfo = fl->Info();
   
   if (!fsinfo.Exists()) {
      EagleError() << "Folder " << fsinfo.Path() << " does not exist." << std::endl;
      return resids;
   }
   else if (!fsinfo.Mode().CanRead()) {
      EagleError() << "Folder " << fsinfo.Path() << " can not be read." << std::endl;
      return resids;
   }
   else if (fsinfo.Mode().IsFile()) {
      EagleError() << "FS Object " << fsinfo.Path() << " is not a folder." << std::endl;
      return resids;
   }

   Folder::FILEMAP fmap = fl->Files();

   for (Folder::FILEMAP::iterator it = fmap.begin() ; it != fmap.end() ; it++) {
      resids.push_back(LoadFileResource(it->second));
   }

   if (descend) {
      Folder::SUBFOLDERMAP flmap = fl->SubFolders();
      for (Folder::SUBFOLDERMAP::iterator it = flmap.begin() ; it != flmap.end() ; ++it) {
         std::vector<RESOURCEID> rid2 = LoadFolderResource(it->second , descend);
         resids.insert(resids.end() , rid2.begin() , rid2.end());
      }
   }
   
   Folder::ARCHIVEMAP arcmap = fl->Archives();
   for (Folder::ARCHIVEMAP::iterator it = arcmap.begin() ; it != arcmap.end() ; ++it) {
         std::vector<RESOURCEID> rid2 = LoadArchiveResource(it->second);
         resids.insert(resids.end() , rid2.begin() , rid2.end());
   }
   
   return resids;
}



std::vector<RESOURCEID> Allegro5ResourceLibrary::LoadArchiveResource(std::shared_ptr<ArchiveFile> archive) {
   ArchiveFile* arc = archive.get();
   
   std::vector<RESOURCEID> resids;
   
   EAGLE_ASSERT(arc);
   if (!arc) {
      EagleError() << "NULL archive in LoadFileResource" << std::endl;
      return resids;
   }
   
   FSInfo fsinfo = arc->File::Info();
   
   if (!fsinfo.Exists()) {
      EagleError() << "Archive " << fsinfo.Path() << " does not exist." << std::endl;
      return resids;
   }
   else if (!fsinfo.Mode().CanRead()) {
      EagleError() << "Archive " << fsinfo.Path() << " can not be read." << std::endl;
      return resids;
   }
   else if (!fsinfo.Mode().IsFile()) {
      EagleError() << "FS Object " << fsinfo.Path() << " is not an archive file." << std::endl;
      return resids;
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
   if (mount) {
      resids = LoadFolderResource(root , true);
      fs->UnmountArchive();
   }
   return resids;
}








