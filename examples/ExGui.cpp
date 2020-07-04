



#include "Eagle/backends/Allegro5Backend.hpp"
#include "Eagle.hpp"

void AllegroLog(const char* text) {EagleInfo() << text << std::endl;}

int main(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   SendOutputToFile("ExGui.log" , "ExGui\n" , false);
   
   
   EagleSystem* sys = GetAllegro5System();
   if (!sys) {return -1;}
   
   int ret = sys->Initialize(EAGLE_FULL_SETUP);
   
   int sw = 1280;
   int sh = 800;
   
   al_register_trace_handler(AllegroLog);
   
   EagleGraphicsContext* win = sys->CreateGraphicsContext("Gui window" , sw , sh , EAGLE_OPENGL | EAGLE_WINDOWED);
   
   if (!(win && win->Valid())) {
      return -2;
   }
   
   
   WidgetHandler gui(win , "WidgetHandler" , "MainGUI");
   gui.SetupBuffer(sw , sh , win);
   gui.SetWidgetArea(Rectangle(0,0,sw,sh) , false);
   
   SHAREDOBJECT<WidgetColorset> cols = gui.GetWidgetColorset();
   (*cols.get())[HVRCOL] = EagleColor(0,255,0);
   
   
   RelativeLayout rl("RelativeLayout" , "MainRLayout");
   rl.Resize(1);
   
   gui.SetRootLayout(&rl);
   
   BasicTextButton tbtn;
   
   rl.PlaceWidget(tbtn , 0 , LayoutRectangle(0.25,0.25,0.5,0.5));
   
   
   
   
   
   
/**
   DropDownList ddl;
   ListBox lbox;
   
   GuiButton btns[3];

   btns[0].SetFont(win->DefaultFont());
   btns[1].SetFont(win->DefaultFont());
   btns[2].SetFont(win->DefaultFont());

   btns[0].SetLabel("A");
   btns[1].SetLabel("B");
   btns[2].SetLabel("C");
   
   btns[0].SetButtonType(ROUNDED_BTN , TOGGLE_BTN , BUTTON_CLASS_HOVER);
   btns[1].SetButtonType(RECTANGLE_BTN , TOGGLE_BTN , BUTTON_CLASS_HOVER);
   btns[2].SetButtonType(ELLIPSE_BTN , TOGGLE_BTN , BUTTON_CLASS_HOVER);
   
   lbox.SetWidgetArea(Rectangle(0,0,sw/2,sh/2) , false);
   lbox.Resize(3);
   lbox.AddWidget(&btns[0]);
   lbox.AddWidget(&btns[1]);
   lbox.AddWidget(&btns[2]);
   
   rl.PlaceWidget(&ddl , 0 , LayoutRectangle(0.25f,0.25f,0.25f,0.05f));//(Rectangle(0,0,sw,sh) , Rectangle(sw/4,sh/4,sw/2,sh/2)));
   rl.PlaceWidget(&lbox , 1 , LayoutRectangle(0.25 , 0.25 , 0.25 , 0.5));
*/

   bool quit = false;
   bool redraw = true;
   
   sys->GetSystemTimer()->Start();
   
   while (!quit) {
      if (redraw) {
         win->Clear();
///         gui.SetFullRedraw();
         gui.Display(win , 0 , 0);
         win->FlipDisplay();
         redraw = false;
      }
      do {
         EagleEvent e = sys->WaitForSystemEventAndUpdateState();
         if (e.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_ESCAPE) {
            quit = true;
         }
         if (e.type == EAGLE_EVENT_TIMER) {
            redraw = true;
            gui.Update(sys->GetSystemTimer()->SPT());
         }
         gui.HandleEvent(e);
         while (gui.HasMessages()) {
            WidgetMsg msg = gui.TakeNextMessage();
            EagleLog() << msg << std::endl;
         }
      } while (!sys->UpToDate());
   }
   
   
   
   
   return 0;
   
}













