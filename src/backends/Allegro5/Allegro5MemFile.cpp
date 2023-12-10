
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
 * @file Allegro5MemFile.cpp
 * @brief Allegro 5 memory file implementation.
 */



#include "Eagle/MemFile.hpp"
#include "Eagle/Exception.hpp"
#include "Eagle/StringWork.hpp"

#include "Eagle/backends/Allegro5/Allegro5MemFile.hpp"

#include <cstdio>

#include "allegro5/allegro.h"

Allegro5MemFile::Allegro5MemFile(std::string file) : 
      MemFile(FSInfo(FilePath(file)))
{}



Allegro5MemFile::Allegro5MemFile(FSInfo info) :
      MemFile(info)
{}



bool Allegro5MemFile::ReadFileIntoMemory() {
   std::string fpath = finfo.Path();
   const char* path = fpath.c_str();
   ALLEGRO_FILE* file = al_fopen(path , "rb");
   if (!file) {
      EagleError() << StringPrintF("MemFile::ReadFileIntoMemory - failed to open file '%s' for reading" , path) << std::endl;
      return false;
   }
   
   fmem = std::vector<uint8_t>(finfo.Size() , '0');
   
   if (finfo.Size() != al_fread(file , (void*)&fmem[0] , sizeof(uint8_t)*finfo.Size())) {
      throw EagleException("File::ReadFileIntoMemory - failed to read full file size!\n");
   }
   
   al_fclose(file);
   return true;
}



bool Allegro5MemFile::WriteFileToDisk() {
   ALLEGRO_FILE* file = al_fopen(finfo.Path().c_str() , "wb");
   if (!file) {
      throw EagleException(StringPrintF("File::WriteFileToDisk - failed to open file '%s' for writing!\n" , finfo.Path().c_str()));
   }
   if (fmem.size() != al_fwrite(file , (const void*)&fmem[0] , sizeof(uint8_t)*fmem.size())) {
      al_fclose(file);
      throw EagleException("File::WriteFileToDisk - failed to write entire buffer to file.\n");
   }
   al_fclose(file);
   
   return true;
}


uint8_t* Allegro5MemFile::Begin() {
   if (fmem.empty()) {return 0;}
   return &fmem[0];
}



uint8_t* Allegro5MemFile::End() {
   if (fmem.empty()) {return 0;}
   return (&fmem[0] + fmem.size());
}


