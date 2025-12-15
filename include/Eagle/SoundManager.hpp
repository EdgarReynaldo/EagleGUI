
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
 *    Copyright 2009-2025+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file SoundManager.hpp
 * @brief Some classes to tidy up the soundscape in Eagle
 * 
 */

#ifndef SoundManager_HPP
#define SoundManager_HPP



#include "Eagle/File.hpp"
#include <cstdint>


class EagleSound;
class EagleSoundSample;
class EagleSoundInstance;
class EagleSoundStream;
class EagleSoundRecorder;
class EagleEventSource;


/***
 * @class SoundManager
 * @brief Workhorse class to make audio simpler
 * The @ref SoundManager class lets you setup audio, samples, instances, 
      streams, and recording and all audio made available by Allegro.
 */
struct ALLEGRO_EVENT_SOURCE;


class SoundManager {
   
protected :
   virtual EagleSoundSample* PrivateCreateSoundSample(FilePath fp)=0;
   virtual EagleSoundSample* PrivateCreateBlankSample(int32_t freq , int32_t samples , int32_t sample_size , bool stereo)=0;
   virtual EagleSoundStream* PrivateCreateSoundStream(FilePath fp)=0;
   virtual EagleSoundInstance* PrivateCreateSoundInstance(EagleSoundSample* psample)=0;
   virtual EagleSoundRecorder* PrivateCreateSoundRecorder()=0;
   
public :
   SoundManager() {}
   virtual ~SoundManager() {}
   virtual bool Initialize()=0;
   virtual void CheckInstances()=0;
   virtual bool ReserveInstances(size_t n , size_t max)=0;
   virtual EagleSoundInstance* GetInstance(size_t index)=0;
   
   EagleSoundSample* CreateSoundSample(FilePath fp);
   EagleSoundSample* CreateBlankSample(int32_t freq , int32_t samples , int32_t sample_size , bool stereo);
   EagleSoundInstance* CreateSoundInstance(EagleSoundSample* psample);///< Will create a new instance, attach it to the sample mixer, and pause it, may be NULL
   EagleSoundStream* CreateSoundStream(FilePath fp);
   EagleSoundRecorder* CreateSoundRecorder(int32_t freq , int32_t nsamples_per_fragment , int32_t nfragments);
   
   virtual void ClearSoundSamples()=0;
   virtual void ClearSoundInstances()=0;
   virtual void ClearBGStream()=0;
   virtual void ClearRecorder()=0;

   virtual void ReadyBGStream(EagleSoundStream* stream)=0;

   virtual void SetBGStreamPlaying(bool playing)=0;
   virtual void SetInstancePlaying(EagleSoundInstance* sound , bool play)=0;
   
   virtual void SetSampleMixerPlaying(bool playing)=0;

   virtual void SetMasterMixerPlaying(bool play)=0;

   virtual EagleSoundInstance* PlayNewSampleInstance(EagleSoundSample* psample)=0;
   virtual bool SetSample(EagleSoundInstance* inst , EagleSoundSample* sample)=0;///< Use with @fn ReserveInstances
   
   virtual bool Record(EagleSoundRecorder* recorder)=0;

   virtual bool Play(EagleSound* sound)=0;
   virtual bool Pause(EagleSound* sound)=0;
   virtual bool Continue(EagleSound* sound)=0;
   virtual bool Stop(EagleSound* sound)=0;
   
   virtual bool SeekIndex(EagleSound* sound , uint32_t sample_index)=0;
   virtual uint64_t TellIndex(EagleSound* sound)=0;
   
   virtual EagleEventSource* GetSoundSource(ALLEGRO_EVENT_SOURCE* src)=0;
};





#endif // SoundManager_HPP
