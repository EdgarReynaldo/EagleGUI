
/**

TINS 2026 @Copyright Marc Davenport aka bugsquasher aka Modulo5k aka Edgar Reynaldo

Here is a brief summary: 
1. It's a zoo! 
2. Science fiction reference. 
3. Catastrophic overflow. 
4. Quick save. 
5. Bonus: Let a chatbot replace a rule.



*/
#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"
#include "Physfs.h"

int main(int argc , char** argv) {

   (void)argc;
   (void)argv;

   PHYSFS_init(argv[0]);
   
   /// To do most anything in Eagle, we need a system driver
   Allegro5System* sys = GetAllegro5System();
   EAGLE_ASSERT(sys);/// EAGLE_ASSERT is a regular assert but it logs and
                     /// throws an exception if null. EAGLE_DEBUG must
                     /// be defined or it will reduce to nothing.
   
   /// Now we have to initialize the system. Generally EAGLE_FULL_SETUP is used
   int sysret = sys->Initialize(EAGLE_FULL_SETUP);/// Return value is the
                                                  /// degree of success
   if (EAGLE_FULL_SETUP != sysret) {
      /// Some part of setup failed
      PrintFailedEagleInitStates(EAGLE_FULL_SETUP , sysret);/// Log the failure
      /// Test for ...
      if (!(sysret & EAGLE_STANDARD_SETUP)) {
         /// Failed to init an essential service
         return -1;
      }
      /// Just keep going. Either touch or shaders failed but we don't need
      /// them in this case
   }
   
   EagleLog() << "Setup eagle successfully" << std::endl;


   EagleGraphicsContext* win = sys->CreateGraphicsContext("TINS2026" , 1024 , 768 , EAGLE_WINDOWED | EAGLE_OPENGL);
   EAGLE_ASSERT(win && win->Valid());
   
   EagleFont* emoji32 = win->GetFont("Data/Fonts/NotoColorEmoji.ttf" , -48);
   ALLEGRO_FONT* emoji = dynamic_cast<Allegro5Font*>(emoji32)->AllegroFont();
   
   
   bool quit = false;
   bool redraw = true;
   sys->GetSystemTimer()->Start();/// The system timer is created automatically, but needs to be manually started
   
   while (!quit) {
      if (redraw) {
         win->Clear();
//         al_draw_text(emoji , al_map_rgba(255,255,255,255) , 10 , 10 , 0 , U+007f);
         win->DrawTextString(emoji32 , "🐴" , 512 , 384 , EagleColor(255,255,255,255) , HALIGN_CENTER , VALIGN_CENTER);
         win->FlipDisplay();
      }
      do {
         /// Let's handle two birds with one stone. WaitForEventAndUpdateState will do this for us.
         /// WaitForEvent will wait for an event from the system queue. Update state will update the input handler for us.
         /// We rarely want to do this manually.
         EagleEvent ee = sys->WaitForSystemEventAndUpdateState();
         if (ee.type == EAGLE_EVENT_TIMER) {
            redraw = true;
         }
         if (ee.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         if ((ee.type == EAGLE_EVENT_KEY_DOWN) && (ee.keyboard.keycode == EAGLE_KEY_ESCAPE)) {
            quit = true;
         }
      } while (!sys->UpToDate());
   }


   return 0;
}
