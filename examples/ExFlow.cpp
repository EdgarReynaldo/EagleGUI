


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
   
   HBoxLayout flow;
//   FlowLayout flow;
   flow.Resize(9);
//   GridLayout flow;
//   flow.ResizeGrid(3,3);
   VBoxLayout vbox("VBOXLayout" , "vbox");
//   FlowLayout vbox("FlowLayout" , "VBox");
   vbox.Resize(9);
//   GridLayout vbox;
//   vbox.ResizeGrid(3,3);
   
//   WidgetMover mover("Widget mover");
   mover.WhiteList(&vbox);

   rl.PlaceWidget(&vbox , 1 , LayoutRectangle(0.3 , 0.3 , 0.7 , 0.7));
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
      win->LoadImageFromFile("Data/Images/Up.png"),
      win->LoadImageFromFile("Data/Images/Down.png")
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
   
   EagleFont* verdana = win->LoadFont("Verdana.ttf" , -8);
   EagleFont* verdana12 = win->LoadFont("Verdana.ttf" , -12);
   
   EAGLE_ASSERT(verdana && verdana->Valid());
   EAGLE_ASSERT(verdana12 && verdana12->Valid());
   
   
   GridLayout rgrids[4];
   
   rgrids[0].ResizeGrid(1 , 8);
   rgrids[1].ResizeGrid(1 , 2);
   rgrids[2].ResizeGrid(1 , 6);
   rgrids[3].ResizeGrid(1 , 4);
   
   GuiButton btns[20];
   BasicRadioButton rbtns[20];
   BasicText rtext[20];
   std::string text[20] = {
      "W",
      "E"
      "N",
      "S",
      "NW",
      "NE",
      "SE",
      "SW",
      "HORIZ",
      "VERT",
      "HLEFT",
      "HCENTER",
      "HRIGHT",
      "VTOP",
      "VCENTER",
      "VBOTTOM",
      "ALIGN",
      "EXPAND",
      "BETWEEN",
      "EVEN"
   };
   
   for (unsigned int i = 0 ; i < 20 ; ++i) {
      btns[i].SetButtonType(ROUNDED_BTN , TOGGLE_BTN , BUTTON_CLASS_HOVER);
      WidgetArea wa = btns[i].GetWidgetArea();
      wa.SetBoxesContract(0,0,5);
      btns[i].SetWidgetArea(wa , false);
      rtext[i].SetupText(text[i] , font);
      rbtns[i].SetWidgets(&btns[i] , &rtext[i]);
   }
   
   RadioGroup radios[5];
   radios[0].SetRadioGroup(std::vector<BasicButton*>({&btns[0] , &btns[1] , &btns[2] , &btns[3] ,
                                                       &btns[4] , &btns[5] , &btns[6] , &btns[7]}) , &btns[0]);
   radios[1].SetRadioGroup(std::vector<BasicButton*>({&btns[8] , &btns[9]}) , &btns[8]);
   radios[2].SetRadioGroup(std::vector<BasicButton*>({&btns[10] , &btns[11] , &btns[12]}) , &btns[12]);
   radios[3].SetRadioGroup(std::vector<BasicButton*>({&btns[13] , &btns[14] , &btns[15]}) , &btns[13]);
   radios[4].SetRadioGroup(std::vector<BasicButton*>({&btns[16] , &btns[17] , &btns[18] , &btns[19]}) , &btns[16]);
   
   sys->GetSystemQueue()->ListenTo(&radios[0]);
   sys->GetSystemQueue()->ListenTo(&radios[1]);
   sys->GetSystemQueue()->ListenTo(&radios[2]);
   sys->GetSystemQueue()->ListenTo(&radios[3]);
   sys->GetSystemQueue()->ListenTo(&radios[4]);
   
   rl.AddWidget(&rgrids[0] , LayoutRectangle(0.05 , 0.05 , 0.2 , 0.3));
   rl.AddWidget(&rgrids[1] , LayoutRectangle(0.45 , 0.05 , 0.2 , 0.2));
   rl.AddWidget(&rgrids[2] , LayoutRectangle(0.05 , 0.4 , 0.2 , 0.5));
   rl.AddWidget(&rgrids[3] , LayoutRectangle(0.75 , 0.05 , 0.2 , 0.2));
   
   rgrids[0].PlaceWidget(&rbtns[0] , 0);
   rgrids[0].PlaceWidget(&rbtns[1] , 1);
   rgrids[0].PlaceWidget(&rbtns[2] , 2);
   rgrids[0].PlaceWidget(&rbtns[3] , 3);
   rgrids[1].PlaceWidget(&rbtns[4] , 0);
   rgrids[1].PlaceWidget(&rbtns[5] , 1);
   rgrids[2].PlaceWidget(&rbtns[6] , 0);
   rgrids[2].PlaceWidget(&rbtns[7] , 1);
   rgrids[2].PlaceWidget(&rbtns[8] , 2);
   rgrids[2].PlaceWidget(&rbtns[9] , 3);
   rgrids[2].PlaceWidget(&rbtns[10] , 4);
   rgrids[2].PlaceWidget(&rbtns[11] , 5);
   rgrids[3].PlaceWidget(&rbtns[12] , 0);
   rgrids[3].PlaceWidget(&rbtns[13] , 1);
   rgrids[3].PlaceWidget(&rbtns[14] , 2);
   rgrids[3].PlaceWidget(&rbtns[15] , 3);
   
   
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
         if (e.type == EAGLE_EVENT_WIDGET) {
            if (e.source == &radios[0]) {
               for (unsigned int i = 0 ; i < 8 ; ++i) {
                  if (e.widget.from == &btns[i]) {
                     vbox.SetAnchorPosition((BOX_ANCHOR_POINT)i);
                  }
               }
            }
            if (e.source == &radios[1]) {
               for (unsigned int i = 8 ; i < 10 ; ++i) {
                  if (e.widget.from == &btns[i]) {
                     vbox.SetAnchorPosition((BOX_ANCHOR_POINT)(i - 8));
                  }
               }
            }
            if (e.source == &radios[2]) {
               for (unsigned int i = 10 ; i < 13 ; ++i) {
                  if (e.widget.from == &btns[i]) {
                     vbox.SetAlignment((HALIGNMENT)(i - 10) , vbox.GetVAlignment());
                  }
               }
            }
            if (e.source == &radios[3]) {
               for (unsigned int i = 13 ; i < 16 ; ++i) {
                  if (e.widget.from == &btns[i]) {
                     vbox.SetAlignment(vbox.GetHAlignment() , (VALIGNMENT)(i - 13));
                  }
               }
            }
            if (e.source == &radios[4]) {
               for (unsigned int i = 16 ; i < 20 ; ++i) {
                  if (e.widget.from == &btns[i]) {
                     vbox.SetBoxSpacing((BOX_SPACE_RULES)(i-16));
                  }
               }
            }
         }

         if (e.type == EAGLE_EVENT_TIMER) {
            redraw = true;
         }
         if (e.type == EAGLE_EVENT_KEY_DOWN) {
            if (e.keyboard.keycode == EAGLE_KEY_ESCAPE) {
               quit = true;
               break;
            }
            if (e.keyboard.keycode == EAGLE_KEY_P) {
               EagleLog() << vbox << std::endl;
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


