
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
 * @file BinStream.hpp
 * @brief For streaming binary data to a data file
 * 
 * 
 * 
 */

#ifndef BinStream_HPP
#define BinStream_HPP



#include <vector>
#include <string>
#include <cstring>
#include "Eagle/Endian.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/Exception.hpp"
#include "Eagle/File.hpp"




class BinStream {
   mutable uint64_t roffset;
   uint64_t woffset;
   
   std::vector<uint8_t> bytes;

public :

   const uint8_t* Data() const;
   uint64_t Size() const;
   
   bool SaveDataToFile(FilePath fp) const ;
   bool ReadDataFromFile(FilePath fp);
   
   void RewindReadHead() const;
   void RewindWriteHead();

   bool SeekReadHead(uint64_t bytenum) const;
   bool SeekWriteHead(uint64_t bytenum);
   
   void Clear();
   void Resize(uint64_t nbytes);

   uint64_t ReadOffset() const;
   uint64_t WriteOffset() const;
   
   uint64_t ReadData(uint8_t* dest , uint32_t sz , bool reverse) const;/// returns number of bytes read or zero on error
   uint64_t WriteData(const uint8_t* src , uint32_t sz , bool reverse);/// returns number of bytes written or zero on error

   template <class D>
   uint64_t ReadData(D& data , bool reverse) const;
   template <class D>
   uint64_t WriteData(const D& data , bool reverse);

   
   template <class D>
   const BinStream& operator>>(D& data) const;

   template <class T>
   BinStream& operator<<(const T* t);

   template <class T>
   BinStream& operator<<(const T& t);


};




template <class D>
uint64_t BinStream::ReadData(D& data , bool reverse) const {
   return ReadData((uint8_t*)&data , sizeof(D) , reverse);
}



template <class D>
uint64_t BinStream::WriteData(const D& data , bool reverse) {
   return WriteData(data , reverse);
}



template <class D>
const BinStream& BinStream::operator>>(D& data) const {
   bool dataread = sizeof(D) == ReadData(data , big_endian);
   EAGLE_ASSERT(dataread);
   if (!dataread) {
      EagleError() << "Failed to read " << sizeof(D) << " bytes from bin stream." << std::endl;
   }
   return *this;
}



template <class D>
BinStream& BinStream::operator<<(const D& data) {
   const bool datawritten = sizeof(D) == WriteData(data , big_endian);
   EAGLE_ASSERT(datawritten);
   if (!datawritten) {
      EagleError() << "Failed to write " << sizeof(D) << " bytes to bin stream." << std::endl;
   }
   return *this;
}



template <class T>
BinStream& BinStream::operator<<(const T* t) {
   (*this) << *t;
   return *this;
}



template <>
BinStream& BinStream::operator<<(const char* str);



template <>
BinStream& BinStream::operator<<(const std::string& str);















#endif // BinStream_HPP
