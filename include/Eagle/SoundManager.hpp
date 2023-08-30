
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
 * @file 
 * @brief
 * 
 *
 */

#ifndef SoundManager_HPP
#define SoundManager_HPP



#include "Eagle/File.hpp"




class SoundSample;
class SoundStream;
class SoundInstance;



class SoundManager {
   
protected :
   virtual SoundSample* PrivateCreateSoundSample(FilePath fp)=0;
   virtual SoundStream* PrivateCreateSoundStream(FilePath fp)=0;
   virtual SoundInstance* PrivateCreateSoundInstance(SoundSample* psample)=0;
   
public :
   SoundManager() {}
   virtual ~SoundManager() {}
   virtual bool Initialize()=0;
   virtual void CheckInstances()=0;
   virtual bool ReserveInstances(size_t n)=0;
   virtual SoundInstance* GetInstance(size_t index)=0;
   
   SoundSample* CreateSoundSample(FilePath fp);
   SoundInstance* CreateSoundInstance(SoundSample* psample);///< Will create a new instance, attach it to the sample mixer, and play it, may be NULL
   SoundStream* CreateSoundStream(FilePath fp);
   
   virtual void ReadyBGStream(SoundStream* stream)=0;
   virtual void SetBGStreamPlaying(bool playing)=0;
   virtual void SetSampleMixerPlaying(bool playing)=0;

   virtual void PlayAllSound(bool play)=0;
   void PauseAllSound(bool pause) {PlayAllSound(!pause);}

   virtual SoundInstance* PlayNewSampleInstance(SoundSample* psample)=0;
   virtual bool SetSample(SoundInstance* inst , SoundSample* sample)=0;///< Use with @fn ReserveInstances
};





#endif // SoundManager_HPP
