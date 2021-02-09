

#define ALLEGRO_UNSTABLE


#include "Eagle/backends/Allegro5Backend.hpp"
#include "Eagle.hpp"

void AllegroLog(const char* text) {EagleInfo() << text;}







class PSlider : public Slider {
   bool drag = false;
public :
   PSlider() :
         Slider("PSlider" , "Percent slider" , false , false)
{}
//   virtual void Display(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateHandleEvent(EagleEvent e) {
      if (IsMouseEvent(e)) {
         if (e.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN) {
            if (InnerArea().Contains(e.mouse.x , e.mouse.y)) {
               Slider::SetPercent((double)(e.mouse.x - InnerArea().X())/InnerArea().W());
               drag = true;
            }
         }
         else if (e.type == EAGLE_EVENT_MOUSE_AXES) {
            if (drag) {
               Slider::SetPercent((double)(e.mouse.x - InnerArea().X())/InnerArea().W());
            }
         }
         else if (e.type == EAGLE_EVENT_MOUSE_BUTTON_UP) {
            drag = false;
         }
      }
      return DIALOG_OKAY;
   }
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
      Rectangle r = InnerArea();
      win->DrawFilledRectangle(r , GetColor(BGCOL));
      Rectangle pct(r.X() , r.Y() , r.W()*this->GetPercent() , r.H());
      win->DrawFilledRectangle(pct , GetColor(FGCOL));
   }
};

class NumberButton : public GuiButton {
   int num;
//   virtual void Display(EagleGraphicsContext* win , int xpos , int ypos);
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
      text = StringPrintF("%d" , num);
      GuiButton::PrivateDisplay(win,xpos,ypos);
//      win->DrawTextString(text_font , StringPrintF("%d" , num) , InnerArea().CX() , InnerArea().CY() , GetColor(TXTCOL) , HALIGN_CENTER , VALIGN_CENTER);
   }
public :
   void SetNum(int n) {num = n;}
};

int main(int argc , char** argv) {
   Allegro5System* sys = GetAllegro5System();
   
   if (sys->Initialize(EAGLE_FULL_SETUP) != EAGLE_FULL_SETUP) {
      EagleWarn() << "Some subsystems not initialized. Proceeding" << std::endl;
   }
   
   int sw = 300;
   int sh = 600;
   
   EagleGraphicsContext* win = sys->CreateGraphicsContext("Main Window" , sw , sh , EAGLE_OPENGL | EAGLE_WINDOWED | EAGLE_RESIZABLE);
   
   EAGLE_ASSERT(win && win->Valid());
   
   EagleFont* font = win->LoadFont("Verdana.ttf" , -20);
   
   EAGLE_ASSERT(font && font->Valid());
   
   
   
   WidgetHandler gui(win , "GUI" , "Example GUI");
   
   
   
   gui.SetupBuffer(sw , sh , win);
   
   gui.SetWidgetArea(Rectangle(0 , 0 , sw , sh));
   
   RelativeLayout rl("RL" , "RelativeLayout");
   
   rl.Resize(7);/// layout has 7 basic rows

   gui.SetRootLayout(&rl);

   /// TOP ROW OF SHIP SEND PANEL   
   GridLayout toprow(3 , 1 , "GridLayout" , "TopRow");
   BasicText trtext1(font , "100" , HALIGN_RIGHT , VALIGN_CENTER , 2 , 2 , 0);
   BasicText trtext2(font , "SOL\nTO\nULATTE" , HALIGN_CENTER , VALIGN_CENTER , 2 , 2 , 0);
   BasicText trtext3(font , "20" , HALIGN_LEFT , VALIGN_CENTER , 2 , 2 , 0);
   toprow.PlaceWidget(&trtext1 , 0);
   toprow.PlaceWidget(&trtext2 , 1);
   toprow.PlaceWidget(&trtext3 , 2);
   
   float y = 0.0;
   rl.PlaceWidget(&toprow , 0 , LayoutRectangle(0.0f , 0.0f , 1.0f , 0.2f));
   y += 0.2f;
   
   
   /// 2ND ROW
   GridLayout num1;
   num1.ResizeGrid(10 , 1);
   num1.SetGlobalPadding(2,2);
   rl.PlaceWidget(&num1 , 2 , LayoutRectangle(0.0f , y , 1.0f , 0.1));
   y += 0.1f;
   
   /// 3ND ROW
   IntEditText numinput;
   numinput.SetupText("" , font , HALIGN_RIGHT , VALIGN_CENTER);
   rl.PlaceWidget(&numinput , 1 , LayoutRectangle(0.0f , y , 1.0f , 0.1));
   y += 0.1f;

   /// 4TH ROW
   GridLayout num2;
   num2.ResizeGrid(11 , 1);
   num2.SetGlobalSpacing(4 , 4);

   NumberButton numbtns1[10];
   NumberButton numbtns2[10];
   
   for (unsigned int i = 0 ; i < 10 ; ++i) {
      numbtns1[i].SetFont(font);
      numbtns2[i].SetFont(win->DefaultFont());
      numbtns1[i].SetNum(i);
      numbtns2[i].SetNum((i+1)*10);
      num1.PlaceWidget(&numbtns1[i] , i);
      num2.PlaceWidget(&numbtns2[i] , i + 1);
   }
   BasicText pctbtn;
   pctbtn.SetupText("%" , font , HALIGN_CENTER , VALIGN_CENTER);
   num2.PlaceWidget(&pctbtn , 0);
   
   rl.PlaceWidget(&num2 , 3 , LayoutRectangle(0.0f , y , 1.0f , 0.2/3.0f));
   y += 0.2f/3.0f;
   
   /// Row 5
   BasicScrollButton lrbtn[2];
   lrbtn[0].SetScrollDirection(true , true);
   lrbtn[1].SetScrollDirection(false , true);
   /// Row 5 col 2 -> right
   PSlider pslider;
   BasicTextButton clrbtn;
   clrbtn.SetupText("Clear" , font , HALIGN_CENTER , VALIGN_CENTER);
   
   RelativeLayout rl2;
   rl2.Resize(4);
   rl2.PlaceWidget(&lrbtn[0] , 0 , LayoutRectangle(0.0f , 0.0f , 0.05f , 1.0f));
   rl2.PlaceWidget(&lrbtn[1] , 1 , LayoutRectangle(0.05f , 0.0f , 0.05f , 1.0f));
   rl2.PlaceWidget(&pslider  , 2 , LayoutRectangle(0.1f , 0.0f , 0.7f , 1.0f));
   rl2.PlaceWidget(&clrbtn   , 3 , LayoutRectangle(0.8f  , 0.0f , 0.2f  , 1.0f));
   
   rl.PlaceWidget(&rl2 , 4 , LayoutRectangle(0.0f , y , 1.0f , 0.2/3.0f));
   y += 0.2f/3.0f;
   
   /// Row 6
   BasicCheckBox cbox[2];
   BasicText cboxlabels[2];

   cbox[0].SetButtonState(false , false , false);
   cbox[1].SetButtonState(false , false , false);
   cboxlabels[0].SetupText("Instant % Box" , font , HALIGN_LEFT , VALIGN_CENTER);
   cboxlabels[1].SetupText("Instant % Bar" , font , HALIGN_LEFT , VALIGN_CENTER);
   
   RelativeLayout rl3;
   rl3.Resize(5);
   rl3.PlaceWidget(&cbox[0]       , 0 , LayoutRectangle(0.0f  , 0.0f , 0.1f , 1.0f));
   rl3.PlaceWidget(&cboxlabels[0] , 1 , LayoutRectangle(0.1f , 0.0f , 0.4f , 1.0f));
   rl3.PlaceWidget(&cbox[1]       , 2 , LayoutRectangle(0.5f  , 0.0f , 0.1f , 1.0f));
   rl3.PlaceWidget(&cboxlabels[1] , 3 , LayoutRectangle(0.6f , 0.0f , 0.4f , 1.0f));
   
   rl.PlaceWidget(&rl3 , 5 , LayoutRectangle(0.0f , y , 1.0f , 0.2f/3.0f));
   y += 0.2f/3.0f;

   /// Row 7
   BasicTextButton send_btns[4];
   send_btns[0].SetupText("Kill" , font , HALIGN_CENTER , VALIGN_CENTER , 2 , 2);
   send_btns[1].SetupText("Chase" , font , HALIGN_CENTER , VALIGN_CENTER , 2 , 2);
   send_btns[2].SetupText("No Retreat" , font , HALIGN_CENTER , VALIGN_CENTER , 2 , 2);
   send_btns[3].SetupText("Send" , font , HALIGN_CENTER , VALIGN_CENTER , 2 , 2);

   FlowLayout flow;
   for (unsigned int i = 0 ; i < 4 ; ++i) {
      BasicTextButton* pbtn = &send_btns[i];
      pbtn->SetPreferredSize();
      flow.AddWidget(pbtn);
   }
   rl.PlaceWidget(&flow , 6 , LayoutRectangle(0.0f , y , 1.0f , 0.13f));
//   rl.PlaceWidget(&flow , 6 , LayoutRectangle(0.0f , 0.0 , 1.0f , 1.0f));
   
   
   bool quit = false;
   bool redraw = true;
   
   sys->GetSystemTimer()->Start();
   
   while (!quit) {
      if (redraw) {
         win->Clear();
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



int main2(int argc , char** argv) {
   
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













