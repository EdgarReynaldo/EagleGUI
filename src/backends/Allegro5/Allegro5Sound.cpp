

#include "Eagle/backends/Allegro5/Allegro5Sound.hpp"
#include "Eagle/StringWork.hpp"



bool Allegro5SoundSample::LoadFromArgs(std::vector<std::string> args) {
   (void)args;
   return LoadFromFile(filepath);
}



Allegro5SoundSample::Allegro5SoundSample() :
      EagleSoundSample(),
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



/// -----------------------     Allegro5SoundInstance    -------------------------




Allegro5SoundInstance::Allegro5SoundInstance(EagleSoundSample* psample) :
      EagleSoundInstance(psample),
      pinst(0)
{
   Allegro5SoundSample* asample = dynamic_cast<Allegro5SoundSample*>(psample);

   ALLEGRO_SAMPLE* smp = asample?asample->Data():0;
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




void Allegro5SoundInstance::SetSample(EagleSoundSample* psample) {
   parent = psample;
   Allegro5SoundSample* a5sample = dynamic_cast<Allegro5SoundSample*>(psample);
   ALLEGRO_SAMPLE* smp = a5sample?a5sample->Data():0;
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



/// -----------------------     Allegro5SoundStream     ----------------------




bool Allegro5SoundStream::LoadFromArgs(std::vector<std::string> args) {
   int nbuffers = 3;
   int fragcount = 32768;
   if (args.size()) {
      nbuffers = STOI(args[0]);
      if (args.size() > 1) {
         fragcount = STOI(args[1]);
      }
   }
   return LoadFromFile(nbuffers , fragcount);
}



Allegro5SoundStream::Allegro5SoundStream() :
      astream(0)
{}



Allegro5SoundStream::~Allegro5SoundStream() {
   Free();
}



bool Allegro5SoundStream::LoadFromFile(size_t nbuffers , size_t buf_sample_count) {
   Free();
   std::string p = filepath.Path();
   return (astream = al_load_audio_stream(p.c_str() , nbuffers , buf_sample_count));
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










