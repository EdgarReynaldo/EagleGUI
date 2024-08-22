



#ifndef BinFileStream2_HPP
#define BinFileStream2_HPP



template<typename T>
uint32_t BinFileStream::ReadFromStream(T& t) {
   EAGLE_ASSERT(fdat);
   if (!fdat) {return false;}
   
   int read = 0;
   char* d = (char*)&t;
   
   if (little_endian) {// no conversion necessary
      for (unsigned int i = 0 ; i < sizeof(T) ; ++i) {
         char c = fgetc(fdat);
         if (c == EOF) {return false;}
         ++read;
         ++rwoffset;
         d[i] = c;
      }
   } else {/// need to convert endianness
      for (unsigned int i = (sizeof(T) - 1) ; i >= 0 ; --i) {
         char c = fgetc(fdat);
         if (c == EOF) {
            return false;
         }
         ++read;
         ++rwoffset;
         d[i] = c;
      }
   }
   return read;
}



   
template<>
uint32_t BinFileStream::ReadFromStream<std::string>(std::string& s) {
   s.clear();
   EAGLE_ASSERT(fdat);
   
   uint32_t n = 0;
   int c = 0;
   do {
      c = fgetc(fdat);
      if (EOF == c) {
         EagleError() << "Reached end of file trying to read a string. Null terminate your strings." << std::endl;
         return 0;
      }
      s.push_back((char)c);
      ++n;
   } while (c != '\0');
   return n;
}



template<typename T>
uint32_t BinFileStream::WriteToStream(const T& t) {
   EAGLE_ASSERT(fdat);
   if (!fdat) {return false;}
   
   const char* d = (const char*)&t;
   
   uint32_t n = 0;
   
   if (little_endian) {/// No need to convert endianness
      for (unsigned int i = 0 ; i < sizeof(T) ; ++i) {
         const int ret = fputc(d[i] , fdat);
         if (ret == d[i]) {
            ++n;
            ++rwoffset;
         }
         else {
            if (EOF == ret) {
               EagleError() << "EOF reached while trying to write to stream." << std::endl;
               return 0;
            }
         }
      }
   }
   else {/// Need to reverse endianness
      d += sizeof(T);
      d--;
      for (unsigned int i = 0 ; i < sizeof(T) ; ++i) {
         const int ret = fputc(d[0] , fdat);
         if (ret == d[0]) {
            ++n;
            d--;
            rwoffset++;
         }
         else if (EOF == ret) {
            EagleError() << "EOF reached while trying to write to stream." << std::endl;
            return 0;
         }
      }
   }
   if (rwoffset >= bytesize) {
      bytesize = rwoffset;
   }
   return n;
}



template<>
uint32_t BinFileStream::WriteToStream<const char*&>(const char*& s) {
   EAGLE_ASSERT(fdat);
   uint32_t n = 0,sz = strlen(s) + 1;
   for (unsigned int i = 0 ; i < sz ; ++i) {
      const int ret = fputc(s[i] , fdat);
      if (ret == (int)s[i]) {
         ++rwoffset;
         ++n;
         if (rwoffset >= bytesize) {
            bytesize = rwoffset;
         }
      }
      else {
            
      }
   }
   return n;
}



template<>
uint32_t BinFileStream::WriteToStream<const std::string&>(const std::string& s) {
   return WriteToStream(s.c_str());
}



template<class T>
BinFileStream& BinFileStream::operator>>(T& t) {
   EAGLE_ASSERT(sizeof(T) == ReadFromStream(t));
   return *this;
}



template<class T>
BinFileStream& BinFileStream::operator<<(const T& t) {
   EAGLE_ASSERT(sizeof(T) == WriteToStream(t));
   return *this;
}




#endif // BinFileStream2_HPP
