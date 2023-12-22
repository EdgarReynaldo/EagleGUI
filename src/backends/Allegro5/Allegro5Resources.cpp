



#include "Eagle/backends/Allegro5/Allegro5Resources.hpp"
#include "Eagle/Lib.hpp"
#include "Eagle/System.hpp"
#include "Eagle/FileSystem.hpp"
#include "allegro5/allegro.h"


bool Allegro5TextResource::LoadFromArgs(std::vector<std::string> args) {
   (void)args;
   EagleSystem* sys = Eagle::EagleLibrary::System("Any");
   EAGLE_ASSERT(sys);
   FileSystem* fs = sys->GetFileSystem();
   EAGLE_ASSERT(fs);
   
   std::shared_ptr<File> fl = fs->ReadFile(filepath);
   if (!fl.get()->Info().Exists()) {
      EagleError() << "Failed to find file " << filepath.Path() << std::endl;
      return false;
   }
   if (!fl.get()->Info().Mode().CanRead()) {
      EagleError() << "Cannot read file " << filepath.Path() << std::endl;
   }
   ALLEGRO_FILE* f = al_fopen(filepath.Path().c_str() , "rb");
   EAGLE_ASSERT(f);
   filetext.resize(fl.get()->Info().Size() + 1);
   bool success = (al_fread(f , &filetext.at(0) , sizeof(char)*fl.get()->Info().Size()) == sizeof(char)*fl.get()->Info().Size());
   filetext[fl.get()->Info().Size()] = '\0';
   al_fclose(f);
   return success;
}
