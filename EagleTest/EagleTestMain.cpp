



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
   
   
   Allegro5System* sys = GetAllegro5System();
   
   if (EAGLE_FULL_SETUP != sys->Initialize(EAGLE_FULL_SETUP)) {
      EagleWarn() << "Failed to install some components." << std::endl;
   }
   EagleGraphicsContext* win = sys->GetWindowManager()->CreateWindow("win" , 1024 , 768 , EAGLE_OPENGL | EAGLE_WINDOWED);
   
   win->Clear(EagleColor(0,255,255));
   win->FlipDisplay();
   
//   WidgetHandler gui1(win , "WidgetHandler" , "GUI1");
//   gui1.SetWidgetArea(Rectangle(62,84,900,600),false);
   
   WidgetHandler gui2(win , "WidgetHandler" , "GUI2");
///   gui2.SetWidgetArea(Rectangle(0,0,1024,768) , false);
   gui2.SetWidgetArea(Rectangle(112,84,800,600) , false);
   
//   gui1.SetBackgroundColor(EagleColor(0,255,255));
   gui2.SetBackgroundColor(EagleColor(255,0,255));
   
   RelativeLayout rl1("RLAYOUT1");
   rl1.Resize(2);
   rl1.SetLayoutRectangle(0 , LayoutRectangle(0.2 , 0.2 , 0.6 , 0.2));
   rl1.SetLayoutRectangle(1 , LayoutRectangle(0.2 , 0.6 , 0.6 , 0.2));
   
   gui2.SetRootLayout(&rl1);
//   gui1.AddWidget(&gui2);
   
   TestWidget tw1("TESTWIDGET1") , tw2("TESTWIDGET2");
   
   rl1.PlaceWidget(&tw1 , 0);
   rl1.PlaceWidget(&tw2 , 1);
   
   tw1.SetWidgetArea(WIDGETAREA(tw1.OuterArea() , 3,5,7));
   tw2.SetWidgetArea(WIDGETAREA(tw2.OuterArea() , 7,5,3));
         
   EagleLog() << rl1 << std::endl;

///   EagleEventHandler* q = sys->GetSystemQueue();
   
   bool quit = false;
   bool redraw = true;
   
   sys->GetSystemTimer()->Start();
   
   while (!quit) {
///      if (redraw) {
      if (1) {
         win->DrawToBackBuffer();
         win->Clear(EagleColor(0,0,0));
         gui2.Display(win , 0 , 0);
         win->FlipDisplay();
         redraw = false;
      }
      do {
         EagleEvent ev = sys->WaitForSystemEventAndUpdateState();
         if (ev.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         else if (ev.type == EAGLE_EVENT_KEY_DOWN && ev.keyboard.keycode == EAGLE_KEY_ESCAPE) {
            quit = true;
         }
         else if (ev.type == EAGLE_EVENT_TIMER) {
            gui2.Update(ev.timer.eagle_timer_source->SPT());
            /// redraw = true;
         }
         else {
            EagleLog() << "GUI2 Handling event " << EagleEventName(ev.type) << std::endl;
            gui2.HandleEvent(ev);
         }
         
         while (gui2.HasMessages()) {
            WidgetMsg wmsg = gui2.TakeNextMessage();
            (void)wmsg;
         }
         if (gui2.Flags().FlagOn(NEEDS_REDRAW)) {
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
}



int TestWidget::PrivateHandleEvent(EagleEvent ee) {
   if (ee.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN) {
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
