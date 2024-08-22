
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\____/     \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2024+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file BinFileStream.hpp
 * @brief Proper treatment of binary file streams
 */
 
#ifndef BinFileStream_HPP
#define BinFileStream_HPP


 
#include "Eagle/Lib.hpp"
#include "Eagle/System.hpp"
#include "Eagle/Endian.hpp"
#include "Eagle/File.hpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <memory>


/** @class BinFileStream
 *  @brief A binary file stream for convenience
 *
 *  Files are always saved little endian and converted for read and write operations
 *
 *  For best portability of data files across machines, use defined sizes for your data, like int16_t and int32_t and int64_t
 *  Although the bin stream is opened in ab+ mode, we always seek to 0 from SEEK_SET so the file is ready to read and write;
 *  Word to the wise, avoid the 'sparse' file as it will create a zombie the size you request of it. It's your responsibility
 *  to 'Rewind()' or seek the stream with SeekAbsolute(uint64_t foffsetbytes) or SeekRelative(reloffset)
 */

class BinFileStream {
protected :
   FILE*  fdat;
   uint64_t bytesize;
   uint64_t rwoffset;
   std::shared_ptr<File> file;
   
   void Close();
public :
   
   BinFileStream();
   ~BinFileStream();

   /// Opening and closing stream
   bool OpenFileStream(FilePath path);
   void CloseFileStream();
   
   /// Getters
   uint64_t FileSize() {return bytesize;}
   uint64_t RWOffset() {return rwoffset;}
   std::shared_ptr<File> FileInfo() {return file;}
   
   /// Seeking in stream
   bool Rewind();
   uint64_t SeekAbsolute(uint64_t fabsoffsetbytes);/// Will always give same position since we're in binary mode, beware sparse files!!XX##
                                                   /// Returns the position in the file or ~(uint64_t)0 on failure
   uint64_t SeekRelative(int64_t reloffsetbytes);/// returns the new absolute position or ~(uint64_t)0 on failure

   /// Stream reading methods. Returns the number of bytes read or 0 on error
   template<typename T>
   uint32_t ReadFromStream(T& t);
//   template<>
//   uint32_t ReadFromStream<std::string&>(std::string& s);

   /// Stream writing methods. Returns the number of bytes written or 0 on error
   template<typename T>
   uint32_t WriteToStream(const T& t);
//   template<>
//   uint32_t WriteToStream<const char*&>(const char*& str);
//   template<>
//   uint32_t WriteToStream<const std::string&>(const std::string& s);
   
   template<class T>
   BinFileStream& operator>>(T& t);/// Reads from file at rwoffset, advancing rwoffset and storing data in T

   template<class T>
   BinFileStream& operator<<(const T& t);/// Writes to file at rwoffset, advancing offset and appending if necessary
};



#endif // BinFileStream_HPP

