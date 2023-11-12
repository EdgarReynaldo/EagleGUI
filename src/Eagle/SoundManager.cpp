



#include "Eagle/SoundManager.hpp"




EagleSoundSample* SoundManager::CreateSoundSample(FilePath fp) {
   return PrivateCreateSoundSample(fp);
}



EagleSoundInstance* SoundManager::CreateSoundInstance(EagleSoundSample* psample) {
   return PrivateCreateSoundInstance(psample);
}



EagleSoundStream* SoundManager::CreateSoundStream(FilePath fp) {
   return PrivateCreateSoundStream(fp);
}

