


#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"
#include "allegro5/allegro.h"


int main(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   
   Allegro5System* sys = GetAllegro5System();
   
   if (sys->Initialize(EAGLE_FULL_SETUP) != EAGLE_FULL_SETUP) {
      EagleWarn() << "Some subsystems not initialized. Proceeding" << std::endl;
   }
   
   int sw = 800;
   int sh = 600;
   
   EagleGraphicsContext* win = sys->CreateGraphicsContext("Main Window" , sw , sh , EAGLE_OPENGL | EAGLE_WINDOWED | EAGLE_RESIZABLE);
   
   EAGLE_ASSERT(win && win->Valid());
   
   EagleFont* font = win->LoadFont("Verdana.ttf" , -20);
   
   EAGLE_ASSERT(font && font->Valid());
   
   
   
   
   WidgetHandler gui(win , "GUI" , "Example GUI");
   
   gui.SetupBuffer(sw , sh , win);
   
   gui.SetWidgetArea(Rectangle(0 , 0 , sw , sh));

   gui.GetWidgetPainter()->bgtype = BG_AREA_CSSMODEL;
   
   
   WidgetMover mover;
   mover.SetHotKey(input_key_held(EAGLE_KEY_ANY_CTRL) && input_key_press(EAGLE_KEY_M));
   
   sys->GetSystemQueue()->ListenTo(&mover);
   
   RelativeLayout rl("RelativeLayout" , "RLayout");
   
   gui.SetRootLayout(&rl);
   
   rl.Resize(2);
   
   
   FlowLayout flow;
   flow.Resize(9);
//   GridLayout flow;
//   flow.ResizeGrid(3,3);
   
   FlowLayout vbox("FlowLayout" , "VBox");
   vbox.Resize(9);
//   GridLayout vbox;
//   vbox.ResizeGrid(3,3);
   
//   WidgetMover mover("Widget mover");
   mover.WhiteList(&vbox);

   rl.PlaceWidget(&vbox , 1 , LayoutRectangle(0.1 , 0.1 , 0.8 , 0.8));
   WidgetArea wa = vbox.GetWidgetArea();
   wa.SetBoxesContract(5,10,5);
   vbox.SetWidgetArea(wa , false);
   WidgetColorset wcol = default_eagle_color_array;
   wcol[MARGCOL] = EagleColor(0,255,0);
   wcol[BORDCOL] = EagleColor(255,255,255);
   wcol[PADCOL] = EagleColor(0,0,0,0);
   vbox.SetWidgetColorset(wcol);
   
///   rl.PlaceWidget(&vbox , 1 , LayoutRectangle(0.1 , 0.1 , 0.2 , 0.6));
   rl.PlaceWidget(&mover , 0 , LayoutRectangle(-1.0 , -1.0 , 0.1 , 0.1));
   
   EagleImage* btnimgs[2] = {
      win->LoadImageFromFile("Up.png"),
      win->LoadImageFromFile("Down.png")
   };
   
   
   std::map<EAGLE_ID , WidgetBase*> rwidgets;
   std::vector<WidgetBase*> widgets = {0,0,0,0,0,0,0,0,0};
   std::vector<WidgetBase*> uwidgets = {0,0,0,0,0,0,0,0,0};
   
   /// Add three buttons
   GuiButton* w[3] = {
      new GuiButton() , new GuiButton() , new GuiButton()
   };
   w[0]->SetButtonType(ROUNDED_BTN , TOGGLE_BTN , BUTTON_CLASS_HOVER);
   w[1]->SetButtonType(ELLIPSE_BTN , TOGGLE_BTN , BUTTON_CLASS_HOVER);
   w[2]->SetButtonType(CIRCLE_BTN  , SPRING_BTN , BUTTON_CLASS_HOVER);
   w[0]->SetPreferredSize(100,20);
   w[1]->SetPreferredSize(120,20);
   w[2]->SetPreferredSize(40,40);
   w[0]->SetLabel("Label 0");
   w[0]->SetFont(font);
   w[1]->SetLabel("Label 1");
   w[1]->SetFont(font);
   w[2]->SetLabel("Label 2");
   w[2]->SetFont(font);
   rwidgets[w[0]->GetEagleId()] = w[0];
   rwidgets[w[1]->GetEagleId()] = w[1];
   rwidgets[w[2]->GetEagleId()] = w[2];
   widgets[0] = w[0];
   widgets[1] = w[1];
   widgets[2] = w[2];
   vbox.PlaceWidget(w[0] , 0);
   vbox.PlaceWidget(w[1] , 1);
   vbox.PlaceWidget(w[2] , 2);

   HoverIcon* ibtn[3] = {
      new HoverIcon() , new HoverIcon() , new HoverIcon()
   };
   ibtn[0]->SetPreferredSize(64,64);
   ibtn[1]->SetPreferredSize(96,96);
   ibtn[2]->SetPreferredSize(32,32);
   ibtn[0]->SetImages(btnimgs[0] , btnimgs[1] , btnimgs[0] , btnimgs[1]);
   ibtn[1]->SetImages(btnimgs[0] , btnimgs[1] , btnimgs[0] , btnimgs[1]);
   ibtn[2]->SetImages(btnimgs[0] , btnimgs[1] , btnimgs[0] , btnimgs[1]);
   rwidgets[ibtn[0]->GetEagleId()] = ibtn[0];
   rwidgets[ibtn[1]->GetEagleId()] = ibtn[1];
   rwidgets[ibtn[2]->GetEagleId()] = ibtn[2];
   widgets[3] = ibtn[0];
   widgets[4] = ibtn[1];
   widgets[5] = ibtn[2];
   vbox.PlaceWidget(ibtn[0] , 3);
   vbox.PlaceWidget(ibtn[1] , 4);
   vbox.PlaceWidget(ibtn[2] , 5);

   Slider* sliders[3] = {
      new Slider("Slider" , "Slider1" , true , false),
      new Slider("Slider" , "Slider2" , true , false),
      new Slider("Slider" , "Slider3" , true , false)
   };
   sliders[0]->SetPreferredSize(20 , 60);
   sliders[1]->SetPreferredSize(30 , 80);
   sliders[2]->SetPreferredSize(40 , 100);
   rwidgets[sliders[0]->GetEagleId()] = sliders[0];
   rwidgets[sliders[1]->GetEagleId()] = sliders[1];
   rwidgets[sliders[2]->GetEagleId()] = sliders[2];
   widgets[6] = sliders[0];
   widgets[7] = sliders[1];
   widgets[8] = sliders[2];
   vbox.PlaceWidget(sliders[0] , 6);
   vbox.PlaceWidget(sliders[1] , 7);
   vbox.PlaceWidget(sliders[2] , 8);


   std::vector<WidgetBase*> wc = vbox.WChildren();
   for (unsigned int i = 0 ; i < wc.size() ; ++i) {
      WidgetArea warea = wc[i]->GetWidgetArea();
      warea.SetBoxesContract(1,2,1);
      wc[i]->SetWidgetArea(warea , false);
      wc[i]->SetRedrawFlag();
   }
   
   
   WidgetColorset wcolset = *(vbox.GetWidgetColorset().get());
   
   EagleInfo() << wcolset << std::endl;
   
   sys->GetSystemTimer()->Start();
   
   bool quit = false;
   bool redraw = true;
   WidgetBase* hover = 0;
   while (!quit) {
      if (redraw) {
         win->SetDrawingTarget(win->GetBackBuffer());
         win->Clear();
         gui.Display(win , 0 , 0);
         if (hover) {
            win->DrawTextString(win->DefaultFont() , hover->ShortName() , 10 , 10 , EagleColor(255,255,255));
         }
         win->FlipDisplay();
         redraw = false;
      }
      while (!sys->UpToDate()) {
         EagleEvent e = sys->WaitForSystemEventAndUpdateState();
         gui.HandleEvent(e);
         while (gui.HasMessages()) {
            WidgetMsg msg = gui.TakeNextMessage();
            (void)msg;
         }
         if (e.type == EAGLE_EVENT_TIMER) {
            redraw = true;
         }
         if (e.type == EAGLE_EVENT_KEY_DOWN) {
            if (e.keyboard.keycode == EAGLE_KEY_ESCAPE) {
               quit = true;
               break;
            }
         }
         if (e.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
            break;
         }
         if (e.type == EAGLE_EVENT_MOUSE_AXES) {
            hover = gui.GetWidgetAt(e.mouse.x , e.mouse.y);
         }
         if (e.type == EAGLE_EVENT_WIDGET_DRAG_AND_DROP) {
            EagleInfo() << "DND event" << std::endl;
            if (e.widget.topic == TOPIC_WIDGET_MOVER && e.widget.msgs == WIDGET_MOVER_SIZING) {
               DRAG_AND_DROP_DATA dnd = e.widget.dnd;
               EagleInfo() << "Sizing event" << std::endl;
               vbox.SetWidgetArea(Rectangle(dnd.dropx , dnd.dropy , dnd.dropw , dnd.droph));
            }
         };
         for (int i = 0 ; i < 9 ; ++i) {
            if (input_key_press(EAGLE_KEY_1 + i)) {
               if (!input_key_held(EAGLE_KEY_ANY_SHIFT)) {
                  /// Key pressed without shift, move to flow layout
                  if (!uwidgets[i]) {
                     uwidgets[i] = widgets[i];
                     widgets[i] = 0;
                     vbox.RemoveWidget(uwidgets[i]);
                     flow.AddWidget(uwidgets[i]);
                  }
               }
               else {
                  /// Shift is held, remove from flow layout
                  if (!widgets[i]) {
                     widgets[i] = uwidgets[i];
                     uwidgets[i] = 0;
                     flow.RemoveWidget(widgets[i]);
                     vbox.AddWidget(widgets[i]);
                  }
               }
            }
         }
      }
   }
   
   
      
   return 0;
}


