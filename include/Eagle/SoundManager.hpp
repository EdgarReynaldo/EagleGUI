
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



class EagleSound;
class EagleSoundSample;
class EagleSoundInstance;
class EagleSoundStream;



/***
 * @class SoundManager
 * @brief Workhorse class to make audio simpler
 * The @ref SoundManager class lets you setup audio, samples, instances, 
      streams, and recording and all audio made available by Allegro.
 */



class SoundManager {
   
protected :
   virtual EagleSoundSample* PrivateCreateSoundSample(FilePath fp)=0;
   virtual EagleSoundStream* PrivateCreateSoundStream(FilePath fp)=0;
   virtual EagleSoundInstance* PrivateCreateSoundInstance(EagleSoundSample* psample)=0;
   
public :
   SoundManager() {}
   virtual ~SoundManager() {}
   virtual bool Initialize()=0;
   virtual void CheckInstances()=0;
   virtual bool ReserveInstances(size_t n , size_t max)=0;
   virtual EagleSoundInstance* GetInstance(size_t index)=0;
   
   EagleSoundSample* CreateSoundSample(FilePath fp);
   EagleSoundInstance* CreateSoundInstance(EagleSoundSample* psample);///< Will create a new instance, attach it to the sample mixer, and pause it, may be NULL
   EagleSoundStream* CreateSoundStream(FilePath fp);
   
   virtual void ClearSoundSamples()=0;
   virtual void ClearSoundInstances()=0;
   virtual void ClearBGStream()=0;

   virtual void ReadyBGStream(EagleSoundStream* stream)=0;

   virtual void SetBGStreamPlaying(bool playing)=0;
   virtual void SetInstancePlaying(EagleSoundInstance* sound , bool play)=0;
   
   virtual void SetSampleMixerPlaying(bool playing)=0;

   virtual void SetMasterMixerPlaying(bool play)=0;

   virtual EagleSoundInstance* PlayNewSampleInstance(EagleSoundSample* psample)=0;
   virtual bool SetSample(EagleSoundInstance* inst , EagleSoundSample* sample)=0;///< Use with @fn ReserveInstances
   
   virtual bool Record(EagleSoundRecorder* recorder)=0;

   virtual void Play(EagleSound* sound)=0;
   virtual void Pause(EagleSound* sound)=0;
   virtual void Continue(EagleSound* sound)=0;
   virtual void Stop(EagleSound* sound)=0;
   
   virtual bool SeekIndex(EagleSound* sound , uint32_t sample_index)=0;
   virtual uint32_t TellIndex(EagleSound* sound)=0;
};





#endif // SoundManager_HPP
