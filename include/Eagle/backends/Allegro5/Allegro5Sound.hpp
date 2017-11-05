


#ifndef Allegro5Sound_HPP
#define Allegro5Sound_HPP


#include "Eagle/Sound.hpp"

#include "allegro5/allegro_audio.h"



class Allegro5Sound : public Sound {

   ALLEGRO_SAMPLE* a5sample;
   
public :   
   
   Allegro5Sound() : Sound() , a5sample(0) {}
   
   bool Load(std::string file);
   void Play();
   void Free();
};


#include <set>



class Allegro5SoundManager : public SoundManager {
   
   std::set<Sound*> snds;
   
public :
   Allegro5SoundManager() : SoundManager() , snds() {
      SetupDefaultSoundEnvironment();
   }


   void SetupDefaultSoundEnvironment();
   Sound* CreateSound(std::string sound_file);
   void FreeSound(Sound* s);
   
};


#endif // Allegro5Sound_HPP




