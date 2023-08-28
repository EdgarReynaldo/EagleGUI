

#include "Eagle/backends/Allegro5/Allegro5Sound.hpp"



Allegro5SoundSample::Allegro5SoundSample() :
      SoundSample(),
      sample_data(0)
{}
   


Allegro5SoundSample::~Allegro5SoundSample() {
   
}



bool Allegro5SoundSample::LoadFromFile(FilePath fp) {
   Free();
   sample_data = al_load_sample(fp.Path().c_str());
   return sample_data;
}



void Allegro5SoundSample::Free() {
   if (sample_data) {
      al_destroy_sample(sample_data);
      sample_data = 0;
   }
}



ALLEGRO_SAMPLE* Allegro5SoundSample::Data() {
   return sample_data;
}



Allegro5SoundInstance::Allegro5SoundInstance(SoundSample* psample) :
      SoundInstance(0),
      pinst(0)
{
   Allegro5SoundSample* asample = dynamic_cast<Allegro5SoundSample*>(psample);
   
   ALLEGRO_SAMPLE* smp = asample->Data();
   pinst = al_create_sample_instance(smp);
   SetSample(asample);
}



Allegro5SoundInstance::~Allegro5SoundInstance() {
   Free();
}




void Allegro5SoundInstance::Free() {
   if (pinst) {
      al_destroy_sample_instance(pinst);
      pinst = 0;
   }
}
   



void Allegro5SoundInstance::SetSample(SoundSample* psample) {
   parent = psample;
   ALLEGRO_SAMPLE* smp = dynamic_cast<Allegro5SoundSample*>(psample)->Data();
   if (pinst) {
      al_set_sample(pinst , smp);
      if (smp) {
         al_set_sample_instance_playing(pinst , true);
      }
   }
}



ALLEGRO_SAMPLE_INSTANCE* Allegro5SoundInstance::AllegroInstance() 
{
   return pinst;
}









Allegro5SoundStream::Allegro5SoundStream() :
      astream(0)
{}



Allegro5SoundStream::~Allegro5SoundStream() {
   Free();
}



bool Allegro5SoundStream::LoadFromFile(FilePath fp , size_t nbuffers , size_t buf_sample_count) {
   Free();
   return (astream = al_load_audio_stream(fp.Path().c_str() , nbuffers , buf_sample_count));
}



void Allegro5SoundStream::Free() {
   if (astream) {
      al_destroy_audio_stream(astream);
   }
   astream = 0;
}



ALLEGRO_AUDIO_STREAM* Allegro5SoundStream::AllegroStream() {
   return astream;
}










