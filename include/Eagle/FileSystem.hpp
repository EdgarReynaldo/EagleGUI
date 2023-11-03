
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
 *    Copyright 2009-2023+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file FileSystem.hpp
 * @brief The interface for working with file systems in Eagle
 * 
 * 
 * 
 */

#ifndef FileSystem_HPP
#define FileSystem_HPP



#include <vector>

#include "Eagle/File.hpp"



/// Info

///< Returns the native path separator on this platform. Runtime only. For compile time path sep, use @ref EAGLE_NATIVE_PATH_SEP
char NativePathSeparator();

/// Utility functions

///< Gets the file name from a path
std::string GetFileName(std::string path);

///< Gets the file extension from a filename. (All characters after the last dot (NOTE : Will not work for dotfiles like .git))
std::string GetFileExt(std::string filename);

/// Path

///< Explodes a path into all of its components. Drive is first, path is in the middle, and filename is last
std::vector<std::string> ExplodePath(std::string path);

///< Get an absolute path from a relative one, depends on the Current Working Directory...
std::vector<std::string> GetAbsolutePath(std::string rpath);///< Eagle must be initialized before calling this function.

///< Sanitize a vector of strings by adding directory separators as appropriate, shortcut to SanitizePath(GetAbsolutePath(path))
std::string SanitizePath(std::vector<std::string> abspath);

///< Sanitize a path by removing all relative directories, and replacing all separators with forward slash
std::string SanitizePath(std::string path);

///< Get the current directory - internal use only
//static std::string CurrentDirectory();

///< Shortcut to @ref CurrentDirectory internal use only
//static std::string GetCWD();


class Drive;



/**! @class FileSystem
 *   @brief A class to represent the user's file system
 */

class FileSystem {
protected :
   std::vector<Drive*> drives;
   
//   bool archive_mounted;/// TODO : This works only for non nested mounts
//   FilePath mount_file_path;
   std::stack<FilePath> mount_file_path_stack;
   
   /// Not for public use - does not reflect physfs paths properly - internal use only
   static std::string CurrentDirectory();

   /// Not for public use - shortcut to @ref CurrentDirectory - internal use only
   static std::string GetCWD();
   
   virtual void ReadDirectoryContents(Folder* folder , bool descending = false)=0;
   
   void RegisterFile(Folder* parent , std::shared_ptr<File> file);
   void RegisterArchiveFile(Folder* parent , std::shared_ptr<ArchiveFile> afile);
   void RegisterSubFolder(Folder* parent , std::shared_ptr<Folder> sub);
   
public :
   FileSystem();///< Empty constructor does nothing

   virtual ~FileSystem() {}
   
   virtual FSInfo GetFileInfo(FilePath path)=0;///< Pure virtual function for getting an @ref FSInfo object from a path

   FSInfo         GetFileInfo(std::string path);///< Get an @ref FSInfo object for the given string path
   FSInfo         GetFileInfo(const char* path);///< Get an @ref FSInfo object for the given c string path
   
   /// Read functions
   
   ///< Pure virtual function for reading a file
   virtual std::shared_ptr<File>   ReadFile  (FilePath path)=0;

   ///< Pure virtual function for reading a folder
   virtual std::shared_ptr<Folder> ReadFolder(FilePath path , bool descending = false)=0;
   
   ///< Pure virtual function for reading an archive
   virtual std::shared_ptr<ArchiveFile> ReadArchive(FilePath path)=0;
   
   
   /// Directory functions
   
   ///< Mount an archive file. Returns true on success
   virtual bool MountArchive(FilePath fp)=0;
   
   ///< Unmount the current archive
   virtual void UnmountArchive()=0;
   
   ///< Change the working directory
   virtual bool ChangeFSDirectory(std::string dir)=0;

   ///< Get the current working file system directory
   virtual std::string CurrentFSDirectory()=0;
   
};



#endif // FileSystem_HPP



