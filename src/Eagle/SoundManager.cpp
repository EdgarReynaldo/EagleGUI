



#include "Eagle/SoundManager.hpp"




EagleSoundSample* SoundManager::CreateSoundSample(FilePath fp) {
   return PrivateCreateSoundSample(fp);
}



EagleSoundSample* SoundManager::CreateBlankSample(int32_t freq , int32_t samples , int32_t sample_size , bool stereo) {
   return PrivateCreateBlankSample(freq , samples , sample_size , stereo);
}



EagleSoundInstance* SoundManager::CreateSoundInstance(EagleSoundSample* psample) {
   return PrivateCreateSoundInstance(psample);
}



EagleSoundStream* SoundManager::CreateSoundStream(FilePath fp) {
   return PrivateCreateSoundStream(fp);
}

