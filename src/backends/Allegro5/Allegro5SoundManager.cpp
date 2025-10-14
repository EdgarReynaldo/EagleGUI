
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
 * @file Allegro5SoundManager.cpp
 * @brief Allegro 5 implementation of the sound driver for Eagle
 *
 *
 */

#include "Eagle/StringWork.hpp"
#include "Eagle/backends/Allegro5/Allegro5SoundManager.hpp"
#include "allegro5/allegro_audio.h"

EagleSoundSample* Allegro5SoundManager::PrivateCreateSoundSample(FilePath fp) {
   EagleSoundSample* psample = new Allegro5SoundSample(this);
   bool success = psample->LoadFromFile(fp);
   if (!success) {
      EagleError() << StringPrintF("Failed to load file path '%s' from file into new Allegro 5 sound sample" , fp.Path().c_str()) << std::endl;
      delete psample;
      psample = 0;
   }
   return psample;
}



EagleSoundSample* Allegro5SoundManager::PrivateCreateBlankSample(int32_t freq , int32_t samples , int32_t sample_size , bool stereo) {
   EagleSoundSample* blank = new Allegro5SoundSample(this);
   static uint32_t i = 0;
   bool success = false;
   if(blank->CreateSample(samples , freq , sample_size , stereo)) {
      sample_map[StringPrintF("BLINK%u" , i)] = dynamic_cast<Allegro5SoundSample*>(blank);
      success = true;
   }
   else {
      delete blank;
   }
   return success?blank:0;
}



EagleSoundStream* Allegro5SoundManager::PrivateCreateSoundStream(FilePath fp) {
   EagleSoundStream* pstream = new Allegro5SoundStream(this);
   bool success = pstream->LoadFromFile(fp);
   if (!success) {
      delete pstream;
      pstream = 0;
   }
   return pstream;
}



EagleSoundInstance* Allegro5SoundManager::PrivateCreateSoundInstance(EagleSoundSample* psample) {
   Allegro5SoundInstance* pinstance = new Allegro5SoundInstance(this , psample);
   pinstance->SetSample(psample);
   al_attach_sample_instance_to_mixer(pinstance->AllegroInstance() , psample_mixer);
   al_set_sample_instance_playing(pinstance->AllegroInstance() , false);
   return pinstance;
}



EagleSoundRecorder* Allegro5SoundManager::PrivateCreateSoundRecorder() {
   return new Allegro5SoundRecorder(this);
}



Allegro5SoundManager::Allegro5SoundManager() :
      SoundManager(),
      phardware(0),
      pmaster_mixer(0),
      psample_mixer(0),
      hwfreq(0),
      hwdepth(ALLEGRO_AUDIO_DEPTH_INT16),
      hwconf(ALLEGRO_CHANNEL_CONF_2),
      mastergain(1.0),
      samplegain(1.0),
      bgmusicgain(1.0),
      sample_map(),
      instances(),
      bgstream(0),
      recorder(0),
      recorder_sample(0),
      recorder_sample_instance(0)
{}



Allegro5SoundManager::~Allegro5SoundManager() {
   Free();
}



void Allegro5SoundManager::Free() {
   ///< Mutex lock here?
   if (pmaster_mixer) {
      al_detach_mixer(pmaster_mixer);
   }
   if (psample_mixer) {
      al_detach_mixer(psample_mixer);
   }
   ClearBGStream();
   ClearRecorder();
   hwfreq = 0;
   hwdepth = ALLEGRO_AUDIO_DEPTH_INT16;
   hwconf = ALLEGRO_CHANNEL_CONF_2;
   mastergain = 1.0;
   samplegain = 1.0;
   bgmusicgain = 1.0;

   ClearSoundSamples();/// This calls ClearSoundInstances for us and stops all sample instances from playing
///   ClearSoundInstances();
   if (pmaster_mixer) {
      al_destroy_mixer(pmaster_mixer);
   }
   if (psample_mixer) {
      al_destroy_mixer(psample_mixer);
   }
   al_destroy_voice(phardware);
   phardware = 0;
   pmaster_mixer = 0;
   psample_mixer = 0;
}



bool Allegro5SoundManager::Initialize() {
   Free();
   phardware = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
   if (!phardware) {
      return false;
   }
   hwfreq = al_get_voice_frequency(phardware);
   hwdepth = al_get_voice_depth(phardware);
   hwconf = al_get_voice_channels(phardware);
   pmaster_mixer = al_create_mixer(hwfreq , ALLEGRO_AUDIO_DEPTH_FLOAT32 , hwconf);
   psample_mixer = al_create_mixer(hwfreq , ALLEGRO_AUDIO_DEPTH_FLOAT32 , hwconf);
   if (!pmaster_mixer || !psample_mixer) {
      Free();
      return false;
   }
   if (al_attach_mixer_to_mixer(psample_mixer , pmaster_mixer) &&
          al_attach_mixer_to_voice(pmaster_mixer , phardware) &&
           al_set_mixer_playing(pmaster_mixer , true) &&
            al_set_mixer_playing(psample_mixer , true)) {
      return true;
   }
   Free();
   return false;
}



void Allegro5SoundManager::CheckInstances() {
   if (!instances.size()) {return;}
   std::deque<Allegro5SoundInstance*>::iterator it = instances.begin();
   while (it != instances.end()) {
      if (al_get_sample_instance_position((*it)->AllegroInstance()) >= al_get_sample_instance_length((*it)->AllegroInstance())) {
         delete *it;
         it = instances.erase(it);
      }
      else {
         ++it;
      }
   }
}



bool Allegro5SoundManager::ReserveInstances(size_t n , size_t max) {
   if (max > 64) {max = 64;}///< Set a max of 64 instances for some arbitrary reason

   if (n > max) {n = max;}

   int nnew = 0;
   int nold = 0;

   if (n > instances.size()) {
      nnew = n - instances.size();
   }
   else if (n < instances.size()) {
      nold = instances.size() - n;
   }
   else {
      return true;
   }

   /// Old instances
   for (int iold = 0 ; iold < nold ; ++iold) {
      Allegro5SoundInstance* inst = *instances.begin();
      delete inst;
      instances.pop_front();
   }

   /// New instances
   for (int inew = 0 ; inew < nnew ; ++inew) {
      Allegro5SoundInstance* inst = dynamic_cast<Allegro5SoundInstance*>(PrivateCreateSoundInstance(0));
      EAGLE_ASSERT(inst);
      instances.push_back(inst);
   }

   return true;///?
}



Allegro5SoundInstance* Allegro5SoundManager::GetInstance(size_t index) {
   return instances[index];
}



Allegro5SoundRecorder* Allegro5SoundManager::GetRecorder() {
   return recorder;
}



Allegro5SoundStream* Allegro5SoundManager::GetBGStream() {
   return bgstream;
}



void Allegro5SoundManager::ClearSoundSamples() {
   ClearSoundInstances();
   for (std::map<std::string , Allegro5SoundSample*>::iterator it = sample_map.begin() ; it != sample_map.end() ; ++it) {
      delete it->second;
   }
   sample_map.clear();
}



void Allegro5SoundManager::ClearSoundInstances() {
   std::deque<Allegro5SoundInstance*>::iterator it = instances.begin();
   while (it != instances.end()) {
      Allegro5SoundInstance* pinst = *it;
      if (pinst) {
         al_set_sample_instance_playing(pinst->AllegroInstance() , false);
         al_detach_sample_instance(pinst->AllegroInstance());
         pinst->SetSample(0);
         delete pinst;
      }
      ++it;
   }
   instances.clear();
}



void Allegro5SoundManager::ClearBGStream() {
   if (bgstream) {
      al_detach_audio_stream(bgstream->AllegroStream());
      delete bgstream;
      bgstream = 0;
   }
}



void Allegro5SoundManager::ClearRecorder() {
   if (recorder) {
      al_stop_audio_recorder(recorder->GetRecorder());
      al_destroy_audio_recorder(recorder->GetRecorder());
      sample_map["RECORDER:"] = 0;
      delete recorder;
      recorder = 0;
   }
   if (recorder_sample_instance) {
      delete recorder_sample_instance;
      recorder_sample_instance = 0;
   }
   if (recorder_sample) {
      delete recorder_sample;
      recorder_sample = 0;
   }
}





void Allegro5SoundManager::ReadyBGStream(EagleSoundStream* stream) {

   if (bgstream) {
      al_detach_audio_stream(bgstream->AllegroStream());
   }

   bgstream = dynamic_cast<Allegro5SoundStream*>(stream);

   if (bgstream) {
      al_attach_audio_stream_to_mixer(bgstream->AllegroStream() , pmaster_mixer);
   }
}



void Allegro5SoundManager::SetBGStreamPlaying(bool playing) {
   if (bgstream) {
      al_set_audio_stream_playing(bgstream->AllegroStream() , playing);
   }
}



void Allegro5SoundManager::SetInstancePlaying(EagleSoundInstance* instance, bool play) {
   if (instance) {
      Allegro5SoundInstance* a5inst = dynamic_cast<Allegro5SoundInstance*>(instance);
      if (a5inst) {
         ALLEGRO_SAMPLE_INSTANCE* inst = a5inst->AllegroInstance();
         if (inst) {
            al_set_sample_instance_playing(inst , play);
         }
      }
   }
}


void Allegro5SoundManager::SetSampleMixerPlaying(bool playing) {
   al_set_mixer_playing(psample_mixer , playing);
}



void Allegro5SoundManager::SetMasterMixerPlaying(bool play) {
   al_set_mixer_playing(pmaster_mixer , play);
}



EagleSoundInstance* Allegro5SoundManager::PlayNewSampleInstance(EagleSoundSample* psample) {
   Allegro5SoundInstance* inst = dynamic_cast<Allegro5SoundInstance*>(PrivateCreateSoundInstance(psample));
   ALLEGRO_SAMPLE_INSTANCE* ainst = inst->AllegroInstance();
   al_attach_sample_instance_to_mixer(ainst , psample_mixer);
   al_set_sample_instance_playing(ainst , true);
   instances.push_back(inst);
   return inst;
}



bool Allegro5SoundManager::SetSample(EagleSoundInstance* inst , EagleSoundSample* sample) {
   Allegro5SoundInstance* a5inst = dynamic_cast<Allegro5SoundInstance*>(inst);
   if (a5inst) {
      a5inst->SetSample(sample);
      return true;
   }
   return false;
}




bool Allegro5SoundManager::Record(EagleSoundRecorder* rec) {
   Allegro5SoundRecorder* a5recorder = dynamic_cast<Allegro5SoundRecorder*>(rec);
   ALLEGRO_AUDIO_RECORDER* sndrec = a5recorder?a5recorder->GetRecorder():0;
   EAGLE_ASSERT(sndrec);
   if (sndrec) {
      return al_start_audio_recorder(sndrec);
   }
   return false;
}



bool Allegro5SoundManager::Play(EagleSound* sound) {
   Allegro5SoundInstance* a5inst = dynamic_cast<Allegro5SoundInstance*>(sound);
   Allegro5SoundStream* a5strm = dynamic_cast<Allegro5SoundStream*>(sound);
   Allegro5SoundRecorder* a5recorder = dynamic_cast<Allegro5SoundRecorder*>(recorder);
   if (a5inst) {
      ALLEGRO_SAMPLE_INSTANCE* smp = a5inst->AllegroInstance();
      if (smp) {
         return al_set_sample_instance_playing(smp , true);
      }
   }
   if (a5strm) {
      ALLEGRO_AUDIO_STREAM* stream = a5strm->AllegroStream();
      if (stream) {
         return al_set_audio_stream_playing(stream , true);
      }
   }
   if (a5recorder) {
      ALLEGRO_AUDIO_RECORDER* sndrec = a5recorder?a5recorder->GetRecorder():0;
      if (sndrec) {
         
         if (!recorder_sample) {
            recorder_sample = new Allegro5SoundSample(this);
         }
         if (recorder_sample->CreateSample(88200 , 44100, ALLEGRO_AUDIO_DEPTH_INT16 , true)) {
            ALLEGRO_SAMPLE* smp = recorder_sample->Data();
            sample_map["RECORDER:"] = recorder_sample;
            ///< Copy data from recorder buffer to sample buffer
            uint32_t l = al_get_sample_length(smp);
            int channels = (int)al_get_sample_channels(smp);
            int depth = (int)al_get_sample_depth(smp);
            uint8_t* sampbuf = (uint8_t*)al_get_sample_data(smp);
            for (uint32_t i = 0 ; i < l*channels*depth ; ++i) {
               sampbuf[i] = a5recorder->Buffer(i);
            }
            return PlayNewSampleInstance(recorder_sample);
         }
      }
   }
   return false;
}



bool Allegro5SoundManager::Pause(EagleSound* sound) {
   Allegro5SoundInstance* a5inst = dynamic_cast<Allegro5SoundInstance*>(sound);
   Allegro5SoundStream* a5strm = dynamic_cast<Allegro5SoundStream*>(sound);
   Allegro5SoundRecorder* a5rec = dynamic_cast<Allegro5SoundRecorder*>(sound);
   if (a5inst) {
      ALLEGRO_SAMPLE_INSTANCE* smp = a5inst->AllegroInstance();
      if (smp) {
         return al_set_sample_instance_playing(smp , false);
      }
   }
   if (a5strm) {
      ALLEGRO_AUDIO_STREAM* stream = a5strm->AllegroStream();
      if (stream) {
         return al_set_audio_stream_playing(stream , false);
      }
   }
   if (a5rec) {
      ALLEGRO_AUDIO_RECORDER* srec = a5rec->GetRecorder();
      if (srec) {
         recorder_sample_instance->Pause();
      }
   }
   return false;
}



bool Allegro5SoundManager::Continue(EagleSound* sound) {
   Allegro5SoundInstance* a5inst = dynamic_cast<Allegro5SoundInstance*>(sound);
   Allegro5SoundStream* a5strm = dynamic_cast<Allegro5SoundStream*>(sound);
   Allegro5SoundRecorder* a5rec = dynamic_cast<Allegro5SoundRecorder*>(sound);
   if (a5inst) {
      ALLEGRO_SAMPLE_INSTANCE* smp = a5inst->AllegroInstance();
      if (smp) {
         return al_set_sample_instance_playing(smp , true);
      }
   }
   if (a5strm) {
      ALLEGRO_AUDIO_STREAM* stream = a5strm->AllegroStream();
      if (stream) {
         return al_set_audio_stream_playing(stream , true);
      }
   }
   if (a5rec) {
      ALLEGRO_AUDIO_RECORDER* sndrec = a5rec->GetRecorder();
      if (sndrec) {
         recorder_sample_instance->Continue();
      }
      return true;
   }
   return false;
}



bool Allegro5SoundManager::Stop(EagleSound* sound) {
   Allegro5SoundInstance* a5inst = dynamic_cast<Allegro5SoundInstance*>(sound);
   Allegro5SoundStream* a5strm = dynamic_cast<Allegro5SoundStream*>(sound);
   Allegro5SoundRecorder* a5rec = dynamic_cast<Allegro5SoundRecorder*>(sound);
   if (a5inst) {
      ALLEGRO_SAMPLE_INSTANCE* smp = a5inst->AllegroInstance();
      if (smp) {
         return (al_set_sample_instance_playing(smp , false) &&
                 al_set_sample_instance_position(smp , 0));
      }
   }
   if (a5strm) {
      ALLEGRO_AUDIO_STREAM* stream = a5strm->AllegroStream();
      if (stream) {
         return (al_set_audio_stream_playing(stream , false) && 
                 al_rewind_audio_stream(stream));
      }
   }
   if (a5rec) {
      ALLEGRO_AUDIO_RECORDER* srec = a5rec->GetRecorder();
      if (srec) {
         al_stop_audio_recorder(srec);
         al_set_sample_instance_playing(recorder_sample_instance->AllegroInstance() , false);
         return true;
      }
   }
   return false;
}



bool Allegro5SoundManager::SeekIndex(EagleSound* sound , uint32_t sample_index) {
   Allegro5SoundInstance* a5inst = dynamic_cast<Allegro5SoundInstance*>(sound);
   Allegro5SoundStream* a5strm = dynamic_cast<Allegro5SoundStream*>(sound);
   int32_t ret = 0;
   if (a5inst) {
      ALLEGRO_SAMPLE_INSTANCE* smp = a5inst->AllegroInstance();
      if (smp) {
         ret = al_set_sample_instance_position(smp , sample_index);
      }
   }
   if (a5strm) {
      ALLEGRO_AUDIO_STREAM* stream = a5strm->AllegroStream();
      if (stream) {
         ret = al_seek_audio_stream_secs(stream , sample_index/hwfreq);
      }
   }
   return ret;
}



uint64_t Allegro5SoundManager::TellIndex(EagleSound* sound) {
   Allegro5SoundInstance* a5inst = dynamic_cast<Allegro5SoundInstance*>(sound);
   Allegro5SoundStream* a5strm = dynamic_cast<Allegro5SoundStream*>(sound);
   if (a5inst) {
      ALLEGRO_SAMPLE_INSTANCE* smp = a5inst->AllegroInstance();
      if (smp) {
         return al_get_sample_instance_position(smp);
      }
   }
   if (a5strm) {
      ALLEGRO_AUDIO_STREAM* stream = a5strm->AllegroStream();
      if (stream) {
         return al_get_audio_stream_played_samples(stream);
      }
   }
   return ~(uint64_t)0;
}



EagleEventSource* Allegro5SoundManager::GetSoundSource(ALLEGRO_EVENT_SOURCE* src) {
   if (recorder) {
      if (src == al_get_audio_recorder_event_source(recorder->GetRecorder())) {
         return recorder;
      }
   }
   if (bgstream) {
      if (src == al_get_audio_stream_event_source(bgstream->AllegroStream())) {
         return bgstream;
      }
   }
   for (unsigned int i = 0 ; i < instances.size() ; ++i) {
      Allegro5SoundInstance* inst = instances[i];
      ///<   if (al_get_instance_event_source(inst->AllegroInstance()) == src) {
         return inst;
///      }
   }
   return (EagleEventSource*)0;
}








