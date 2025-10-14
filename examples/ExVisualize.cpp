



#define ALLEGRO_UNSTABLE

#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"


#include "allegro5/allegro.h"
#include "allegro5/allegro_audio.h"



int main(int argc , char** argv) {

   Allegro5System* a5sys = GetAllegro5System();
   
   if (!a5sys) {return -1;}
   
   int init = a5sys->Initialize(EAGLE_FULL_SETUP);
   if (init != EAGLE_FULL_SETUP) {
      EagleWarn() << "Failed to fully setup Eagle." << std::endl;
   }
   
   
   int sw = 0;
   int sh = 0;
   int ww = 1280;
   int wh = 800;
   int cw = 0;
   int ch = 0;
   
   EagleGraphicsContext* win = a5sys->CreateGraphicsContext("VizzuaLazer" , ww , wh , EAGLE_OPENGL | EAGLE_WINDOWED);
   EAGLE_ASSERT(win && win->Valid());
   sw = win->Width();
   sh = win->Height();
   cw = sw;
   ch = sh;
   
   win->Clear();
   win->FlipDisplay();
   
   
   
   
//al_create_audio_recorder
//ALLEGRO_AUDIO_RECORDER *al_create_audio_recorder(size_t fragment_count,
//   unsigned int samples, unsigned int frequency,
//   ALLEGRO_AUDIO_DEPTH depth, ALLEGRO_CHANNEL_CONF chan_conf)
   int frequency = 44100;
   int fragment_count = 60;/// Buffer one second at 60 hz
   int samples = frequency/fragment_count;
   double buf_duration = fragment_count*samples/(double)frequency;
   
   ALLEGRO_AUDIO_RECORDER* rec = al_create_audio_recorder(4 , 11025 , 44100 , ALLEGRO_AUDIO_DEPTH_INT16 , ALLEGRO_CHANNEL_CONF_2);
   
   if (!rec) {
      EagleError() << "Failed to create audio recorder." << std::endl;
      return 1;
   }
   
   ALLEGRO_EVENT_QUEUE* receiver = al_create_event_queue();
   
   al_register_event_source(receiver , al_get_audio_recorder_event_source(rec));
   
   
   al_start_audio_recorder(rec);
   
   a5sys->GetSystemTimer()->Start();
   
   bool quit = false;
   bool redraw = true;
   
   std::vector<int8_t> audio_storage;
   int storage_size = fragment_count*samples*2*2;/// 2 for uint16_t and 2 for LR pairs per sample
   audio_storage.resize(storage_size , 0);
   int record_index = 0;
   int play_index = 0;
   
   
   while (!quit) {
      if (redraw) {
         win->Clear();
         const double y = sh/2.0;
         /// Just draw the input monitor
         for (int i =  (record_index - samples)%storage_size; i < record_index ; i+=2) {
///         for (int i = record_index - samples + 4 ; i < record_index ; i += 2) {
///         for (int i = record_index/8 + 4 ; i < (record_index/8 + 1)*samples ; i += 2) {
//         for (int i = 4 ; i < storage_size ; i+=2) {
            double x1 = ((double)(i-2)/storage_size)*sw;
            double x2 = ((double)i/storage_size)*sw;
            int16_t l1 = 0;
            int16_t r1 = 0;
            int16_t l2 = 0;
            int16_t r2 = 0;
            l1 = *(int16_t*)(&audio_storage[i-4]);
            r1 = *(int16_t*)(&audio_storage[i-2]);
            l2 = *(int16_t*)(&audio_storage[i]);
            r2 = *(int16_t*)(&audio_storage[i+2]);
            int y1 = y/2.0;
            y1 += l1*(double)(sh/4.0)/32767.0;
            int y2 = y/2.0;
            y2 += l2*(double)(sh/4.0)/32767.0;


            int y3 = 3*y/2.0;
            y3 += r1*(double)(sh/4.0)/32767.0;
            int y4 = 3*y/2.0;
            y4 += r2*(double)(sh/4.0)/32767.0;
            
            win->DrawLine(x1 , y1 , x2 , y2 , 1.0 , EagleColor(255,255,255));
            win->DrawLine(x1 , y3 , x2 , y4 , 1.0 , EagleColor(255,255,255));
            
         }
         win->DrawLine(sw*record_index/(double)storage_size , 0 , sw*record_index/(double)storage_size , sh , 1.0 , EagleColor(0,255,0,255));
         win->FlipDisplay();
         redraw = false;
      }
      
      do {
         EagleEvent ee = a5sys->WaitForSystemEventAndUpdateState();
         if (ee.type == EAGLE_EVENT_TIMER) {
            redraw = true;
         }
/**
ALLEGRO_AUDIO_RECORDER_EVENT
typedef struct ALLEGRO_AUDIO_RECORDER_EVENT ALLEGRO_AUDIO_RECORDER_EVENT;
Source Code

Structure that holds the audio recorder event data. Every event type will contain:

.source: pointer to the audio recorder 
The following will be available depending on the event type:

.buffer: pointer to buffer containing the audio samples 
.samples: number of samples (not bytes) that are available 
Since 5.1.1

See also: al_get_audio_recorder_event

//*/
         if (ee.type == EAGLE_EVENT_AUDIO_RECORDER_FRAGMENT) {
            /// Add fragment to buffer. We just got at least one fragment of 1/4 second a piece
            for (int i = 0 ; i < 4*ee.audio.sample_count ; i += 4) {
               audio_storage[(record_index++)%storage_size] = ((int8_t*)(ee.audio.rec_byte_buffer))[i];
               audio_storage[(record_index++)%storage_size] = ((int8_t*)(ee.audio.rec_byte_buffer))[i+1];
               audio_storage[(record_index++)%storage_size] = ((int8_t*)(ee.audio.rec_byte_buffer))[i+2];
               audio_storage[(record_index++)%storage_size] = ((int8_t*)(ee.audio.rec_byte_buffer))[i+3];
               record_index = record_index%storage_size;
            }
         }
         
         
         
         if (ee.type == EAGLE_EVENT_KEY_DOWN) {
            if (ee.keyboard.keycode == EAGLE_KEY_ESCAPE) {quit = true;}
            
         }
         if (ee.type == EAGLE_EVENT_DISPLAY_CLOSE) {quit = true;}
      } while (!a5sys->GetSystemQueue()->HasEvent());
      
      
      
   }
   
   
   
   al_stop_audio_recorder(rec);
   
   
   
   
   al_destroy_audio_recorder(rec);
   
   
   return 0;
}
