
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


class SoundManager;

class EagleSound {
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

   virtual bool LoadFromArgs(std::vector<std::string> args)=0;/*(void)args*/
public :
   EagleSoundSample();

   virtual ~EagleSoundSample();

   virtual void Free()=0;
};



class EagleSoundInstance : public EagleSound {
public :
   EagleSoundSample* parent;

   EagleSoundInstance(EagleSoundSample* parent_sample);
   virtual ~EagleSoundInstance();
   
   EagleSoundSample* Parent() {return parent;}
};



class EagleSoundStream : public EagleSound , public ResourceBase {

   virtual bool LoadFromArgs(std::vector<std::string> args)=0;/*size_t nbuffers = 3 , size_t buf_sample_count = 32768*/
public :
   EagleSoundStream();
   virtual ~EagleSoundStream();

   virtual void Free()=0;
};



class EagleSoundRecorder : public EagleSoundInstance {
protected :
   
   int32_t freq;// nsamples/second
   int32_t num_samples;/// number of samples in a fragment
   int32_t frag_count;/// how many fragments to buffer with
   int32_t size_of_data;
   bool    stereo;
   int32_t total_buffer_size;/// duh, 4*nsamples due to stereo int16_t buffer in recorder
   double  buffer_duration;/// secs
   int32_t record_index;
   
   
   std::vector<char> buffer;
   
///   ALLEGRO_SAMPLE* smp;/// For playback
   
public :
//   EagleSoundRecorder();
   EagleSoundRecorder() :
         EagleSoundInstance(0),
         freq(-1),
         num_samples(-1),
         frag_count(-1),
         size_of_data(0),
         stereo(false),
         total_buffer_size(0),
         buffer_duration(0),
         record_index(0),
         buffer()
   {}
   
   bool Record();
};



#endif // Sound_HPP






