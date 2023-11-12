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
 * @file Allegro5FileSystem.cpp
 * @brief Allegro 5 implementation of the Eagle FileSystem class
 * 
 */

#include "allegro5/allegro.h"

#include "Eagle/backends/Allegro5/Allegro5System.hpp"
#include "Eagle/backends/Allegro5/Allegro5FileSystem.hpp"
#include "Eagle/backends/Allegro5/Allegro5ResourceLib.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/Exception.hpp"
#include "Eagle/Resources.hpp"
#include "Eagle/Lib.hpp"

#include "allegro5/allegro_physfs.h"
#include "physfs.h"



FSMode GetFSModeFromAllegroFileMode(uint32_t amode) {
   int flags = 0;
   if (amode & ALLEGRO_FILEMODE_READ) {flags |= FS_MODE_READ;}
   if (amode & ALLEGRO_FILEMODE_WRITE) {flags |= FS_MODE_WRITE;}
   if (amode & ALLEGRO_FILEMODE_EXECUTE) {flags |= FS_MODE_EXECUTE;}
   if (amode & ALLEGRO_FILEMODE_HIDDEN) {flags |= FS_MODE_HIDDEN;}
   if (!(amode & ALLEGRO_FILEMODE_ISDIR)) {flags |= FS_MODE_FILE;}
   return FSMode(flags);
}



FSInfo GetFSInfo(ALLEGRO_FS_ENTRY* f) {
   EAGLE_ASSERT(al_fs_entry_exists(f));
   std::string fpath = al_get_fs_entry_name(f);
   EagleInfo() << fpath << std::endl;
   FilePath fp(fpath);
   uint32_t amode = al_get_fs_entry_mode(f);
   FSMode fmode = GetFSModeFromAllegroFileMode(amode);
   time_t fcreate = al_get_fs_entry_ctime(f);
   time_t fmodify = al_get_fs_entry_mtime(f);
   time_t faccess = al_get_fs_entry_atime(f);
   unsigned long long int fsize = al_get_fs_entry_size(f);
   return FSInfo(fp , true , fmode , fcreate , fmodify , faccess , fsize);
}




std::shared_ptr<Folder> Allegro5FileSystem::ReadFolderInfo(ALLEGRO_FS_ENTRY* f) {
   if (!f) {return 0;}
   if (!al_fs_entry_exists(f)) {
      return 0;
   }
   return std::shared_ptr<Folder>(new Folder(GetFSInfo(f)));
}



std::shared_ptr<File> Allegro5FileSystem::ReadFileInfo(ALLEGRO_FS_ENTRY* f) {
   EAGLE_ASSERT(f);
   if (!f) {return 0;}
   if (!al_fs_entry_exists(f)) {
      return 0;
   }
   return std::shared_ptr<File>(new File(GetFSInfo(f)));
}



std::shared_ptr<ArchiveFile> Allegro5FileSystem::ReadArchiveInfo(ALLEGRO_FS_ENTRY* f) {
   EAGLE_ASSERT(f);
   if (!f) {return 0;}
   if (!al_fs_entry_exists(f)) {
      return 0;
   }
   
   return std::shared_ptr<ArchiveFile>(new ArchiveFile(GetFSInfo(f)));
}



void Allegro5FileSystem::ReadDirectoryContents(Folder* folder , bool descend) {
   if (!folder) {
      throw EagleException("Allegro5FileSystem::ReadDirectory - folder is NULL!\n");
   }
   ArchiveFile* archive = dynamic_cast<ArchiveFile*>(folder);
   ALLEGRO_FS_ENTRY* fs = 0;
   
   std::string p = folder->Path();
   if (archive) {
      MountArchive(p);
/**
      if (!archive_mounted) {
         MountArchive(p);
         p = "";/// This is now root
      }
      else {
         /// There is already an archive mounted here
         EagleWarn() << "Allegro5FileSystem::ReadDirectoryContents - reading inside nested archives is not currently possible" << std::endl;
         return;
      }
*/
   }
   else {
      if (!(folder->Info().Mode().IsDir())) {
         throw EagleException(StringPrintF(
            "Allegro5FileSystem::ReadDirectoryContents - folder entry '%s' is not a directory!\n" , folder->Path().c_str()));
      }
   }

   fs = al_create_fs_entry(p.c_str());
   
   if (!fs) {
      throw EagleException(StringPrintF(
         "Allegro5FileSystem::ReadDirectoryContents - failed to read %s path '%s'\n" , archive?"archive":"folder" , p.c_str()));
   }
   
   if (al_open_directory(fs)) {
      ALLEGRO_FS_ENTRY* f = 0;
      while ((f = al_read_directory(fs))) {
         FSInfo info = GetFSInfo(f);
         if (info.Mode().IsFile()) {
            std::string ext = GetFileExt(GetFileName(info.Path()));
            if (ResourceLibrary::GetResourceType(ext) == RT_ARCHIVE) {
               RegisterArchiveFile(folder , std::shared_ptr<ArchiveFile>(new ArchiveFile(info)));
            }
            else {
               RegisterFile(folder , std::shared_ptr<File>(new File(info)));
            }
         }
         else if (info.Mode().IsDir()) {
            RegisterSubFolder(folder , std::shared_ptr<Folder>(new Folder(info)));
         }
         al_destroy_fs_entry(f);
      }
   }
   
   al_destroy_fs_entry(fs);
   
   if (descend) {
      Folder::SUBFOLDERMAP subfoldermap = folder->SubFolders();
      Folder::SUBFOLDERMAP::iterator it = subfoldermap.begin();
      while (it != subfoldermap.end()) {
         Folder* sf = it->second.get();
         ReadDirectoryContents(sf , true);
         ++it;
      }
      Folder::ARCHIVEMAP archivemap = folder->Archives();
      Folder::ARCHIVEMAP::iterator ait = archivemap.begin();
      while (ait != archivemap.end()) {
         ArchiveFile* af = ait->second.get();
         ReadDirectoryContents(af , true);
         ++ait;
      }
   }
   
   if (archive) {
      UnmountArchive();
   }
}



bool Allegro5FileSystem::RemountArchive() {
   EAGLE_ASSERT(!mount_stack.empty());;
   bool success = MountArchive(mount_stack.top());
   if (success) {
      mount_stack.pop();///< To offset the remount of an old directory
   }
   return success;
}



Allegro5FileSystem::Allegro5FileSystem() : 
      FileSystem() 
{
   #warning TODO : PHYSFS_INIT needs to be called here but we need argv[0]
///   PHYSFS_init();
}



Allegro5FileSystem::~Allegro5FileSystem() {
   PHYSFS_deinit();
   (void)0;
}



FSInfo Allegro5FileSystem::GetFileInfo(FilePath fpath) {
   std::string path = fpath.Path();/// e:/usr64/projects/Spiraloid2/Graphics.cfg
   ALLEGRO_FS_ENTRY* fs = al_create_fs_entry(path.c_str());
   FSInfo info;
   if (al_fs_entry_exists(fs)) {
      info = GetFSInfo(fs);
   }
   else {
      info.SetPath(fpath);/// This file does not exist yet, but we still need the path
   }
   al_destroy_fs_entry(fs);
   return info;
}



std::shared_ptr<File> Allegro5FileSystem::ReadFile(FilePath fpath) {
   std::string path = fpath.Path();
   ALLEGRO_FS_ENTRY* f = al_create_fs_entry(path.c_str());
   
   std::shared_ptr<File> file = ReadFileInfo(f);
   al_destroy_fs_entry(f);
   if (!file) {
      EagleWarn() << StringPrintF("Failed to read file '%s' from filesystem.\n" , path.c_str());
   }
   return file;
}



std::shared_ptr<Folder> Allegro5FileSystem::ReadFolder(FilePath fpath , bool descending) {

   std::string path = fpath.Path();

   ALLEGRO_FS_ENTRY* f = al_create_fs_entry(path.c_str());
   EAGLE_ASSERT(f);
   if (!f) {return std::shared_ptr<Folder>(0);}
   
   std::shared_ptr<Folder> folder = ReadFolderInfo(f);
   al_destroy_fs_entry(f);
   if (!folder) {
      EagleWarn() << StringPrintF("Failed to read directory '%s' from filesystem.\n" , path.c_str());
   }
   else {
      ReadDirectoryContents(folder.get() , descending);
   }
   return folder;
}



std::shared_ptr<ArchiveFile> Allegro5FileSystem::ReadArchive(FilePath fpath) {

   std::string path = fpath.Path();

   ALLEGRO_FS_ENTRY* f = al_create_fs_entry(path.c_str());
   EAGLE_ASSERT(f);
   if (!f) {return std::shared_ptr<ArchiveFile>(0);}

   std::shared_ptr<ArchiveFile> archive = ReadArchiveInfo(f);
   al_destroy_fs_entry(f);

   if (!archive) {
      EagleWarn() << StringPrintF("Failed to read archive '%s' from filesystem.\n" , path.c_str());
   }
   else {
      ReadDirectoryContents(dynamic_cast<Folder*>(archive.get()) , true);
   }
   return archive;
}



void Allegro5FileSystem::UnmountArchive() {
   if (!mount_stack.empty()) {
      FilePath fp = mount_stack.top();
      mount_stack.pop();
      std::string oldmount = fp.Path();
      PHYSFS_unmount(oldmount.c_str());
      al_set_standard_file_interface();
      al_set_standard_fs_interface();
   }
   if (!mount_stack.empty()) {
      if (!RemountArchive()) {
         EagleError() << "Failed to remount back of mount stack." << std::endl;
      }
   }
}



bool Allegro5FileSystem::MountArchive(FilePath fp) {
   std::string archive = fp.Path();
   if (PHYSFS_mount(archive.c_str() , NULL , false) != 0) {
      mount_stack.push(fp);
      al_set_physfs_file_interface();
      return true;
   }
   return false;
}



bool Allegro5FileSystem::ChangeFSDirectory(std::string dir) {
   return al_change_directory(dir.c_str());
}



std::string Allegro5FileSystem::CurrentFSDirectory() {
   if (!mount_stack.empty()) {
      return al_get_current_directory();
   }
   return GetCWD();
}



Allegro5FileSystem* GetAllegro5FileSystem() {
   return dynamic_cast<Allegro5FileSystem*>(GetAllegro5System()->GetFileSystem());
}




