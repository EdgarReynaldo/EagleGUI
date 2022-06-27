



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
   
   EagleGraphicsContext* win = sys->CreateGraphicsContext("Main window" , 400 , 300 , EAGLE_NOFRAME | EAGLE_WINDOWED | EAGLE_OPENGL);
   
   WidgetHandler gui(win , "WidgetHandler" , "GUI");
   gui.SetWidgetArea(Rectangle(0,0,400,300) , false);
   gui.SetupBuffer(400,300,win);
   
   SHAREDOBJECT<WidgetColorset> guicset = gui.GetWidgetColorset();
   
//   (*cset.get())[TXTCOL] = EagleColor(0,0,0);
   (*guicset.get())[HLCOL] = EagleColor(0,255,0,127);
   (*guicset.get())[SDCOL] = EagleColor(0,0,127,127);
   (*guicset.get())[BGCOL] = EagleColor(0,0,127,255);
   (*guicset.get())[MGCOL] = EagleColor(0,127,255);
   (*guicset.get())[FGCOL] = EagleColor(0,255,255);
   (*guicset.get())[HVRCOL] = EagleColor(0,255,0);
   (*guicset.get())[TXTCOL] = EagleColor(255,255,255);

   gui.SetWidgetColorset(guicset);
   
   EagleFont* alex = win->GetFont("Data/Fonts/AlexBrush.ttf" , -20);
   EAGLE_ASSERT(alex && alex->Valid());
   
   GuiButton guibtn;
   guibtn.SetButtonType(RECTANGLE_BTN , SPRING_BTN , BUTTON_CLASS_PLAIN);
   guibtn.SetFont(alex);
   guibtn.SetLabel("Press ME!");
   guibtn.SetWidgetArea(Rectangle(100 , 75 , 200 , 150) , false);
   guibtn.SetWidgetColorset(guicset);

   
   
   gui << &guibtn;
   
   
   
   
   
   
   
   PopupText ptext(-1 , -1 , 0 , "Hello popup" , "Data/Fonts/Verdana.ttf" , -20);
   
   sys->GetSystemQueue()->ListenTo(&guibtn);
   
   sys->GetSystemTimer()->Start();
   
//   ptext.Show();
   

   bool quit = false;
   bool redraw = false;
   
   while (!quit) {
      if (redraw) {
         ptext.Display();
      }
      if (0) {
         win->DrawToBackBuffer();
         win->Clear();
         win->DrawTextString(win->DefaultFont() , "Hello dialog" , 10 , 10 , EagleColor(255,255,255) , HALIGN_LEFT , VALIGN_TOP);
//         gui.Display(win , 0 , 0);
         win->FlipDisplay();

//         ptext.Display();

         redraw = false;
      }
      do {
         EagleEvent e = sys->WaitForSystemEventAndUpdateState();
//         gui.HandleEvent(e);
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
      } while (!sys->UpToDate());
         
   }



///   ptext.Hide();
   
   return 0;
}
