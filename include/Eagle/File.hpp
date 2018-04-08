

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

class FSInfo {
protected :
   std::string fpath;
   FSMode fmode;
   time_t tcreate;
   time_t tmodify;
   time_t taccess;
   unsigned long long fsize;
public :
   
   FSInfo(std::string path , FSMode mode , time_t creation_time , time_t modify_time , time_t access_time , unsigned long long size);
   
   std::string Path() {return fpath;}
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
   std::map<std::string , File*> files;
   std::map<std::string , Folder*> subfolders;

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



class FileSystem {
protected :
   std::vector<Drive*> drives;
   
   
   typedef FileSystem* (*FSCREATOR) ();
   typedef void (*FSDESTRUCTOR) ();
public :/// Need to friend Allegro5System but Eagle doesn't know about it
   static FSCREATOR CreateFunc;
protected :
   static FSDESTRUCTOR DestroyFunc;
   static FileSystem* fsys;
   
   FileSystem() : drives() {}
   
   static void DestroyFileSystem();
   
   virtual void ReadDirectoryContents(Folder* folder)=0;
   
   void RegisterFile(Folder* parent , File* file);
   void RegisterSubFolder(Folder* parent , Folder* sub);
   
public :
   virtual ~FileSystem() {}
   
   static FileSystem* Instance();
   
   /// Utility functions
   virtual std::string GetFileName(std::string path)=0;
   virtual std::string GetFileExt(std::string path)=0;
   virtual FSInfo GetFileInfo(std::string path)=0;
   
   /// Read functions
   virtual Folder* ReadFolder(std::string path)=0;
   virtual File* ReadFile(std::string path)=0;
};


std::string GetFileName(std::string path);/// Returns everything after the last path separator
std::string GetFileExt(std::string name);
FSInfo      GetFileInfo(std::string path);


#endif // File_HPP











