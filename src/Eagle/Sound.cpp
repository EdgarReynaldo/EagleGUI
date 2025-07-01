
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
 * @file Sound.cpp
 * @brief Base class implementations of sound objects
 * 
 */


 
#include "Eagle/Sound.hpp"
#include "Eagle/SoundManager.hpp"
 


EagleSound::EagleSound(SoundManager* sound_man) :
      owner(sound_man),
      sample_index_master(0UL)
{
   EAGLE_ASSERT(sound_man);
}


void EagleSound::Play() {
   if (sound_man) {
      sound_man->PlayMe(this);
   }
}
void EagleSound::Record() {
   if (sound_man) {
      sound_man->Record(this);
   }
}
void EagleSound::Stop() {
   if (sound_man) {
      sound_man->Stop(this);
   }
}
void EagleSound::Pause() {
   if (sound_man) {
      sound_man->Pause(this);
   }
}
void EagleSound::Continue() {
   if (sound_man) {
      sound_man->Continue(this);
   }
}
bool EagleSound::SeekIndex(uint32_t sample_index) {
   if (sound_man) {
      return sound_man->SeekIndex(sample_index);
   }
   return false;
}
uint32_t EagleSound::TellIndex() {
   return sample_index_master;
}



///< EagleSoundSample



EagleSoundSample::EagleSoundSample() :
      ResourceBase(RT_AUDIO_SAMPLE)
{}



EagleSoundSample::~EagleSoundSample() 
{}



EagleSoundInstance::EagleSoundInstance(EagleSoundSample* parent_sample) :
      EagleSound(0),
      parent(parent_sample)
{}



EagleSoundInstance::~EagleSoundInstance() 
{}



EagleSoundStream::EagleSoundStream() :
      EagleSound(),
      ResourceBase(RT_AUDIO_STREAM)
{}



EagleSoundStream::~EagleSoundStream() 
{}



EagleSoundRecorder::EagleSoundRecorder() :
      freq(44100),
      num_samples(0),
      frag_count(1),
      size_of_data(2),
      stereo(true),
      total_buffer_size(0),
      buffer_duration(0.0),
      record_index(0),
      buffer(),
      smp(0)
{}



bool EagleSoundRecorder::Record() {
   if (sound_man) {
      sound_man->Record(this);
   }
}






