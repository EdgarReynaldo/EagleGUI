



#include "Eagle/SoundManager.hpp"



SoundSample* SoundManager::CreateSoundSample(FilePath fp) {
   return PrivateCreateSoundSample(fp);
}
SoundStream* SoundManager::CreateSoundStream(FilePath fp) {
   return PrivateCreateSoundStream(fp);
}

