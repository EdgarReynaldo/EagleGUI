
/**
 *
 *         _______       ___       ____      __       _______
 *        /\  ____\    /|   \     /  __\    /\ \     /\  ____\
 *        \ \ \___/_   ||  _ \   |  /__/____\ \ \    \ \ \___/_
 *         \ \  ____\  || |_\ \  |\ \ /\_  _\\ \ \    \ \  ____\
 *          \ \ \___/_ ||  ___ \ \ \ \\//\ \/ \ \ \____\ \ \___/_
 *           \ \______\||_|__/\_\ \ \ \_\/ |   \ \_____\\ \______\
 *            \/______/|/_/  \/_/  \_\_____/    \/_____/ \/______/
 *
 *
 *    Eagle Agile Gui Library and Extensions
 *
 *    Copyright 2009-2023+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Sound.hpp
 * @brief The interface for all sounds in Eagle
 * 
 * Base classes for audio drivers to use
 */

#ifndef Sound_HPP
#define Sound_HPP

#include <string>

#include "Eagle/File.hpp"
#include "Eagle/Resources.hpp"
#include "Eagle/Events.hpp"



class SoundManager;

class EagleSound : public EagleEventSource {
protected :
   SoundManager* owner;
   uint32_t sample_index_master;
   bool playing;
   bool paused;///< !playing
   bool stopped;///< !playing and index at zero
   
public :
   
   EagleSound(SoundManager* sound_man);
   virtual ~EagleSound() {}
//   void Record()
   
   void Play();
   void Pause();///< Temporarily pause the sound
   void Continue();///< Continue playing / recording
   void Stop();///< Stops the sample and rewinds to the beginning
   
   bool SeekIndex(uint32_t sample_index = 0UL);///< Set the playback / recording to the specified sample index
   uint32_t TellIndex();
   
   bool Playing() {return playing;}
   bool Paused() {return paused;}
   bool Stopped() {return paused && (sample_index_master == 0UL);}
};



class EagleSoundSample : public ResourceBase {

protected:
   SoundManager* parent;
   virtual bool LoadFromArgs(std::vector<std::string> args)=0;/*(void)args*/
public :
   EagleSoundSample(SoundManager* sound_man);

   virtual ~EagleSoundSample();

   virtual void Free()=0;

   virtual bool LoadFromFile(FilePath fp)=0;
   virtual bool CreateSample(int32_t nsamples , int32_t freq , int32_t sampledepth , bool stereo)=0;
};



class EagleSoundInstance : public EagleSound {
public :
   EagleSoundSample* parent;

   EagleSoundInstance(SoundManager* sound_man , EagleSoundSample* parent_sample);
   virtual ~EagleSoundInstance();
   
   EagleSoundSample* Parent() {return parent;}
};



class EagleSoundStream : public EagleSound , public ResourceBase {

protected:
   int32_t freq;// nsamples/second
   int32_t num_samples;/// number of samples in a fragment
   int32_t frag_count;/// how many fragments to buffer with
   int32_t size_of_data;
   bool    stereo;
   int32_t total_buffer_size;/// duh, 4*nsamples due to stereo int16_t buffer in recorder
   double  buffer_duration;/// secs
   std::vector<uint8_t> buffer;
   
   virtual bool LoadFromArgs(std::vector<std::string> args)=0;/*size_t nbuffers = 3 , size_t buf_sample_count = 32768*/
public :
   EagleSoundStream(SoundManager* sound_man);
   virtual ~EagleSoundStream();

   virtual void Free()=0;
   
   virtual bool Load(FilePath fp , size_t nbuffers = 3 , size_t buf_sample_count = 32768)=0;/// Automatically fed by Allegro
   virtual bool Create(int32_t nfragments , int32_t nsamples_per_fragment , int32_t frequency)=0;/// Filling buffer is done manually
};



class EagleSoundRecorder : public EagleSound {
protected :
   
   SoundManager* sound_manager;
   
   int32_t freq;// nsamples/second
   int32_t num_samples;/// number of samples in a fragment
   int32_t frag_count;/// how many fragments to buffer with
   int32_t size_of_data;
   bool    stereo;
   int32_t total_buffer_size;/// duh, 4*nsamples due to stereo int16_t buffer in recorder
   double  buffer_duration;/// secs
   int32_t record_index;
   
   
   std::vector<int8_t> buffer;
   
   EagleSoundSample* recorder_sample;
   EagleSoundInstance* recorder_sample_instance;
   
///   ALLEGRO_SAMPLE* smp;/// For playback
   
public :
//   EagleSoundRecorder();
   EagleSoundRecorder(SoundManager* sound_man);
   
   bool Record();
   
   int8_t Buffer(uint32_t index);
};



#endif // Sound_HPP






