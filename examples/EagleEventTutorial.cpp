
#include "Eagle.hpp"

#include "Eagle/backends/Allegro5Backend.hpp"



int main(int argc , char** argv) {

   (void)argc;
   (void)argv;

   /// To do most anything in Eagle, we need a system driver
   Allegro5System* sys = GetAllegro5System();
   EAGLE_ASSERT(sys);/// EAGLE_ASSERT is a regular assert but it logs and
                     /// throws an exception if null. EAGLE_DEBUG must
                     /// be defined or it will reduce to nothing.

   /// Now we have to initialize the system. Generally EAGLE_FULL_SETUP is used
   int sysret = sys->Initialize(EAGLE_FULL_SETUP);/// Return value is the
   EAGLE_ASSERT(sysret & EAGLE_STANDARD_SETUP);

   EagleGraphicsContext* win = sys->CreateGraphicsContext("Our Window" , 640 , 480 , EAGLE_OPENGL | EAGLE_WINDOWED);
   EAGLE_ASSERT(win && win->Valid());

   win->Clear();
   win->FlipDisplay();

   EagleEventHandler* q = sys->GetSystemQueue();

   bool quit = false;
   bool redraw = true;
   bool pause = false;
   std::string message;

   std::deque<std::string> messages;

   sys->GetSystemTimer()->Start();

   int mscnt = 0;

   while (!quit) {
      if (redraw) {
         win->Clear();
         if (messages.size()) {
            int i = 0;
            for (std::deque<std::string>::iterator it = messages.begin() ; it != messages.end() ; ++it) {
               win->DrawTextString(win->DefaultFont() , *it , 320 , 460 - 10*i , EagleColor(0,255,0) , HALIGN_CENTER , VALIGN_CENTER);
               ++i;
            }
         }
         win->FlipDisplay();
         redraw = false;

         while (messages.size() > 9) {
            messages.pop_front();
         }
      }
      if (q->HasEvent()) {
         EagleEvent e = sys->WaitForSystemEventAndUpdateState();
         if (e.type == EAGLE_EVENT_MOUSE_AXES) {
            mscnt++;
         }
         else {
            mscnt = 0;
         }
         if (e.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            message = "Display closed.";
            messages.push_back(message);
            quit = true;
         }
         else if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_ESCAPE) {
            message = "Escape pressed";
            messages.push_back(message);
            quit = true;
         }
         else if (e.type == EAGLE_EVENT_TIMER) {
            redraw = true;
         }
         else if (e.type == EAGLE_EVENT_MOUSE_AXES) {
            message = StringPrintF("EAGLE_EVENT_MOUSE_AXES %d" , mscnt);
            if (mscnt == 1) {
               messages.push_back(message);
            }
            else {
               messages[messages.size() - 1] = message;
            }
         }
         else {
            message = EagleEventName(e.type);
            messages.push_back(message);
         }
      }
   }
   return 0;
}



