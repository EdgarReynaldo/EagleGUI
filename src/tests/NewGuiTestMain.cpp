


#include "NewGuiTestMain.hpp"


#include "Eagle\backends\Allegro5Backend.hpp"

using std::endl;



int NewGuiTestMain(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   EagleSystem* sys = new Allegro5System();
   
   
   if (EAGLE_FULL_SETUP != sys->Initialize(EAGLE_FULL_SETUP)) {
      return 1;
   }
   
   EagleGraphicsContext* win = sys->CreateGraphicsContext(800,600,EAGLE_WINDOWED);
   if (!win->Valid()) {
      return 2;
   }
   
   Allegro5Font bfont("Verdana.ttf" , 20 , 0);
   
   WidgetHandler gui;
   gui.SetDrawWindow(win);
   gui.SetupBufferDimensions(800,600);
   gui.SetArea(0,0,800,600);
   
   Button b1;
   b1.SetArea(200,150,400,300);
   b1.SetInputGroup(Input(KB,PRESS,EAGLE_KEY_SPACE));
   b1.SetLabel("Button 1");
   b1.SetFont(&bfont);
   
   gui.AddWidget(&b1,false);
   
   EagleLog() << gui << endl;
   EagleLog() << b1 << endl;
   
   bool quit = false;
   bool redraw = true;
   
///   sys->GetSystemTimer()->Create(0.5);
   sys->GetSystemTimer()->Start();
   
   do {
      if (redraw) {
///         win->Clear(EagleColor(0,255,0));
///         win->FlipDisplay();
         gui.Display(win,0,0);
         win->FlipDisplay();
         redraw = false;
      }
      
      do {
         EagleEvent ee;
         ee = sys->WaitForSystemEventAndUpdateState();
         
         gui.HandleEvent(ee);

         while (gui.HasMessages()) {
            WidgetMsg wmsg = gui.TakeNextMessage();
            EagleLog() << wmsg << endl;
            if (wmsg.From() == &b1) {
///               EagleLog() << b1 << endl;
            }
         }
         if (ee.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         if (ee.type == EAGLE_EVENT_KEY_DOWN && ee.keyboard.keycode == EAGLE_KEY_ESCAPE) {
            quit = true;
         }
         if (ee.type == EAGLE_EVENT_TIMER) {
            redraw = true;
         }
         
      } while(!sys->UpToDate());
   } while (!quit);
   
   return 0;
};

