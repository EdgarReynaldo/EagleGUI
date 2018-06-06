

/*
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
 *    EAGLE
 *    Edgar's Agile Gui Library and Extensions
 *
 *    Copyright 2009-2018+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#ifndef Allegro5FileSystem_HPP
#define Allegro5FileSystem_HPP

#include "allegro5/allegro.h"
#include "Eagle/FileSystem.hpp"
 
FSInfo GetFSInfo(ALLEGRO_FS_ENTRY* f);

FSMode GetFSModeFromAllegroFileMode(uint32_t amode);


class Allegro5FileSystem : public FileSystem {
   

protected :
   std::shared_ptr<Folder> ReadFolderInfo(ALLEGRO_FS_ENTRY* f);
   std::shared_ptr<File> ReadFileInfo(ALLEGRO_FS_ENTRY* f);

   virtual void ReadDirectoryContents(Folder* folder , bool descending = false);

   
public :

   Allegro5FileSystem();
   ~Allegro5FileSystem();
   
   virtual FSInfo GetFileInfo(FilePath path);

   virtual std::shared_ptr<File>   ReadFile  (FilePath path);
   virtual std::shared_ptr<Folder> ReadFolder(FilePath path , bool descending = false);

   virtual void UnmountArchive();
   virtual bool MountArchive(FilePath fp);
   
   virtual bool ChangeFSDirectory(std::string dir);
   virtual std::string CurrentFSDirectory();

};

Allegro5FileSystem* GetAllegro5FileSystem();



#endif // Allegro5FileSystem_HPP




