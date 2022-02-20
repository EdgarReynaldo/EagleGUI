


#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"
#include "allegro5/allegro.h"


int main(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   SendOutputToFile("ExFlow.log" , "" , false);
   
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
   
   rl.Resize(8);
   
   HBoxLayout hbox("HBOXLayout" , "hbox");
   VBoxLayout vbox("VBOXLayout" , "vbox");
   FlowLayout flowbox("FLOWLayout" , "flow");
   hbox.Resize(9);
   vbox.Resize(9);
   flowbox.Resize(9);
   
//   WidgetMover mover("Widget mover");
   mover.WhiteList(&hbox);
   mover.WhiteList(&vbox);
   mover.WhiteList(&flowbox);

   WidgetColorset wcol = default_eagle_color_array;
   wcol[MARGCOL] = EagleColor(0,0,96,0);
   wcol[BORDCOL] = EagleColor(255,255,255,255);
   wcol[PADCOL] = EagleColor(0,0,96,255);
   vbox.SetWidgetColorset(wcol);
   hbox.SetWidgetColorset(wcol);
   flowbox.SetWidgetColorset(wcol);
   
   hbox.SetWidgetArea(hbox.GetWidgetArea().SetBoxesContract(2,4,4) , false);
   vbox.SetWidgetArea(vbox.GetWidgetArea().SetBoxesContract(2,4,4) , false);
   flowbox.SetWidgetArea(flowbox.GetWidgetArea().SetBoxesContract(2,4,4) , false);
   
   
   
///   rl.PlaceWidget(&vbox , 1 , LayoutRectangle(0.1 , 0.1 , 0.2 , 0.6));
   rl.PlaceWidget(&mover , 0 , LayoutRectangle(-1.0 , -1.0 , 0.1 , 0.1));
   
   EagleImage* btnimgs[2] = {
      win->LoadImageFromFile("Data/Images/Up.png"),
      win->LoadImageFromFile("Data/Images/Down.png")
   };
   
   
   std::vector<WidgetBase*> widgets(9 , 0);/// Track our content widgets
   
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
   widgets[0] = w[0];
   widgets[1] = w[1];
   widgets[2] = w[2];

   HoverIcon* ibtn[3] = {
      new HoverIcon() , new HoverIcon() , new HoverIcon()
   };
   ibtn[0]->SetPreferredSize(64,64);
   ibtn[1]->SetPreferredSize(96,96);
   ibtn[2]->SetPreferredSize(32,32);
   ibtn[0]->SetImages(btnimgs[0] , btnimgs[1] , btnimgs[0] , btnimgs[1]);
   ibtn[1]->SetImages(btnimgs[0] , btnimgs[1] , btnimgs[0] , btnimgs[1]);
   ibtn[2]->SetImages(btnimgs[0] , btnimgs[1] , btnimgs[0] , btnimgs[1]);
   widgets[3] = ibtn[0];
   widgets[4] = ibtn[1];
   widgets[5] = ibtn[2];

   Slider* sliders[3] = {
      new Slider("Slider" , "Slider1" , true , false),
      new Slider("Slider" , "Slider2" , true , false),
      new Slider("Slider" , "Slider3" , true , false)
   };
   sliders[0]->SetPreferredSize(20 , 60);
   sliders[1]->SetPreferredSize(30 , 80);
   sliders[2]->SetPreferredSize(40 , 100);
   widgets[6] = sliders[0];
   widgets[7] = sliders[1];
   widgets[8] = sliders[2];

//   EagleFont* verdana = win->LoadFont("Verdana.ttf" , -8);
//   EagleFont* verdana12 = win->LoadFont("Verdana.ttf" , -12);
   
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
      "E",
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
      btns[i].SetButtonType(RECTANGLE_BTN , TOGGLE_BTN , BUTTON_CLASS_HOVER);
      btns[i].SetWidgetArea(btns[i].GetWidgetArea().SetBoxesContract(2,2,2) , false);
      btns[i].SetFont(font);
      btns[i].SetLabel(text[i]);
      
      SHAREDOBJECT<WidgetColorset> wc = btns[i].GetWidgetColorset();
      (*wc.get())[PADCOL] = EagleColor(0,0,0,0);
      (*wc.get())[MARGCOL] = EagleColor(0,0,0,0);
      (*wc.get())[BORDCOL] = EagleColor(255,255,255,255);
      (*wc.get())[HVRCOL] = EagleColor(192,255,255,127);
      (*wc.get())[MGCOL] = EagleColor(96,152,152,127);
      (*wc.get())[TXTCOL] = EagleColor(255,255,255,255);
   }
   
   RadioGroup radios[7];
   radios[0].SetRadioGroup(std::vector<BasicButton*>({&btns[0] , &btns[1]}) , &btns[0]);
   radios[1].SetRadioGroup(std::vector<BasicButton*>({&btns[2] , &btns[3]}) , &btns[2]);
   radios[2].SetRadioGroup(std::vector<BasicButton*>({&btns[4] , &btns[5] , &btns[6] , &btns[7]}) , &btns[4]);
   radios[3].SetRadioGroup(std::vector<BasicButton*>({&btns[8] , &btns[9]}) , &btns[8]);
   radios[4].SetRadioGroup(std::vector<BasicButton*>({&btns[10] , &btns[11] , &btns[12]}) , &btns[10]);
   radios[5].SetRadioGroup(std::vector<BasicButton*>({&btns[13] , &btns[14] , &btns[15]}) , &btns[13]);
   radios[6].SetRadioGroup(std::vector<BasicButton*>({&btns[16] , &btns[17] , &btns[18] , &btns[19]}) , &btns[16]);
   
   sys->GetSystemQueue()->ListenTo(&radios[0]);
   sys->GetSystemQueue()->ListenTo(&radios[1]);
   sys->GetSystemQueue()->ListenTo(&radios[2]);
   sys->GetSystemQueue()->ListenTo(&radios[3]);
   sys->GetSystemQueue()->ListenTo(&radios[4]);
   sys->GetSystemQueue()->ListenTo(&radios[5]);
   sys->GetSystemQueue()->ListenTo(&radios[6]);
   
   rl.AddWidget(&rgrids[0] , LayoutRectangle(0.05 , 0.05 , 0.2 , 0.4));
   rl.AddWidget(&rgrids[1] , LayoutRectangle(0.45 , 0.05 , 0.2 , 0.2));
   rl.AddWidget(&rgrids[2] , LayoutRectangle(0.05 , 0.45 , 0.2 , 0.45));
   rl.AddWidget(&rgrids[3] , LayoutRectangle(0.75 , 0.05 , 0.2 , 0.2));
   
   rl.AddWidget(&hbox , LayoutRectangle(0.4 , 0.25 , 0.5 , 0.1));
   rl.AddWidget(&vbox , LayoutRectangle(0.25 , 0.4 , 0.1 , 0.5));
   rl.AddWidget(&flowbox , LayoutRectangle(0.4 , 0.4 , 0.5 , 0.5));
   
   rgrids[0].PlaceWidget(&btns[0] , 0);
   rgrids[0].PlaceWidget(&btns[1] , 1);
   rgrids[0].PlaceWidget(&btns[2] , 2);
   rgrids[0].PlaceWidget(&btns[3] , 3);
   rgrids[0].PlaceWidget(&btns[4] , 4);
   rgrids[0].PlaceWidget(&btns[5] , 5);
   rgrids[0].PlaceWidget(&btns[6] , 6);
   rgrids[0].PlaceWidget(&btns[7] , 7);
   rgrids[1].PlaceWidget(&btns[8] , 0);
   rgrids[1].PlaceWidget(&btns[9] , 1);
   rgrids[2].PlaceWidget(&btns[10] , 0);
   rgrids[2].PlaceWidget(&btns[11] , 1);
   rgrids[2].PlaceWidget(&btns[12] , 2);
   rgrids[2].PlaceWidget(&btns[13] , 3);
   rgrids[2].PlaceWidget(&btns[14] , 4);
   rgrids[2].PlaceWidget(&btns[15] , 5);
   rgrids[3].PlaceWidget(&btns[16] , 0);
   rgrids[3].PlaceWidget(&btns[17] , 1);
   rgrids[3].PlaceWidget(&btns[18] , 2);
   rgrids[3].PlaceWidget(&btns[19] , 3);
   
   mover.WhiteList(&rgrids[0]);
   mover.WhiteList(&rgrids[1]);
   mover.WhiteList(&rgrids[2]);
   mover.WhiteList(&rgrids[3]);
   
   sys->GetSystemTimer()->Start();
   
   mover.SetAbilities(true , true);
   
   gui.AlwaysClear(true);
   
   bool quit = false;
   bool redraw = true;
   WidgetBase* hover = 0;
   Rectangle dndrect = BADRECTANGLE;
   WidgetBase* dndwidget = 0;
   while (!quit) {
      if (redraw) {
         win->SetDrawingTarget(win->GetBackBuffer());
         win->Clear();
         gui.Display(win , 0 , 0);
         if (hover) {
            win->DrawTextString(win->DefaultFont() , hover->ShortName() , 10 , 10 , EagleColor(255,255,255));
         }
         if (dndrect != BADRECTANGLE) {
            dndwidget->Display(win , 0 , 0);
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
               for (unsigned int i = 0 ; i < 2 ; ++i) {
                  if (e.widget.from == &btns[i]) {
                     if (i == HBOX_ANCHOR_E || i == HBOX_ANCHOR_W) {
                        hbox.SetAnchorPosition((BOX_ANCHOR_POINT)i);
                     }
                  }
               }
            }
            if (e.source == &radios[1]) {
               for (unsigned int i = 2 ; i < 4 ; ++i) {
                  if (e.widget.from == &btns[i]) {
                     if (i == VBOX_ANCHOR_N || i == VBOX_ANCHOR_S) {
                        vbox.SetAnchorPosition((BOX_ANCHOR_POINT)i);
                     }
                  }
               }
            }
            if (e.source == &radios[2]) {
               for (unsigned int i = 4 ; i < 8 ; ++i) {
                  if (e.widget.from == &btns[i]) {
                     flowbox.SetAnchorPosition((BOX_ANCHOR_POINT)i);
                  }
               }
            }
            if (e.source == &radios[3]) {
               for (unsigned int i = 8 ; i < 10 ; ++i) {
                  if (e.widget.from == &btns[i]) {
                     /// Horizontal or vertical flow button pressed, only affects FlowBox
                     flowbox.SetFlowDirection((FLOW_FAVORED_DIRECTION)(i-8));
                  }
               }
            }
            if (e.source == &radios[4]) {
               for (unsigned int i = 10 ; i < 13 ; ++i) {
                  if (e.widget.from == &btns[i]) {
                     hbox.SetAlignment((HALIGNMENT)(i - 10) , hbox.GetVAlignment());
                     vbox.SetAlignment((HALIGNMENT)(i - 10) , vbox.GetVAlignment());
                     flowbox.SetAlignment((HALIGNMENT)(i - 10) , flowbox.GetVAlignment());
                  }
               }
            }
            if (e.source == &radios[5]) {
               for (unsigned int i = 13 ; i < 16 ; ++i) {
                  if (e.widget.from == &btns[i]) {
                     hbox.SetAlignment(hbox.GetHAlignment() , (VALIGNMENT)(i - 13));
                     vbox.SetAlignment(vbox.GetHAlignment() , (VALIGNMENT)(i - 13));
                     flowbox.SetAlignment(flowbox.GetHAlignment() , (VALIGNMENT)(i - 13));
                  }
               }
            }
            if (e.source == &radios[6]) {
               for (unsigned int i = 16 ; i < 20 ; ++i) {
                  if (e.widget.from == &btns[i]) {
                     hbox.SetBoxSpacing((BOX_SPACE_RULES)(i-16));
                     vbox.SetBoxSpacing((BOX_SPACE_RULES)(i-16));
                     flowbox.SetBoxSpacing((BOX_SPACE_RULES)(i-16));
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
               EagleLog() << "HBOX:\n";
               EagleLog() << hbox << std::endl;
               EagleLog() << "VBOX:\n";
               EagleLog() << vbox << std::endl;
               EagleLog() << "FLOWBOX:\n";
               EagleLog() << flowbox << std::endl;
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
            DRAG_AND_DROP_DATA dnd = e.widget.dnd;
            EagleInfo() << "DND event" << std::endl;
            if (e.widget.topic == TOPIC_WIDGET_MOVER && e.widget.msgs == WIDGET_MOVER_SIZING) {
               EagleInfo() << "Sizing event" << std::endl;
               dndrect = Rectangle(dnd.dropx , dnd.dropy , dnd.dropw , dnd.droph);
               dndwidget = dnd.mwidget;
               
            }
            else if (e.widget.topic == TOPIC_WIDGET_MOVER && e.widget.msgs == WIDGET_MOVER_DROP) {
               dnd.mwidget->SetWidgetArea(Rectangle(dnd.dropx , dnd.dropy , dnd.dropw , dnd.droph));
               dndrect = BADRECTANGLE;
            }
            else {
               dndrect = BADRECTANGLE;
            }
         };
         for (int i = 0 ; i < 9 ; ++i) {
            if (input_key_press(EAGLE_KEY_1 + i)) {
               if (input_key_held(EAGLE_KEY_ONLY_CTRL)) {
                  /// Ctrl is held, add to flow layout
                  hbox.RemoveWidget(widgets[i]);
                  vbox.RemoveWidget(widgets[i]);
                  flowbox.RemoveWidget(widgets[i]);
                  flowbox.AddWidget(widgets[i]);
               }
               else if (input_key_held(EAGLE_KEY_ONLY_SHIFT)) {
                  /// Shift is held, move to vbox layout
                  hbox.RemoveWidget(widgets[i]);
                  vbox.RemoveWidget(widgets[i]);
                  flowbox.RemoveWidget(widgets[i]);
                  vbox.AddWidget(widgets[i]);
               }
               else {
                  /// No key held, move to hbox
                  hbox.RemoveWidget(widgets[i]);
                  vbox.RemoveWidget(widgets[i]);
                  flowbox.RemoveWidget(widgets[i]);
                  hbox.AddWidget(widgets[i]);
               }
            }
         }
         if (input_key_press(EAGLE_KEY_C)) {
             hbox.ClearWidgets();
             vbox.ClearWidgets();
             flowbox.ClearWidgets();
         }
         if (input_key_press(EAGLE_KEY_ENTER)) {
            hbox.ClearWidgets();
            vbox.ClearWidgets();
            flowbox.ClearWidgets();
            if (input_key_held(EAGLE_KEY_ONLY_CTRL)) {
               for (unsigned int i = 0 ; i < 9 ; ++i) {
                  flowbox.AddWidget(widgets[i]);
               }
            }
            else if (input_key_held(EAGLE_KEY_ONLY_SHIFT)) {
               /// Shift key, move all to vbox
               for (unsigned int i = 0 ; i < 9 ; ++i) {
                  vbox.AddWidget(widgets[i]);
               }
            }
            else {
               /// No mod key, move all to hbox
               for (unsigned int i = 0 ; i < 9 ; ++i) {
                  hbox.AddWidget(widgets[i]);
               }
            }
         }
      }
   }
   
   
      
   return 0;
}


