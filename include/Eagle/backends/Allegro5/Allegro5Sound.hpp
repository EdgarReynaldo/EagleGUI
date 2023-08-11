
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



class Allegro5Sound : public Sound {
protected :
   Allegro5SoundManager* a5sndman;
   
   
   
public :
   virtual ~Sound() {}
   virtual bool Load(std::string file)=0;
   virtual void Free()=0;
   virtual void Play()=0;
   virtual void Pause()=0;
   virtual void Stop()=0;
   
};


class Allegro5SoundSample : public Allegro5Sound {
protected:
   std::string shortname;///< For accessing the sound manager and our sample to play instances
   ALLEGRO_SAMPLE* psample;///< We own this
   std::set<Allegro5SoundInstance*> our_active_instances;
public :
   virtual ~Sound();
   
   void Free();
void Allegro5SoundSample::Free() {
   if (psample) {
      a5sndman->DestroySampleInstances(this);
      al_destroy_sample(psample);
      psample = 0;
   }
}
   
   virtual bool Load(FilePath fp);

   virtual bool Load(FilePath fp) {
      Free();
      psample = al_load_sample(fp.Path());
      if (psample) {
         shortname = f->Name();
      }
      return psample;
   }
   virtual void Play();
   virtual void Pause();
   virtual void Stop();
   
   std::string GetShortName() {return shortname;}
   
};

void Allegro5SoundSample::Play() {
   Allegro5SoundInstance* instance = a5sndman->PlayNewSampleInstance(shortname);
   our_active_instances.insert(instance);
}
void Allegro5SoundSample::Pause() {

}
void Allegro5SoundSample::Stop() {

}




#endif // Allegro5Sound_HPP




