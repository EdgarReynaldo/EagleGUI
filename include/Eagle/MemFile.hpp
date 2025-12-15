
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2023+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file MemFile.hpp
 * @brief Interface for working with memory files
 *
 * Note: Plain MemFile objects do not work with archives. Use an Allegro5MemFile instead.
 */

#ifndef MemFile_HPP
#define MemFile_HPP



#include <vector>
#include <cstdint>


#include "Eagle/File.hpp"


/**! @class MemFile
 *   @brief A class to store files in memory, edit, and save them
 */

class MemFile : public File {
protected :
   std::vector<uint8_t> fmem;
   
public :
   MemFile(std::string file) : File(FSInfo(FilePath(file))) {}
   MemFile(FSInfo info) : File(info) {}///< Construct a MemFile from @param info

   virtual ~MemFile() {Clear();}///< Destructor frees reserved memory
   
   void Clear();///< Free the stored memory
   
   bool ReadFileIntoMemory();///< Read the file from the stored file path
   bool WriteFileToDisk();///< Write the file back out to disk
   
   uint8_t* Begin();///< Get a pointer to the beginning of the file memory
   uint8_t* End();///< Get a pointer to the end of the file memory

   uint32_t Size() {return fmem.size();}///< Get the size in bytes of the memfile
};




#endif // MemFile_HPP



