


#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"
#include "allegro5/allegro.h"

#include "Eagle/Gui/SimpleMenu.hpp"



void PipeToEagleLog(const char* str) {
   EagleInfo() << str << std::endl;
}



int main(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   al_set_config_value(al_get_system_config() , "trace" , "level" , "error");
   
   al_register_trace_handler(PipeToEagleLog);
   
   SendOutputToFile("ExMenu.log" , "" , false);
   
   Allegro5System* sys = GetAllegro5System();
   
   if (sys->Initialize(EAGLE_FULL_SETUP) != EAGLE_FULL_SETUP) {
      EagleWarn() << "Some subsystems not initialized. Proceeding" << std::endl;
   }
   
   int sw = 800;
   int sh = 600;
   
   EagleGraphicsContext* win = sys->CreateGraphicsContext("Main Window" , sw , sh , EAGLE_OPENGL | EAGLE_WINDOWED | EAGLE_RESIZABLE);
   
   EAGLE_ASSERT(win && win->Valid());
   
   EagleFont* font = win->GetFont("Verdana.ttf" , -20);
   
   EAGLE_ASSERT(font && font->Valid());
   
   
   
   
   WidgetHandler gui(win , "GUI" , "Example GUI");
   
   gui.SetupBuffer(sw , sh , win);
   
   gui.SetWidgetArea(Rectangle(0 , 0 , sw , sh));
   

   SIMPLE_MENU_ITEM fmenu[] = {{SPRING_BTN , false , (EagleImage*)0 , (EagleImage*)0 , font , "&Load"  , input_key_press(EAGLE_KEY_L) , "L"},
                               {SPRING_BTN , false , (EagleImage*)0 , (EagleImage*)0 , font , "&Save"  , input_key_press(EAGLE_KEY_S) , "S"},
                               {SPRING_BTN , false , (EagleImage*)0 , (EagleImage*)0 , font , "&Print" , input_key_press(EAGLE_KEY_P) , "P"},
                               {SPRING_BTN , false , (EagleImage*)0 , (EagleImage*)0 , font , "&Quit"  , input_key_held(EAGLE_KEY_ANY_CTRL) || input_key_press(EAGLE_KEY_Q) , "CTRL-Q"}};

   SIMPLE_MENU_ITEM omenu[] = {{TOGGLE_BTN , true , checkbox_up , checkbox_down , font , "Option &1" , input_key_press(EAGLE_KEY_1) , "1"},
                               {TOGGLE_BTN , true , checkbox_up , checkbox_down , font , "Option &2" , input_key_press(EAGLE_KEY_2) , "2"},
                               {TOGGLE_BTN , true , checkbox_up , checkbox_down , font , "Option &3" , input_key_press(EAGLE_KEY_3) , "3"}};

   SIMPLE_MENU_BAR_ITEM mbar[] = {{font , "&File"    , input_key_press(EAGLE_KEY_F)},
                                  {font , "&Options" , input_key_press(EAGLE_KEY_O)}};
   
   
   ClassicMenu options;
   ClassicMenu foptions;
   ClassicMenuBar menubar;
   
   options.SetItems(&omenu[9] , 3);
   foptions.SetItems(&fmenu[0] , 4);
   menubar.SetBarItems(&mbar[0] , 2);
   
   menubar.SetSubMenu(0 , &foptions);
   menubar.SetSubMenu(1 , &options);
   
   
   
   
   
   
   bool quit = false;
   bool redraw = true;
   while (!quit) {
      if (redraw) {
         win->SetDrawingTarget(win->GetBackBuffer());
         win->Clear();
         redraw = false;
      }
      while (!sys->UpToDate()) {
         EagleEvent e = sys->WaitForSystemEventAndUpdateState();
         gui.HandleEvent(e);
         while (gui.HasMessages()) {
            WidgetMsg msg = gui.TakeNextMessage();
            (void)msg;
         }

      
   return 0;
}


