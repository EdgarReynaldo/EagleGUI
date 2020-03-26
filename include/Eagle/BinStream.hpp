



#ifndef BinStream_HPP
#define BinStream_HPP

extern const unsigned int endian_value;
extern const bool big_endian;
extern const bool little_endian;

#include <vector>
#include <string>
#include <cstring>
#include "Eagle/StringWork.hpp"
#include "Eagle/Exception.hpp"


class BinStream {
   unsigned int offset;
   
   std::vector<unsigned char> bytes;
   
   void PushData(const void* dat , unsigned int sz , bool reverse);
   
   template <class D>
   void PopDataFront(D& data , bool reverse);

public :

   const void* Data() const {return &bytes[0];}
   unsigned int Size() const {return bytes.size();}

   void Rewind() {offset = 0;}
   bool Seek(unsigned int bytenum);
   
   void Clear() {bytes.clear();}

   unsigned int Offset() {return offset;}

   template <class T>
   BinStream& operator<<(const T* t);

   template <class T>
   BinStream& operator<<(const T& t);

   
   template <class D>
   BinStream& operator>>(D& data);

//   template <>
//   BinStream& operator>>(std::string& str);
};




template <class D>
BinStream& BinStream::operator>>(D& data) {
   PopDataFront(data , big_endian);
   return *this;
}



template <class D>
void BinStream::PopDataFront(D& data , bool reverse) {
   
   if (sizeof(data) + offset > Size()) {
      throw EagleException(StringPrintF("Tried to read past end of read buffer!"));
   }
   
   if (!reverse) {
      memcpy((void*)&data , &bytes[offset] , sizeof(data));
   }
   else {
      for (unsigned int i = 1 ; i <= sizeof(data) ; ++i) {
         *(unsigned char*)(&data) = bytes[bytes.size() - i];
      }
   }
   offset += sizeof(data);
}



template <class T>
BinStream& BinStream::operator<<(const T* t) {
   (*this) << *t;
   return *this;
}



template <class T>
BinStream& BinStream::operator<<(const T& t) {
   this->PushData((const void*)t , sizeof(t) , big_endian);
   return *this;
}

template <>
BinStream& BinStream::operator<<(const char* str);

template <>
BinStream& BinStream::operator<<(const std::string& str);















#endif // BinStream_HPP
