



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
   virtual FSInfo GetFileInfo(std::string path)=0;
   
   /// Read functions
   virtual std::shared_ptr<Folder> ReadFolder(std::string path)=0;
   virtual std::shared_ptr<File>   ReadFile  (std::string path)=0;
   
   virtual char PathSeparator()=0;
};


std::string GetFileName(std::string path);/// Returns everything after the last path separator
std::string GetFileExt(std::string name);
FSInfo      GetFileInfo(std::string path);


std::shared_ptr<Folder> ReadFolder(std::string path);
std::shared_ptr<File> ReadFile(std::string path);
std::string SanitizePath(std::string path);
std::vector<std::string> ExplodePath(std::string path);
char PathSeparator();



#endif // FileSystem_HPP

