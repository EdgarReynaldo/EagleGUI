
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



class SoundSample {
protected:
   
public :
   SoundSample():{}
   virtual ~SoundSample() {}
   
   virtual bool LoadFromFile(FilePath fp)=0;
   
   virtual void Free()=0;
};



class SoundStream;
class SoundInstance;



class SoundManager {
   
protected :
   virtual SoundSample* PrivateCreateSoundSample(FilePath fp)=0;
   virtual SoundStream* PrivateCreateSoundStream(FilePath fp)=0;
   virtual SoundInstance* PrivateCreateSoundInstance(SoundSample* psample)=0;
   
public :
   virtual bool Initialize()=0;
   
   SoundSample* CreateSoundSample(FilePath fp);
   SoundInstance* CreateSoundInstance(SoundSample* psample);
   SoundStream* CreateSoundStream(FilePath fp);
   
   virtual SoundInstance* PlayNewSampleInstance(std::string shortfilename)=0;
   virtual void ReadyBGStream(std::string shortfilename)=0;
   virtual void SetBGStreamPlaying(bool playing)=0;
   virtual void SetSampleMixerPlaying(bool playing)=0;
};





#endif // SoundManager_HPP
