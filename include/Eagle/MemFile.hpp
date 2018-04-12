



#ifndef MemFile_HPP
#define MemFile_HPP

#include <vector>

#include "Eagle/File.hpp"



class MemFile : public File {
protected :
   std::vector<unsigned char> fmem;
public :
   bool ReadFileIntoMemory();
   bool WriteFileToDisk();
};


#endif // MemFile_HPP

