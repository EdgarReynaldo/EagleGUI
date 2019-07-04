
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
 *    Copyright 2009-2019+ by Edgar Reynaldo
 *
 *    See EagleLicense.txt for allowed uses of this library.
 *
 * @file Allegro5Sound.hpp
 * @brief The interface for sound using Allegro 5
 */

#ifndef Allegro5Sound_HPP
#define Allegro5Sound_HPP


#include "Eagle/Sound.hpp"

#include "allegro5/allegro_audio.h"

/**

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

//*/


#endif // Allegro5Sound_HPP




