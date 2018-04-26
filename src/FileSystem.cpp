



#include "Eagle/FileSystem.hpp"
#include "Eagle/File.hpp"
#include "Eagle/FileWork.hpp"
#include "Eagle/Exception.hpp"
#include "Eagle/StringWork.hpp"



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
   return fsys;
}



std::string GetFileName(std::string path) {
   return FileSystem::Instance()->GetFileName(path);
}



std::string GetFileExt(std::string name) {
   return FileSystem::Instance()->GetFileExt(name);
}



FSInfo GetFileInfo(FilePath path) {
   return FileSystem::Instance()->GetFileInfo(path);
}



FSInfo GetFileInfo(std::string path) {
   return GetFileInfo(FilePath(path));
}



FSInfo GetFileInfo(const char* path) {
   return GetFileInfo(FilePath(std::string(path)));
}



std::shared_ptr<Folder> ReadFolder(FilePath fpath) {
   return FileSystem::Instance()->ReadFolder(fpath);
}



std::shared_ptr<File> ReadFile(FilePath fpath) {
   return FileSystem::Instance()->ReadFile(fpath);
}



std::vector<std::string> ExplodePath(std::string path) {
      
   std::vector<std::string> components;
   
   /// separate into path components
   int stop = path.length();
   int index = 0;

   while (index < stop) {
      unsigned int j = path.find_first_of("\\/" , index);
      components.push_back(path.substr(index , j));
      if (j == std::string::npos) {
         break;
      }
      index = SkipSeparators(path , j);
   }
   return components;
}




std::vector<std::string> GetAbsolutePath(std::string path) {
   /// CWD/./../../abc/..
   /// Filter all references to the current directory
   std::vector<std::string> paths = ExplodePath(path);
   
   std::vector<std::string>::iterator it = paths.begin();
   while (it != paths.end()) {
      if (it->compare(".") == 0) {
         it = paths.erase(it);
      }
      else {
         ++it;
      }
   }
   bool relative = false;
   /// Check for references to parent directories
   for (int i = 0 ; i < (int)paths.size() ; ++i) {
      if (paths[i].compare("..") == 0) {
         relative = true;
         break;
      }
   }
   
   if (!relative) {
      return paths;
   }
   
   std::vector<std::string> abspath = ExplodePath(CurrentDirectory());
   
   abspath.insert(abspath.end() , paths.begin() , paths.end());/// Append relative directories to absolute working directory
   
   std::vector<std::string>::iterator it1 = abspath.begin();
   while(it1 != abspath.end()) {
      std::vector<std::string>::iterator it2 = it1;
      if (it1->compare("..") == 0) {
         /// Our absolute path starts with a relative directory
         throw EagleException(StringPrintF("GetAbsolutePathComponents - cannot resolve path '%s'\n" , path.c_str()));
      }
      ++it2;
      if (it2 != abspath.end() && it2->compare("..") == 0) {
         abspath.erase(it1 , it2);
         it = abspath.begin();
      }
      else {
         ++it1;
      }
   }
   return abspath;
}



std::string SanitizePath(std::string path) {
   std::vector<std::string> abspath = GetAbsolutePath(path);
   std::string p;
   for (int i = 0 ; i < (int)abspath.size() ; ++i) {
      p += path[i];
      if (i != (int)abspath.size() - 1) {
         p += PathSeparator();
      }
   }
   return p;
}



char PathSeparator() {
   return FileSystem::Instance()->PathSeparator();
}



bool ChangeDirectory(std::string dir) {
   return FileSystem::Instance()->ChangeDirectory(dir);
}



std::string CurrentDirectory() {
   return FileSystem::Instance()->CurrentDirectory();
}




