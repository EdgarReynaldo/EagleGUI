

#include "Eagle/backends/Allegro5/Allegro5Sound.hpp"




/**
bool Allegro5Sound::Load(std::string file) {
   Free();
   a5sample = al_load_sample(file.c_str());
   return a5sample;
}



void Allegro5Sound::Play() {
   if (a5sample) {
      al_play_sample(a5sample , 1.0 , 0.0 , 1.0 , ALLEGRO_PLAYMODE_ONCE , 0);
   }
}



void Allegro5Sound::Free() {
   if (a5sample) {
      al_destroy_sample(a5sample);
      a5sample = 0;
   }
}



/// -----------------------------     Allegro 5 Sound Manager     ------------------------



void Allegro5SoundManager::SetupDefaultSoundEnvironment() {
   al_reserve_samples(2);
}



Sound* Allegro5SoundManager::CreateSound(std::string sound_file) {
   Allegro5Sound* s = new Allegro5Sound;
   if (!s->Load(sound_file)) {
      delete s;
      return 0;
   }
   snds.insert(s);
   return s;
}



void Allegro5SoundManager::FreeSound(Sound* s) {
   if (snds.find(s) != snds.end()) {
      s->Free();
      snds.erase(snds.find(s));
   }
}


//*/


