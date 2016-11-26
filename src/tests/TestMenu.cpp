




#include "TestMenu.hpp"

#include "Eagle/Gui/Factory/WidgetFactory.hpp"


void AnimatedWedgeLayout::OnSetAnimationPercent() {
   if (animation_running) {
      move_percent = animation_percent;
      RepositionAllChildren();
   }
}



void AnimatedWedgeLayout::OnLoopComplete() {}



void AnimatedWedgeLayout::OnComplete() {
   if (animation_running) {
      animation_running = false;
      CyclePins(direction_forward);
   }
}


void AnimatedWedgeLayout::SetAnchors(Pos2d head , Pos2d tail1 , Pos2d tail2) {
   head_pt = head;
   tail_pt1 = tail1;
   tail_pt2 = tail2;
   ResetPositions();
}



void AnimatedWedgeLayout::ResetPositions() {
   int sz = GetLayoutSize();
   if (!sz) {return;}
   if (sz == 1) {
      SetPinPosition(0 , head_pt.X() , head_pt.Y());
      return;
   }
   
   /// Widgets are laid out in a wedge from tail 1 to head to tail 2, spread out evenly
   Pos2d vec1 = head_pt - tail_pt1;
   Pos2d vec2 = tail_pt2 - head_pt;
   
   int half = sz/2 + 1;
   for (int i = 0 ; i < half ; ++i) {
      double pct = i/(double)(half - 1);
      Pos2d v = tail_pt1 + vec1*pct;
      SetPinPosition(i , v.X() , v.Y());
   }
   for (int i = half  ; i < sz ; ++i) {
      double pct = (i - half + 1)/(double)(sz - half);
      Pos2d v = head_pt + vec2*pct;
      SetPinPosition(i , v.X() , v.Y());
   }
   RepositionAllChildren();
}



Rectangle AnimatedWedgeLayout::RequestWidgetArea(int widget_slot , int newx , int newy , int newwidth , int newheight) {
   /// We need to interpolate between the old and new position. Pins hold the old position, and the new position
   /// is one pin away (the previous or next pin depending on direction).
   
   if (animation_running) {
      WidgetBase* widget = GetWidget(widget_slot);
      if (!widget) {
         return Rectangle(-1,-1,-1,-1);
      }
      
      Rectangle current = widget->OuterArea();

      int dir = direction_forward?1:-1;
      Pos2d start = GetPin(widget_slot).GetPosition();
      Pos2d next = GetPin(widget_slot + dir).GetPosition();
      
      Pos2d newpos = start + move_percent*(next - start);
      
      Pin& pin = pins[widget_slot];
      if (newx != INT_MAX) {
         pin.px = newx;
      }
      if (newy != INT_MAX) {
         pin.py = newy;
      }
      if (newwidth == INT_MAX) {
         newwidth = current.W();
      }
      if (newheight == INT_MAX) {
         newheight = current.H();
      }
      if (newx == INT_MAX && newy == INT_MAX) {
         Rectangle newrect(newpos.X() , newpos.Y() , current.W() , current.H());
         return newrect;
      }
      return pin.GetPinArea(newwidth , newheight);
   }
   else {
      return PinLayout::RequestWidgetArea(widget_slot , newx , newy , newwidth , newheight);
   }
}



int AnimatedWedgeLayout::PrivateHandleEvent(EagleEvent e) {
   /// If key down or key up, start animation
   if (!animation_running) {
      if (e.type == EAGLE_EVENT_KEY_DOWN) {
         if (e.keyboard.keycode == EAGLE_KEY_UP || e.keyboard.keycode == EAGLE_KEY_DOWN) {
            ResetAnimation();
            animation_running = true;
            if (e.keyboard.keycode == EAGLE_KEY_UP) {
               direction_forward = false;
            }
            if (e.keyboard.keycode == EAGLE_KEY_DOWN) {
               direction_forward = true;
            }
            return DIALOG_INPUT_USED;
         }
      }
   }
   return DIALOG_OKAY;
}



int AnimatedWedgeLayout::Update(double tsec) {
   if (animation_running) {
      AdvanceAnimationTime(tsec);
   }
   return DIALOG_OKAY;
}



/// --------------------------     TestMenu     ----------------------------------



TestMenu::TestMenu(EagleGraphicsContext* window) :
      win(window),
      gui(),
      wedge_layout(),
      relative_layout(),
      buttons(),
      quit_button(0),
      font(0),
      quit(false),
      selected(false),
      selected_user_branch("")
{
   EAGLE_ASSERT(win);
   EAGLE_ASSERT(win->Valid());

   int w = win->Width();
   int h = win->Height();
   
   font = win->LoadFont("Verdana.ttf" , -20);
   EAGLE_ASSERT(font->Valid());
   
   font->SetName("Verdana20");
   
   gui.SetupBuffer(w , h , win);
   gui.SetWidgetArea(0 , 0 , w , h , false);
   gui.SetRootLayout(&relative_layout);
   
   relative_layout.AddWidget(quit_button , LayoutRectangle(0.1 , 0.4 , 0.4 , 0.2));
   relative_layout.AddWidget(&wedge_layout , LayoutRectangle(0.0 , 0.0 , 1.0 , 1.0));
   
   vector<const Test*> tests = TestRegistry::GetRegistryInstance().GetRegisteredTests();
   
   wedge_layout.SetAlignment(HALIGN_CENTER , VALIGN_CENTER);
   wedge_layout.SetAnchors(Pos2d(3*w/4 , h/2) , Pos2d(w/4 , h/4) , Pos2d(w/4 , 3*h/4));
   wedge_layout.Resize(tests.size());
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
   while (!quit) {
      do {
         
         EagleEvent ev = sys->WaitForSystemEventAndUpdateState();
         if (ev.type == EAGLE_EVENT_TIMER) {
            gui.Update(sys->GetSystemTimer()->SPT());
         }
         else if (ev.type == EAGLE_EVENT_DISPLAY_CLOSE || (ev.type == EAGLE_EVENT_KEY_DOWN && ev.keyboard.keycode == EAGLE_KEY_ESCAPE)) {
            quit = true;
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
      /// Display
      
      gui.Display(win , 0 , 0);
      win->FlipDisplay();
      
   }
}
