




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
   
   font->SetName("Verdana20");
   
   gui.SetupBuffer(w , h , win);
   gui.SetWidgetArea(0 , 0 , w , h , false);
   gui.SetRootLayout(&relative_layout);
   
   quit_button = CreateWidget<TextButton>("TextButton" , "DIM:200,50 ; FONT:Verdana20 ; TEXT:Quit ; NAME:QuitButton");
   EAGLE_ASSERT(quit_button);

///   basic_button = new BasicButton();
   
   relative_layout.AddWidget(quit_button->GetDecoratorRoot() , LayoutRectangle(0.1 , 0.4 , 0.3 , 0.2));
   relative_layout.AddWidget(&wedge_layout , LayoutRectangle(0.0 , 0.0 , 1.0 , 1.0));
   
   vector<const Test*> tests = TestRegistry::GetRegistryInstance().GetRegisteredTests();
   
   wedge_layout.Resize(tests.size());
   wedge_layout.SetAlignment(HALIGN_CENTER , VALIGN_CENTER);
   wedge_layout.SetAnchors(Pos2d(3*w/4 , h/2) , Pos2d(w/4 , h/4) , Pos2d(w/4 , 3*h/4));

   buttons.resize(tests.size());

   for (int i = 0 ; i < (int)tests.size() ; ++i) {
      TextButton* btn = CreateWidget<TextButton>("TextButton" , "DIM:200,50 ; FONT:Verdana20");
      EAGLE_ASSERT(btn);
      btn->SetText(tests[i]->Name());
      wedge_layout.PlaceWidget(btn , i);
   }
   
   
   EagleLog() << gui << std::endl << std::endl;
   
}



void TestMenu::Run() {
   sys->GetSystemQueue()->Clear();
   sys->GetSystemTimer()->Start();
   selected = false;
   bool redraw = true;
   bool clip = true;
   bool debug_draw = true;
   WidgetBase::DisplayWidgetArea(debug_draw);
   while (!quit) {
      /// Display
      
      if (redraw) {
         gui.Display(win , 0 , 0);
         
         if (gui.CurrentHover()) {
            win->DrawTextString(font , StringPrintF("Hover is %p (%s)" , gui.CurrentHover() , gui.CurrentHover()->GetName().c_str()),
                                win->Width() - 10 , 10 , EagleColor(255,255,255) , HALIGN_RIGHT);
         }
         
         win->DrawTextString(font , status_message.c_str() , win->Width()/2 , win->Height() - font->Height() - 10 ,
                             EagleColor(255,255,255) , HALIGN_CENTER , VALIGN_TOP);
         
         win->FlipDisplay();
         redraw = false;
      }

      /// Handle events
      do {
         
         EagleEvent ev = sys->WaitForSystemEventAndUpdateState();
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
                  }
               }
            }
         }
      } while (!sys->UpToDate());
      if (selected) {
         break;
      }
      
   }
}



void TestMenu::SetStatusMessage(std::string status) {
   status_message = status;
}
