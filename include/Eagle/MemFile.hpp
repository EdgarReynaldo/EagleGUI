
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

