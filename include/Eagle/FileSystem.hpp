
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
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */




#ifndef FileSystem_HPP
#define FileSystem_HPP



#include <vector>


#include "Eagle/File.hpp"


/// Info
char NativePathSeparator();

/// Utility functions
std::string GetFileName(std::string path);
std::string GetFileExt(std::string filename);

/// Path
std::vector<std::string> ExplodePath(std::string path);

std::vector<std::string> GetAbsolutePath(std::string rpath);

std::string SanitizePath(std::string path);/// Removes all references to relative directories and replaces all separators with native ones


std::string CurrentDirectory();
std::string GetCWD();


class Drive;



class FileSystem {
protected :
   std::vector<Drive*> drives;
   
   bool archive_mounted;
   FilePath mount_file_path;
   
   
   virtual void ReadDirectoryContents(Folder* folder , bool descending = false)=0;
   
   void RegisterFile(Folder* parent , std::shared_ptr<File> file);
   void RegisterArchiveFile(Folder* parent , std::shared_ptr<ArchiveFile> afile);
   void RegisterSubFolder(Folder* parent , std::shared_ptr<Folder> sub);
   
public :
   FileSystem();

   virtual ~FileSystem() {}
   
   virtual FSInfo GetFileInfo(FilePath path)=0;

   FSInfo         GetFileInfo(std::string path);
   FSInfo         GetFileInfo(const char* path);
   
   /// Read functions
   virtual std::shared_ptr<File>   ReadFile  (FilePath path)=0;
   virtual std::shared_ptr<Folder> ReadFolder(FilePath path , bool descending = false)=0;
   
   /// Directory functions
   
   virtual void UnmountArchive()=0;
   virtual bool MountArchive(FilePath fp)=0;
   
   
///   virtual bool ChangeFSDirectory(std::string dir)=0;
///   virtual std::string CurrentFSDirectory()=0;

///   std::string GetCFSD() {return CurrentFSDirectory();}

};

#endif // FileSystem_HPP

