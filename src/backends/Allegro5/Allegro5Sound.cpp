

#include "Eagle/backends/Allegro5/Allegro5Sound.hpp"
#include "Eagle/StringWork.hpp"
#include "Eagle/System.hpp"
#include "Eagle/backends/Allegro5/Allegro5SoundManager.hpp"


bool Allegro5SoundSample::LoadFromArgs(std::vector<std::string> args) {
   (void)args;
   return LoadFromFile(filepath);
}



Allegro5SoundSample::Allegro5SoundSample(SoundManager* sound_man) :
      EagleSoundSample(sound_man),
      internal_buffer(),
      sample_data(0)
{}



Allegro5SoundSample::~Allegro5SoundSample() {

}



bool Allegro5SoundSample::LoadFromFile(FilePath fp) {
   Free();
   sample_data = al_load_sample(fp.Path().c_str());
   return sample_data;
}



bool Allegro5SoundSample::CreateSample(int32_t nsamples , int32_t freq , int32_t sampledepth , bool stereo) {
   Free();
   ALLEGRO_AUDIO_DEPTH depth = ALLEGRO_AUDIO_DEPTH_INT16;
   if (sampledepth == 4) {
      depth = ALLEGRO_AUDIO_DEPTH_FLOAT32;
   }
   else if (sampledepth == 2) {
      depth = ALLEGRO_AUDIO_DEPTH_INT16;
   }
   internal_buffer.resize(nsamples*sampledepth*(stereo?2:1) , 0);
   sample_data = al_create_sample((void*)(&internal_buffer[0]) , nsamples , freq , depth , 
                                  stereo?ALLEGRO_CHANNEL_CONF_2:ALLEGRO_CHANNEL_CONF_1 , false);
   return (bool)sample_data;
}



void Allegro5SoundSample::Free() {
   if (sample_data) {
      al_destroy_sample(sample_data);
      sample_data = 0;
   }
   internal_buffer.clear();
}



ALLEGRO_SAMPLE* Allegro5SoundSample::Data() {
   return sample_data;
}



/// -----------------------     Allegro5SoundInstance    -------------------------




Allegro5SoundInstance::Allegro5SoundInstance(SoundManager* sound_man , EagleSoundSample* psample) :
      EagleSoundInstance(sound_man , psample),
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
      filepath.SetPath(args[0]);
      if (args.size() > 2) {
         nbuffers = STOI(args[1]);
         fragcount = STOI(args[2]);
      }
   }
   return Load(filepath , nbuffers , fragcount);
}



Allegro5SoundStream::Allegro5SoundStream(SoundManager* sound_man) :
      EagleSoundStream(sound_man),
      astream(0)
{}



Allegro5SoundStream::~Allegro5SoundStream() {
   Free();
}



bool Allegro5SoundStream::Load(FilePath fp , size_t nbuffers , size_t buf_sample_count) {
   Free();
   return (astream = al_load_audio_stream(fp.Path().c_str() , nbuffers , buf_sample_count));
}



bool Allegro5SoundStream::Create(int32_t nfragments , int32_t nsamples_per_fragment , int32_t frequency) {
   Free();
   frag_count = nfragments;
   num_samples = nsamples_per_fragment;
   freq = frequency;
   size_of_data = 2;
   assert(sizeof(int16_t) == 2);
   stereo = true;
   total_buffer_size = (stereo?2:1)*size_of_data*num_samples*frag_count;
   buffer_duration = total_buffer_size/(double)freq;
   buffer.resize(total_buffer_size , 0);
   astream = al_create_audio_stream(frag_count , num_samples , freq , ALLEGRO_AUDIO_DEPTH_INT16 , ALLEGRO_CHANNEL_CONF_2);
   if (!astream) {
      Free();
      return false;
   }
   return true;
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



/// ********************************    Allegro5SoundRecorder     ********************



Allegro5SoundRecorder::Allegro5SoundRecorder(SoundManager* sound_man) :
      EagleSoundRecorder(sound_man),
      recorder(0)
{}



bool Allegro5SoundRecorder::Create(int32_t frequency , int32_t nsamples_per_fragment , int32_t fragment_count) {
   Destroy();
   freq = frequency;
   num_samples = nsamples_per_fragment;
   frag_count = fragment_count;
   size_of_data = 2;/// int16_t audio data
   assert(sizeof(int16_t) == 2);
   stereo = true;
   total_buffer_size = (stereo?2:1)*size_of_data*num_samples*frag_count;
   buffer_duration = total_buffer_size/(double)freq;
   record_index = 0;
   buffer.resize(total_buffer_size , 0);
/**
ALLEGRO_AUDIO_RECORDER *al_create_audio_recorder(size_t fragment_count,
unsigned int samples, unsigned int frequency,
ALLEGRO_AUDIO_DEPTH depth, ALLEGRO_CHANNEL_CONF chan_conf)
//*/
   recorder = al_create_audio_recorder(frag_count , num_samples , freq , ALLEGRO_AUDIO_DEPTH_INT16 , ALLEGRO_CHANNEL_CONF_2);
   return recorder;
}



void Allegro5SoundRecorder::Destroy() {
   if (recorder) {
      if (al_is_audio_recorder_recording(recorder)) {
         al_stop_audio_recorder(recorder);
      }
      al_destroy_audio_recorder(recorder);
      recorder = 0;
   }
}



ALLEGRO_AUDIO_RECORDER* Allegro5SoundRecorder::GetRecorder() {
   return recorder;
}






