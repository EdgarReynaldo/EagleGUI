


#include "Eagle/backends/Allegro5backend.hpp"




int main(int argc , char** argv) {
   
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
   
   GuiButton* btn1 = new GuiButton("btn1");
   BasicButton* btn2 = new BasicButton("btn2");
   
   Input key1 = input_key_press(EAGLE_KEY_Q);
   Input key2 = input_key_press(EAGLE_KEY_P);
   InputGroup g1 = key1;
   InputGroup g2 = key2;
   InputGroup g3 = g1;
   InputGroup g4 = g2;

   btn1->SetInputGroup(key1);
   btn2->SetInputGroup(key2);
   
   /// These two statements do nothing - they are a trap
///   btn1->InputKey() = g1;
///   btn2->InputKey() = g2;
   
   GridLayout* gl = new GridLayout(2,1,"gl");
   gl->PlaceWidget(btn1 , 0);
   gl->PlaceWidget(btn2 , 1);
   
   RelativeLayout* rl = new RelativeLayout("rl");

   gui->SetRootLayout(rl);
   gui->SetBackgroundColor(EagleColor(0,0,0,0));

   rl->AddWidget(gl , LayoutRectangle(0.3 , 0.4 , 0.4 , 0.2));

   gui->SetWidgetArea(0 , 0 , win->Width() , win->Height());
   
   EagleLog() << *gui << std::endl;
   
   WidgetBase::ClipWidgets(true);
   WidgetBase::DisplayWidgetArea(true);
   if (!gui) {
      EAGLE_ASSERT(gui);
      return 1;
   }
   bool quit = false;
   bool redraw = true;
   
   sys->GetSystemTimer()->Start();
   
   while (!quit) {
      
      if (redraw) {
         win->Clear(EagleColor(0 , 0 , 96));
         gui->Display(win , 0 , 0);
         win->FlipDisplay();
         redraw = false;
      }
      do {
         EagleEvent e;
         e = sys->GetSystemQueue()->WaitForEvent(0);
         
         sys->GetSystemInput()->HandleInputEvent(e);
         
         if (key1 || key2) {
            EagleInfo() << "Key press detected." << std::endl;
         }
         if (g1 || g2) {
            EagleInfo() << "Key group detected." << std::endl;
         }
         if (g3 || g4) {
            EagleInfo() << "Copied Key group detected." << std::endl;
/*
            if (btn1->HandleEvent(e) != DIALOG_INPUT_USED) {
               if (btn2->HandleEvent(e) != DIALOG_INPUT_USED) {
                  EagleInfo() << "The buttons are stupid" << std::endl;
               }
            }
*/
         }
         
         
         int input_used = gui->HandleEvent(e) == DIALOG_INPUT_USED;
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
            redraw = true;
         }
      } while (sys->GetSystemQueue()->HasEvent(0));
   }
   
   
   
   return 0;
}


