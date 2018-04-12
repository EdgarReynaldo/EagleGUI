



#include "Eagle/MemFile.hpp"
#include "Eagle/Exception.hpp"
#include "Eagle/StringWork.hpp"

#include <cstdio>



bool MemFile::ReadFileIntoMemory() {
   FILE* file = fopen(finfo.Path().c_str() , "rb");
   if (!file) {return false;}
   
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

