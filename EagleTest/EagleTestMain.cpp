



#include "Eagle/backends/Allegro5Backend.hpp"
#include "allegro5/allegro_color.h"



int main2(int argc , char** argv) {
   (void)argc;(void)argv;
   EAGLE_ASSERT(ALLEGRO5_REGISTERED);
   EagleSystem* sys = Eagle::EagleLibrary::System("Allegro5");
   (void)sys;
   return 0;
}


//**

class TestWidget : public WidgetBase {
   
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateHandleEvent(EagleEvent ee);
   
   virtual void OnFlagChanged(WIDGET_FLAGS f , bool on);

   
public :
///   TestWidget(std::string objname);
   TestWidget(std::string objname) :
         WidgetBase("TestWidget" , objname) {}
   
};



void PaintTest(EagleGraphicsContext* win , NPAREA np);


int main(int argc , char** argv) {

   (void)argc;
   (void)argv;
   
///   EagleSystem* sys = Eagle::EagleLibrary::System("Allegro5");
///   EagleSystem* sys = Eagle::EagleLibrary::System("Allegro5");
   Allegro5System* sys = GetAllegro5System();
   
   int sw = 1200;
   int sh = 900;
   
   if (EAGLE_FULL_SETUP != sys->Initialize(EAGLE_FULL_SETUP)) {
      EagleWarn() << "Failed to install some components." << std::endl;
   }
   EagleGraphicsContext* win = sys->GetWindowManager()->CreateWindow("win" , sw , sh , EAGLE_OPENGL | EAGLE_WINDOWED | EAGLE_RESIZABLE);

   win->Clear(EagleColor(0,255,255));
   win->FlipDisplay();

   std::string bgfile = "stallions.jpg";
   EagleImage* bg = win->LoadImageFromFile(bgfile);
   if (!bg || (bg && !bg->Valid())) {
      EagleError() << StringPrintF("Failed to load %s from disk.\n" , bgfile.c_str());
      return -8;
   }
   
/**   
   WidgetHandler gui(win , "Gui" , "gui");
   
   gui.SetWidgetArea(Rectangle(0,0,sw,sh) , false);
   
   
   RelativeLayout rl("RLayout");
   
   gui.SetRootLayout(&rl);
   
   std::vector<WidgetBase*> hbtns = {
      new GuiButton("GuiButton" , "Menu button #1"),
      new GuiButton("GuiButton" , "Menu button #2"),
      new GuiButton("GuiButton" , "Menu button #3"),
      new GuiButton("GuiButton" , "Menu button #4")
   };

   std::vector<WidgetBase*> vbtns = {
      new GuiButton("GuiButton" , "Menu button #5"),
      new GuiButton("GuiButton" , "Menu button #6"),
      new GuiButton("GuiButton" , "Menu button #7"),
      new GuiButton("GuiButton" , "Menu button #8")
   };

   for (int i = 0 ; i < (int)hbtns.size() ; ++i) {
      GuiButton* hb = dynamic_cast<GuiButton*>(hbtns[i]);
      GuiButton* vb = dynamic_cast<GuiButton*>(vbtns[i]);

      hb->SetFont(win->DefaultFont());
      hb->SetInputGroup(input_key_press(EAGLE_KEY_1 + i));
      hb->SetLabel(StringPrintF("Button #%d" , i + 1));

      vb->SetFont(win->DefaultFont());
      vb->SetInputGroup(input_key_press(EAGLE_KEY_5 + i));
      vb->SetLabel(StringPrintF("Button #%d" , i + 5));
   }
   
   ClassicMenuLayout vmenu("VMenu");
   ClassicMenuLayout hmenu("HMenu");
   
   rl.Resize(2);

   rl.SetLayoutRectangle(0 , LayoutRectangle(0.0 , 0.0 , 0.5 , 0.05));
   rl.SetLayoutRectangle(1 , LayoutRectangle(0.4 , 0.15 , 0.2 , 0.7));

   rl.PlaceWidget(&hmenu , 0);
   rl.PlaceWidget(&vmenu , 1);

   vmenu.ResizeMenu(4 , MENU_VERTICAL);
   hmenu.ResizeMenu(4 , MENU_HORIZONTAL);
   
   vmenu.SetWChildren(vbtns);
   hmenu.SetWChildren(hbtns);
   
   vmenu.SetGlobalSpacing(0,20);
   hmenu.SetGlobalSpacing(10,0);
   
   
   
   
   
   EagleLog() << gui << std::endl;
//*/   
//**
   
   WidgetHandler gui(win , "WidgetHandler" , "GUI1");
   gui.SetWidgetArea(WIDGETAREA(10 , 15 , 25 , Rectangle(150,150,900,600)) , false);
   
   WidgetHandler gui2(win , "WidgetHandler" , "GUI2");
   gui2.SetupBuffer(1280,960,win);
   gui2.SetWidgetArea(WIDGETAREA(5,10,15 , Rectangle(130,60,640,480)) , false);
   gui.AllowMiddleMouseButtonDrag(false);
   gui2.AllowMiddleMouseButtonDrag(true);
   
   gui.SetBackgroundColor(EagleColor(0,0,127));
   gui2.SetBackgroundColor(EagleColor(0,127,0));
   
   gui << gui2;

   RelativeLayout rl1("RLAYOUT1");
   rl1.Resize(5);
   rl1.SetLayoutRectangle(0 , LayoutRectangle(0.2 , 0.2 , 0.6 , 0.2));
   rl1.SetLayoutRectangle(1 , LayoutRectangle(0.2 , 0.6 , 0.6 , 0.2));
   rl1.SetLayoutRectangle(2 , LayoutRectangle(0.4 , 0.2 , 0.2 , 0.6));
   rl1.SetLayoutRectangle(3 , LayoutRectangle(0.1 , 0.2 , 0.2 , 0.6));
   rl1.SetLayoutRectangle(4 , LayoutRectangle(0.7 , 0.2 , 0.2 , 0.6));
   
   gui2.SetRootLayout(&rl1);
//   gui1.AddWidget(&gui2);
   
   TestWidget tw1("TESTWIDGETH1");
   TestWidget tw2("TESTWIDGETH2");
   TestWidget tw3("TESTWIDGETV1");
   TestWidget tw4("TESTWIDGETV2");
   TestWidget tw5("TESTWIDGETV3");
   
   rl1.PlaceWidget(&tw1 , 0);
   rl1.PlaceWidget(&tw2 , 1);
   rl1.PlaceWidget(&tw3 , 2);
   rl1.PlaceWidget(&tw4 , 3);
   rl1.PlaceWidget(&tw5 , 4);
   
   tw1.SetWidgetArea(WIDGETAREA(tw1.OuterArea() , 3,5,7));
   tw2.SetWidgetArea(WIDGETAREA(tw2.OuterArea() , 7,5,3));
   tw3.SetWidgetArea(WIDGETAREA(tw3.OuterArea() , 2,4,6));
   tw4.SetWidgetArea(WIDGETAREA(tw4.OuterArea() , 6,4,2));
   tw5.SetWidgetArea(WIDGETAREA(tw5.OuterArea() , 5,5,5));
   
//*/
         
   EagleLog() << "******* SETUP COMPLETE ********" << std::endl;
         
   bool quit = false;
   bool redraw = true;
   
   int mx = 0;
   int my = 0;
   
   sys->GetSystemTimer()->Start();
   
   
   
   while (!quit) {
///      if (redraw) {
      if (redraw) {
         win->DrawToBackBuffer();
         win->Clear(EagleColor(0,0,0));
         win->DrawStretched(bg , Rectangle(0 , 0 , sw , sh));
         gui.Display(win , 0 , 0);
         WidgetBase* hw = gui.GetWidgetAt(mx,my);
         std::string name = (hw?hw->FullName():"NULL");
         win->DrawTextString(win->DefaultFont() , StringPrintF("Widget at %d,%d is [%s]" , mx , my , name.c_str()) , sw - 10 , sh - win->DefaultFont()->Height() - 5 , EagleColor(0,0,0) , HALIGN_RIGHT , VALIGN_TOP);
         win->FlipDisplay();
         redraw = false;
      }
      do {
         EagleEvent ev = sys->WaitForSystemEventAndUpdateState();
         if (ev.type != EAGLE_EVENT_TIMER && ev.type != EAGLE_EVENT_MOUSE_AXES) {
            EagleInfo() << "Event " << EagleEventName(ev.type) << " received in main." << std::endl;
         }
         
         if (ev.type == EAGLE_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode >= EAGLE_KEY_1 && ev.keyboard.keycode <= EAGLE_KEY_4) {
/*
               int index = ev.keyboard.keycode - EAGLE_KEY_1;
               Pos2I p(0,0);
               p.MoveBy((index % 2 == 1)?640:0 , (index / 2 == 1)?480:0);
               gui2.MoveViewTlxTo(p.X() , p.Y());
*/
            }
         }
         
         if (ev.type == EAGLE_EVENT_MOUSE_AXES) {
            mx = ev.mouse.x;
            my = ev.mouse.y;
            redraw = true;
         }
         
         if (ev.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         else if (ev.type == EAGLE_EVENT_KEY_DOWN && ev.keyboard.keycode == EAGLE_KEY_ESCAPE) {
            quit = true;
         }
         else if (ev.type == EAGLE_EVENT_TIMER) {
            gui.Update(ev.timer.eagle_timer_source->SPT());
            /// redraw = true;
         }
         else {
            gui.HandleEvent(ev);
         }
         
         while (gui.HasMessages()) {
            WidgetMsg wmsg = gui.TakeNextMessage();
            EagleLog() << "Widget Message [" << wmsg << "]" << std::endl;
         }
         if (gui.Flags().FlagOn(NEEDS_REDRAW)) {
            redraw = true;
         }
         
      } while (!sys->UpToDate());
   }
   
///   sys->GetSystemQueue()->WaitForEvent(EAGLE_EVENT_KEY_DOWN , 0);
   
   EagleLog() << "Exited main loop." << std::endl;
   
///   sys->Shutdown();
   atexit(Eagle::EagleLibrary::ShutdownEagle);

   return 0;
}



void PaintTest(EagleGraphicsContext* win , NPAREA np) {
   float h = 0.0;
   float s = 1.0;
   float l = 0.5;
   
   for (int i = 0 ; i < 9 ; ++i) {
      h = 360.0*i/9;
      HCELL_AREA hcell = (HCELL_AREA)(i%3);
      VCELL_AREA vcell = (VCELL_AREA)(i/3);
      Rectangle rect = np.GetNPCell(hcell , vcell);
      float r,g,b;
      al_color_hsl_to_rgb(h,s,l,&r,&g,&b);
      win->DrawFilledRectangle(rect , EagleColor(r,g,b));
   }
}



void TestWidget::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   Rectangle r = InnerArea();
///   Rectangle r = InnerArea();
   r.MoveBy(xpos,ypos);
   unsigned int flags = Flags();
   EagleColor c1 = GetColor(MGCOL);
   EagleColor c2 = GetColor(BGCOL);
   if (flags & HASFOCUS) {
      c1 = GetColor(HLCOL);
      c2 = GetColor(FGCOL);
   }
   win->DrawFilledRectangle(r , (flags & HOVER)?c1:c2);
   std::string n = ShortName();
   win->DrawTextString(win->DefaultFont() , n , InnerArea().CX() , InnerArea().CY() , EagleColor(255,0,0) , HALIGN_CENTER , VALIGN_CENTER);
}



int TestWidget::PrivateHandleEvent(EagleEvent ee) {
   if (ee.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN && ee.mouse.button == 1) {
      if (OuterArea().Contains(ee.mouse.x , ee.mouse.y)) {
         return DIALOG_TAKE_FOCUS;
      }
   }
   return DIALOG_OKAY;
}



void TestWidget::OnFlagChanged(WIDGET_FLAGS f , bool on) {
   if (f == HOVER) {
      EagleLog() << "HOVER changed to " << (on?"on":"off") << std::endl;
      SetRedrawFlag();
   }
   if (f == HASFOCUS) {
      EagleLog() << "HASFOCUS changed to " << (on?"on":"off") << std::endl;
      SetRedrawFlag();
   }
}
//*/
