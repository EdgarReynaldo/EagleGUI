


#include "allegro5/allegro.h"

#include "Eagle/backends/Allegro5/Allegro5FileSystem.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/Exception.hpp"



FileSystem* CreateAllegro5FileSystem() {return new Allegro5FileSystem();}




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
   uint32_t amode = al_get_fs_entry_mode(f);
   FSMode fmode = GetFSModeFromAllegroFileMode(amode);
   time_t fcreate = al_get_fs_entry_ctime(f);
   time_t fmodify = al_get_fs_entry_mtime(f);
   time_t faccess = al_get_fs_entry_atime(f);
   unsigned long long int fsize = al_get_fs_entry_size(f);
   return FSInfo(fpath , true , fmode , fcreate , fmodify , faccess , fsize);
}




std::shared_ptr<Folder> Allegro5FileSystem::ReadFolderInfo(ALLEGRO_FS_ENTRY* f) {
   if (!f) {return 0;}
   if (!al_fs_entry_exists(f)) {
      return 0;
   }
   return std::shared_ptr<Folder>(new Folder(GetFSInfo(f)));
}



std::shared_ptr<File> Allegro5FileSystem::ReadFileInfo(ALLEGRO_FS_ENTRY* f) {
   if (!f) {return 0;}
   if (!al_fs_entry_exists(f)) {
      return 0;
   }
   return std::shared_ptr<File>(new File(GetFSInfo(f)));
}



void Allegro5FileSystem::ReadDirectoryContents(Folder* folder) {
   if (!folder) {
      throw EagleException("Allegro5FileSystem::ReadDirectory - folder is NULL!\n");
   }
   if (!(folder->Info().Mode().IsDir())) {
      throw EagleException(StringPrintF(
         "Allegro5FileSystem::ReadDirectoryContents - folder entry '%s' is not a directory!\n" , folder->Path().c_str()));
   }

   ALLEGRO_FS_ENTRY* fs = al_create_fs_entry(folder->Path().c_str());
   if (!fs) {
      throw EagleException(StringPrintF(
         "Allegro5FileSystem::ReadDirectoryContents - failed to read folder path '%s'\n" , folder->Path().c_str()));
   }
   
   if (al_open_directory(fs)) {
      ALLEGRO_FS_ENTRY* f = 0;
      while ((f = al_read_directory(fs))) {
         FSInfo info = GetFSInfo(f);
         if (info.Mode().IsFile()) {
            RegisterFile(folder , new File(info));
         }
         else if (info.Mode().IsDir()) {
            RegisterSubFolder(folder , new Folder(info));
         }
         al_destroy_fs_entry(f);
      }
   }
   
   al_destroy_fs_entry(fs);
   
}



Allegro5FileSystem::Allegro5FileSystem() : 
      FileSystem() 
{}



Allegro5FileSystem::~Allegro5FileSystem() {
   (void)0;
}



std::string Allegro5FileSystem::GetFileName(std::string path) {
   
   int index = -1;
   int psize = path.length();
   /// Remove trailing slashes
///   index = path.find_last_of(ALLEGRO_PATH_SEP);

   for (int i = psize - 1 ; i >= 0 ; --i) {
      if (path[i] == ALLEGRO_NATIVE_PATH_SEP) {
         index = i;
      }
   }
   return path.substr(index + 1);
}



std::string Allegro5FileSystem::GetFileExt(std::string path) {
   unsigned int index = 0;
   index = path.find_last_of('.');
   return path.substr(index + 1);
}



FSInfo Allegro5FileSystem::GetFileInfo(FilePath fpath) {
   std::string path = fpath.Path();
   ALLEGRO_FS_ENTRY* fs = al_create_fs_entry(path.c_str());
   FSInfo info(path);
   if (al_fs_entry_exists(fs)) {
      info = GetFSInfo(fs);
   }
   al_destroy_fs_entry(fs);
   return info;
}



std::shared_ptr<Folder> Allegro5FileSystem::ReadFolder(FilePath fpath) {
   std::string path = fpath.Path();
   ALLEGRO_FS_ENTRY* f = al_create_fs_entry(path.c_str());
   std::shared_ptr<Folder> folder = ReadFolderInfo(f);
   al_destroy_fs_entry(f);
   if (!folder) {
      EagleWarn() << StringPrintF("Failed to read directory '%s' from filesystem.\n" , path.c_str());
   }
   else {
      ReadDirectoryContents(folder.get());
   }
   return folder;
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



char Allegro5FileSystem::PathSeparator() {
   return ALLEGRO_NATIVE_PATH_SEP;
}



bool Allegro5FileSystem::ChangeDirectory(std::string dir) {
   return al_change_directory(dir.c_str());
}



std::string Allegro5FileSystem::CurrentDirectory() {
   return al_get_current_directory();
}







