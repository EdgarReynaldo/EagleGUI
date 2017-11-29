


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
   
   GuiButton* gbtn1 = new GuiButton("gbtn1");
   GuiButton* gbtn2 = new GuiButton("gbtn2");
   GuiButton* gbtn3 = new GuiButton("gbtn3");
   GuiButton* gbtn4 = new GuiButton("gbtn4");
   BasicButton* bbtn1 = new BasicButton("bbtn1");
   BasicButton* bbtn2 = new BasicButton("bbtn2");
   BasicButton* bbtn3 = new BasicButton("bbtn3");
   BasicButton* bbtn4 = new BasicButton("bbtn4");
   
   gbtn1->SetButtonType(RECTANGLE_BTN , SPRING_BTN , BUTTON_CLASS_PLAIN);
   gbtn2->SetButtonType(ROUNDED_BTN , SPRING_BTN , BUTTON_CLASS_PLAIN);
   gbtn3->SetButtonType(ELLIPSE_BTN , SPRING_BTN , BUTTON_CLASS_PLAIN);
   gbtn4->SetButtonType(CIRCLE_BTN , SPRING_BTN , BUTTON_CLASS_PLAIN);
   gbtn1->SetLabel("GBTN&1");
   gbtn2->SetLabel("GBTN&2");
   gbtn3->SetLabel("GBTN&3");
   gbtn4->SetLabel("GBTN&4");
   gbtn1->SetFont(win->DefaultFont());
   gbtn2->SetFont(win->DefaultFont());
   gbtn3->SetFont(win->DefaultFont());
   gbtn4->SetFont(win->DefaultFont());
//   bbtn1->SetButtonType(RECTANGLE_BTN , SPRING_BTN , BUTTON_CLASS_PLAIN);
//   bbtn2->SetButtonType(ROUNDED_BTN , SPRING_BTN , BUTTON_CLASS_PLAIN);
//   bbtn3->SetButtonType(ELLIPSE_BTN , SPRING_BTN , BUTTON_CLASS_PLAIN);
//   bbtn4->SetButtonType(CIRCLE_BTN , SPRING_BTN , BUTTON_CLASS_PLAIN);

   Input key1 = input_key_press(EAGLE_KEY_1);
   Input key2 = input_key_press(EAGLE_KEY_2);
   Input key3 = input_key_press(EAGLE_KEY_3);
   Input key4 = input_key_press(EAGLE_KEY_4);
   Input key5 = input_key_press(EAGLE_KEY_7);
   Input key6 = input_key_press(EAGLE_KEY_8);
   Input key7 = input_key_press(EAGLE_KEY_9);
   Input key8 = input_key_press(EAGLE_KEY_0);

   gbtn1->SetInputGroup(key1);
   gbtn2->SetInputGroup(key2);
   gbtn3->SetInputGroup(key3);
   gbtn4->SetInputGroup(key4);
   bbtn1->SetInputGroup(key5);
   bbtn2->SetInputGroup(key6);
   bbtn3->SetInputGroup(key7);
   bbtn4->SetInputGroup(key8);
   
   bbtn1->SetButtonType(TOGGLE_BTN);
   bbtn2->SetButtonType(TOGGLE_BTN);
   bbtn3->SetButtonType(TOGGLE_BTN);
   bbtn4->SetButtonType(TOGGLE_BTN);
   
   /// These two statements do nothing - they are a trap
///   btn1->InputKey() = g1;
///   btn2->InputKey() = g2;
   
   GridLayout* gl = new GridLayout(4,2,"gl");
   gl->PlaceWidget(gbtn1 , 0);
   gl->PlaceWidget(gbtn2 , 1);
   gl->PlaceWidget(gbtn3 , 2);
   gl->PlaceWidget(gbtn4 , 3);
   gl->PlaceWidget(bbtn1 , 4);
   gl->PlaceWidget(bbtn2 , 5);
   gl->PlaceWidget(bbtn3 , 6);
   gl->PlaceWidget(bbtn4 , 7);
   
   RelativeLayout* rl = new RelativeLayout("rl");

   gui->SetRootLayout(rl);
   gui->SetBackgroundColor(EagleColor(0,0,0,0));

   rl->AddWidget(gl , LayoutRectangle(0.1 , 0.4 , 0.8 , 0.2));

   gui->SetWidgetArea(0 , 0 , win->Width() , win->Height());
   
   EagleLog() << *gui << std::endl;
   
///   WidgetBase::ClipWidgets(true);
///   WidgetBase::DisplayWidgetArea(true);
   if (!gui) {
      EAGLE_ASSERT(gui);
      return 1;
   }
   bool quit = false;
   bool redraw = true;
   
   sys->GetSystemTimer()->Start();
   
   while (!quit) {
      
      if (redraw) {
         gui->SetFullRedraw();
         
         win->DrawToBackBuffer();
         win->Clear(EagleColor(0 , 0 , 96));

         gui->Display(win , 0 , 0);
/**
         gbtn1->Display(win , 0 , 0);
         gbtn2->Display(win , 0 , 0);
         gbtn3->Display(win , 0 , 0);
         gbtn4->Display(win , 0 , 0);
         bbtn1->Display(win , 0 , 0);
         bbtn2->Display(win , 0 , 0);
         bbtn3->Display(win , 0 , 0);
         bbtn4->Display(win , 0 , 0);
*/
         
         win->DrawTextString(win->DefaultFont() , StringPrintF("GBTN1 %s" , gbtn1->Up()?"UP":"DOWN") , 10 , 10 , EagleColor(255,255,255));
         win->DrawTextString(win->DefaultFont() , StringPrintF("GBTN2 %s" , gbtn2->Up()?"UP":"DOWN") , 10 , 30 , EagleColor(255,255,255));
         win->DrawTextString(win->DefaultFont() , StringPrintF("GBTN3 %s" , gbtn3->Up()?"UP":"DOWN") , 10 , 50 , EagleColor(255,255,255));
         win->DrawTextString(win->DefaultFont() , StringPrintF("GBTN4 %s" , gbtn4->Up()?"UP":"DOWN") , 10 , 70 , EagleColor(255,255,255));
         win->DrawTextString(win->DefaultFont() , StringPrintF("BBTN1 %s" , bbtn1->Up()?"UP":"DOWN") , 10 , 90 , EagleColor(255,255,255));
         win->DrawTextString(win->DefaultFont() , StringPrintF("BBTN2 %s" , bbtn2->Up()?"UP":"DOWN") , 10 , 110 , EagleColor(255,255,255));
         win->DrawTextString(win->DefaultFont() , StringPrintF("BBTN3 %s" , bbtn3->Up()?"UP":"DOWN") , 10 , 130 , EagleColor(255,255,255));
         win->DrawTextString(win->DefaultFont() , StringPrintF("BBTN4 %s" , bbtn4->Up()?"UP":"DOWN") , 10 , 150 , EagleColor(255,255,255));
         
         win->FlipDisplay();
         redraw = false;
      }
      do {
         EagleEvent e;
         e = sys->GetSystemQueue()->WaitForEvent(0);
         
         sys->GetSystemInput()->HandleInputEvent(e);
         
         bool input_used = gui->HandleEvent(e) & DIALOG_INPUT_USED;
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
            EagleInfo() << "Timer tick." << std::endl;
            redraw = true;
         }
      } while (sys->GetSystemQueue()->HasEvent(0));
   }
   
   
   
   return 0;
}


