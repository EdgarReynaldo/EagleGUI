


#include "NewGuiTestMain.hpp"


#include "Eagle\backends\Allegro5Backend.hpp"

using std::endl;



int NewGuiTestMain(int argc , char** argv , TestRunner* test_runner) {
   
   (void)argc;
   (void)argv;
   
   EagleGraphicsContext* win = sys->CreateGraphicsContext(800,600,EAGLE_OPENGL | EAGLE_WINDOWED | EAGLE_RESIZABLE);
   if (!win->Valid()) {
      return 2;
   }
   

   Allegro5Font bfont("Verdana.ttf" , 20 , LOAD_FONT_MONOCHROME);
   Allegro5Font bfont2("Consola.ttf" , 20 , LOAD_FONT_MONOCHROME);
   
   EagleImage* npimage = win->LoadImageFromFile("Data/ninePatch/NPBlueBorder.png");
   EagleImage* npimage2 = win->LoadImageFromFile("Data/ninePatch/OrangeYellowNP.png");
///   EagleImage* npimage = win->LoadImageFromFile("Data/ninePatch/NPBlueBorder2.bmp");
///   EagleImage* npimage = win->LoadImageFromFile("Data/ninePatch/NPBlueBorder.tga");
   
   EAGLE_ASSERT(npimage->Valid());
   
   
   
   WidgetArea warea;
   warea.SetOuterArea(Rectangle(0,0,800,600));///npimage->W(),npimage->H()));
   EagleLog() << warea;
   warea.SetMarginsContractFromOuter(100,100,75,75);
   EagleLog() << warea;
   for (int i = 0 ; i < 9 ; ++i) {
      warea.SetImage(npimage2 , (MARGIN_HCELL)(i/3) , (MARGIN_VCELL)(i%3));
   }
   win->Clear(EagleColor(0,0,255));
   warea.PaintImages(win , 0 , 0 , DRAW_NORMAL);
   win->FlipDisplay();
   sys->GetSystemQueue()->WaitForEvent(EAGLE_EVENT_KEY_DOWN);

   NinePatch np = MakeNinePatch(win , npimage , warea);
   warea.SetImages(np.imgs);
   
   win->Clear(EagleColor(255,255,255));
   win->SetPMAlphaBlender();
   NinePatchImageBackgroundPainter np_painter;
   np_painter.PaintBackground(win , warea , WidgetColorset() , 0 , 0);
   win->RestoreLastBlendingState();
   win->FlipDisplay();
   sys->GetSystemQueue()->WaitForEvent(EAGLE_EVENT_KEY_DOWN);
   
   win->Clear(EagleColor(0,0,0));
   win->Draw(npimage , 0 , 0);
   win->FlipDisplay();
   sys->GetSystemQueue()->WaitForEvent(EAGLE_EVENT_KEY_DOWN);
      
   RelativeLayout r;
   
   WidgetHandler gui(win);
   gui.SetRootLayout(&r);

   gui.SetDrawWindow(win);
   gui.SetupBuffer(800,600 , win);
   gui.SetWidgetArea(0,0,800,600);
   
   GuiButton b1;
   b1.SetButtonType(RECTANGLE_BTN , SPRING_BTN , BUTTON_CLASS_PLAIN);
   b1.SetInputGroup(Input(KB,PRESS,EAGLE_KEY_1));
   b1.SetLabel("Spring Button &1");
   b1.SetFont(&bfont);

   GuiButton b2;
   b2.SetButtonType(ROUNDED_BTN , SPRING_BTN , BUTTON_CLASS_PLAIN);
   b2.SetInputGroup(Input(KB,PRESS,EAGLE_KEY_2));
   b2.SetLabel("Spring Button &2");
   b2.SetFont(&bfont);

   GuiButton b3;
   b3.SetButtonType(ELLIPSE_BTN , SPRING_BTN , BUTTON_CLASS_PLAIN);
   b3.SetInputGroup(Input(KB,PRESS,EAGLE_KEY_3));
   b3.SetLabel("Spring Button &3");
   b3.SetFont(&bfont);

   GuiButton b4;
   b4.SetButtonType(CIRCLE_BTN , SPRING_BTN , BUTTON_CLASS_PLAIN);
   b4.SetInputGroup(Input(KB,PRESS,EAGLE_KEY_4));
   b4.SetLabel("Spring Button &4");
   b4.SetFont(&bfont);
   
   GuiButton b5;
   b5.SetButtonType(RECTANGLE_BTN , TOGGLE_BTN , BUTTON_CLASS_PLAIN);
   b5.SetInputGroup(Input(KB,PRESS,EAGLE_KEY_5));
   b5.SetLabel("Toggle Button &5");
   b5.SetFont(&bfont2);

   GuiButton b6;
   b6.SetButtonType(ROUNDED_BTN , TOGGLE_BTN , BUTTON_CLASS_PLAIN);
   b6.SetInputGroup(Input(KB,PRESS,EAGLE_KEY_6));
   b6.SetLabel("Toggle Button &6");
   b6.SetFont(&bfont2);

   GuiButton b7;
   b7.SetButtonType(ELLIPSE_BTN , TOGGLE_BTN , BUTTON_CLASS_PLAIN);
   b7.SetInputGroup(Input(KB,PRESS,EAGLE_KEY_7));
   b7.SetLabel("Toggle Button &7");
   b7.SetFont(&bfont2);

   GuiButton b8;
   b8.SetButtonType(CIRCLE_BTN , TOGGLE_BTN , BUTTON_CLASS_PLAIN);
   b8.SetInputGroup(Input(KB,PRESS,EAGLE_KEY_8));
   b8.SetLabel("Toggle Button &8");
   b8.SetFont(&bfont2);

   BasicText text1;
///   text1.SetWidgetArea(0,0,800,200);
   text1.SetupText(HALIGN_CENTER , VALIGN_CENTER , 10 , 20 , 10 , "",  &bfont );
   text1.SetText("This Is A \nMultiline Text String\nDumb Text Test" , &bfont);
   text1.SetMarginsContractFromOuter(10,10,10,10);
   text1.SetBgImages(np.imgs);
   text1.SetBackgroundDrawType(BG_DRAW_BACKGROUND_NINEPATCH);

   r.Resize(9);
   r.PlaceWidget(&b1 , 0 , LayoutRectangle(0.0,0.0,0.4,0.4));
   r.PlaceWidget(&b2 , 1 , LayoutRectangle(0.0,0.6,0.4,0.4));
   r.PlaceWidget(&b3 , 2 , LayoutRectangle(0.6,0.0,0.4,0.4));
   r.PlaceWidget(&b4 , 3 , LayoutRectangle(0.6,0.6,0.4,0.4));
   
   r.PlaceWidget(&b5 , 4 , LayoutRectangle(0.0,0.4,0.4,0.2));
   r.PlaceWidget(&b6 , 5 , LayoutRectangle(0.4,0.6,0.2,0.4));
   r.PlaceWidget(&b7 , 6 , LayoutRectangle(0.4,0.0,0.2,0.4));
   r.PlaceWidget(&b8 , 7 , LayoutRectangle(0.6,0.4,0.4,0.2));
   
   r.PlaceWidget(&text1 , 8 , LayoutRectangle(0.4 , 0.4 , 0.2 , 0.2));
   
   EagleLog() << "Text widget 1 is :" << endl;
   EagleLog() << text1 << endl;
   
   Layout* layout = gui.GetRootLayout();
   layout->SetMarginsContractFromOuter(20,20,20,20);
   layout->SetBackgroundDrawType(BG_DRAW_BACKGROUND_CONTRAST_BORDER);
   
   EagleLog() << text1 << std::endl;

   bool quit = false;
   bool redraw = true;
   
///   sys->GetSystemTimer()->Create(0.5);
   sys->GetSystemTimer()->Start();
   
   do {
      if (test_runner->ShouldStop()) {
         break;
      }
         
      if (redraw) {
///         win->Clear(EagleColor(0,255,0));
///         win->FlipDisplay();
         gui.Display(win,0,0);
         win->FlipDisplay();
         redraw = false;
      }
      
      do {
         EagleEvent ee;
         ee = sys->WaitForSystemEventAndUpdateState();
         
         if (ee.type == EAGLE_EVENT_DISPLAY_RESIZE) {
            win->AcknowledgeResize();
            gui.SetWidgetArea(0 , 0 , win->Width() , win->Height());
         }

         gui.HandleEvent(ee);

         while (gui.HasMessages()) {
            WidgetMsg wmsg = gui.TakeNextMessage();
            EagleLog() << wmsg << endl;
            if (wmsg.From() == &b1) {
               /// Respond to widget message from b1 here
///               EagleLog() << b1 << endl;
            }
         }
         if (ee.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         if (ee.type == EAGLE_EVENT_KEY_DOWN && ee.keyboard.keycode == EAGLE_KEY_ESCAPE) {
            quit = true;
         }
         if (ee.type == EAGLE_EVENT_TIMER) {
            redraw = true;
         }
         
      } while(!sys->UpToDate());
   } while (!quit);
   
   return 0;
};

