

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


#ifndef File_HPP
#define File_HPP

#include <string>
#include <vector>
#include <map>

#include <ctime>
#include <memory>


enum FS_MODE_FLAGS {
   FS_MODE_READ    = 1 << 0,
   FS_MODE_WRITE   = 1 << 1,
   FS_MODE_EXECUTE = 1 << 2,
   FS_MODE_HIDDEN  = 1 << 3,
   FS_MODE_SYSTEM  = 1 << 4,
   FS_MODE_FILE    = 1 << 5
};

class FSMode {
protected :
   unsigned int mode;
public :
   FSMode(int flags) : mode(flags) {}
   bool CanRead() {return mode & FS_MODE_READ;}
   bool CanWrite() {return mode & FS_MODE_WRITE;}
   bool CanExecute() {return mode & FS_MODE_EXECUTE;}
   bool IsHidden() {return mode & FS_MODE_HIDDEN;}
   bool IsSystem() {return mode & FS_MODE_SYSTEM;}
   bool IsFile() {return mode & FS_MODE_FILE;}
   bool IsDir() {return !IsFile();}
};


class FilePath {

protected :
   std::string drive;
   std::vector<std::string> folderpath;   
   std::string file;
   
   void SetPathComponents(const std::vector<std::string>& paths);

public :
   FilePath(std::string path);/// This uses SanitizePath, so it can be as dumb as you want

   std::string Drive();
   std::string Folder();
   std::string File();
   std::string Path();/// Returns full path, sanitized

};


class FSInfo {
protected :
   FilePath fpath;
   FSMode fmode;
   time_t tcreate;
   time_t tmodify;
   time_t taccess;
   unsigned long long fsize;
public :
   
   FSInfo(FilePath path , FSMode mode , time_t creation_time , time_t modify_time , time_t access_time , unsigned long long size);
   
   std::string Path() {return fpath.Path();}
   FSMode Mode() {return fmode;}
   time_t TimeCreated() {return tcreate;}
   time_t TimeModified() {return tmodify;}
   time_t TimeAccessed() {return taccess;}
   unsigned long long Size() {return fsize;}
};



class File;

class Folder {
protected :
   Folder* parent;
   FSInfo finfo;
   std::string fname;
   std::map<std::string , std::shared_ptr<File>> files;
   std::map<std::string , std::shared_ptr<Folder>> subfolders;

   friend class FileSystem;
   
   void RegisterFile(File* f);
   void RegisterSubFolder(Folder* f);
   
public :
   Folder(FSInfo info);
   
   Folder* Parent() {return parent;}
   FSInfo Info() {return finfo;}
   std::string Path() {return finfo.Path();}
   std::string Name() {return fname;}
   
};



class Drive : public Folder {
protected :
   
public :
   Drive(FSInfo info) : Folder(info) {}
};



class File {

protected :
   Folder* parent;
   FSInfo finfo;
   std::string fname;
   std::string fext;
   std::vector<unsigned char> fmem;
   bool file_open;
   
   
public :   
   
   File(FSInfo info);
   
   bool ReadFileIntoMemory();
   bool WriteFileToDisk();
   
   FSInfo Info() {return finfo;}
   std::string Name() {return fname;}
   std::string Ext() {return fext;}
   
};



#endif // File_HPP











