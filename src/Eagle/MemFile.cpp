
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
 *    Copyright 2009-2021+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 */



#include "Eagle/MemFile.hpp"
#include "Eagle/Exception.hpp"
#include "Eagle/StringWork.hpp"

#include <cstdio>



bool MemFile::ReadFileIntoMemory() {
   std::string fpath = finfo.Path();
   const char* path = fpath.c_str();
   FILE* file = fopen(path , "rb");
   if (!file) {
      EagleError() << StringPrintF("MemFile::ReadFileIntoMemory - failed to open file '%s' for reading" , path) << std::endl;
      return false;
   }
   
   fmem = std::vector<unsigned char>(finfo.Size() , '0');
   
   if (finfo.Size() != fread(&fmem[0] , sizeof(unsigned char) , finfo.Size() , file)) {
      throw EagleException("File::ReadFileIntoMemory - failed to read full file size!\n");
   }
   
   fclose(file);
   return true;
}



bool MemFile::WriteFileToDisk() {
   FILE* file = fopen(finfo.Path().c_str() , "wb");
   if (!file) {
      throw EagleException(StringPrintF("File::WriteFileToDisk - failed to open file '%s' for writing!\n" , finfo.Path().c_str()));
   }
   if (fmem.size() != fwrite(&fmem[0] , sizeof(unsigned char) , fmem.size() , file)) {
      throw EagleException("File::WriteFileToDisk - failed to write entire buffer to file.\n");
   }
   fclose(file);
   
   return true;
}


uint8_t* MemFile::Begin() {
   if (fmem.empty()) {return 0;}
   return &fmem[0];
}



uint8_t* MemFile::End() {
   if (fmem.empty()) {return 0;}
   return (&fmem[0] + fmem.size());
}

