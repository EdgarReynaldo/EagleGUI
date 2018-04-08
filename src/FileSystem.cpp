



#include "Eagle/FileSystem.hpp"
#include "Eagle/File.hpp"
#include "Eagle/FileWork.hpp"
#include "Eagle/Exception.hpp"


/// ----------------------------      FileSystem     ----------------------------------------



FileSystem::FSCREATOR FileSystem::CreateFunc = 0;
FileSystem::FSDESTRUCTOR FileSystem::DestroyFunc = FileSystem::DestroyFileSystem;
FileSystem* FileSystem::fsys = 0;



void FileSystem::DestroyFileSystem() {
   if (fsys) {
      delete fsys;
   }
   fsys = 0;
}




void FileSystem::RegisterFile(Folder* parent , File* file) {
   EAGLE_ASSERT(parent);
   EAGLE_ASSERT(file);
   parent->RegisterFile(file);
}



void FileSystem::RegisterSubFolder(Folder* parent , Folder* sub) {
   EAGLE_ASSERT(parent);
   EAGLE_ASSERT(sub);
   parent->RegisterSubFolder(sub);
}



FileSystem* FileSystem::Instance() {
   if (fsys) {
      return fsys;
   }
   else {
      if (!CreateFunc) {
         throw EagleException("FileSystem::Instance - no filesystem creation function registered!\n");
      }
      fsys = CreateFunc();
      if (fsys) {
         atexit(DestroyFileSystem);
      }
   }
   return 0;
}



std::string GetFileName(std::string path) {
   return FileSystem::Instance()->GetFileName(path);
}



std::string GetFileExt(std::string name) {
   return FileSystem::Instance()->GetFileExt(name);
}



FSInfo GetFileInfo(std::string path) {
   return FileSystem::Instance()->GetFileInfo(path);
}



std::shared_ptr<Folder> ReadFolder(std::string path) {
   return FileSystem::Instance()->ReadFolder(path);
}



std::shared_ptr<File> ReadFile(std::string path) {
   return FileSystem::Instance()->ReadFile(path);
}



std::string SanitizePath(std::string path) {
   FilePath fp(path);
   return fp.Path();
}



std::vector<std::string> ExplodePath(std::string path) {
      
   std::vector<std::string> components;
   
   /// separate into path components
   int stop = path.length();
   int index = 0;

   while (index < stop) {
      unsigned int j = path.find_first_of("\\/" , index);
      components.push_back(path.substr(index , j));
      index = SkipSeparators(path , j);
   }
   return components;
}



char PathSeparator() {
   return FileSystem::Instance()->PathSeparator();
}




