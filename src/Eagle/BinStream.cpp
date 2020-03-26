



#include "Eagle/BinStream.hpp"
#include "Eagle/Exception.hpp"


const unsigned int endian_value = ('B' << 24 | 'E' << 16 | 'E' << 8 | 'L');
const bool big_endian = endian_value == 0x4245454C;
const bool little_endian = !big_endian;




void BinStream::PushData(const void* dat , unsigned int sz , bool reverse) {
   EAGLE_ASSERT(sz);
   if (!sz) {return;}

   const unsigned int end = bytes.size();
   bytes.resize(bytes.size() + sz);
   if (!reverse) {
      memcpy(&bytes[end] , dat , sz);
      return;
   }

   unsigned char* rdat = (unsigned char*)dat + ((int)sz - 1);
   for (unsigned int i = 0 ; i < sz ; ++i) {
      bytes[end + i] = *(rdat--);
   }
}



bool BinStream::Seek(unsigned int bytenum) {
   if (bytenum > Size()) {return false;}
   offset = bytenum;
   return true;
}



template <>
BinStream& BinStream::operator>>(std::string& str) {
   str.clear();
   unsigned int sz = Size();
   while (offset < sz) {
      if (bytes[offset] != '\0') {
         str.push_back(bytes[offset++]);
      }
      else {
         offset++;/// skip the null
         break;
      }
   }
   return *this;
}



template <>
BinStream& BinStream::operator<<(const char* str) {
   this->PushData((const void*)str , strlen(str) + 1 , false);
   return *this;
}



template <>
BinStream& BinStream::operator<<(const std::string& str) {
   (*this) << str.c_str();
   return *this;
}



