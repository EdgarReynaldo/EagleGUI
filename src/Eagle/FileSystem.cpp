
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
 *    Copyright 2009-2023+ by Edgar Reynaldo
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

#include <deque>






char NativePathSeparator() {
   return EAGLE_NATIVE_PATH_SEP;
}



/// Global utility functions



std::string GetFileName(std::string path) {
   
   size_t index = path.find_last_of("\\/");
   
   if (index == std::string::npos) {
      return path;
   }
   
   return path.substr(index + 1);
}



std::string GetFileExt(std::string filename) {
   size_t index = filename.find_last_of('.');
   if (index == std::string::npos) {
      return "";
   }
   return filename.substr(index + 1);
}



std::vector<std::string> ExplodePath(std::string path) {
      
   std::vector<std::string> components;
   
   /// separate into path components
   size_t stop = path.length();
   size_t index = 0;

   while (index < stop) {
      size_t j = path.find_first_of("\\/" , index);
      if (j == std::string::npos) {
         j = stop;
      }
      components.push_back(path.substr(index , j - index));
      index = SkipSeparators(path , j);
   }
   return components;
}



/// For EagleSystem and FileSystem
#include "Eagle/System.hpp"
#include "Eagle/Lib.hpp"

std::vector<std::string> GetAbsolutePath(std::string path) {
   
   
   EagleSystem* sys = Eagle::EagleLibrary::System("Any");
   EAGLE_ASSERT(sys);
   FileSystem* fs = sys->GetFileSystem();
   EAGLE_ASSERT(fs);
   
   std::string CWD = fs->CurrentFSDirectory();
   std::vector<std::string> cwdir = ExplodePath(CWD);

   if (!path.size()) {
      /// Disregard below comment. Must return an empty vector of strings to avoid returning false paths
      return std::vector<std::string>();
      /// Empty path string. Give them the current working directory, because they
      /// neither passed an absolute path nor a relative one.
      /// return cwdir;
   }
   
   std::vector<std::string> paths = ExplodePath(path);
   std::deque<std::string> cleanpaths;
   
   bool relative = false;
   /// An absolute path starts with a drive, anything else is assumed to be relative
   /// On Windows, we have ex. c: and on linux we have ex. "" an empty string for root drive
   if ((paths[0].compare("") != 0) && /// Linux root drive is the empty string
       (paths[0].find_first_of(":") == std::string::npos)) {/// Windows drives use colon
      relative = true;
   }
   
   if (!relative) {
      return paths;/// Already an absolute path
   }
   
   /// Relative path
   /// CWD/./../../abc/..
   /// Filter all references to the parent or current directory
   for (int i = paths.size() - 1 ; i >= 0 ; i--) {
      std::string s = paths[i];
      if (s.compare(".") == 0) {
         continue;
      }
      else if (s.compare("..") == 0) {
         i--;
         continue;
      }
      else {
         cleanpaths.push_front(s);
      }
   }

   cwdir.insert(cwdir.end() , cleanpaths.begin() , cleanpaths.end());/// Append relative directories to absolute working directory
   return cwdir;
}



std::string SanitizePath(std::vector<std::string> abspath) {
   std::string p;
   for (int i = 0 ; i < (int)abspath.size() ; ++i) {
      p += abspath[i];
      if (i != (int)abspath.size() - 1) {
         p += "/";/// DONT USE NativePathSeparator();
      }
   }
   return p;
}




std::string SanitizePath(std::string path) {
   std::vector<std::string> abspath = GetAbsolutePath(path);
   return SanitizePath(abspath);
}



std::string FileSystem::CurrentDirectory() {
   const int SIZE = 4096;// 4k SHOULD be enough....
   char buf[SIZE];
#ifdef EAGLE_WIN32
   _getcwd(buf , SIZE - 1);
#else
   getcwd(buf , SIZE - 1);
#endif
   return buf;
}


std::string FileSystem::GetCWD() {return CurrentDirectory();}



/// ----------------------------      FileSystem     ----------------------------------------



FileSystem::FileSystem() : 
      drives(),
      archive_mounted(false),
      mount_file_path()
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




