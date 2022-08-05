

#include "Eagle/backends/Allegro5Backend.hpp"
#include "Eagle.hpp"
#include "allegro5/allegro_color.h"


int main(int argc , char** argv) {
   (void)argc;
   (void)argv;
   
   Allegro5System* sys = GetAllegro5System();
   
   int ret = sys->Initialize(EAGLE_FULL_SETUP);
   (void)ret;
   
   EagleGraphicsContext* win = sys->CreateGraphicsContext("win" , 1024 , 768 , EAGLE_OPENGL | EAGLE_WINDOWED);
   EAGLE_ASSERT(win && win->Valid());
   
   win->Clear();
   win->FlipDisplay();
   
   bool quit = false;
   bool redraw = true;
   int mx = 0 , sx = 0;
   int my = 0 , sy = 0;
   bool drag = false , shiftdrag = false;
   int x1=0,x2=0,x3=0,x4=0;
   int y1=0,y2=0,y3=0,y4=0;
   
   sys->GetSystemTimer()->Start();
   
   while (!quit) {
      if (redraw) {
         Rectangle r1,r2;
         r1.SetCorners(x1,y1,x2,y2);
         r2.SetCorners(x3,y3,x4,y4);
         std::list<Rectangle> left = SubtractRectangle(r2,r1);
         
         win->Clear();
         win->DrawFilledRectangle(r1 , EagleColor(0,255,0));
         win->DrawFilledRectangle(r2 , EagleColor(255,0,0));
         float h = 30.0f;
         float s = 1.0f;
         float l = 0.5;
         for (std::list<Rectangle>::iterator it = left.begin() ; it != left.end() ; ++it) {
            float r,g,b;
            al_color_hsl_to_rgb(h,s,l,&r,&g,&b);
            win->DrawFilledRectangle(*it , EagleColor(r,g,b));
            
            h += 30.0f;
         }
         win->FlipDisplay();
         redraw = false;
      }
      do {
         EagleEvent e = sys->WaitForSystemEventAndUpdateState();
         if (e.type == EAGLE_EVENT_TIMER) {
            redraw = true;
         }
         if (e.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_ESCAPE) {
            quit = true;
         }
         if (e.type == EAGLE_EVENT_MOUSE_AXES) {
            mx = e.mouse.x;
            my = e.mouse.y;
            if (drag) {
               x1 = mx;
               y1 = my;
            }
            if (shiftdrag) {
               x3 = mx;
               y3 = my;
            }
         }
         if (e.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN) {
            sx = e.mouse.x;
            sy = e.mouse.y;
            if (input_key_held(EAGLE_KEY_ANY_SHIFT)) {
               shiftdrag = true;
               x3 = x4 = sx;
               y3 = y4 = sy;
            }
            else {
               drag = true;
               x1 = x2 = sx;
               y1 = y2 = sy;
            }
         }
         if (e.type == EAGLE_EVENT_MOUSE_BUTTON_UP) {
            drag = shiftdrag = false;
         }
      } while (!sys->UpToDate());
         
   }
   
   return 0;
}
