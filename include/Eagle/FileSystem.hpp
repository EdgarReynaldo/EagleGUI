



#ifndef FileSystem_HPP
#define FileSystem_HPP



#include <vector>


#include "Eagle/File.hpp"

class Drive;



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

   virtual FSInfo GetFileInfo(FilePath path)=0;
   
   /// Read functions
   virtual std::shared_ptr<Folder> ReadFolder(FilePath path)=0;
   virtual std::shared_ptr<File>   ReadFile  (FilePath path)=0;
   
   /// Info
   virtual char PathSeparator()=0;
   
   /// Directory functions
   virtual bool ChangeDirectory(std::string dir)=0;
   virtual std::string CurrentDirectory()=0;

};


std::string GetFileName(std::string path);/// Returns everything after the last path separator
std::string GetFileExt(std::string name);

FSInfo      GetFileInfo(FilePath path);
FSInfo      GetFileInfo(std::string path);
FSInfo      GetFileInfo(const char* path);

std::shared_ptr<Folder> ReadFolder(FilePath fpath);
std::shared_ptr<File> ReadFile(FilePath fpath);

std::vector<std::string> ExplodePath(std::string path);
std::vector<std::string> GetAbsolutePath(std::string rpath);

std::string SanitizePath(std::string path);/// Removes all references to relative directories and replaces all separators with native ones



char PathSeparator();
bool ChangeDirectory(std::string dir);
std::string CurrentDirectory();


#endif // FileSystem_HPP

