



#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"


int main(int argc , char** argv) {
   (void)argc;
   (void)argv;
   
   Allegro5System* sys = GetAllegro5System();
   EAGLE_ASSERT(sys);
   
   if (EAGLE_FULL_SETUP != sys->Initialize(EAGLE_FULL_SETUP)) {
      EagleWarn() << "Failed to initialize some subsystem. Continuiing." << std::endl;
   }

   Allegro5SoundManager soundman;
   bool soundup = soundman.Initialize();
   if (!soundup) {
      EagleCritical() << "Could not initialize sound manager." << std::endl;
      return -1;
   }
   
   soundman.ReserveInstances(10);
   EAGLE_ASSERT(soundman.InstanceCount() == 10);
   
   EagleSoundStream* stream = soundman.CreateSoundStream(FilePath("bgmusic.mp3"));
   
   
   
   return 0;
}
