
#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"
#include "allegro5/allegro.h"
#include "allegro5/allegro_font.h"

#include <cstdio>

void* test_thread(ALLEGRO_THREAD* t , void* d) {
   (void)t;
   (void)d;
   int j = 0;
   al_rest(0.5);
   return (void*)(int)al_get_time();
}
int main(int argc , char** argv) {

   if (!al_init()) {
      return 1;
   }
   ALLEGRO_THREAD* thread = al_create_thread(test_thread , 0);
   al_start_thread(thread);
   al_rest(2.0);
   al_join_thread(thread , 0);
   al_destroy_thread(thread);

   return 0;

}



int main2(int argc , char** argv) {

   EagleSystem* sys = Eagle::EagleLibrary::System("Allegro5");
//**
   sys->Initialize(EAGLE_FULL_SETUP);

   EagleGraphicsContext* win = sys->CreateGraphicsContext(800 , 600 , EAGLE_WINDOWED);

   bool green = true;
   bool redraw = true;
   bool quit = false;

   sys->GetSystemTimer()->Create(1.5);
   sys->GetSystemTimer()->Start();

   while (!quit) {
      if (redraw) {
         if (green) {
            win->Clear(EagleColor(0,255,0));
         }
         else {
            win->Clear(EagleColor(0,0,0));
         }
         win->FlipDisplay();
         redraw = false;
      }
      do {
         EagleEvent ee = sys->GetSystemQueue()->WaitForEvent(0);
         EagleLog() << "Received EagleEvent type " << EagleEventName(ee.type) << std::endl;
         if (ee.type == EAGLE_EVENT_DISPLAY_CLOSE || (ee.type == EAGLE_EVENT_KEY_DOWN && ee.keyboard.keycode == EAGLE_KEY_ESCAPE)) {
            quit = true;
         }
         if (ee.type == EAGLE_EVENT_TIMER) {
            green = !green;
            redraw = true;
         }
      } while (sys->GetSystemQueue()->HasEvent(0));
   }

///   Eagle::EagleLibrary::ShutdownEagle();
//*/
   return 0;
}






int main3(int argc , char** argv) {

///   EagleSystem* sys = Eagle::EagleLibrary::System("Allegro5");
///   sys->Initialize(EAGLE_FULL_SETUP);

///   EagleGraphicsContext* win = sys->CreateGraphicsContext(800 , 600 , EAGLE_WINDOWED);

   if (!al_init()) {return 1;}

   al_install_keyboard();
   al_install_mouse();

   ALLEGRO_DISPLAY* d = al_create_display(800,600);
   if (!d) {return 2;}

   ALLEGRO_EVENT_QUEUE* q = al_create_event_queue();
   if (!q) {return 3;}

   ALLEGRO_TIMER* t = al_create_timer(1.5);

   ALLEGRO_FONT* f = al_create_builtin_font();
   if (!f) {return 15;}

   al_register_event_source(q , al_get_display_event_source(d));
   al_register_event_source(q , al_get_keyboard_event_source());
   al_register_event_source(q , al_get_mouse_event_source());
   al_register_event_source(q , al_get_timer_event_source(t));

   bool redraw = true;
   bool quit = false;
   bool green = true;

   al_start_timer(t);

   while (!quit) {

      if (redraw) {
         if (green) {
            al_clear_to_color(al_map_rgb(0,255,0));
         }
         else {
            al_clear_to_color(al_map_rgb(0,0,0));
         }
         al_flip_display();
         redraw = false;
      }

      do {
         ALLEGRO_EVENT ev;
         al_wait_for_event(q , &ev);

         printf("Received event %d\n" , ev.type);

         if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         if (ev.type == ALLEGRO_EVENT_KEY_CHAR && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            quit = true;
         }
         if (ev.type == ALLEGRO_EVENT_TIMER) {
            printf("TIMER event received\n");
            green = !green;
            redraw = true;
         }
      } while (!al_is_event_queue_empty(q));

   }



/*
///   win->Clear(EagleColor(0,255,0));
   al_clear_to_color(al_map_rgb(0,255,0));

///   win->FlipDisplay();
   al_flip_display();


///   sys->Rest(3.0);
   al_rest(1.0);
*/
   return 0;
}

