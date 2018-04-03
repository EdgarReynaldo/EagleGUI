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
      float l = (r-rad)/(float)(r*10.0);///1. / ((float) r);
      al_draw_filled_circle(x , y , rad , al_map_rgb_f(fr*l,fg*l,fb*l));
      rad -= 10;
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
///   al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW | ALLEGRO_OPENGL);
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
   DrawLightA(mx - 100 , my - 100 , 400 , 1.0 , 0.0 , 0.0);
   DrawLightA(mx + 100 , my - 100 , 400 , 0.0 , 1.0 , 0.0);
   DrawLightA(mx       , my + 73  , 400 , 0.0 , 0.0 , 1.0);

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


/**
#include "allegro5/allegro.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"



void DrawLight2(int x , int y , int r);
void DrawLight2(int x , int y , int r) {
   int rad = r;
   while (rad > 0) {
      int l = (r - rad)*255/r;
      al_draw_filled_circle(x , y , rad , al_map_rgba(0,0,0,l));
      rad -= 15;
   }
}

int mainx(int argc , char** argv) {
   
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
   
   al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_OPENGL);
   ALLEGRO_DISPLAY* d = al_create_display(800,600);
   
   ALLEGRO_TIMER* t = al_create_timer(1.0/60.0);
   
   ALLEGRO_EVENT_QUEUE* q = al_create_event_queue();
   
   al_register_event_source(q , al_get_timer_event_source(t));
   al_register_event_source(q , al_get_display_event_source(d));
   al_register_event_source(q , al_get_keyboard_event_source());
   al_register_event_source(q , al_get_mouse_event_source());
   
   ALLEGRO_BITMAP* lmap = al_create_bitmap(800,600);
   ALLEGRO_BITMAP* bg = al_load_bitmap("BG.png");
   
   if (!bg || !lmap) {return 7;}
   
   int mx = 0;
   int my = 0;
   
   bool run = true;
   bool draw = true;
   while (run) {
      if (draw) {
         /// draw our light map
         al_set_target_bitmap(lmap);
         
         al_clear_to_color(al_map_rgba(0,0,0,0));
         
         al_set_separate_blender(ALLEGRO_ADD , ALLEGRO_ZERO , ALLEGRO_ZERO , ALLEGRO_ADD , ALLEGRO_ALPHA , ALLEGRO_ALPHA);
         DrawLight2(mx , my , 300);
         DrawLight2(0 , 0 , 300);
            
         al_set_target_backbuffer(d);
         al_clear_to_color(al_map_rgb(0,0,0));
         al_set_blender(ALLEGRO_ADD , ALLEGRO_ONE , ALLEGRO_ZERO);
         al_draw_bitmap(bg , 0 , 0 , 0);
         
         al_set_blender(ALLEGRO_ADD , ALLEGRO_ZERO , ALLEGRO_ALPHA);
///         al_set_blender(ALLEGRO_ADD , ALLEGRO_ZERO , ALLEGRO_INVERSE_ALPHA);
         
///         al_set_separate_blender()
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




#include "Eagle/backends/Allegro5backend.hpp"



int main3(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   Allegro5System* sys = GetAllegro5System();
   
   sys->Initialize(EAGLE_FULL_SETUP);
   
   EagleGraphicsContext* win = sys->GetWindowManager()->CreateWindow("GUI test main" , 800 , 600 , EAGLE_OPENGL | EAGLE_WINDOWED);
   
   EAGLE_ASSERT(win && win->Valid());
   
   EagleGuiScript egs;
   
   if (!egs.LoadScript("eagle.gui")) {
///      return 1;
   }
   
///   WidgetHandler* gui = dynamic_cast<WidgetHandler*>(egs.GetWH("gui"));

   WidgetHandler* gui = new WidgetHandler(win , "gui");
   gui->SetupBuffer(win->Width() , win->Height() , win);
   






   return 0;
}


int main2(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   Allegro5System* sys = GetAllegro5System();
   
   sys->Initialize(EAGLE_FULL_SETUP);
   
   EagleGraphicsContext* win = sys->GetWindowManager()->CreateWindow("GUI test main" , 800 , 600 , EAGLE_OPENGL | EAGLE_WINDOWED);
   
   EAGLE_ASSERT(win && win->Valid());
   
   EagleGuiScript egs;
   
   if (!egs.LoadScript("eagle.gui")) {
///      return 1;
   }
   
///   WidgetHandler* gui = dynamic_cast<WidgetHandler*>(egs.GetWH("gui"));

   WidgetHandler* gui = new WidgetHandler(win , "gui");
   gui->SetupBuffer(win->Width() , win->Height() , win);
   
   GuiButton* gbtn1 = new GuiButton("gbtn1");
   GuiButton* gbtn2 = new GuiButton("gbtn2");
   GuiButton* gbtn3 = new GuiButton("gbtn3");
   GuiButton* gbtn4 = new GuiButton("gbtn4");
   BasicButton* bbtn1 = new BasicButton("bbtn1");
   BasicButton* bbtn2 = new BasicButton("bbtn2");
   BasicButton* bbtn3 = new BasicButton("bbtn3");
   BasicButton* bbtn4 = new BasicButton("bbtn4");
   
   gbtn1->SetButtonType(RECTANGLE_BTN , SPRING_BTN , BUTTON_CLASS_PLAIN);
   gbtn2->SetButtonType(ROUNDED_BTN , SPRING_BTN , BUTTON_CLASS_PLAIN);
   gbtn3->SetButtonType(ELLIPSE_BTN , SPRING_BTN , BUTTON_CLASS_PLAIN);
   gbtn4->SetButtonType(CIRCLE_BTN , SPRING_BTN , BUTTON_CLASS_PLAIN);
   gbtn1->SetLabel("GBTN&1");
   gbtn2->SetLabel("GBTN&2");
   gbtn3->SetLabel("GBTN&3");
   gbtn4->SetLabel("GBTN&4");
   gbtn1->SetFont(win->DefaultFont());
   gbtn2->SetFont(win->DefaultFont());
   gbtn3->SetFont(win->DefaultFont());
   gbtn4->SetFont(win->DefaultFont());
//   bbtn1->SetButtonType(RECTANGLE_BTN , SPRING_BTN , BUTTON_CLASS_PLAIN);
//   bbtn2->SetButtonType(ROUNDED_BTN , SPRING_BTN , BUTTON_CLASS_PLAIN);
//   bbtn3->SetButtonType(ELLIPSE_BTN , SPRING_BTN , BUTTON_CLASS_PLAIN);
//   bbtn4->SetButtonType(CIRCLE_BTN , SPRING_BTN , BUTTON_CLASS_PLAIN);

   Input key1 = input_key_press(EAGLE_KEY_1);
   Input key2 = input_key_press(EAGLE_KEY_2);
   Input key3 = input_key_press(EAGLE_KEY_3);
   Input key4 = input_key_press(EAGLE_KEY_4);
   Input key5 = input_key_press(EAGLE_KEY_7);
   Input key6 = input_key_press(EAGLE_KEY_8);
   Input key7 = input_key_press(EAGLE_KEY_9);
   Input key8 = input_key_press(EAGLE_KEY_0);

   gbtn1->SetInputGroup(key1);
   gbtn2->SetInputGroup(key2);
   gbtn3->SetInputGroup(key3);
   gbtn4->SetInputGroup(key4);
   bbtn1->SetInputGroup(key5);
   bbtn2->SetInputGroup(key6);
   bbtn3->SetInputGroup(key7);
   bbtn4->SetInputGroup(key8);
   
   bbtn1->SetButtonType(TOGGLE_BTN);
   bbtn2->SetButtonType(TOGGLE_BTN);
   bbtn3->SetButtonType(TOGGLE_BTN);
   bbtn4->SetButtonType(TOGGLE_BTN);
   
   /// These two statements do nothing - they are a trap
///   btn1->InputKey() = g1;
///   btn2->InputKey() = g2;
   
   GridLayout* gl = new GridLayout(4,2,"gl");
   gl->PlaceWidget(gbtn1 , 0);
   gl->PlaceWidget(gbtn2 , 1);
   gl->PlaceWidget(gbtn3 , 2);
   gl->PlaceWidget(gbtn4 , 3);
   gl->PlaceWidget(bbtn1 , 4);
   gl->PlaceWidget(bbtn2 , 5);
   gl->PlaceWidget(bbtn3 , 6);
   gl->PlaceWidget(bbtn4 , 7);
   
   RelativeLayout* rl = new RelativeLayout("rl");

   gui->SetRootLayout(rl);
   gui->SetBackgroundColor(EagleColor(0,0,0,0));

   rl->AddWidget(gl , LayoutRectangle(0.1 , 0.4 , 0.8 , 0.2));

   gui->SetWidgetArea(0 , 0 , win->Width() , win->Height());
   
   EagleLog() << *gui << std::endl;
   
///   WidgetBase::ClipWidgets(true);
///   WidgetBase::DisplayWidgetArea(true);
   if (!gui) {
      EAGLE_ASSERT(gui);
      return 1;
   }
   bool quit = false;
   bool redraw = true;
   
   sys->GetSystemTimer()->Start();
   
   while (!quit) {
      
      if (redraw) {
         gui->SetFullRedraw();
         
         win->DrawToBackBuffer();
         win->Clear(EagleColor(0 , 0 , 96));

         gui->Display(win , 0 , 0);

         gbtn1->Display(win , 0 , 0);
         gbtn2->Display(win , 0 , 0);
         gbtn3->Display(win , 0 , 0);
         gbtn4->Display(win , 0 , 0);
         bbtn1->Display(win , 0 , 0);
         bbtn2->Display(win , 0 , 0);
         bbtn3->Display(win , 0 , 0);
         bbtn4->Display(win , 0 , 0);

         
         win->DrawTextString(win->DefaultFont() , StringPrintF("GBTN1 %s" , gbtn1->Up()?"UP":"DOWN") , 10 , 10 , EagleColor(255,255,255));
         win->DrawTextString(win->DefaultFont() , StringPrintF("GBTN2 %s" , gbtn2->Up()?"UP":"DOWN") , 10 , 30 , EagleColor(255,255,255));
         win->DrawTextString(win->DefaultFont() , StringPrintF("GBTN3 %s" , gbtn3->Up()?"UP":"DOWN") , 10 , 50 , EagleColor(255,255,255));
         win->DrawTextString(win->DefaultFont() , StringPrintF("GBTN4 %s" , gbtn4->Up()?"UP":"DOWN") , 10 , 70 , EagleColor(255,255,255));
         win->DrawTextString(win->DefaultFont() , StringPrintF("BBTN1 %s" , bbtn1->Up()?"UP":"DOWN") , 10 , 90 , EagleColor(255,255,255));
         win->DrawTextString(win->DefaultFont() , StringPrintF("BBTN2 %s" , bbtn2->Up()?"UP":"DOWN") , 10 , 110 , EagleColor(255,255,255));
         win->DrawTextString(win->DefaultFont() , StringPrintF("BBTN3 %s" , bbtn3->Up()?"UP":"DOWN") , 10 , 130 , EagleColor(255,255,255));
         win->DrawTextString(win->DefaultFont() , StringPrintF("BBTN4 %s" , bbtn4->Up()?"UP":"DOWN") , 10 , 150 , EagleColor(255,255,255));
         
         win->FlipDisplay();
         redraw = false;
      }
      do {
         EagleEvent e;
         e = sys->GetSystemQueue()->WaitForEvent(0);
         
         sys->GetSystemInput()->HandleInputEvent(e);
         
         bool input_used = gui->HandleEvent(e) & DIALOG_INPUT_USED;
         while (gui->HasMessages()) {
            WidgetMsg msg = gui->TakeNextMessage();
            EagleLog() << msg << std::endl;
         }
         if (input_used) {
            continue;
         }
         if (e.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_ESCAPE) {
            quit = true;
         }
         if (e.type == EAGLE_EVENT_TIMER) {
            gui->Update(e.timer.eagle_timer_source->SecondsPerTick());
            EagleInfo() << "Timer tick." << std::endl;
            redraw = true;
         }
      } while (sys->GetSystemQueue()->HasEvent(0));
   }
   
   
   
   return 0;
}

*/


