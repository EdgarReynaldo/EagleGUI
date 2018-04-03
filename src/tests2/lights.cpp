#include "allegro5/allegro.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"



void DrawLightB(int x , int y , int r , float fr , float fg , float fb);
void DrawLightB(int x , int y , int r , float fr , float fg , float fb) {
   al_draw_filled_circle(x,y,r,al_map_rgb_f(fr,fg,fb));
}
void DrawLightA(int x , int y , int r , float fr , float fg , float fb);
void DrawLightA(int x , int y , int r , float fr , float fg , float fb) {
   int rad = r;
   float l = 1. / ((float) r);
   while (rad > 0) {
      al_draw_filled_circle(x , y , rad , al_map_rgb_f(fr*l,fg*l,fb*l));
      rad -= 1;
   }
}
void DrawLightC(int x , int y , int r , float fr , float fg , float fb);
void DrawLightC(int x , int y , int r , float fr , float fg , float fb) {
   int rad = r;
   while (rad > 0) {
      float l = (r-rad)/(float)(r*20.0);///1. / ((float) r);
      al_draw_filled_circle(x , y , rad , al_map_rgb_f(fr*l,fg*l,fb*l));
      rad -= 20;
   }
}



///#include "allegro5/allegro_direct3d.h"



int main(int argc , char** argv) {

   (void)argc;
   (void)argv;


   if (!al_init()) {
      return 1;
   }
   if (!al_init_image_addon()) {return 2;}
   if (!al_init_font_addon()) {return 3;}
   if (!al_init_ttf_addon()) {return 4;}
   if (!al_init_primitives_addon()) {return 5;}

   al_install_keyboard();
   al_install_mouse();

   al_set_new_display_flags(ALLEGRO_FULLSCREEN | ALLEGRO_OPENGL);
///   al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_NOFRAME | ALLEGRO_OPENGL);
///   al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW | ALLEGRO_OPENGL);
///   ALLEGRO_DISPLAY* d = al_create_display(1800,1000);
   ALLEGRO_DISPLAY* d = al_create_display(1920,1080);

   ALLEGRO_TIMER* t = al_create_timer(1.0/60.0);

   ALLEGRO_EVENT_QUEUE* q = al_create_event_queue();

   al_register_event_source(q , al_get_timer_event_source(t));
   al_register_event_source(q , al_get_display_event_source(d));
   al_register_event_source(q , al_get_keyboard_event_source());
   al_register_event_source(q , al_get_mouse_event_source());

   ALLEGRO_BITMAP* lmap = al_create_bitmap(1920,1080);
   ALLEGRO_BITMAP* bg = al_load_bitmap("BG.png");

   if (!bg || !lmap) {return 7;}

   int mx = 0;
   int my = 0;

   
   
   ALLEGRO_BITMAP* lmap2 = al_create_bitmap(2048,2048);
   
   /// draw our light map
   al_set_target_bitmap(lmap2);
   al_clear_to_color(al_map_rgb(0,0,0));

   mx = 1024;
   my = 1024;
   
   // Additive blending
   al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE);
///         DrawLight(0 , 0 , 200);
///         DrawLight(mx , my , 200 , 1.0 , 1.0 , 1.0);
///   DrawLightA(mx - 100 , my - 100 , 400 , 1.0 , 0.0 , 0.0);
///   DrawLightA(mx + 100 , my - 100 , 400 , 0.0 , 1.0 , 0.0);
///   DrawLightA(mx       , my + 73  , 400 , 0.0 , 0.0 , 1.0);

   DrawLightC(mx - 100 , my - 100 , 400 , 1.0 , 0.0 , 1.0);
   DrawLightC(mx + 100 , my - 100 , 400 , 1.0 , 1.0 , 0.0);
   DrawLightC(mx       , my + 73  , 400 , 0.0 , 1.0 , 1.0);

   al_start_timer(t);
   
   bool run = true;
   bool draw = true;
   while (run) {
      if (draw) {
         /// draw our light map
         al_set_target_bitmap(lmap);
         al_clear_to_color(al_map_rgb(0,0,0));

         // Additive blending
         al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE);
         
         al_draw_bitmap(lmap2 , mx - 1024 , my - 1024 , 0);
         
///         DrawLight(0 , 0 , 200);
///         DrawLight(mx , my , 200 , 1.0 , 1.0 , 1.0);
/**

         DrawLightA(mx - 100 , my - 100 , 400 , 1.0 , 0.0 , 0.0);
         DrawLightA(mx + 100 , my - 100 , 400 , 0.0 , 1.0 , 0.0);
         DrawLightA(mx       , my + 73  , 400 , 0.0 , 0.0 , 1.0);

         DrawLightB(mx - 100 , my - 100 , 300 , 1.0 , 0.0 , 0.0);
         DrawLightB(mx + 100 , my - 100 , 300 , 0.0 , 1.0 , 0.0);
         DrawLightB(mx       , my + 73  , 300 , 0.0 , 0.0 , 1.0);

         DrawLightC(mx - 100 , my - 100 , 400 , 1.0 , 0.0 , 0.0);
         DrawLightC(mx + 100 , my - 100 , 400 , 0.0 , 1.0 , 0.0);
         DrawLightC(mx       , my + 73  , 400 , 0.0 , 0.0 , 1.0);
*/

         al_set_target_backbuffer(d);
         al_clear_to_color(al_map_rgb_f(1,1,1));

         // Default blending (premult alpha)
         al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
         al_draw_bitmap(bg , (1920 - al_get_bitmap_width(bg))/2 , (1080 - al_get_bitmap_height(bg))/2 , 0);

         // Mult source & dest blending
         al_set_blender(ALLEGRO_ADD, ALLEGRO_DEST_COLOR, ALLEGRO_ZERO);
         al_draw_bitmap(lmap , 0 , 0 , 0);
         al_flip_display();
         draw = false;
      }

      do {
         ALLEGRO_EVENT ev;
         al_wait_for_event(q , &ev);
         if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
            mx = ev.mouse.x;
            my = ev.mouse.y;
            draw = true;
         }
         if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            run = false;
         }
         if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            run = false;
         }
         if (ev.type == ALLEGRO_EVENT_TIMER) {
            draw = true;
         }
      } while (!al_is_event_queue_empty(q));

   }
   return 0;
}



