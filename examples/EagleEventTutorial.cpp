
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
   std::string message;

   std::deque<std::string> messages;

   sys->GetSystemTimer()->Start();

   int mscnt = 0;
   int tcnt = 0;
   int ccnt = 0;

   while (!quit) {
      if (redraw) {
         win->Clear();
         if (messages.size()) {
            int i = 0;
            for (std::deque<std::string>::reverse_iterator rit = messages.rbegin() ; rit != messages.rend() ; ++rit) {
               win->DrawTextString(win->DefaultFont() , *rit , 20 , 20+ 10*i , EagleColor(0,255,0) , HALIGN_LEFT , VALIGN_CENTER);
               ++i;
            }
         }
         win->FlipDisplay();
         redraw = false;

         while (messages.size() > 36) {
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
         if (e.type == EAGLE_EVENT_TIMER) {
            tcnt++;
         }
         else {
            tcnt = 0;
         }
         if (e.type == EAGLE_EVENT_KEY_CHAR) {
            ccnt++;
         }
         else {
            ccnt = 0;
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
            message = StringPrintF("EAGLE_EVENT_TIMER %d" , tcnt);
            if (tcnt == 1) {
               messages.push_back(message);
            }
            else {
               messages[messages.size() - 1] = message;
            }
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
         else if (e.type == EAGLE_EVENT_KEY_CHAR) {
            message = StringPrintF("EAGLE_EVENT_KEY_CHAR '%s' %d" , keycode_to_name(e.keyboard.keycode) , ccnt);
            if (ccnt == 1) {
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
   sys->Rest(2.0);
   return 0;
}



