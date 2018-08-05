





#include "Eagle/backends/Allegro5Backend.hpp"


class EDISPLAY {
public :
   EagleGraphicsContext* window;
   EagleFont* font;
   EagleImage* image;
   EDISPLAY() :
      window(0),
      font(0),
      image(0)
   {}
};

EDISPLAY* CreateDisplay(int w , int h) {
   EDISPLAY* e = new EDISPLAY();
   e->window = GetAllegro5WindowManager()->CreateWindow(w,h,EAGLE_WINDOWED | EAGLE_OPENGL);
   e->font = new Allegro5Font(al_load_ttf_font("Verdana.ttf" , 20 , 0));
   e->image = new Allegro5Image(20,20);
   e->window->SetDrawingTarget(e->image);
   e->window->Clear(EagleColor(255,0,0));
   return e;
}

void DestroyDisplay(EDISPLAY* edisplay) {
   edisplay->window->FreeImage(edisplay->image);
   edisplay->window->FreeFont(edisplay->font);
   GetAllegro5WindowManager()->DestroyWindow(edisplay->window->GetEagleId());
   delete edisplay;
}

class PROGGIE {
public:
   Allegro5System* sys;
   Allegro5WindowManager* winman;
   EAGLE_ID idmain;
   
   std::map<EAGLE_ID , EDISPLAY*> dispmap;
   
   PROGGIE() :
      sys(0),
      winman(0),
      idmain(-1)
   {
      sys = GetAllegro5System();
      sys->Initialize(EAGLE_FULL_SETUP);
      sys->GetSystemTimer()->SetSecondsPerTick(1);
      
      winman = GetAllegro5WindowManager();
      
      idmain = CreateWindow(400,300);
      
      Run();
   }
   
   ~PROGGIE() {
      Eagle::EagleLibrary::ShutdownEagle();
   }
   
   EAGLE_ID CreateWindow(int w , int h) {
      EDISPLAY* e = CreateDisplay(w,h);
      EAGLE_ID id = e->window->GetEagleId();
      dispmap[id] = e;
      return id;
   }
   void FreeWindow(EagleGraphicsContext* win) {
      EAGLE_ID winid = win->GetEagleId();
      EDISPLAY* e = dispmap[winid];
      DestroyDisplay(e);
      dispmap.erase(dispmap.find(winid));
   }
   
   void Draw() {
      for (std::map<EAGLE_ID , EDISPLAY*>::iterator it = dispmap.begin() ; it != dispmap.end() ; ++it) {
         EAGLE_ID id = it->first;
         EagleGraphicsContext* active_window = winman->GetActiveWindow();
         
///         ALLEGRO_DISPLAY* d = dynamic_cast<Allegro5GraphicsContext*>(it->second->window)->AllegroDisplay();
         
         EAGLE_ID active_id = active_window?active_window->GetEagleId():-1;
         EagleGraphicsContext* win = it->second->window;
///         ALLEGRO_FONT* f = (dynamic_cast<Allegro5Font*>(it->second->font))->AllegroFont();
         ALLEGRO_FONT* f = al_load_ttf_font("Verdana.ttf" , 20 , 0);
         EagleImage* i = it->second->image;
         win->DrawToBackBuffer();
/**         
         ALLEGRO_TRANSFORM vt,pt;
         al_identity_transform(&vt);
         al_use_transform(&vt);
         al_identity_transform(&pt);
         al_orthographic_transform(&pt, 0, 0, -1.0, al_get_bitmap_width(al_get_target_bitmap()),
                                   al_get_bitmap_height(al_get_target_bitmap()), 1.0);

         al_use_projection_transform(&pt); 
*/        
         if (id == idmain) {
            win->Clear(EagleColor(0,0,0));
            al_draw_textf(f , al_map_rgb(255,255,255) , 10 , 10 , 0 , "Main Window ID %d" , id);
            al_draw_textf(f , al_map_rgb(255,255,255) , 10 , 30 , 0 , "Active Window ID %d" , active_id);
///            win->DrawTextString(f , StringPrintF("Main window ID %d" , id) , 10 , 10 , EagleColor(255,255,255));
///            win->DrawTextString(f , StringPrintF("Active window ID = %d" , active_id) , 10 , 30 , EagleColor(255,255,255));
         }
         else {
            if (id == active_id) {
               win->Clear(EagleColor(0,127,127));
            }
            else {
               win->Clear(EagleColor(0,0,127));
            }
            al_draw_textf(f , al_map_rgb(255,255,255) , 10 , 10 , 0 , "Window ID %d" , id);
///            win->DrawTextString(f , StringPrintF("ID %d" , id) , 10 , 10 , EagleColor(255,255,255));
         }
         ALLEGRO_GLYPH g;
         memset(&g , 0 , sizeof(ALLEGRO_GLYPH));
         bool exists = al_get_glyph(f , 0 , 'i' , &g);
         EAGLE_ASSERT(exists);
         ALLEGRO_BITMAP* b = g.bitmap;
         EAGLE_ASSERT(b);
         EAGLE_ASSERT(al_is_compatible_bitmap(b));
         al_destroy_font(f);
         win->FlipDisplay();
      }
   }
   
   int CheckEvents() {
      int ret = 0;
      do {
         EagleEvent ee = sys->GetSystemQueue()->WaitForEvent(0);
         if (ee.type == EAGLE_EVENT_TIMER) {
            ret = 1;
         }
         if (ee.type == EAGLE_EVENT_DISPLAY_SWITCH_IN || ee.type == EAGLE_EVENT_DISPLAY_SWITCH_OUT) {
            ret = 1;
         }
         if (ee.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            EagleGraphicsContext* w = ee.window;
            EAGLE_ID id = w->GetEagleId();
            if (id == idmain) {
               ret = -1;
            }
            else {
               FreeWindow(w);
               ret = 1;
            }
         }
         if (ee.type == EAGLE_EVENT_KEY_DOWN) {
            if (ee.keyboard.keycode == EAGLE_KEY_ESCAPE) {
               ret = -1;
            }
            if (ee.keyboard.keycode == EAGLE_KEY_N) {
               CreateWindow(300,200);
               ret = 1;
            }
         }
      } while (sys->GetSystemQueue()->HasEvent(0));
      
      return ret;
      
   }
   
   void Run() {
      sys->GetSystemTimer()->Start();
      
      int redraw = 1;
      
      while (redraw != -1) {
         if (redraw) {
            Draw();
            redraw = 0;
         }
         redraw = CheckEvents();
      }
   }
};


