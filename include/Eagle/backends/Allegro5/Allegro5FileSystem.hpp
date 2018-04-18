

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


FileSystem* CreateAllegro5FileSystem();

class Allegro5FileSystem : public FileSystem {
   

protected :
   std::shared_ptr<Folder> ReadFolderInfo(ALLEGRO_FS_ENTRY* f);
   std::shared_ptr<File> ReadFileInfo(ALLEGRO_FS_ENTRY* f);

   virtual void ReadDirectoryContents(Folder* folder);

   
public :

   Allegro5FileSystem();
   ~Allegro5FileSystem();

   virtual std::string GetFileName(std::string path);/// Relative paths okay
   virtual std::string GetFileExt(std::string name);
   virtual FSInfo GetFileInfo(FilePath path);

   virtual std::shared_ptr<Folder> ReadFolder(FilePath path);
   virtual std::shared_ptr<File>   ReadFile  (FilePath path);

   virtual char PathSeparator();

   virtual bool ChangeDirectory(std::string dir);
   virtual std::string CurrentDirectory();


};



#endif // Allegro5FileSystem_HPP

