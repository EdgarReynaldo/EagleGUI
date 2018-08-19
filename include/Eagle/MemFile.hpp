



#ifndef MemFile_HPP
#define MemFile_HPP

#include <vector>

#include "Eagle/File.hpp"



class MemFile : public File {
protected :
   std::vector<unsigned char> fmem;
public :
   
   MemFile(FSInfo info) : File(info) {}
   ~MemFile() {Clear();}
   
   void Clear() {fmem.clear();}
   
   bool ReadFileIntoMemory();
   bool WriteFileToDisk();
   
   unsigned char* Begin();
   unsigned char* End();
   unsigned int Size() {return fmem.size();}
};


#endif // MemFile_HPP

