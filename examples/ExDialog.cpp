



#include "Eagle/backends/Allegro5Backend.hpp"
#include "Eagle.hpp"

#include <iostream>


void LogAllegro(const char* str) {EagleError() << str << std::endl;}

int main(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   SendOutputToFile("ExDialog.log" , "ExDialog\n" , false);
   
   al_register_trace_handler(LogAllegro);
   
   Allegro5System* sys = GetAllegro5System();
   
   int ret = sys->Initialize(EAGLE_FULL_SETUP);
   if (EAGLE_FULL_SETUP != ret) {
      EagleWarn() << "Failed to initialize some subsystem. Continuing anyway." << std::endl;
   }
   
   EagleGraphicsContext* win = sys->CreateGraphicsContext("Main window" , 400 , 300 , EAGLE_WINDOWED | EAGLE_OPENGL , 100 , 75);
   
   WidgetHandler gui(win , "WidgetHandler" , "GUI");
   gui.SetWidgetArea(Rectangle(0,0,400,300) , false);
   gui.SetupBuffer(400,300,win);
   gui.SetBackgroundColor(EagleColor(255,255,255));
   
   Allegro5Font* alex = dynamic_cast<Allegro5Font*>(win->GetFont("Data/Fonts/AlexBrush.ttf" , -40));
   EAGLE_ASSERT(alex && alex->Valid());
   
   GuiButton guibtn;
   WidgetColorset wc = *(guibtn.GetWidgetColorset().get());
   wc[TXTCOL] = EagleColor(0,0,0);
   wc[BGCOL] = EagleColor(255,0,0);
   guibtn.SetWidgetColorset(wc);
   guibtn.SetButtonType(RECTANGLE_BTN , SPRING_BTN , BUTTON_CLASS_HOVER);
   guibtn.SetFont(alex);
   al_draw_text(alex->AllegroFont() , al_map_rgb(0,0,0) , -40 , -40 , 0 , "Press ME!");
   guibtn.SetLabel("Press ME!");

   
   
   gui << &guibtn;
   guibtn.SetWidgetArea(Rectangle(100 , 75 , 200 , 150) , false);
   
   
   
   
   
   
   PopupText ptext(-1 , -1 , 0 , "Hello\npopup" , "Data/Fonts/AlexBrush.ttf" , -80);
   ptext.SetBackgroundColor(EagleColor(255,255,0));
   ptext.Hide();
   
   (*ptext.GetWidgetColorset().get())[TXTCOL] = EagleColor(0,0,0);
   
   sys->GetSystemQueue()->ListenTo(&guibtn);
   
   sys->GetSystemTimer()->Start();
   
//   ptext.Show();
   
   EagleInfo() << gui << std::endl;

   bool quit = false;
   bool redraw = false;
   
   while (!quit) {
      if (redraw) {
         ptext.Display();

         win->DrawToBackBuffer();
         win->Clear();
         gui.Display(win,0,0);
//         win->DrawTextString(win->GetFont("Data/Fonts/AlexBrush.ttf" , -20) , "Alex" , 10 , 10 , EagleColor(0,0,255));
//         win->DrawTextString(win->GetFont("Data/Fonts/Verdana.ttf" , -20) , "Verdana" , 10 , 30 , EagleColor(0,255,255));
         win->FlipDisplay();

         redraw = false;
      }
      do {
         EagleEvent e = sys->WaitForSystemEventAndUpdateState();
         gui.HandleEvent(e);
         while (gui.HasMessages()) {
            WidgetMsg m = gui.TakeNextMessage();
            if (m.from == &guibtn) {
               ptext.Show();
            }
         }
         ptext.HandleEvent(e);
         if (e.type == EAGLE_EVENT_TIMER) {
            gui.Update(e.timer.eagle_timer_source->SPT());
            redraw = true;
//            EagleGraphicsContext* w = ptext.OurWindow();
//            ptext.OurWindow()->SetWindowPosition(win->XPos() + mouse_x - ptext.OuterArea().W()/2 , win->YPos() + mouse_y - ptext.OuterArea().H()/2);
         }
         if (e.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_ESCAPE) {
            quit = true;
         }
         if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_S) {
            ptext.Show();
         }
         if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_H) {
            ptext.Hide();
         }
         if (e.type == EAGLE_EVENT_DISPLAY_SWITCH_OUT) {
            if (e.source == &ptext) {
               ptext.Hide();
            }
         }
      } while (!sys->UpToDate());
         
   }



///   ptext.Hide();
   
   return 0;
}
