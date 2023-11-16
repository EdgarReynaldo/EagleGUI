



#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"


int main(int argc , char** argv) {
   (void)argc;
   (void)argv;
   
   Allegro5System* sys = GetAllegro5System();
   EAGLE_ASSERT(sys);
   
   if (EAGLE_FULL_SETUP != sys->Initialize(EAGLE_FULL_SETUP)) {
      EagleWarn() << "Failed to initialize some subsystem. Continuiing." << std::endl;
   }

   Allegro5SoundManager soundman;
   bool soundup = soundman.Initialize();
   if (!soundup) {
      EagleCritical() << "Could not initialize sound manager." << std::endl;
      return -1;
   }
   
   soundman.ReserveInstances(10);
   EAGLE_ASSERT(soundman.InstanceCount() == 10);
   
   
   
   std::vector<EagleSoundSample*> soundboard;
   std::vector<EagleSoundInstance*> instances;
   
   EagleSoundStream* stream = soundman.CreateSoundStream(FilePath("Data/Audio/GrandDarkWaltzAllegro.mp3"));
   soundman.ReadyBGStream(stream);
   std::shared_ptr<Folder> soundfolder = sys->GetFileSystem()->ReadFolder(FilePath("Data/Audio/WET") , false);
   Folder* f = soundfolder.get();
   Folder::FILEMAP sfxmap = f->Files();

{
   
   int i = 0;
   for (Folder::FILEMAP::iterator it = sfxmap.begin() ; it != sfxmap.end() ; ++it) {
      std::shared_ptr<File> file = it->second;
      soundboard.push_back(soundman.CreateSoundSample(file->Path()));
      soundman.SetSample(soundman.GetInstance(i) , soundboard[i]);
      instances.push_back(soundman.GetInstance(i));
      ++i;
   }
}   
   soundman.PlayAllSound(true);
   bool musicon = true;
   bool sampleson = true;
   
   std::vector<int32_t> keycodes = {EAGLE_KEY_0 , EAGLE_KEY_1  , EAGLE_KEY_2 , EAGLE_KEY_3 , EAGLE_KEY_4 , EAGLE_KEY_5 , EAGLE_KEY_6 , EAGLE_KEY_7 , EAGLE_KEY_8 , EAGLE_KEY_9};
   
   EagleGraphicsContext* window = sys->CreateGraphicsContext("Soundboard" , 800 , 600 , EAGLE_OPENGL | EAGLE_WINDOWED , -1 , -1);
   EAGLE_ASSERT(window && window->Valid());
   
   window->Clear();
   window->FlipDisplay();
   
   EagleFont* font = window->GetFont("Data/Fonts/Verdana.ttf" , -40 , 0 , VIDEO_IMAGE);
   EAGLE_ASSERT(font);
   
   bool quit = false;
   bool redraw = true;
   
   while (!quit) {
      if (redraw) {
         window->Clear();
         window->DrawTextString(font , StringPrintF("Welcome To Sound Board") , 10 , 10 , EagleColor(255,255,255));
         window->DrawTextString(font , StringPrintF("Keys: S Sound M Music 0123456789 SFX0-9") , 10 , 550 , EagleColor(255,255,255) , HALIGN_LEFT , VALIGN_TOP);
         window->FlipDisplay();
      }
      do {
         EagleEvent e = sys->WaitForSystemEventAndUpdateState();
         if (e.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         if (e.type == EAGLE_EVENT_KEY_DOWN) {
            EagleInfo() << "Key down event" << std::endl;
            if (e.keyboard.keycode == EAGLE_KEY_ESCAPE) {
               quit = true;
            }
            for (unsigned int i = 0 ; i < keycodes.size() ; ++i) {
               if (e.keyboard.keycode == keycodes[i]) {
                  if (input_key_held(EAGLE_KEY_ANY_SHIFT)) {
                     soundman.SetInstancePlaying(instances[i] , false);
                  }
                  else {
                     //    keys 0-9 will start and stop samples 0 thru 9
                     soundman.SetInstancePlaying(instances[i] , true);
                  }
               }
            }
            if (e.keyboard.keycode == EAGLE_KEY_M) {
               musicon = !musicon;
               soundman.SetBGStreamPlaying(musicon);
            }
            if (e.keyboard.keycode == EAGLE_KEY_S) {
               sampleson = !sampleson;
               soundman.SetSampleMixerPlaying(sampleson);
            }
         }
      } while (!sys->UpToDate());
   }
   












   return 0;
}



























