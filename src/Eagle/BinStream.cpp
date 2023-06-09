
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
 * @file BinStream.cpp
 * @brief Implementations for streaming binary data to a data file
 * 
 */



#include "Eagle/BinStream.hpp"
#include "Eagle/Exception.hpp"

#include "Eagle/Lib.hpp"
#include "Eagle/System.hpp"
#include "Eagle/FileSystem.hpp"



const uint8_t* BinStream::Data() const {
   return &bytes[0];
}



uint64_t BinStream::Size() const {
   return bytes.size();
}



bool BinStream::SaveDataToFile(FilePath fp) const  {
   EagleSystem* sys = Eagle::EagleLibrary::System("Any");
   EAGLE_ASSERT(sys);
   FileSystem* fs = sys->GetFileSystem();
   std::shared_ptr<File> fl = fs->ReadFile(fp.Path());
   File* f = fl.get();
   if (f->Info().Exists() && !f->Info().Mode().CanWrite()) {
      EagleError() << "Cannot write to file " << fp.Path() << std::endl;
      return false;
   }
   FILE* fdat = fopen(fp.Path().c_str() , "wb");
   if (!fdat) {
      EagleError() << "fopen failed to open file " << fp.Path() << std::endl;
      return false;
   }
   uint64_t w = fwrite((const void*)Data() , sizeof(uint8_t) , Size() , fdat);
   if (w != Size()) {
      EagleError() << "fwrite failed to write " << Size() << " bytes to file " << fp.Path() << std::endl;
      fclose(fdat);
      return false;
   }
   fclose(fdat);
   return true;
}



bool BinStream::ReadDataFromFile(FilePath fp) {
   EagleSystem* sys = Eagle::EagleLibrary::System("Any");
   EAGLE_ASSERT(sys);
   FileSystem* fs = sys->GetFileSystem();
   EAGLE_ASSERT(fs);
   std::shared_ptr<File> fatt = fs->ReadFile(fp);
   if (!fatt.get()->Info().Exists()) {
      EagleError() << "File " << fp.Path() << " does not exist. File cannot be read." << std::endl;
      return false;
   }
   if (!fatt.get()->Info().Mode().CanRead()) {
      EagleError() << "Cannot read file " << fp.Path() << std::endl;
      return false;
   }
   FILE* fdat = fopen(fp.Path().c_str() , "rb");
   if (!fdat) {
      EagleError() << "Failed to open file " << fp.Path() << "for reading in binary mode" << std::endl;
      return false;
   }
   Resize(fatt.get()->Info().Size());
   uint64_t read = fread(&bytes[0] , sizeof(uint8_t) , fatt.get()->Info().Size() , fdat);
   if (read != Size()) {
      EagleError() << "Failed to read entire file " << fp.Path() << " into memory." << std::endl;
      fclose(fdat);
      return false;
   }
   fclose(fdat);
   return true;
}



void BinStream::RewindReadHead() const {
   roffset = 0;
}



void BinStream::RewindWriteHead() {
   woffset = 0;
}



bool BinStream::SeekReadHead(uint64_t bytenum) const  {
   roffset = (bytenum >= Size())?Size():bytenum;
   return bytenum == roffset;
}



bool BinStream::SeekWriteHead(uint64_t bytenum) {
   woffset = (bytenum >= Size())?Size():bytenum;
   return woffset == bytenum;
}



void BinStream::Clear() {
   bytes.clear();
}



void BinStream::Resize(uint64_t nbytes) {
   bytes.resize(nbytes);
}



uint64_t BinStream::ReadOffset() const {
   return roffset;
}



uint64_t BinStream::WriteOffset() const {
   return woffset;
}



uint64_t BinStream::ReadData(uint8_t* dest , uint32_t sz , bool reverse) const {
   EAGLE_ASSERT(dest);
   EAGLE_ASSERT(sz);
   uint64_t n = 0;
   if (!reverse) {
      memcpy((void*)dest , (const void*)(&bytes[roffset]) , sz);
      roffset += sz;
      n += sz;
   }
   else {
      const uint8_t* p = &bytes[roffset];
      for (int64_t i = sz - 1 ; i >= 0 ; --i) {
         dest[i] = *p++;
         roffset++;
         ++n;
      }
   }
   return n;
}



uint64_t BinStream::WriteData(const uint8_t* src , uint32_t sz , bool reverse) {
   uint64_t totalsize = woffset + sz;
   uint64_t written = 0;
   if (totalsize >= bytes.size()) {
      bytes.resize(totalsize);
   }
   void* dest = &bytes[woffset];
   if (!reverse) {
      memcpy(dest , (const void*)src , sz);
      woffset += sz;
      written += sz;
   }
   else {
      for (uint64_t i = totalsize - 1 ; i >= woffset ; --i) {
         bytes[i] = (*src++);
      }
      woffset += sz;
      written += sz;
   }
   return written;
}



template <>
const BinStream& BinStream::operator>>(std::string& str) const {
   str.clear();
   unsigned int sz = Size();
   while (roffset < sz) {
      if (bytes[roffset] != '\0') {
         str.push_back(bytes[roffset++]);
      }
      else {
         roffset++;/// skip the null
         break;
      }
   }
   return *this;
}



template <>
BinStream& BinStream::operator<<(const char* str) {
   this->WriteData((const uint8_t*)str , strlen(str) + 1 , false);
   return *this;
}



template <>
BinStream& BinStream::operator<<(const std::string& str) {
   (*this) << str.c_str();
   return *this;
}



