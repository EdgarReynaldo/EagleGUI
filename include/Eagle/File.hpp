
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
 * @file File.hpp
 * @brief The main interface for working with files
 */

#ifndef File_HPP
#define File_HPP



#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <memory>



#include "Eagle/Indenter.hpp"


/**! @enum FS_MODE_FLAGS
 *   @brief An enumeration defining flags for a bitfiled to describe the file system attributes for this file or folder
 */

enum FS_MODE_FLAGS {
   FS_MODE_READ    = 1 << 0,///< This fs object is readable
   FS_MODE_WRITE   = 1 << 1,///< This fs object is writable
   FS_MODE_EXECUTE = 1 << 2,///< This fs object is executable
   FS_MODE_HIDDEN  = 1 << 3,///< This fs object is hidden
   FS_MODE_SYSTEM  = 1 << 4,///< This fs object belongs to the system
   FS_MODE_FILE    = 1 << 5 ///< This fs object is a file
};



/**! @class FSMode
 *   @brief Simple data class to wrap file attributes
 */

class FSMode {
protected :
   unsigned int mode;
public :
   FSMode(int flags) : mode(flags) {}
   
   bool CanRead()    {return mode & FS_MODE_READ;}   ///< True if readable
   bool CanWrite()   {return mode & FS_MODE_WRITE;}  ///< True if writable
   bool CanExecute() {return mode & FS_MODE_EXECUTE;}///< True if executable
   bool IsHidden()   {return mode & FS_MODE_HIDDEN;} ///< True if hidden
   bool IsSystem()   {return mode & FS_MODE_SYSTEM;} ///< True if system entry
   bool IsFile()     {return mode & FS_MODE_FILE;}   ///< True if file
   bool IsDir()      {return !IsFile();}             ///< True if directory
};



/**! @class FilePath
 *   @brief A class to sanitize and deal with absolute and relative file paths
 */

class FilePath {

protected :
   std::string drive;///< Standard 'drive' on windows like "C:" on linux always "" for root
   std::vector<std::string> folderpath;///< Vector of folder names
   std::string file;///< File name

   std::string fullpath;

   std::vector<std::string> kablooey;/// All path components
   


   void SetPathComponents();

public :
   ///< FilePath constructor. This uses SanitizePath, so it can be as dumb as you want
   FilePath(std::string path);
   
   
   std::string Drive();///< Get the drive letter (on Linux is always empty string "")
   std::string Folder();///< Get the full folder path
   std::string File();///< Get the file name
   std::string Path();///< Get the full path, sanitized for your protection...
};



/**! @class FSInfo
 *   @brief File system info for a file system object
 */

class FSInfo {
protected :
   FilePath fpath;
   bool fexists;
   FSMode fmode;
   time_t tcreate;
   time_t tmodify;
   time_t taccess;
   unsigned long long fsize;

public :
   FSInfo();
   FSInfo(FilePath path);/// Uses the path to read the file attributes
   FSInfo(FilePath path , bool exists , FSMode mode , 
          time_t creation_time , time_t modify_time , time_t access_time , unsigned long long size);
   virtual ~FSInfo() {}
          
   
   void SetPath(FilePath fp);
   
   FilePath FPath() {return fpath;}
   std::string Path() {return fpath.Path();}///< Returns the full string path
   bool Exists() {return fexists;}///< True if this path exists
   FSMode Mode() {return fmode;}///< Returns the @ref FSMode
   time_t TimeCreated() {return tcreate;}///< Returns the file creation time
   time_t TimeModified() {return tmodify;}///< Returns the file modification time
   time_t TimeAccessed() {return taccess;}///< Returns the last access time of the file
   unsigned long long Size() {return fsize;}///< Returns the size in bytes
};



class File;
class ArchiveFile;



/**! @class Folder
 *   @brief A class to represent folders and their contents
 */

class Folder {
public :
   typedef std::map<std::string , std::shared_ptr<File> > FILEMAP;
   typedef std::map<std::string , std::shared_ptr<ArchiveFile> > ARCHIVEMAP;
   typedef std::map<std::string , std::shared_ptr<Folder> > SUBFOLDERMAP;

protected :
   Folder* parent;
   FSInfo finfo;
   std::string fname;

   FILEMAP files;
   ARCHIVEMAP archives;
   SUBFOLDERMAP subfolders;

   friend class FileSystem;
   
   void RegisterFile(std::shared_ptr<File> f);
   void RegisterArchiveFile(std::shared_ptr<ArchiveFile> af);
   void RegisterSubFolder(std::shared_ptr<Folder> f);
   
public :
   Folder(FSInfo info);///< Create a folder object from an @ref FSInfo object (which may be constructed with an absolute or relative path)
   
   virtual ~Folder();
   
   Folder* Parent() {return parent;}///< Get a pointer to the parent folder
   FSInfo Info() {return finfo;}///< Get this folder's @ref FSInfo

   virtual std::string Path() {return finfo.Path();}///< Get this folder's full path
   std::string Name() {return fname;}///< Get this folder's name
   
   FILEMAP Files() {return files;}///< Get a list of files in this folder
   ARCHIVEMAP Archives() {return archives;}///< Get a list of archive files in this folder
   SUBFOLDERMAP SubFolders() {return subfolders;}///< Get the list of sub folders for this folder
   
   void PrintContents(Indenter indent = Indenter(0,3));///< Print relative paths of all contents read
   void PrintContentsAbsolute();///< Print absolute paths of all contents
};



/**! @class Drive
 *   @brief The Drive class is a sub class of @ref Folder, and represents a drive on a filesystem
 *
 *   It differs from @ref Folder only in name
 */

class Drive : public Folder {
protected :
   
public :
   Drive(FSInfo info) : Folder(info) {}
   ~Drive() {}
};



/**! @class File
 *   @brief A simple class for representing files
 */

class File {

   friend class Folder;

protected :
   Folder* parent;
   FSInfo finfo;
   std::string fname;
   std::string fext;
   
   void SetParent(Folder* parent_folder);///< Sets the parent, for internal use only

public :   
   
   File(FSInfo info);///< Create a file object from an @ref FSInfo object (a path)
   virtual ~File() {}
   
   
   bool IsArchive();
   
   FSInfo Info() {return finfo;}///< Get the @ref FSInfo for this file object
   std::string Name() {return fname;}///< Get the file's name
   std::string Ext() {return fext;}///< Get the file's extension (everything after the last dot)
   
   std::string Path() {return finfo.Path();}///< Get the file's full path
};



/**! @class ArchiveFile
 *   @brief This class is for working with archive files transparently.
 *   
 *   ArchiveFile objects are both @ref File objects and @ref Folder objects. You can read their contents using those class's methods.
 */

class ArchiveFile : public File , public Folder {
   
public :
///   ArchiveFile(FSInfo fsinfo);
ArchiveFile(FSInfo fsinfo) :
      File(fsinfo),
      Folder(fsinfo)
{}
   
   virtual std::string Path() {return this->File::finfo.Path();}///< Get the full path to this archive
};




#endif // File_HPP



