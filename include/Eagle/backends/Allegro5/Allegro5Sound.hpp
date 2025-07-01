
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
 * @file Allegro5Sound.hpp
 * @brief The interface for sound using Allegro 5
 */

#ifndef Allegro5Sound_HPP
#define Allegro5Sound_HPP



#include "Eagle/Sound.hpp"
#include "Eagle/File.hpp"
#include "Eagle/FileSystem.hpp"
#include "Eagle/System.hpp"

#include "allegro5/allegro_audio.h"



class Allegro5SoundSample : public EagleSoundSample {
protected:
   ALLEGRO_SAMPLE* sample_data;

   bool LoadFromArgs(std::vector<std::string> args) override;

public :
   Allegro5SoundSample();
   ~Allegro5SoundSample();
   
   bool LoadFromFile(FilePath fp);

   void Free();
   
   ALLEGRO_SAMPLE* Data();
};



class Allegro5SoundInstance : public EagleSoundInstance {
   ALLEGRO_SAMPLE_INSTANCE* pinst;

public :

   Allegro5SoundInstance(EagleSoundSample* psample);

   ~Allegro5SoundInstance();
   
   void Free();

   void SetSample(EagleSoundSample* psample);///< Resets the sample for this instance and plays it, recycling the instance
   
   ALLEGRO_SAMPLE_INSTANCE* AllegroInstance();
};



class Allegro5SoundStream : public EagleSoundStream {

   ALLEGRO_AUDIO_STREAM* astream;

   bool LoadFromArgs(std::vector<std::string> args) override;/*size_t nbuffers = 3 , size_t buf_sample_count = 32768*/

public :
   Allegro5SoundStream();

   ~Allegro5SoundStream();

   virtual bool LoadFromFile(size_t nbuffers = 3 , size_t buf_sample_count = 32768);

   void Free();
   
   ALLEGRO_AUDIO_STREAM* AllegroStream();
};


/**

class EagleSoundRecorder : public EagleSound {
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

//*/
class Allegro5SoundRecorder : public EagleSoundRecorder {

protected :
   ALLEGRO_AUDIO_RECORDER* recorder;

public :
   Allegro5SoundRecorder();
   Allegro5SoundRecorder() :
         EagleSoundRecorder(),
         recorder(0)
   {
   }

   bool Create(int32_t frequency , int32_t nsamples_per_fragment , int32_t fragment_count)
   void Destroy();

};



#endif // Allegro5Sound_HPP












