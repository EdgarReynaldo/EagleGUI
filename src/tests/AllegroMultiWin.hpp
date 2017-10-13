


#ifndef AllegroMultiWin_HPP
#define AllegroMultiWin_HPP

#include "allegro5/allegro.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"



struct DISPLAY {
   ALLEGRO_DISPLAY* d;
   ALLEGRO_FONT* f;
};

DISPLAY* create_display(int w , int h) {
   DISPLAY* d = (DISPLAY*)malloc(sizeof(DISPLAY));
   al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_OPENGL);
   d->d = al_create_display(w,h);
   al_set_target_bitmap(al_get_backbuffer(d->d));
   d->f = al_load_ttf_font("Verdana.ttf" , 20 , 0);
   assert(d->d && d->f);
   return d;
}

class PROG {
public :
   int dispid;
   std::map<int , DISPLAY*> dispmap;
   std::map<DISPLAY* , int> rdispmap;

   ALLEGRO_TIMER* timer;
   ALLEGRO_EVENT_QUEUE* queue;
   ALLEGRO_DISPLAY* active_display;
   
   PROG() :
      dispid(0),
      dispmap(),
      rdispmap(),
      timer(0),
      queue(0),
      active_display(0)
   {
      int ret = Create();
      if (ret != 0) {
         exit(ret);
      }
      
      Run();
      Close();
   }
   
   int Create() {
      if (!al_init()) {return 1;}
      if (!al_init_font_addon()) {return 2;}
      if (!al_init_ttf_addon()) {return 3;}
      
      if (!al_install_keyboard()) {return 4;}
      
      timer = al_create_timer(1);
      queue = al_create_event_queue();
      
      al_register_event_source(queue , al_get_keyboard_event_source());
      al_register_event_source(queue , al_get_timer_event_source(timer));
      
      int idmain = CreateWindow(400,300);
      (void)idmain;
      
      return 0;
   }
   
   int CreateWindow(int w, int h) {
      DISPLAY* d = create_display(w,h);
      al_register_event_source(queue , al_get_display_event_source(d->d));
      dispmap[dispid] = d;
      rdispmap[d] = dispid;
      return ++dispid;
   }
   
   std::map<DISPLAY* , int>::iterator FindDisplay(ALLEGRO_DISPLAY* disp) {
      std::map<DISPLAY* , int>::iterator it = rdispmap.begin();
      for (; it != rdispmap.end() ; ++it) {
         if (it->first->d == disp) {
            break;
         }
      }
      return it;
   }
   void FreeWindow(ALLEGRO_DISPLAY* disp) {
      std::map<DISPLAY* , int>::iterator it = FindDisplay(disp);
      if (it != rdispmap.end()) {
         int id = it->second;
         dispmap.erase(dispmap.find(id));
         al_unregister_event_source(queue , al_get_display_event_source(it->first->d));
         al_destroy_font(it->first->f);
         al_destroy_display(it->first->d);
         rdispmap.erase(it);
      }
   }
   void FreeWindow(int windowid) {
      std::map<int , DISPLAY*>::iterator it = dispmap.find(windowid);
      if (it != dispmap.end()) {
         al_unregister_event_source(queue , al_get_display_event_source(it->second->d));
         al_destroy_font(it->second->f);
         al_destroy_display(it->second->d);
         rdispmap.erase(rdispmap.find(it->second));
         dispmap.erase(dispmap.find(windowid));
      }
   }
   
   void Close() {
      std::map<int , DISPLAY*> dmap = dispmap;
      std::map<int , DISPLAY*>::iterator it = dmap.begin();
      while (it != dmap.end()) {
         FreeWindow(it->first);
         ++it;
      }
   }
   
   void Run() {
      bool quit = false;
      int redraw = 1;
      
      while (!quit) {
         if (redraw) {
            Draw();
            redraw = 0;
         }
         redraw = CheckEvents();
         if (redraw == -1) {
            break;
         }
      }
   }
   
   void Draw() {
      for (std::map<int , DISPLAY*>::iterator it = dispmap.begin() ; it != dispmap.end() ; ++it) {
         ALLEGRO_DISPLAY* d = it->second->d;
         ALLEGRO_FONT* f = it->second->f;
         int id = it->first;
         al_set_target_backbuffer(d);
         if (id == 0) {
            /// Main window
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_text(f , al_map_rgb(255,255,255) , 10 , 10 , 0 , "Main window ID 0");
            al_draw_textf(f , al_map_rgb(255,255,255) , 10 , 30 , 0 , "Active window = %p" , active_display);
         }
         else {
            if (d == active_display) {
               al_clear_to_color(al_map_rgb(0,127,127));
            }
            else {
               al_clear_to_color(al_map_rgb(0,0,127));
            }
            al_draw_textf(f , al_map_rgb(255,255,255) , 10 , 10 , 0 , "Window ID : %d" , id);
         }
         al_flip_display();
         al_set_target_bitmap(al_get_backbuffer(d));
      }
   }
   
   int CheckEvents() {
      int ret = 0;
      do {
         ALLEGRO_EVENT ev;
         al_wait_for_event(queue , &ev);
         
         if (ev.type == ALLEGRO_EVENT_DISPLAY_SWITCH_IN) {
            active_display = ev.display.source;
            ret = 1;
         }
         if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            if (0 == FindDisplay(ev.display.source)->second) {
               return -1;
            }
            else {
               FreeWindow(ev.display.source);
            }
         }
         if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            return -1;
         }
         if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_N) {
            CreateWindow(300,200);
            ret = 1;
         }
         if (ev.type == ALLEGRO_EVENT_TIMER) {
            ret = 1;
         }
      } while (!al_is_event_queue_empty(queue));
      return ret;
   }
};


#endif // AllegroMultiWin_HPP
