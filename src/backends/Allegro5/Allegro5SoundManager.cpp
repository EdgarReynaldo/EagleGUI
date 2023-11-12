
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

#include "Eagle/backends/Allegro5/Allegro5SoundManager.hpp"




EagleSoundSample* Allegro5SoundManager::PrivateCreateSoundSample(FilePath fp) {
   EagleSoundSample* psample = new Allegro5SoundSample();
   bool success = psample->LoadFromFile(fp);
   if (!success) {
      delete psample;
      psample = 0;
   }
   return psample;
}



EagleSoundStream* Allegro5SoundManager::PrivateCreateSoundStream(FilePath fp) {
   EagleSoundStream* pstream = new Allegro5SoundStream();
   bool success = pstream->LoadFromFile(fp);
   if (!success) {
      delete pstream;
      pstream = 0;
   }
   return pstream;
}



EagleSoundInstance* Allegro5SoundManager::PrivateCreateSoundInstance(EagleSoundSample* psample) {
   Allegro5SoundInstance* pinstance = new Allegro5SoundInstance(psample);
   pinstance->SetSample(psample);
   al_attach_sample_instance_to_mixer(pinstance->AllegroInstance() , psample_mixer);
   al_set_sample_instance_playing(pinstance->AllegroInstance() , true);
   return pinstance;
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
      bgstream(0)
{}



Allegro5SoundManager::~Allegro5SoundManager() {
   Free();
}



void Allegro5SoundManager::Free() {
   ///< Mutex lock here?
   al_detach_mixer(pmaster_mixer);
   al_detach_mixer(psample_mixer);
   if (bgstream) {  
      al_detach_audio_stream(bgstream->AllegroStream());
      delete bgstream;
      bgstream = 0;
   }
   std::deque<Allegro5SoundInstance*>::iterator it = instances.begin();
   while (it != instances.end()) {
      Allegro5SoundInstance* pinst = *it;
      if (pinst) {
         al_detach_sample_instance(pinst->AllegroInstance());
         delete pinst;
      }
      ++it;
   }
   instances.clear();
   al_destroy_mixer(pmaster_mixer);
   al_destroy_mixer(psample_mixer);
   al_destroy_voice(phardware);
   phardware = 0;
   pmaster_mixer = 0;
   psample_mixer = 0;
   hwfreq = 0;
   hwdepth = ALLEGRO_AUDIO_DEPTH_INT16;
   hwconf = ALLEGRO_CHANNEL_CONF_2;
   mastergain = 1.0;
   samplegain = 1.0;
   bgmusicgain = 1.0;
   std::map<std::string , Allegro5SoundSample*>::iterator mit = sample_map.begin();
   while (mit != sample_map.end()) {
      Allegro5SoundSample* ps = mit->second;
      if (ps) {
         delete ps;
      }
      ++mit;
   }
   sample_map.clear();
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



bool Allegro5SoundManager::ReserveInstances(size_t n) {
   if (n > 32) {n = 32;}///< Set a max of 32 instances for some arbitrary reason
   
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
   
   /// New instances
   for (int inew = 0 ; inew < nnew ; ++inew) {
      Allegro5SoundInstance* inst = dynamic_cast<Allegro5SoundInstance*>(PrivateCreateSoundInstance(0));
      instances.push_back(inst);
   }
   
   /// Old instances
   
   for (int iold = 0 ; iold < nold ; ++iold) {
      Allegro5SoundInstance* inst = *instances.begin();
      delete inst;
      instances.pop_front();
   }
   
   return true;///?
}



EagleSoundInstance* Allegro5SoundManager::GetInstance(size_t index) {
   return instances[index];
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



void Allegro5SoundManager::SetSampleMixerPlaying(bool playing) {
   al_set_mixer_playing(psample_mixer , playing);
}



void Allegro5SoundManager::PlayAllSound(bool play) {
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




