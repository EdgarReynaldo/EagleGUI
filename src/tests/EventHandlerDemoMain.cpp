

#define CRTDBG_MAP_ALLOC
#include <cstdlib>
//#include <crtdbg.h>

#include "EventHandlerDemoMain.hpp"

#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"
#include "allegro5/allegro.h"



int EventHandlerDemoMain(int argc , char** argv) {
   
   (void)argc;
   (void)argv;

   Allegro5System* sys = new Allegro5System();
   if (sys->Initialize(EAGLE_FULL_SETUP) != EAGLE_FULL_SETUP) {
      EagleLog() << "Failed to fully setup. " << std::endl;
      delete sys;
      return 2;
   }
   
   EagleGraphicsContext* win = sys->CreateGraphicsContext(800,600,EAGLE_OPENGL);
   
   if (!win) {
      EagleLog() << "Failed to create window." << std::endl;
      delete sys;
      return 1;
   }

   win->Clear(EagleColor(0,0,0));
   win->FlipDisplay();

   EagleFont* font = win->LoadFont("Data/fonts/verdana.ttf" , 16 , 0);
   if (!font) {
      EagleLog() << "Failed to load font." << std::endl;
      delete sys;
      return -1;
   }

   sys->GetSystemTimer()->Start();

   std::list<EagleEvent> events;
   
   bool close = false;
   bool redraw = true;
   int count = 0;
   
   do {
      if (redraw) {
         win->Clear(EagleColor(0,0,0));
         int n = 0;
         for (std::list<EagleEvent>::iterator it = events.begin() ; it != events.end() ; ++it) {
            win->DrawTextString(font , StringPrintF("Event %s received.\n" , EagleEventName(it->type).c_str()) ,
                                 50 , 600 - 20*n - 20 , EagleColor(255,255,255));
            ++n;
         }
         win->FlipDisplay();
         redraw = false;
      }
      
      do {
         EagleEvent ev = sys->WaitForSystemEventAndUpdateState();
         if (ev.type != EAGLE_EVENT_TIMER) {
            EagleLog() << EagleEventName(ev.type) << std::endl;
            events.push_front(ev);
            if (events.size() > 30) {events.pop_back();}
         }
         if (ev.type == EAGLE_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode == EAGLE_KEY_ESCAPE) {
               close = true;
            }
         }
         if (ev.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            close = true;
         }
         if (ev.type == EAGLE_EVENT_TIMER) {
            redraw = true;
            ++count;
         }
      } while (!sys->UpToDate());
      
      
      
   } while (!close);

   delete sys;

   return 0;
}



