
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
#include "Eagle/SoundManager.hpp"
#include "Eagle/backends/Allegro5/Allegro5Sound.hpp"


#include <set>



class Allegro5SoundManager : public SoundManager {
   
   ALLEGRO_VOICE* phardware;///< Allegro's hardware voice, query for stats, may not match what is was created with
   ALLEGRO_MIXER* pmaster_mixer;///< For background music stream and the sample mixer to attach to
   ALLEGRO_MIXER* psample_mixer;///< For samples only

   size_t               hwfreq;
   ALLEGRO_AUDIO_DEPTH  hwdepth;
   ALLEGRO_CHANNEL_CONF hwconf;
   
   float mastergain;
   float samplegain;
   float bgmusicgain;
   
   std::map<std::string , Allegro5SoundSample*> sample_map;///< map of short filename to loaded sample objects
   
   static const size_t MAXSNDINST = 32;
   
   
   std::deque<Allegro5SoundInstance*> instances;///< Our current set of playing (or completed) instances
   
   Allegro5SoundStream* bgstream;///< For a soundstream object
   
   
   virtual EagleSoundSample* PrivateCreateSoundSample(FilePath fp);
   virtual EagleSoundStream* PrivateCreateSoundStream(FilePath fp);
   virtual EagleSoundInstance* PrivateCreateSoundInstance(EagleSoundSample* psample);

public :
   Allegro5SoundManager();

   virtual ~Allegro5SoundManager();

   void Free();
   virtual bool Initialize();///< Get ready to produce sound. Setup mixers and voice
   virtual void CheckInstances();///< Remove dead instances, no need to call
   virtual bool ReserveInstances(size_t n , size_t max = 32);///< For convenience and sample reuse - saves memory and allocation/deallocations
   virtual EagleSoundInstance* GetInstance(size_t index);///< Get the SoundInstance at the specified index, 0 to InstanceCount()-1;
   size_t InstanceCount() {return instances.size();}

   virtual void ClearSoundSamples();
   virtual void ClearSoundInstances();
   virtual void ClearBGStream();



   virtual void ReadyBGStream(EagleSoundStream* stream);///< May be null to clear the stream
   
   virtual void SetBGStreamPlaying(bool playing);
   virtual void SetInstancePlaying(EagleSoundInstance* sound , bool play);

   virtual void SetSampleMixerPlaying(bool playing);

   virtual void SetMasterMixerPlaying(bool play);
   
   

   virtual EagleSoundInstance* PlayNewSampleInstance(EagleSoundSample* sample);
   virtual bool SetSample(EagleSoundInstance* inst , EagleSoundSample* sample);///< Reuses a sound instance to play this sample, used with @fn ReserveInstances<size_t>


   virtual bool Record(EagleSoundRecorder* recorder);

   virtual void Play(EagleSound* sound);
   virtual void Pause(EagleSound* sound);
   virtual void Continue(EagleSound* sound);
   virtual void Stop(EagleSound* sound);
   
   virtual bool SeekIndex(EagleSound* sound , uint32_t sample_index);
   virtual uint32_t TellIndex(EagleSound* sound);
};















#endif // Allegro5SoundManager_HPP

