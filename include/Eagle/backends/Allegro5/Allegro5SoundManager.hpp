
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
 * @brief Sound management driver for Allegro 5
 * 
 * The Allegro 5 Sound Manager driver for Eagle. The Sound Manager takes care of creating and destroying sound objects.
 * One audio stream is supported only, and has its own dedicated mixer with allegro. This is a limitation of Allegro, not Eagle.
 * All samples are routed through the second mixer and the number of allocated instances may change as needs grow. 
 *
 */

#ifndef Allegro5SoundManager_HPP
#define Allegro5SoundManager_HPP


 
 
#include "Eagle/Sound.hpp"
#include "Eagle/backends/Allegro5/Allegro5Sound.hpp"


#include <set>



class Allegro5SoundManager : public SoundManager {
   
   EagleSystem* sys;
   FileSystem* fsys;
   
   ALLEGRO_VOICE* phardware;///< Allegro's hardware voice, query for stats, may not match what is was created with
   ALLEGRO_MIXER* pstream_mixer;///< For background music stream only
   ALLEGRO_MIXER* psample_mixer;///< For samples only
   
   
   std::map<std::string , ALLEGRO_SAMPLE*> sample_map;///< map of short filename to loaded sample
   
   std::set<ALLEGRO_SAMPLE_INSTANCE*> unused_sample_instances;///< available sample instances for the samples
   std::set<ALLEGRO_SAMPLE_INSTANCE*> used_sample_instances;///< unavailable sample instances for the samples
   
   Allegro5SoundStream* bgstream;///< For a soundstream object
   std::set<Allegro5SoundSample*> sounds;///< Sound objects
   
   
   





   virtual SoundSample* PrivateCreateSoundSample(FilePath fp);
   virtual SoundStream* PrivateCreateSoundStream(FilePath fp);
   virtual SoundInstance* PrivateCreateSoundInstance(SoundSample* psample);
SoundSample* Allegro5SoundManager::PrivateCreateSoundSample(FilePath fp) {
   
}
SoundStream* Allegro5SoundManager::PrivateCreateSoundStream(FilePath fp) {
   
}
SoundInstance* Allegro5SoundManager::PrivateCreateSoundInstance(SoundSample* psample) {
   
}
   
public :
   Allegro5SoundManager(EagleSystem* esys);
Allegro5SoundManager::Allegro5SoundManager(EagleSystem* esys) :
      sys(esys),
      fsys(),
      phardware(0),
      pstream_mixer(0),
      psample_mixer(0),
      sample_map(),
      unused_sample_instances(),
      used_sample_instances(),
      bgstream(0),
      sounds()
{
   EAGLE_ASSERT(esys);
   fsys = sys->GetFileSystem();
}

   virtual bool Initialize();
   
   virtual void PlayNewSampleInstance(std::string shortfilename);
   virtual void ReadyBGStream(std::string shortfilename);
   
   virtual void SetBGStreamPlaying(bool playing);
   virtual void SetSampleMixerPlaying(bool playing);


   
};

bool Allegro5SoundManager::Initialize();

void Allegro5SoundManager::PlayNewSampleInstance(std::string shortfilename);
void Allegro5SoundManager::ReadyBGStream(std::string shortfilename);

void Allegro5SoundManager::SetBGStreamPlaying(bool playing);
void Allegro5SoundManager::SetSampleMixerPlaying(bool playing);













#endif // Allegro5SoundManager_HPP

