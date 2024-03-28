



#include "Eagle/backends/Allegro5/Allegro5BinStream.hpp"
#include "Eagle/System.hpp"
#include "Eagle/FileSystem.hpp"
#include "Eagle/Lib.hpp"


#include "allegro5/allegro.h"


bool Allegro5BinStream::SaveDataToFile(FilePath fp) const  {
   EagleSystem* sys = Eagle::EagleLibrary::System("Any");
   EAGLE_ASSERT(sys);
   FileSystem* fs = sys->GetFileSystem();
   std::shared_ptr<File> fl = fs->ReadFile(fp.Path());
   File* f = fl.get();
   if (f->Info().Exists() && !f->Info().Mode().CanWrite()) {
      EagleError() << "Cannot write to file " << fp.Path() << std::endl;
      return false;
   }
   ALLEGRO_FILE* fdat = al_fopen(fp.Path().c_str() , "wb");
   if (!fdat) {
      EagleError() << "fopen failed to open file " << fp.Path() << std::endl;
      return false;
   }
   uint64_t w = al_fwrite(fdat , (const void*)Data() , sizeof(uint8_t)*Size());
   if (w != Size()) {
      EagleError() << "fwrite failed to write " << Size() << " bytes to file " << fp.Path() << std::endl;
      al_fclose(fdat);
      return false;
   }
   al_fclose(fdat);
   return true;
}



bool Allegro5BinStream::ReadDataFromFile(FilePath fp) {
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
   ALLEGRO_FILE* fdat = al_fopen(fp.Path().c_str() , "rb");
   if (!fdat) {
      EagleError() << "Failed to open file " << fp.Path() << "for reading in binary mode" << std::endl;
      return false;
   }
   Resize(fatt.get()->Info().Size());
   uint64_t read = al_fread(fdat , &bytes[0] , sizeof(uint8_t)*fatt.get()->Info().Size());
   if (read != Size()) {
      EagleError() << "Failed to read entire file " << fp.Path() << " into memory." << std::endl;
      al_fclose(fdat);
      return false;
   }
   al_fclose(fdat);
   return true;
}
