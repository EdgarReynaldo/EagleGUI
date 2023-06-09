
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
 *    Copyright 2009-2023+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file BinFileStream.cpp
 * @brief Implementation of Binary File Stream class not included in header as template
 */



#include "Eagle/BinFileStream.hpp"

#include "Eagle/FileSystem.hpp"



void BinFileStream::Close() {
   if (fdat) {
      fclose(fdat);
      fdat = 0;
      rwoffset = 0ULL;
      bytesize = 0ULL;
      file.reset();
   }
}



BinFileStream::BinFileStream() :
      fdat(0),
      bytesize((uint64_t)0),
      rwoffset((uint64_t)0),
      file()
{
   
}



BinFileStream::~BinFileStream() {
   Close();
}



bool BinFileStream::OpenFileStream(FilePath fp) {
   Close();
   
   EagleSystem* sys = Eagle::EagleLibrary::System("Any");
   EAGLE_ASSERT(sys);
   
   FileSystem* fsys = sys->GetFileSystem();
   EAGLE_ASSERT(fsys);
   
   file = fsys->ReadFile(fp);
   
   if (file.get()->Info().Exists()) {
      bytesize = file.get()->Info().Size();
   }
   else {
      bytesize = 0ULL;
   }
   std::string path = fp.Path();
   fdat = fopen(path.c_str() , "ab+");
   if (!fdat) {
      return false;
   }
   if (0 != fseek(fdat , 0L , SEEK_SET)) {
      return false;
   }
   rwoffset = 0ULL;
   
   return true;
}



void BinFileStream::CloseFileStream() {
   Close();
}



bool BinFileStream::Rewind() {
   return (uint64_t)0 == SeekAbsolute(0);
}



uint64_t BinFileStream::SeekAbsolute(uint64_t fabsoffsetbytes) {
   EAGLE_ASSERT(fdat);
   if (!fdat) {
      return ~(uint64_t)0;
   }

   /// Will always give same position since we're in binary mode
   /// If we seek past the end, it will casually create a zombie file as big as you request
   /// Perform some sanity check, maybe we shouldn't be creating a file larger than a GB or less maybe
   if (fabsoffsetbytes > bytesize) {
      EagleWarn() << "Creating sparse file, expanding size" << std::endl;
   }
   int ret = fseek(fdat , fabsoffsetbytes , SEEK_SET);
   if (0 == ret) {
      rwoffset = fabsoffsetbytes;
   }
   return (ret == 0)?fabsoffsetbytes:~(uint64_t(0));
} 



uint64_t BinFileStream::SeekRelative(int64_t reloffsetbytes) {
   if (!reloffsetbytes) {
      return rwoffset;
   }
   
   uint64_t newoffset = rwoffset;
   
   if (reloffsetbytes > 0) {
      /// Fast forwarding stream
      if ((uint64_t)reloffsetbytes > (~(uint64_t)0 - newoffset)) {
         EagleError() << "SeekRelative encountered an overflow condition" << std::endl;
      }
      else {
         newoffset += reloffsetbytes;
      }
   }
   else {
      /// Rewinding stream
      if (newoffset < (uint64_t)(-reloffsetbytes)) {
         EagleError() << "SeekRelative encountered an underflow condition" << std::endl;
      }
      else {
         newoffset -= (uint64_t)(-reloffsetbytes);
      }
   }
   return SeekAbsolute(newoffset);
}






