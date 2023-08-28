



#include "Eagle/SoundManager.hpp"




SoundSample* SoundManager::CreateSoundSample(FilePath fp) {
   return PrivateCreateSoundSample(fp);
}



SoundInstance* SoundManager::CreateSoundInstance(SoundSample* psample) {
   return PrivateCreateSoundInstance(psample);
}



SoundStream* SoundManager::CreateSoundStream(FilePath fp) {
   return PrivateCreateSoundStream(fp);
}

