
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
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Allegro5FileSystem.hpp
 * @brief The implementation of the Allegro 5 file system
 */

#ifndef Allegro5FileSystem_HPP
#define Allegro5FileSystem_HPP



#include "allegro5/allegro.h"
#include "Eagle/FileSystem.hpp"
 


FSInfo GetFSInfo(ALLEGRO_FS_ENTRY* f);///< Get @ref FSInfo from an ALLEGRO_FS_ENTRY*

FSMode GetFSModeFromAllegroFileMode(uint32_t amode);///< Get the @ref FSMode from an allegro file mode



/**! @class Allegro5FileSystem
 *   @brief The implementation of the Allegro 5 file system interface
 */

class Allegro5FileSystem : public FileSystem {
   

protected :
   std::shared_ptr<Folder> ReadFolderInfo(ALLEGRO_FS_ENTRY* f);
   std::shared_ptr<File> ReadFileInfo(ALLEGRO_FS_ENTRY* f);

   virtual void ReadDirectoryContents(Folder* folder , bool descending = false);
   
public :
   Allegro5FileSystem();
   ~Allegro5FileSystem();
   
   virtual FSInfo GetFileInfo(FilePath path);///< Get @ref FSInfo for a file path

   ///< Get a @ref File for a path
   virtual std::shared_ptr<File> ReadFile(FilePath path);

   /**! @fn ReadFolder <FilePath , bool>
    *   @brief Get a @ref Folder for a path
    *   @param descending Whether or not to read this folder recursively descending into sub-directories
    */
   virtual std::shared_ptr<Folder> ReadFolder(FilePath path , bool descending = false);

   ///< Mount an archive file using the given file path
   virtual bool MountArchive(FilePath fp);
   
   ///< Unmount the currently mounted archive
   virtual void UnmountArchive();
   
   ///< Change the working directory
   virtual bool ChangeFSDirectory(std::string dir);

   ///< Get the current working file system directory
   virtual std::string CurrentFSDirectory();
};



Allegro5FileSystem* GetAllegro5FileSystem();///< Get the Allegro5FileSystem




#endif // Allegro5FileSystem_HPP



