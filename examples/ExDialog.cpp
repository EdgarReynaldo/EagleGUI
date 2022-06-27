



#include "Eagle/backends/Allegro5Backend.hpp"
#include "Eagle.hpp"

#include <iostream>


void LogAllegro(const char* str) {EagleError() << str << std::endl;}

int main(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   al_register_trace_handler(LogAllegro);
   
   Allegro5System* sys = GetAllegro5System();
   
   int ret = sys->Initialize(EAGLE_FULL_SETUP);
   if (EAGLE_FULL_SETUP != ret) {
      EagleWarn() << "Failed to initialize some subsystem. Continuing anyway." << std::endl;
   }
   
   EagleGraphicsContext* win = sys->CreateGraphicsContext("Main window" , 400 , 300 , EAGLE_NOFRAME | EAGLE_WINDOWED | EAGLE_OPENGL);
   
   WidgetHandler gui(win , "WidgetHandler" , "GUI");
   gui.SetupBuffer(400,300,win);
   
   EagleFont* alex = win->GetFont("Data/Fonts/AlexBrush.ttf" , -40);
   EAGLE_ASSERT(alex && alex->Valid());
   
   GuiButton guibtn;
   guibtn.SetFont(alex);
   guibtn.SetLabel("Press ME!");
   guibtn.SetButtonType(ROUNDED_BTN , SPRING_BTN , BUTTON_CLASS_HOVER);
   
   guibtn.SetWidgetArea(WIDGETAREA(Rectangle(100 , 75 , 200 , 150) , BOXAREA(10) , BOXAREA(10) , BOXAREA(10)) , false);
   
   gui << &guibtn;
   
   
   
   
   
   
   
   PopupText ptext(-1 , -1 , 0 , "Hello popup" , "Data/Fonts/Verdana.ttf" , -20);
   
   SHAREDOBJECT<WidgetColorset> cset = ptext.GetWidgetColorset();
   
   (*cset.get())[TXTCOL] = EagleColor(0,0,0);
   
   ptext.SetWidgetColorset(cset);
   
   sys->GetSystemQueue()->ListenTo(&ptext);
   
   sys->GetSystemTimer()->Start();
   
   ptext.Show();
   

   bool quit = false;
   bool redraw = false;
   
   while (!quit) {
      if (redraw) {
         win->DrawToBackBuffer();
         win->Clear();
         gui.Display(win , 0 , 0);
         win->FlipDisplay();

         ptext.Display();

         redraw = false;
      }
      do {
         EagleEvent e = sys->WaitForSystemEventAndUpdateState();
         if (e.type == EAGLE_EVENT_TIMER) {
            redraw = true;
            EagleGraphicsContext* w = ptext.OurWindow();
            ptext.OurWindow()->SetWindowPosition(win->XPos() + mouse_x - ptext.OuterArea().W()/2 , win->YPos() + mouse_y - ptext.OuterArea().H()/2);
         }
         if (e.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_ESCAPE) {
            quit = true;
         }
      } while (!sys->UpToDate());
         
   }



   ptext.Hide();
   
   return 0;
}
