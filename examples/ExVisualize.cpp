



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
   
   
   int sw = 1920;
   int sh = 1080;
   int ww = 1280;
   int wh = 800;
   int cw = 0;
   int ch = 0;
   al_set_new_display_option(ALLEGRO_VSYNC , 2 , ALLEGRO_REQUIRE);
   Allegro5GraphicsContext* win = dynamic_cast<Allegro5GraphicsContext*>(a5sys->CreateGraphicsContext("VizzuaLazer" , sw , sh , EAGLE_OPENGL | EAGLE_FULLSCREEN_WINDOW));
   int vsync = al_get_display_option(win->AllegroDisplay() , ALLEGRO_VSYNC);
   EAGLE_ASSERT(win && win->Valid());
   sw = win->Width();
   sh = win->Height();
   cw = sw;
   ch = sh;
   
   win->Clear();
   win->FlipDisplay();
   
   EagleLog() << StringPrintF("VSYNC IS %s (%d)" , (vsync == 2)?"OFF":"ON" , vsync) << std::endl;
   
   EagleFont* font = win->GetFont("Data/Fonts/Verdana.ttf" , -20);
//al_create_audio_recorder
//ALLEGRO_AUDIO_RECORDER *al_create_audio_recorder(size_t fragment_count,
//   unsigned int samples, unsigned int frequency,
//   ALLEGRO_AUDIO_DEPTH depth, ALLEGRO_CHANNEL_CONF chan_conf)
   int frequency = 96100;
   int fragment_count = 8;
   int samples = 2048;// 15 * 8 = 120 *7 = 105 , // 44100/240 = 22050/120 = 11025/60 = 2205/12 = 1102.5/6 = 183~
   double buf_duration = fragment_count*samples/(double)frequency;
   int frag_index = 0;// up to fragment_count - 1
   
   
   ALLEGRO_AUDIO_RECORDER* rec = al_create_audio_recorder(fragment_count , samples , frequency , ALLEGRO_AUDIO_DEPTH_INT16 , ALLEGRO_CHANNEL_CONF_2);
   
   if (!rec) {
      EagleError() << "Failed to create audio recorder." << std::endl;
      return 1;
   }
   
   ALLEGRO_EVENT_QUEUE* receiver = al_create_event_queue();
   
   al_register_event_source(receiver , al_get_audio_recorder_event_source(rec));
   
   
   al_start_audio_recorder(rec);
   
//   a5sys->GetSystemTimer()->Start();
   
   bool quit = false;
   bool redraw = true;
   
   std::vector<int8_t> audio_storage;
   int storage_size = fragment_count*samples*2*2;/// 2 for uint16_t and 2 for LR pairs per sample
   audio_storage.resize(storage_size , 0);
   int record_index = 0;
   int play_index = 0;
   
   double clear_time = 0.0;
   double draw_time = 0.0;
   double flip_time = 0.0;
   double frag_time = 0.0;
   double flips;
   double clear_time_avg = 0.0;
   double draw_time_avg = 0.0;
   double flip_time_avg = 0.0;
   double frag_time_avg = 0.0;
   double frag_count = 0.0;
   double event_time = 0.0;
   double event_time_avg = 0.0;
   double event_count = 0.0;
   
   double frames = 0.0;
   std::deque<double> frame_array;
   double frametime = 0.0;
   double frametotal = 0.0;
   
   double updatecount = 0.0;
   double updatetime = 0.0;
   double updateavg = 0.0;
   
   while (!quit) {
      if (redraw) {
         ProgramTime frame_time_start(ProgramTime::Now());
         ProgramTime start = frame_time_start;
         win->Clear();
         ProgramTime clear(ProgramTime::Now());
         clear_time += clear - start;
         const double y = sh/2.0;
         /// Just draw the input monitor
///         for (int i =  (record_index - samples)%storage_size; i < record_index ; i+=2) {
///         for (int i = record_index - samples + 4 ; i < record_index ; i += 2) {
///         for (int i = record_index/8 + 4 ; i < (record_index/8 + 1)*samples ; i += 2) {
         int sample_index = 0;
         sample_index = frag_index*samples*4;
         for (int index = 4 ; index < samples*4 ; index += 4) {
            int i = index + sample_index;
            double x1 = (index - 2) / (double)(samples*4)*sw;
            double x2 = (index) / (double)(samples*4)*sw;
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
         win->DrawTextString(font , StringPrintF("%d" , frag_index) , 10 , 10 , EagleColor(255,255,255,255) , HALIGN_LEFT , VALIGN_TOP);
//         win->DrawLine(sw*record_index/(double)storage_size , 0 , sw*record_index/(double)storage_size , sh , 1.0 , EagleColor(0,255,0,255));
         win->DrawTextString(font , StringPrintF("FPS:%4 .1f" , win->GetFPS()) , cw - 10 , 10 , EagleColor(0,255,0,255) , HALIGN_RIGHT , VALIGN_TOP);
         win->DrawTextString(font , StringPrintF("FPS:%4 .1f" , frames/frametotal) , cw - 10 , 40 , EagleColor(0,255,0,255) , HALIGN_RIGHT , VALIGN_TOP);
         win->DrawTextString(font , StringPrintF("FLIP=%lf DRAW=%lf CLEAR=%lf" , flip_time_avg , draw_time_avg , clear_time_avg) , sw/2.0 , sh - 50 , EagleColor(255,255,255) , HALIGN_CENTER , VALIGN_BOTTOM);
         win->DrawTextString(font , StringPrintF("EVTIME=%lf FRAGTIME=%lf UPDATE=%lf" , event_time_avg , frag_time_avg , updatetime) , sw/2.0 , sh - 10 , GetColorByName("white") , HALIGN_CENTER , VALIGN_BOTTOM);
         ProgramTime draw(ProgramTime::Now());
         draw_time += draw - clear;
         win->FlipDisplay();
         flips += 1.0;
         ProgramTime flip(ProgramTime::Now());
         flip_time += flip - draw;
         redraw = false;
         if (flips) {
            flip_time_avg = flip_time / flips;
            draw_time_avg = draw_time / flips;
            clear_time_avg = clear_time / flips;
         }
         ProgramTime frame_time_stop(ProgramTime::Now());
         frames += 1.0;
         frametime = frame_time_stop - frame_time_start;
         frametotal += frametime;
         frame_array.push_back(frametime);
         if (frame_array.size() > 120) {
            frames = 120.0;
            frametotal -= frame_array.front();
            frame_array.pop_front();
         }
//         EagleLog() << StringPrintF("Flip took %lf.\tDraw took %lf.\tClear took %lf.\n" , flip_time_avg , draw_time_avg , clear_time_avg) << std::endl;
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
      EagleEvent ee;
      ProgramTime start2(ProgramTime::Now());
      ProgramTime update(ProgramTime::Now());
      ProgramTime mutex1 = start2;
      while (!redraw) {
         start2 = ProgramTime::Now();
         int rcount = 0;
         ALLEGRO_EVENT evt;
         if (al_get_next_event(receiver , &evt)) {
            event_count += 1.0;
            ProgramTime event(ProgramTime::Now());
            event_time += event - start2;
            event_time_avg = event_time / event_count;

            start2 = event;
            if (evt.type == ALLEGRO_EVENT_AUDIO_RECORDER_FRAGMENT) {
               ++rcount;
               frag_count += 1.0;
               ALLEGRO_AUDIO_RECORDER_EVENT* revt = al_get_audio_recorder_event(&evt);
               /// Add fragment to buffer. We just got at least one fragment of 1/50 second a piece
               for (int i = 0 ; i < 4*revt->samples ; i += 4) {
                  audio_storage[(record_index++)%storage_size] = ((int8_t*)(revt->buffer))[i];
                  audio_storage[(record_index++)%storage_size] = ((int8_t*)(revt->buffer))[i+1];
                  audio_storage[(record_index++)%storage_size] = ((int8_t*)(revt->buffer))[i+2];
                  audio_storage[(record_index++)%storage_size] = ((int8_t*)(revt->buffer))[i+3];
                  record_index = record_index%storage_size;
               }
               frag_index = (frag_index + 1) % fragment_count;
               redraw = true;
               ProgramTime fragment(ProgramTime::Now());
               frag_time += fragment - event;
               frag_time_avg = frag_time/frag_count;
            }
         }
         ee = a5sys->GetSystemQueue()->TakeNextEvent(0);
         if (ee.type == EAGLE_EVENT_NONE) {break;}
         mutex1 = ProgramTime::Now();
         event_time += mutex1 - start2;
         event_count += 1.0;
         event_time_avg = event_time / event_count;
         a5sys->UpdateSystemState();
         update = ProgramTime::Now();
         updatetime += update - mutex1;
         updatecount += 1.0;
         updateavg = updatecount / updatetime;
         if (ee.type == EAGLE_EVENT_TIMER) {
//            redraw = true;
         }
         if (ee.type == EAGLE_EVENT_KEY_DOWN) {
            if (ee.keyboard.keycode == EAGLE_KEY_ESCAPE) {quit = true;}
            
         }
         if (ee.type == EAGLE_EVENT_DISPLAY_CLOSE) {quit = true;}
      }
   }
   
   
   
   
   al_stop_audio_recorder(rec);
   
   
   
   
   al_destroy_audio_recorder(rec);
   
   EagleLog() << StringPrintF("Update average = %.8f" , updatetime / updatecount) << std::endl;
   return 0;
}
