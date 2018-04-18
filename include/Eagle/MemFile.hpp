



#ifndef MemFile_HPP
#define MemFile_HPP

#include <vector>

#include "Eagle/File.hpp"



class MemFile : public File {
protected :
   std::vector<unsigned char> fmem;
public :
   
   MemFile(FSInfo info) : File(info) {}
   
   bool ReadFileIntoMemory();
   bool WriteFileToDisk();
   
   unsigned char* Begin();
   unsigned char* End();
};


#endif // MemFile_HPP

