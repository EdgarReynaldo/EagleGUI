


#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"
#include "allegro5/allegro_color.h"



class TestWidget : public WidgetBase {
   
   virtual void PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos);
   virtual int PrivateHandleEvent(EagleEvent ee);
   
   virtual void OnFlagChanged(WIDGET_FLAGS f , bool on);

   
public :
   TestWidget(std::string objname) :
         WidgetBase("TestWidget" , objname) {}
   
};



void TestWidget::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   Rectangle r = InnerArea();
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


int main(int argc , char** argv) {
   EnableLog();
   
   SendOutputToFile("Eagle.log" , "" , false);
   
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
   EagleGraphicsContext* win = sys->GetWindowManager()->CreateEagleWindow("win" , sw , sh , EAGLE_OPENGL | EAGLE_WINDOWED | EAGLE_RESIZABLE);
   
   win->Clear(EagleColor(0,255,255));
   win->FlipDisplay();

   std::string bgfile = "stallions.jpg";
   EagleImage* bg = win->LoadImageFromFile(bgfile);
   if (!bg || (bg && !bg->Valid())) {
      EagleError() << StringPrintF("Failed to load %s from disk.\n" , bgfile.c_str());
      return -1;
   }
   
   /// Setup some custom colors for our margin, border, and padding
   SHAREDOBJECT<WidgetColorset> pwc = GetColorsetByName("Default");
   EAGLE_ASSERT(pwc);
   WidgetColorset& wc = *pwc.get();
   wc[PADCOL] = EagleColor(127,127,127,255);
   wc[BORDCOL] = EagleColor(255,255,255,255);
   wc[MARGCOL] = EagleColor(0,0,0,255);
   
   
   /// Outer root gui
   WidgetHandler gui(win , "WidgetHandler" , "GUI1");
   gui.SetWidgetArea(WIDGETAREA(10 , 15 , 25 , Rectangle(150,150,900,600)) , false);
   
   
   BasicScrollBar hscroller("BasicScrollBar" , "hscroller");
   BasicScrollBar vscroller("BasicScrollBar" , "vscroller");

   hscroller.SetScrollDirection(true);
   vscroller.SetScrollDirection(false);
   
   hscroller.SetupView(100,25);
   vscroller.SetupView(100,25);
   
/*
   int hx = 0;
   int hy = 0;
   BasicScrollButton bsbleft;
   BasicScrollButton bsbright;
   BasicScrollButton bsbup;
   BasicScrollButton bsbdown;
   
   bsbleft.SetScrollDirection(true , true);
   bsbright.SetScrollDirection(false , true);
   bsbup.SetScrollDirection(true , false);
   bsbdown.SetScrollDirection(false , false);
//*/
/*
   BasicButton bsbleft;
   BasicButton bsbright;
   BasicButton bsbup;
   BasicButton bsbdown;
//*/   

   RelativeLayout rl;
   
/*
   rl.AddWidget(&hscroller , LayoutRectangle(0.3,0.1,0.6,0.2));
   rl.AddWidget(&vscroller , LayoutRectangle(0.1,0.3,0.2,0.6));

   rl.AddWidget(&bsbleft , LayoutRectangle(0.4,0.5,0.1,0.1));
   rl.AddWidget(&bsbright , LayoutRectangle(0.6,0.5,0.1,0.1));
   rl.AddWidget(&bsbup , LayoutRectangle(0.5,0.4,0.1,0.1));
   rl.AddWidget(&bsbdown , LayoutRectangle(0.5,0.6,0.1,0.1));
   
   BasicText hlabel;
   BasicText vlabel;
   
   hlabel.SetFont(win->DefaultFont());
   vlabel.SetFont(win->DefaultFont());
   
   rl.AddWidget(&hlabel , LayoutRectangle(0.6 , 0.05 , 0.3 , 0.1));
   rl.AddWidget(&vlabel , LayoutRectangle(0.05 , 0.6 , 0.1 , 0.3));
   
   gui.SetRootLayout(&rl);
*/   
//**   
   BasicIcon stallion_icon;
   stallion_icon.SetImage(StackObject(bg));
   
   stallion_icon.SetWidgetArea(Rectangle(0,0,bg->W() , bg->H()) , false);
   
   ScrollArea scrollview("IconScroller");
   
   
//   scrollview.SetScrollbarPosition(false , false);
   scrollview.SetScrollbarPosition(true , true);
   scrollview.PlaceWidget(&stallion_icon , 2);
   
   rl.AddWidget(&scrollview , LayoutRectangle(0,0,1,1));
///   scrollview.SetWidgetArea(Rectangle(150 , 100 , 600 , 400) , false);
///   gui.GiveWidgetFocus(&scrollview);
//*/   

   gui.SetRootLayout(&rl);

   EagleLog() << "******* SETUP COMPLETE ********" << std::endl;
   EagleLog() << gui << std::endl << std::endl;
   EagleLog() << "******* SETUP COMPLETE ********" << std::endl;
         
   bool quit = false;
   bool redraw = true;
   
//   int mx = 0;
//   int my = 0;
   
   sys->GetSystemTimer()->Start();
   const char* txt = "Hello Popup";
   PopupText ptext(sw/2 - 50 , sh/2 - 10 , EAGLE_OPENGL , std::string(txt) , win->DefaultFont());
   
   EagleInfo() << ptext << std::endl;
   
   ptext.Hide();
   bool popup_hidden = true;
   
   while (!quit) {
      if (redraw) {
         win->DrawToBackBuffer();
         win->Clear(EagleColor(0,0,0));

         gui.Display(win , 0 , 0);
         win->FlipDisplay();
         if (!popup_hidden) {
            ptext.Display();
         }
         redraw = false;
      }
      do {
         EagleEvent ev = sys->WaitForSystemEventAndUpdateState();
         
         if (!popup_hidden) {
            if (ptext.HandleEvent(ev) & DIALOG_INPUT_USED) {
               continue;
            }
         }
         
         
         if (ev.type != EAGLE_EVENT_TIMER && ev.type != EAGLE_EVENT_MOUSE_AXES) {
            /// Log non timer and non mouse axes events
            EagleInfo() << "Event " << EagleEventName(ev.type) << " received in main." << std::endl;
         }
         
         if (ev.type == EAGLE_EVENT_KEY_DOWN && ev.keyboard.keycode == EAGLE_KEY_F1) {
            popup_hidden = !popup_hidden;
            if (popup_hidden) {
               ptext.Hide();
            }
            else {
               ptext.Show();
            }
         }
         
         /// Event handling
         if (ev.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         else if (ev.type == EAGLE_EVENT_KEY_DOWN && ev.keyboard.keycode == EAGLE_KEY_ESCAPE) {
            quit = true;
         }
         else if (ev.type == EAGLE_EVENT_TIMER) {
            gui.Update(ev.timer.eagle_timer_source->SPT());
            redraw = true;
         }
         else {
            gui.HandleEvent(ev);
         }
         
         /// Check our gui for messages
         while (gui.HasMessages()) {
            WidgetMsg wmsg = gui.TakeNextMessage();
            EagleLog() << "Widget Message [" << wmsg << "]" << std::endl;
            ///const WidgetMsg scrollbar_msg(this , TOPIC_SCROLL_WIDGET , SCROLL_VALUE_CHANGED);
            if (wmsg.IsMessageTopic(TOPIC_SCROLL_WIDGET , SCROLL_VALUE_CHANGED)) {
               std::string name = wmsg.from->ShortName();
               EagleLog() << StringPrintF("Message from widget %s at %p (h = %p , v = %p)\n" ,
                                           name.c_str() , wmsg.from , &hscroller , &vscroller);
               if (wmsg.from == &hscroller) {
//                  hlabel.SetText(StringPrintF("%d" , hscroller.GetScrollValue()));
               }
               else if (wmsg.from == &vscroller) {
//                  vlabel.SetText(StringPrintF("%d" , vscroller.GetScrollValue()));
               }
            }
         }
         if (gui.FlagOn(NEEDS_REDRAW)) {
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



int main2(int argc , char** argv) {

   EnableLog();
   
   SendOutputToFile("Eagle.log" , "" , false);
   
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
   EagleGraphicsContext* win = sys->GetWindowManager()->CreateEagleWindow("win" , sw , sh , EAGLE_OPENGL | EAGLE_WINDOWED | EAGLE_RESIZABLE);
   
   win->Clear(EagleColor(0,255,255));
   win->FlipDisplay();

   std::string bgfile = "stallions.jpg";
   EagleImage* bg = win->LoadImageFromFile(bgfile);
   if (!bg || (bg && !bg->Valid())) {
      EagleError() << StringPrintF("Failed to load %s from disk.\n" , bgfile.c_str());
      return -1;
   }
   
   /// Setup some custom colors for our margin, border, and padding
   SHAREDOBJECT<WidgetColorset> pwc = GetColorsetByName("Default");
   EAGLE_ASSERT(pwc);
   WidgetColorset& wc = *pwc.get();
   wc[PADCOL] = EagleColor(127,127,127,255);
   wc[BORDCOL] = EagleColor(255,255,255,255);
   wc[MARGCOL] = EagleColor(0,0,0,255);
   
   
   /// Outer root gui
   WidgetHandler gui(win , "WidgetHandler" , "GUI1");
   gui.SetWidgetArea(WIDGETAREA(10 , 15 , 25 , Rectangle(150,150,900,600)) , false);
   
   /// A WidgetMover allows us to move any widgets we want
   WidgetMover wmover("Widget mover");
   wmover.SetWidgetArea(Rectangle(-1000,-1000,1,1) , false);
   wmover.SetHotKey(input_key_held(EAGLE_KEY_LSHIFT) && input_key_press(EAGLE_KEY_ENTER));
   wmover.SetAbilities(true , true);

   gui << wmover;
   
   /// Inner gui
   WidgetHandler gui2(win , "WidgetHandler" , "GUI2");
   gui2.SetupBuffer(800,600,win);
   gui2.SetWidgetArea(WIDGETAREA(5,10,15 , Rectangle(100,30,640,480)) , false);
   gui.AllowMiddleMouseButtonScroll(false);
   gui2.AllowMiddleMouseButtonScroll(true);

   gui << gui2;

   /// We can have transparent backgrounds in our gui
   gui.SetBackgroundColor(EagleColor(127,127,127,255));
   gui2.SetBackgroundColor(EagleColor(127,127,127,255));

   /// A RelativeLayout allows us to keep relative positions and sizes of our widgets
   RelativeLayout rl1("RLAYOUT1");
   rl1.Resize(5);
   rl1.SetLayoutRectangle(0 , LayoutRectangle(0.2 , 0.2 , 0.6 , 0.2));
   rl1.SetLayoutRectangle(1 , LayoutRectangle(0.2 , 0.6 , 0.6 , 0.2));
   rl1.SetLayoutRectangle(2 , LayoutRectangle(0.4 , 0.2 , 0.2 , 0.6));
   rl1.SetLayoutRectangle(3 , LayoutRectangle(0.1 , 0.2 , 0.2 , 0.6));
   rl1.SetLayoutRectangle(4 , LayoutRectangle(0.7 , 0.2 , 0.2 , 0.6));
   
   gui2.SetRootLayout(&rl1);
   
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
   
   EagleLog() << "******* SETUP COMPLETE ********" << std::endl;
         
   bool quit = false;
   bool redraw = true;
   
   int mx = 0;
   int my = 0;
   
   sys->GetSystemTimer()->Start();
   
   while (!quit) {
      if (redraw) {
         win->DrawToBackBuffer();
         win->Clear(EagleColor(0,0,0));
         win->DrawStretched(bg , Rectangle(0 , 0 , sw , sh));
         gui.Display(win , 0 , 0);
///         WidgetBase* hw = gui.GetWidgetAt(mx,my);
         WidgetBase* hw = wmover.GetMoveWidget();
         std::string name = (hw?hw->FullName():"NULL");
         win->DrawTextString(win->DefaultFont() , StringPrintF("Widget at %d,%d is [%s]" , mx , my , name.c_str()) , sw - 10 , sh - win->DefaultFont()->Height() - 5 , EagleColor(0,0,0) , HALIGN_RIGHT , VALIGN_TOP);
         win->FlipDisplay();
         redraw = false;
      }
      do {
         EagleEvent ev = sys->WaitForSystemEventAndUpdateState();
         if (ev.type != EAGLE_EVENT_TIMER && ev.type != EAGLE_EVENT_MOUSE_AXES) {
            /// Log non timer and non mouse axes events
            EagleInfo() << "Event " << EagleEventName(ev.type) << " received in main." << std::endl;
         }
         
         /// Event handling
         if (ev.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         else if (ev.type == EAGLE_EVENT_KEY_DOWN && ev.keyboard.keycode == EAGLE_KEY_ESCAPE) {
            quit = true;
         }
         else if (ev.type == EAGLE_EVENT_TIMER) {
            gui.Update(ev.timer.eagle_timer_source->SPT());
            redraw = true;
         }
         else {
            gui.HandleEvent(ev);
         }
         
         /// Check our gui for messages
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


