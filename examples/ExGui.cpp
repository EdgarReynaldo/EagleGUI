



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
   rl.Resize(3);
   
   gui.SetRootLayout(&rl);
   
   BasicScrollBar hbar,vbar;
   hbar.SetScrollDirection(true);
   vbar.SetScrollDirection(false);
   
   hbar.SetupView(100 , 10);
   vbar.SetupView(100 , 10);
   
   rl.PlaceWidget(&hbar , 1 , LayoutRectangle(0.2 , 0.7 , 0.5 , 0.1));
   rl.PlaceWidget(&vbar , 2 , LayoutRectangle(0.7 , 0.2 , 0.1 , 0.5));
   
   BasicScrollButton n,s,e,w;
   n.SetScrollDirection(true , false);
   s.SetScrollDirection(false , false);
   e.SetScrollDirection(false , true);
   w.SetScrollDirection(true , true);
   
//   rl.PlaceWidget(&n , 3 , LayoutRectangle(0.3,0.2,0.1,0.1));
//   rl.PlaceWidget(&s , 4 , LayoutRectangle(0.3,0.4,0.1,0.1));
//   rl.PlaceWidget(&w , 5 , LayoutRectangle(0.2,0.3,0.1,0.1));
//   rl.PlaceWidget(&e , 6 , LayoutRectangle(0.4,0.3,0.1,0.1));
   
   
   
   
   
   
   
   
   ScrollArea sa("Scroll area");
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
   
   BasicIcon icon;
   SHAREDIMAGE img = StackObject(win->LoadImageFromFile("Data/Images/Stallions2a.jpg"));
   icon.SetImage(img);
   icon.SetWidgetArea(Rectangle(0,0,img->W(),img->H()));

//   ScrollView sv(&lbox);

   sa.SetViewWidget(&lbox);

   rl.PlaceWidget(&sa , 0 , LayoutRectangle(0.25f,0.25f,0.25f,0.25f));//(Rectangle(0,0,sw,sh) , Rectangle(sw/4,sh/4,sw/2,sh/2)));
   
//   rl.PlaceWidget(&lbox , 1 , LayoutRectangle(0.25f,0.25f,0.25f,0.25f));

   EagleLog() << lbox << std::endl << std::endl;


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
            if (msg.From() == &hbar || msg.From() == &vbar) {
               sa.SetScrollbarValues(hbar.GetScrollValue() , vbar.GetScrollValue());
            }
         }
      } while (!sys->UpToDate());
   }
   
   
   
   
   return 0;
   
}













