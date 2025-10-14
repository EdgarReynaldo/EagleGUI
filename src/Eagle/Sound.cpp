
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
#include "Eagle/Exception.hpp"


EagleSound::EagleSound(SoundManager* sound_man) :
      EagleEventSource(),
      owner(sound_man),
      sample_index_master(0UL)
{
   EAGLE_ASSERT(sound_man);
}


void EagleSound::Play() {
   if (owner) {
      owner->Play(this);
   }
}
void EagleSound::Stop() {
   if (owner) {
      owner->Stop(this);
   }
}
void EagleSound::Pause() {
   if (owner) {
      owner->Pause(this);
   }
}
void EagleSound::Continue() {
   if (owner) {
      owner->Continue(this);
   }
}
bool EagleSound::SeekIndex(uint32_t sample_index) {
   if (owner) {
      return owner->SeekIndex(this , sample_index);
   }
   return false;
}
uint32_t EagleSound::TellIndex() {
   return sample_index_master;
}



///< EagleSoundSample



EagleSoundSample::EagleSoundSample(SoundManager* sound_man) :
      ResourceBase(RT_AUDIO_SAMPLE),
      parent(sound_man)
{}



EagleSoundSample::~EagleSoundSample() 
{}


///< EagleSoundInstance



EagleSoundInstance::EagleSoundInstance(SoundManager* sound_man , EagleSoundSample* parent_sample) :
      EagleSound(sound_man),
      parent(parent_sample)
{}



EagleSoundInstance::~EagleSoundInstance() 
{}


///< EagleSoundStream



EagleSoundStream::EagleSoundStream(SoundManager* sound_man) :
      EagleSound(sound_man),
      ResourceBase(RT_AUDIO_STREAM)
{}



EagleSoundStream::~EagleSoundStream() 
{}



///< EagleSoundRecorder



EagleSoundRecorder::EagleSoundRecorder(SoundManager* sound_man) :
      EagleSound(sound_man),
      freq(44100),
      num_samples(11025),
      frag_count(8),
      size_of_data(2),
      stereo(true),
      total_buffer_size(0),
      buffer_duration(0.0),
      record_index(0),
      buffer(),
      recorder_sample(0),
      recorder_sample_instance(0)
{}



bool EagleSoundRecorder::Record() {
   EAGLE_ASSERT(buffer.size());
   if (owner) {
      return owner->Record(this);
   }
   return false;
}



int8_t EagleSoundRecorder::Buffer(uint32_t index) {
   return buffer[index];
}








