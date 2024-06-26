




#include "TestMenu.hpp"

#include "Eagle/Gui/Factory/WidgetFactory.hpp"

/// --------------------------     TestMenu     ----------------------------------



TestMenu::TestMenu(EagleGraphicsContext* window) :
      win(window),
      gui(window),
      wedge_layout(),
      relative_layout(),
      buttons(),
      quit_button(0),
      font(0),
      quit(false),
      selected(false),
      selected_user_branch(""),
      status_message("Ready")
{
   EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());

   int w = win->Width();
   int h = win->Height();
   
   font = win->LoadFont("Verdana.ttf" , -20 , LOAD_FONT_MONOCHROME);
   EAGLE_ASSERT(font->Valid());
   
   font->SetShortName("Verdana20");
   
   gui.SetupBuffer(w , h , win);
   gui.SetWidgetArea(0 , 0 , w , h , false);
   gui.SetRootLayout(&relative_layout);
   
   quit_button = CreateWidget<TextButton>("TextButton" , "quit_button" , "DIM:200,50 ; FONT:Verdana20 ; TEXT:Quit ; NAME:QuitButton");
   EAGLE_ASSERT(quit_button);

///   basic_button = new BasicButton();
   
   relative_layout.AddWidget(quit_button->GetDecoratorRoot() , LayoutRectangle(0.1 , 0.4 , 0.3 , 0.2));
   relative_layout.AddWidget(&wedge_layout , LayoutRectangle(0.0 , 0.0 , 1.0 , 1.0));
   
   vector<const Test*> tests = TestRegistry::GetRegistryInstance().GetRegisteredTests();
   
   wedge_layout.Resize(tests.size());
   wedge_layout.SetAlignment(HALIGN_CENTER , VALIGN_CENTER);
   wedge_layout.SetAnchors(Pos2I(3*w/4 , h/2) , Pos2I(w/2 , h/8) , Pos2I(w/2 , 7*h/8));

   buttons.resize(tests.size());

   for (int i = 0 ; i < (int)tests.size() ; ++i) {
      TextButton* btn = CreateWidget<TextButton>("TextButton" , StringPrintF("btn%d" , i) , "DIM:200,50 ; FONT:Verdana20");
      EAGLE_ASSERT(btn);
      btn->SetText(tests[i]->Name());
      btn->SetMarginsContractFromOuter(5,5,5,5);
      wedge_layout.PlaceWidget(btn->GetTextDecorator() , i);
   }
   
   
///   EagleLog() << gui << std::endl << std::endl;
   
}



void TestMenu::Run() {
   
   EagleEventHandler* test_queue = sys->CreateEventHandler("TestMenu::Run() : test_queue" , false);

   test_queue->ListenTo(sys->GetSystemQueue());
      
   
   sys->GetSystemTimer()->Start();
   selected = false;
   bool redraw = true;
   bool clip = true;
   bool debug_draw = true;
   WidgetBase::DisplayWidgetArea(debug_draw);

   vector<TestRunner*> running_tests;
   
   while (!quit) {
      /// Display
      
      if (redraw) {
         gui.Display(win , 0 , 0);
         
         if (gui.CurrentHover()) {
            win->DrawTextString(font , StringPrintF("Hover is %p (%s)" , gui.CurrentHover() , gui.CurrentHover()->ShortName()),
                                win->Width() - 10 , 10 , EagleColor(255,255,255) , HALIGN_RIGHT);
         }
         
         win->DrawTextString(font , status_message.c_str() , win->Width()/2 , win->Height() - font->Height() - 10 ,
                             EagleColor(255,255,255) , HALIGN_CENTER , VALIGN_TOP);
                             
         win->DrawTextString(font , StringPrintF("active_window = %p (win = %p)" , sys->GetActiveWindow() , win).c_str() ,
                             win->Width()/2 , 10 , EagleColor(255,255,255) , HALIGN_CENTER , VALIGN_TOP);
         
         win->FlipDisplay();
         redraw = false;
      }

      /// Handle events
      do {
         
         EagleEvent ev = test_queue->WaitForEvent(0);
         
         if (ev.type != EAGLE_EVENT_TIMER && ev.type != EAGLE_EVENT_MOUSE_AXES) {
            EagleLog() << StringPrintF("Received event %d (%s) on window %p" ,
                                        ev.type , EagleEventName(ev.type).c_str() , ev.window).c_str() << std::endl;
         }
         if (ev.window == win) {
            if (ev.type != EAGLE_EVENT_TIMER && ev.type != EAGLE_EVENT_MOUSE_AXES) {
               EagleLog() << StringPrintF("Handling event %d (%s)" , ev.type , EagleEventName(ev.type).c_str()).c_str() << std::endl;
            }
            sys->GetSystemInput()->HandleInputEvent(ev);
         }
         else {
            continue;
         }
         
         if (ev.type == EAGLE_EVENT_TIMER) {
            gui.Update(sys->GetSystemTimer()->SPT());
            redraw = true;
         }
         else if (ev.type == EAGLE_EVENT_DISPLAY_CLOSE || (ev.type == EAGLE_EVENT_KEY_DOWN && ev.keyboard.keycode == EAGLE_KEY_ESCAPE)) {
            quit = true;
         }
         else if (ev.type == EAGLE_EVENT_KEY_DOWN && ev.keyboard.keycode == EAGLE_KEY_C) {
            WidgetBase::ClipWidgets((clip = !clip));
            gui.SetFullRedraw();
            redraw = true;
         }
         else if (ev.type == EAGLE_EVENT_KEY_DOWN && ev.keyboard.keycode == EAGLE_KEY_D) {
            WidgetBase::DisplayWidgetArea((debug_draw = !debug_draw));
            gui.SetFullRedraw();
            redraw = true;
         }
         else if (ev.type == EAGLE_EVENT_KEY_DOWN && ev.keyboard.keycode == EAGLE_KEY_SPACE) {
            EagleWarn() << *(quit_button->GetTextDecorator()) << std::endl;
            redraw = true;
         }
         else {
            gui.HandleEvent(ev);
            while (gui.HasMessages()) {
               WidgetMsg msg = gui.TakeNextMessage();
               if (msg.Topic() == TOPIC_BUTTON_WIDGET && msg.Message() == BUTTON_CLICKED) {
                  if (msg.From() == quit_button) {
                     quit = true;
                  }
                  else {
                     /// One of our buttons was clicked
                     selected = true;
                     selected_user_branch = dynamic_cast<TextButton*>(msg.From())->GetText();
                     Test* t = TestRegistry::GetRegistryInstance().GetTest(selected_user_branch);
                     if (t) {
                        TestRunner* runner = new TestRunner(win , t->MainFunc());
                        running_tests.push_back(runner);
                     }
                  }
               }
            }
         }
      } while (test_queue->HasEvent(0));
   }
   
   for (int i = 0 ; i < (int)running_tests.size() ; ++i) {
      delete running_tests[i];
   }
   
   
}



void TestMenu::SetStatusMessage(std::string status) {
   status_message = status;
}
