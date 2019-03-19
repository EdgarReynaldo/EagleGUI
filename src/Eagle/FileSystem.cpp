
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




#include "Eagle/Platform.hpp"

#ifndef EAGLE_NATIVE_PATH_SEP
   #error No path separator defined for eagle!
#endif // EAGLE_NATIVE_PATH_SEP

/// For _getcwd and getcwd
#ifdef EAGLE_WIN32
   #include <direct.h>
#else
   #include <unistd.h>
#endif



#include "Eagle/FileSystem.hpp"
#include "Eagle/File.hpp"
#include "Eagle/FileWork.hpp"
#include "Eagle/Exception.hpp"
#include "Eagle/StringWork.hpp"







char NativePathSeparator() {
   return EAGLE_NATIVE_PATH_SEP;
}



/// Global utility functions



std::string GetFileName(std::string path) {
   
   unsigned int index = path.find_last_of("\\/");
   
   if (index == std::string::npos) {
      return path;
   }
   
   return path.substr(index + 1);
}



std::string GetFileExt(std::string filename) {
   unsigned int index = filename.find_last_of('.');
   if (index == std::string::npos) {
      return "";
   }
   return filename.substr(index + 1);
}



std::vector<std::string> ExplodePath(std::string path) {
      
   std::vector<std::string> components;
   
   /// separate into path components
   unsigned int stop = path.length();
   unsigned int index = 0;

   while (index < stop) {
      unsigned int j = path.find_first_of("\\/" , index);
      if (j == std::string::npos) {
         j = stop;
      }
      components.push_back(path.substr(index , j - index));
      index = SkipSeparators(path , j);
   }
   return components;
}





std::vector<std::string> GetAbsolutePath(std::string path) {


   if (!path.size()) {
      return ExplodePath(GetCWD());
   }

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
   if ((paths[0].compare("") != 0) && /// Linux root drive is the empty string
       (paths[0].find_first_of(":") == std::string::npos)) {/// Windows drives use colon
      relative = true;
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
         p += "/";/// DONT USE NativePathSeparator();
      }
   }
   return p;
}



std::string CurrentDirectory() {
   const int SIZE = 512;
   char buf[SIZE];
#ifdef EAGLE_WIN32
   _getcwd(buf , SIZE - 1);
#else
   getcwd(buf , SIZE - 1)
#endif
   return buf;
}


std::string GetCWD() {return CurrentDirectory();}



/// ----------------------------      FileSystem     ----------------------------------------



FileSystem::FileSystem() : 
      drives(),
      archive_mounted(false),
      mount_file_path(std::string(""))
{}



void FileSystem::RegisterFile(Folder* parent , std::shared_ptr<File> file) {
   EAGLE_ASSERT(parent);
   EAGLE_ASSERT(file);
   parent->RegisterFile(file);
}



void FileSystem::RegisterArchiveFile(Folder* parent , std::shared_ptr<ArchiveFile> afile) {
   EAGLE_ASSERT(parent);
   EAGLE_ASSERT(afile);
   parent->RegisterArchiveFile(afile);
}



void FileSystem::RegisterSubFolder(Folder* parent , std::shared_ptr<Folder> sub) {
   EAGLE_ASSERT(parent);
   EAGLE_ASSERT(sub);
   parent->RegisterSubFolder(sub);
}



FSInfo FileSystem::GetFileInfo(std::string path) {
   return GetFileInfo(FilePath(path));
}



FSInfo FileSystem::GetFileInfo(const char* path) {
   return GetFileInfo(FilePath(std::string(path)));
}




