

#define ALLEGRO_STATICLINK

#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"



int main(int argc , char** argv) {
   
   /// Some global variables
   int sw = 800;
   int sh = 600;
   
   /// Our main system
   Allegro5System* a5sys = GetAllegro5System();
   
   EAGLE_ASSERT(a5sys);
   
   /// Register eagle shutdown with atexit
   atexit(Eagle::EagleLibrary::ShutdownEagle);
   
   /// Setup everything we can
   int ret = a5sys->Initialize(EAGLE_FULL_SETUP);
   if (ret != EAGLE_FULL_SETUP) {
      EagleInfo() << "Failed to initialize some subsystems. Attempting to continue." << std::endl;
   }
   
   /// Ask the system to create a window
   EagleGraphicsContext* win = a5sys->CreateGraphicsContext("ExMenu" , sw , sh , EAGLE_WINDOWED);
   
   EAGLE_ASSERT(win && win->Valid());
   
   win->Clear();
   win->FlipDisplay();
   
   EagleFont* efont = win->LoadFont("Verdana.ttf" , -40);
   EAGLE_ASSERT(efont && efont->Valid());
   
   
   WidgetHandler gui(win , "GUI" , "MenuGUI");
   if (!gui.SetupBuffer(sw,sh,win)) {
      EagleError() << "Could not setup gui buffer." << std::endl;
      return 1;
   }
   gui.SetWidgetArea(Rectangle(0,0,sw,sh) , false);
   
   
   RelativeLayout rlayout;
   rlayout.Resize(1);
   
   gui.SetRootLayout(&rlayout);
   
   const int NUMBUTTONS = 5;

   ClassicMenuLayout menu("OurMenu");
   
   rlayout.PlaceWidget(&menu , 0 , LayoutRectangle(0.3 , 0.1 , 0.4 , 0.8));

   GuiButton* btns[NUMBUTTONS] = {0};
   
   menu.Resize(NUMBUTTONS);
   menu.SetGlobalPadding(20,20);
   for (unsigned int i = 0 ; i < NUMBUTTONS ; ++i) {
      GuiButton* btn = new GuiButton("GuiButton" , StringPrintF("Button #%d" , i));
      btn->SetButtonType(ROUNDED_BTN , SPRING_BTN , BUTTON_CLASS_HOVER);
      btn->SetInputGroup(input_key_press(EAGLE_KEY_1 + i));
      btn->SetFont(efont);
      btn->SetLabel(StringPrintF("Button #%d" , i));
      menu.PlaceWidget(btn , i);
      btns[i] = btn;
   }
   
   bool quit = false;
   bool redraw = true;
   
   /// Start the system timer
   a5sys->GetSystemTimer()->Start();

   while (!quit) {
         
      if (redraw) {
         win->Clear();
         gui.Display(win , 0 , 0);
         win->FlipDisplay();
         redraw = false;
      }
      do {
         EagleEvent e = a5sys->WaitForSystemEventAndUpdateState();
         if (e.type == EAGLE_EVENT_DISPLAY_CLOSE) {quit = true;}
         else if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_ESCAPE) {quit = true;}
         else if (e.type == EAGLE_EVENT_TIMER) {
            redraw = true;
         }
         else {
            gui.HandleEvent(e);
            while (gui.HasMessages()) {
               WidgetMsg msg = gui.TakeNextMessage();
               EagleInfo() << msg << std::endl;
            }
         }
      } while (!a5sys->UpToDate());
   
   }
   win->FreeFont(efont);
   gui.FreeImageBuffers();
   gui.ClearLayout();
   rlayout.ClearWidgets();
   menu.ClearWidgets();
   for (unsigned int i = 0 ; i < NUMBUTTONS ; ++i) {
      delete btns[i];
      btns[i] = 0;
   }
//   a5sys->Shutdown();
   return 0;
}
