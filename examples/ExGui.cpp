
#define ALLEGRO_UNSTABLE


#include "Eagle/backends/Allegro5Backend.hpp"
#include "Eagle.hpp"

void AllegroLog(const char* text) {EagleInfo() << text;}



int main4(int argc , char** argv) {
   
   SendOutputToFile("ExGuiFlags.log" , "ExGui\n" , false);

   GuiButton gbtn;
   
   WidgetFlags f = gbtn.Flags();
   
   EagleInfo() << gbtn << std::endl << std::endl;
   
   f.RemoveFlags(VISIBLE | ENABLED);
   
   EagleInfo() << f << std::endl << std::endl;
   
   gbtn.SetWidgetFlags(f);
   
   EagleInfo() << gbtn << std::endl << std::endl;
   
   return 0;
}

int main3(int argc , char** argv) {
   
   
   SendOutputToFile("ExGuiFlags.log" , "ExGui\n" , false);
   
   
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

   
   EagleFont* font = win->LoadFont("Verdana.ttf" , -24);
   
   WidgetHandler gui(win , "WidgetHandler" , "MainGUI");
   gui.SetupBuffer(sw , sh , win);
   gui.SetWidgetArea(Rectangle(0,0,sw,sh) , false);
   
   SHAREDOBJECT<WidgetColorset> cols = gui.GetWidgetColorset();
   (*cols.get())[HVRCOL] = EagleColor(0,255,0);

   RelativeLayout rl("RelativeLayout" , "MainRLayout");
   rl.Resize(1);
   
   gui.SetRootLayout(&rl);

   ListBox lbox;
   
   GuiButton btns[3];

   btns[0].SetFont(font);
   btns[1].SetFont(font);
   btns[2].SetFont(font);

   
   btns[0].SetLabel("Alpha");
   btns[1].SetLabel("Beta");
   btns[2].SetLabel("Gamma");
   
   btns[0].SetButtonType(ROUNDED_BTN , SPRING_BTN , BUTTON_CLASS_HOVER);
   btns[1].SetButtonType(RECTANGLE_BTN , SPRING_BTN , BUTTON_CLASS_HOVER);
   btns[2].SetButtonType(ELLIPSE_BTN , SPRING_BTN , BUTTON_CLASS_HOVER);
   
   lbox.SetWidgetArea(Rectangle(0,0,sw/4 - 20,2*sh/3) , false);
   lbox.Resize(3);
   lbox.AddWidget(&btns[0]);
   lbox.AddWidget(&btns[1]);
   lbox.AddWidget(&btns[2]);
   
   rl.PlaceWidget(&lbox , 0 , LayoutRectangle(0.25 , 0.35 , 0.25 , 0.5));
   
   WidgetFlags flags = lbox.Flags();
   
   EagleInfo() << lbox << std::endl << btns[0] << std::endl;
   
   lbox.HideAndDisable();
   
   EagleInfo() << std::endl << std::endl;
   
   EagleInfo() << lbox << std::endl << btns[0] << std::endl;

   lbox.ShowAndEnable();
   
   EagleInfo() << lbox << std::endl << btns[0] << std::endl;
   
   EagleInfo() << std::endl << std::endl;
   
   
   
   return 0;
}



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
   rl.Resize(2);
   
   gui.SetRootLayout(&rl);
   
   EagleFont* font = win->LoadFont("Verdana.ttf" , -24);
   
   
   
   
   ScrollArea sa("ScrollArea for the Listbox");
   ListBox lbox;
   
   GuiButton btns[3];

   btns[0].SetFont(font);
   btns[1].SetFont(font);
   btns[2].SetFont(font);

   
   btns[0].SetLabel("Alpha");
   btns[1].SetLabel("Beta");
   btns[2].SetLabel("Gamma");
   
   btns[0].SetButtonType(ROUNDED_BTN , SPRING_BTN , BUTTON_CLASS_HOVER);
   btns[1].SetButtonType(RECTANGLE_BTN , SPRING_BTN , BUTTON_CLASS_HOVER);
   btns[2].SetButtonType(ELLIPSE_BTN , SPRING_BTN , BUTTON_CLASS_HOVER);
   
   lbox.SetWidgetArea(Rectangle(0,0,sw/4 - 20,2*sh/3) , false);
   lbox.Resize(3);
   lbox.AddWidget(&btns[0]);
   lbox.AddWidget(&btns[1]);
   lbox.AddWidget(&btns[2]);
   
   sa.SetViewWidget(&lbox);

   DropDownList ddl(&sa , font);

   ddl.ListenTo(&lbox);

   std::vector<std::string> choices = {
      std::string("Alpha") , std::string("Beta") , std::string("Gamma")
   };
   ddl.SetTextChoices(choices);
   ddl.SetChoice(0);

   rl.PlaceWidget(&ddl , 0 , LayoutRectangle(0.25f,0.25f,0.25f,0.05f));//(Rectangle(0,0,sw,sh) , Rectangle(sw/4,sh/4,sw/2,sh/2)));
   rl.PlaceWidget(&sa , 1 , LayoutRectangle(0.25 , 0.35 , 0.25 , 0.5));

   EagleInfo() << sa << std::endl;

   EagleInfo() << sa.our_scroll_view << std::endl;
   EagleInfo() << *(sa.our_scroll_widget) << std::endl;
   
   bool quit = false;
   bool redraw = true;
   bool hide = false;
   
   sys->GetSystemTimer()->Start();
   
   while (!quit) {
      if (redraw) {
         win->Clear();
         gui.SetFullRedraw();
//         al_draw_bitmap(g.bitmap , 0 , 0 , 0);
         gui.Display(win , 0 , 0);
//         win->DrawVTextString(font , "Vertical" , sw - 150 , 150 , EagleColor(255,0,255) , HALIGN_LEFT , VALIGN_TOP);
//         win->DrawVTextString(font , "Vertical" , sw - 100 , 150 , EagleColor(255,0,255) , HALIGN_CENTER , VALIGN_CENTER);
//         win->DrawVTextString(font , "Vertical" , sw - 50 , 150 , EagleColor(255,0,255) , HALIGN_RIGHT , VALIGN_BOTTOM);
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
         if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_H) {
            hide = !hide;
            if (hide) {
               sa.HideAndDisable();
            }
            else {
               sa.ShowAndEnable();
            }
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













