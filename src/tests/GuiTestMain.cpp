



#include "GuiTestMain.hpp"

#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"

#include <sstream>
#include <cstdlib>

using std::endl;


///ShutdownVar gui_global_shutdown_var = ShutdownVar("Main global variable");

DEFINE_TEST(GuiTestMain , "Gui Test #1");
DEFINE_TEST(GuiTestMain2 , "Gui Test #2");
DEFINE_TEST(GuiTestMain3 , "Gui Test #3 (Incomplete)");


#include <signal.h>
#include <cstdlib>

inline void terminator()
{
    int*z = 0; *z=13;
}

inline void signal_handler(int)
{
    terminator();
}
/**
inline void __cdecl invalid_parameter_handler(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t)
{
   terminator();
}
//*/





int GuiTestMain(int argc , char** argv , TestRunner* test_runner) {

   (void)argc;
   (void)argv;

   Allegro5GraphicsContext* win = dynamic_cast<Allegro5GraphicsContext*>(GetAllegro5WindowManager()->CreateWindow(800,600,EAGLE_WINDOWED));

   if (!win->Valid()) {
      throw EagleException("GuiTestMain : Failed to create valid resizable opengl window");
   }

   al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP | ALLEGRO_MAG_LINEAR | ALLEGRO_MIN_LINEAR);

   /// Load resources
//   EagleFont* LoadFont(std::string file , int height , int flags , IMAGE_TYPE type = VIDEO_IMAGE);
   EagleFont* verdana = win->LoadFont("verdana.ttf" , 20 , LOAD_FONT_MONOCHROME);
   EagleFont* consola = win->LoadFont("consola.ttf" , 20 , LOAD_FONT_MONOCHROME);
   if (!verdana->Valid() || !consola->Valid()) {
      if (!verdana->Valid()) {EagleLog() << "Verdana.ttf failed to load." << std::endl;}
      if (!consola->Valid()) {EagleLog() << "Consola.ttf failed to load." << std::endl;}
   }

   EagleImage* stallions = win->LoadImageFromFile("Stallions2a.jpg" , VIDEO_IMAGE);
   if (!stallions->Valid()) {EagleLog() << "Stallions2a.jpg failed to load." << std::endl;}

 //  win->Draw(stallions , (win->Width() - stallions->W())/2 , (win->Height() - stallions->H())/2);


//*
   /// GUI setup

/**
   EagleImage* np2[3][3];
   np2[MARGIN_VCELL_TOP][MARGIN_HCELL_LEFT] = win->LoadImageFromFile("Data/ninePatch/CyanBevel_TL.png" , VIDEO_IMAGE);
   np2[MARGIN_VCELL_TOP][MARGIN_HCELL_CENTER] = win->LoadImageFromFile("Data/ninePatch/CyanBevel_T.png" , VIDEO_IMAGE);
   np2[MARGIN_VCELL_TOP][MARGIN_HCELL_RIGHT] = win->LoadImageFromFile("Data/ninePatch/CyanBevel_TR.png" , VIDEO_IMAGE);
   np2[MARGIN_VCELL_CENTER][MARGIN_HCELL_LEFT] = win->LoadImageFromFile("Data/ninePatch/CyanBevel_L.png" , VIDEO_IMAGE);
   np2[MARGIN_VCELL_CENTER][MARGIN_HCELL_CENTER] = 0;
   np2[MARGIN_VCELL_CENTER][MARGIN_HCELL_RIGHT] = win->LoadImageFromFile("Data/ninePatch/CyanBevel_R.png" , VIDEO_IMAGE);
   np2[MARGIN_VCELL_BOTTOM][MARGIN_HCELL_LEFT] = win->LoadImageFromFile("Data/ninePatch/CyanBevel_BL.png" , VIDEO_IMAGE);
   np2[MARGIN_VCELL_BOTTOM][MARGIN_HCELL_CENTER] = win->LoadImageFromFile("Data/ninePatch/CyanBevel_B.png" , VIDEO_IMAGE);
   np2[MARGIN_VCELL_BOTTOM][MARGIN_HCELL_RIGHT] = win->LoadImageFromFile("Data/ninePatch/CyanBevel_BR.png" , VIDEO_IMAGE);
*/
//   EagleImage* np3img = win->LoadImageFromFile("Data/ninePatch/OrangeYellowNP.png");
   EagleImage* np3img = win->LoadImageFromFile("Data/ninePatch/BlackAlphaGradient.png");
   WidgetArea nparea;
   nparea.SetOuterArea(0,0,np3img->W() , np3img->H());
   nparea.SetMarginsContractFromOuter(30,30,30,30);
   NinePatch np3 = MakeNinePatch(win , np3img , nparea);

   {
      WidgetArea a;
      a.SetOuterArea(0,0,win->Width(),win->Height());
      a.SetMarginsContractFromOuter(48,48,36,36);

      a.SetImages(np3.imgs);
      a.SetImage(stallions , MARGIN_HCELL_CENTER , MARGIN_VCELL_CENTER);

      WidgetColorset c;
      c[SDCOL] = EagleColor(0,0,0);
      win->Clear(EagleColor(255,255,255));
      NinePatchImageBackgroundPainter p;
///      BorderedStretchedImageBackgroundPainter p;
      p.PaintBackground(win , a , c , 0 , 0);
///      WidgetBorderedImagePainterStretch(win , a , c , 0 , 0);
//      WidgetNPPainter(win , a , c , 0 , 0);
      win->FlipDisplay();
      sys->GetSystemQueue()->WaitForEvent(EAGLE_EVENT_KEY_DOWN , 0);
   }

   WidgetHandler wgui(win);
   WidgetHandler* gui = &wgui;


   gui->SetDrawWindow(win);
   gui->SetBackgroundColor(EagleColor(0,127,0));


   Rectangle r(200,150,400,300);
   Pos2d corner1,corner2;
   corner1.SetPos(200,150);
   corner2.SetPos(599,449);
 //*/

//*

///   gui->SetWidgetArea((1280 - 800) / 2 , (768 - 600) / 2 , 800 , 600);
   gui->SetWidgetArea(0 , 0 , win->Width() , win->Height());
//   EagleLog() << "Gui outer area = " << gui->OuterArea() << std::endl;
//   EagleLog() << "Gui inner area = " << gui->InnerArea() << std::endl;

   GridLayout* grid_layout = new GridLayout();

   gui->SetRootLayout(grid_layout);


   int rows = 2;
   int columns = 2;
   int sz = rows*columns;

   grid_layout->ResizeGrid(columns , rows);

   grid_layout->SetName("2X2 Widget Root grid layout");


//   grid_layout->SetMarginsContractFromOuter(10,10,10,10);
//   grid_layout->SetBgImage(stallions , MARGIN_HCELL_CENTER , MARGIN_VCELL_CENTER);
//   grid_layout->WCols()[SDCOL] = EagleColor(255,255,255);
//   grid_layout->SetBgDrawFunc(WidgetBGImagePainterStretch);

//   EagleLog() << "Grid layout outer area = " << grid_layout->OuterArea() << std::endl;
//   EagleLog() << "Grid layout inner area = " << grid_layout->InnerArea() << std::endl;



   GuiButton* warray = new GuiButton[sz - 1];

   for (int i = 0 ; i < sz - 1 ; ++i) {
      grid_layout->AddWidget(&warray[i]);
      warray[i].SetLabel("Grid Button");
      warray[i].SetFont(verdana);
   }
   for (int i = 0 ; i < sz - 1 ; ++i) {
      warray[i].SetMarginsContractFromOuter((i+1)*15 , (i+1)*15 , (i+1)*15 , (i+1)*15);
//      warray[i].SetBgDrawFunc(WidgetNPPainter);
//      EagleLog() << warray[i].OuterArea() << std::endl;
   }
   warray[2].SetMarginsContractFromOuter(25,25,25,25);

   warray[0].UsePrivateColorset(true);
   warray[0].WCols()[SDCOL] = EagleColor(0,255,0);
   warray[0].SetBgImage(stallions , MARGIN_HCELL_CENTER , MARGIN_VCELL_CENTER);
   warray[0].SetBackgroundDrawType(BG_DRAW_BACKGROUND_SOLID_BORDER_IMAGE_STRETCH);
///   warray[0].SetBgDrawFunc(WidgetBorderedImagePainterStretch);
///   warray[0].SetColor(EagleColor(0,0,0,0));

   warray[0].SetButtonType(CIRCLE_BTN , TOGGLE_BTN , BUTTON_CLASS_PLAIN);

//   warray[1].UsePrivateColorset(true);
///   warray[1].SetColor(EagleColor(0,0,0,0));
   warray[1].SetBgImage(stallions , MARGIN_HCELL_CENTER , MARGIN_VCELL_CENTER);
   warray[1].SetBackgroundDrawType(BG_DRAW_BACKGROUND_IMAGE_STRETCH);
///   warray[1].SetBgDrawFunc(WidgetBGImagePainterStretch);

   warray[1].SetButtonType(ROUNDED_BTN , TOGGLE_BTN , BUTTON_CLASS_PLAIN);
//   warray[2].SetBgImages(np2);
///   warray[2].SetColor(EagleColor(0,0,0,0));
//   warray[2].SetBgDrawFunc(WidgetBGImagePainterCenterClientArea);
   warray[2].SetImagesHaveAlpha(true);
   warray[2].SetBgImages(np3.imgs);
   warray[2].SetBackgroundDrawType(BG_DRAW_BACKGROUND_NINEPATCH);
///   warray[2].SetBgDrawFunc(WidgetNPPainter);

   warray[2].SetButtonType(ELLIPSE_BTN , SPRING_BTN , BUTTON_CLASS_PLAIN);
//   warray[2].SetBgImage(stallions , MARGIN_HCELL_CENTER , MARGIN_VCELL_CENTER);


   RelativeLayout* relative_layout = new RelativeLayout();
   relative_layout->SetName("Relative layout");


   grid_layout->AddWidget(relative_layout);

   grid_layout->SetPadding(40,30);

   GuiButton* warray2 = new GuiButton[5];


//   relative_layout.

   relative_layout->Resize(5);

   relative_layout->SetLayoutRectangle(0 , LayoutRectangle(0.0 , 0.0 , 0.2 , 0.2));
   relative_layout->SetLayoutRectangle(1 , LayoutRectangle(0.8 , 0.0 , 0.2 , 0.2));
   relative_layout->SetLayoutRectangle(2 , LayoutRectangle(0.8 , 0.8 , 0.2 , 0.2));
   relative_layout->SetLayoutRectangle(3 , LayoutRectangle(0.0 , 0.8 , 0.2 , 0.2));
   relative_layout->SetLayoutRectangle(4 , LayoutRectangle(0.3 , 0.3 , 0.4 , 0.4));

   for (int i = 0 ; i < 5 ; ++i) {
      warray2[i].UseColorset(false);
      warray2[i].WCols()[SDCOL] = EagleColor(50*(i+1),50*(i+1),50*(i+1));
      warray2[i].SetMarginsExpandFromInner(2*(i+1) , 2*(i+1) , 2*(i+1) , 2*(i+1));
      warray2[i].SetBackgroundDrawType(BG_DRAW_BACKGROUND_SOLID_SHADOW_BORDER);
      warray2[i].SetLabel("Relative Button");
      warray2[i].SetFont(consola);
      relative_layout->Layout::PlaceWidget(&warray2[i] , i);
//      EagleLog() << "warray2[" << i << "].OuterArea() = " << warray2[i].OuterArea() << std::endl;
//      EagleLog() << "warray2[" << i << "].InnerArea() = " << warray2[i].InnerArea() << std::endl;
//      EagleLog() << "warray2[" << i << "].WCols()[SDCOL] = " << warray2[i].WCols()[SDCOL] << std::endl;
   }
//*/

//*
   bool small_window = false;
   bool close = false;
   bool redraw = true;
   bool resize_display = false;

   std::string output;


   EagleLog() << "Printing Widgethandler hierarchy :" << endl;
   EagleLog() << *gui << endl << endl;

   sys->GetSystemQueue()->WaitForEvent(EAGLE_EVENT_KEY_DOWN , 0);
//   sys->GetSystemTimer()->Create(1.0);
   sys->GetSystemTimer()->Start();

   do {

      if (test_runner->ShouldStop()) {
         break;
      }

      if (redraw) {

//         win->Clear(EagleColor(rand()%255 , rand()%255 , rand() % 255));
         win->DrawToBackBuffer();
         win->Clear(EagleColor(0,0,0));

//         win->GetDrawingTarget()->PushClippingRectangle(Rectangle(200 , 150 , 400 , 300));

//         win->GetDrawingTarget()->PushClippingRectangle(r);
//         WidgetBGImagePainterCenter(win , a , c , 0 , 0);
//         win->GetDrawingTarget()->PopClippingRectangle();

//         win->GetDrawingTarget()->PopClippingRectangle();
         gui->Display(win , 0 , 0);

//   virtual void DrawTextString(EagleFont* font , std::string s , float x , float y , EagleColor c,
//                               TEXT_HDRAWING_FLAGS halign = DRAW_TEXT_LEFT ,
//                               TEXT_VDRAWING_FLAGS valign = DRAW_TEXT_TOP);
///         win->DrawTextString(verdana , StringPrintF("Resize display = %s" , resize_display?"On":"Off") ,
///                                 10 , 10 , EagleColor(255,255,255));

         win->DrawTextString(verdana , output , 10 , 10 , EagleColor(255,127,127));

         win->FlipDisplay();
         redraw = false;
      }

      do {
         EagleEvent e;

         EagleEventHandler* q = sys->GetSystemQueue();
         e = q->PeekNextEvent(0);
         if (e.type == EAGLE_EVENT_DISPLAY_RESIZE) {
            sys->Rest(0.5);
            std::vector<EagleEvent> resize_events = q->FilterEvents(EAGLE_EVENT_DISPLAY_RESIZE , 0);
            EagleLog() << resize_events.size() << " resize events." << std::endl;
            e = resize_events[resize_events.size() - 1];
            win->AcknowledgeResize();
            gui->SetWidgetDimensions(e.display.width , e.display.height);
            gui->SetFullRedraw();
            continue;
         }


         e = sys->WaitForSystemEventAndUpdateState();

         if (input_mouse_press(LMB)) {
            EagleLog() << StringPrintF("Main: Mouse x,y = %i,%i\n" , mouse_x , mouse_y);
         }

         if (e.type == EAGLE_EVENT_TIMER) {
            gui->Update(e.timer.eagle_timer_source->SecondsPerTick());
            redraw = true;
         }
         if (e.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            close = true;
         }
         if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_ESCAPE) {
            close = true;
         }
         if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_SPACE) {
            EagleLog() << "Debug : Break" << std::endl;
         }
         if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_R) {
            resize_display = !resize_display;
         }
         if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_W) {
            small_window = !small_window;
            if (small_window) {
               al_resize_display(win->AllegroDisplay() , 400 , 300);
               gui->SetWidgetDimensions(400,300);
            }
            else {
               al_resize_display(win->AllegroDisplay() , 800 , 600);
               gui->SetWidgetDimensions(800,600);
            }
            redraw = true;
         }
         if (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_G) {
            ALLEGRO_TRANSFORM t;
            al_identity_transform(&t);
            al_use_transform(&t);
            gui->SetFullRedraw();
            redraw = true;
         }

         if (e.type == EAGLE_EVENT_DISPLAY_RESIZE) {

            win->AcknowledgeResize();
            gui->SetWidgetDimensions(e.display.width , e.display.height);
            gui->SetFullRedraw();

         }

         if (e.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN) {
            if (e.mouse.button == 1) {
               corner1.SetPos(e.mouse.x , e.mouse.y);
            }
            if (e.mouse.button == 2) {
               corner2.SetPos(e.mouse.x , e.mouse.y);
            }
            r.SetCorners(corner1.X() , corner1.Y() , corner2.X() , corner2.Y());
            redraw = true;
         }

         gui->HandleEvent(e);
         if (gui->Flags() & NEEDS_REDRAW) {redraw = true;}

         while (gui->HasMessages()) {
            WidgetMsg wmsg = gui->TakeNextMessage();
            EagleLog() << wmsg << std::endl;
            redraw = true;
         }

      } while (!sys->UpToDate());

   } while (!close);

   delete [] warray;
   delete [] warray2;
   delete relative_layout;
   delete grid_layout;

   EagleLog() << "Current objects left after cleanup : " << LiveObjectCount() << std::endl;
   OutputLiveObjectsBrief();
   EagleLog() << std::endl;

   return 0;
}







int GuiTestMain2(int argc , char** argv , TestRunner* test_runner) {

   (void)argc;
   (void)argv;

   al_set_new_window_position(112,84);
   al_set_new_window_title("Eagle 5 dynamic layout test ");

   Allegro5GraphicsContext* win = dynamic_cast<Allegro5GraphicsContext*>(GetAllegro5WindowManager()->CreateWindow(800,600,EAGLE_WINDOWED));

   EagleFont* verdana = win->LoadFont("verdana.ttf" , -40 , LOAD_FONT_MONOCHROME);

//   EagleImage* stallions = win->LoadImageFromFile("Data/jpgImages/Stallions.jpg");
   EagleImage* stallions2a = win->LoadImageFromFile("Stallions2a.jpg");
   EagleImage* stallions2 = win->LoadImageFromFile("Stallions2.jpg");

   EAGLE_ASSERT(verdana && verdana->Valid() && stallions2a && stallions2a->Valid() && stallions2 && stallions2->Valid());

   win->Draw(stallions2a , 0 , 0);
   win->DrawTextString(verdana , "Press a key" , 100 , win->Height() - 50 , EagleColor(255,255,255));

   win->FlipDisplay();

   sys->GetSystemQueue()->WaitForEvent(EAGLE_EVENT_KEY_DOWN , 0);

   WidgetHandler gui(win);/// TEST : All other widgets will get destroyed first, make sure we don't call any methods on
                     /// a destroyed object



   TwoWaySplitter hsp(SPLITTER_HORIZONTAL , win);
   TwoWaySplitter vsp1(SPLITTER_VERTICAL , win);
   TwoWaySplitter vsp2(SPLITTER_VERTICAL , win);

   hsp.SetMouseWindow(win);
   vsp1.SetMouseWindow(win);
   vsp2.SetMouseWindow(win);


   hsp.SetDividerPercent(0.5);
   vsp1.SetDividerPercent(0.75);
   vsp2.SetDividerPercent(0.25);

   GuiButton testbtns[3];
   testbtns[0].SetLabel("Upper left");
   testbtns[0].SetButtonType(ELLIPSE_BTN , SPRING_BTN , BUTTON_CLASS_PLAIN);
   testbtns[1].SetLabel("Upper right");
   testbtns[1].SetButtonType(CIRCLE_BTN , SPRING_BTN , BUTTON_CLASS_PLAIN);
   testbtns[2].SetLabel("Lower left");
   testbtns[2].SetButtonType(ROUNDED_BTN , SPRING_BTN , BUTTON_CLASS_PLAIN);
   testbtns[0].SetFont(verdana);
   testbtns[1].SetFont(verdana);
   testbtns[2].SetFont(verdana);

   RelativeLayout relative(StringPrintF("RelativeLayout relative at %p" , &relative));
   relative.Resize(1);

   GridLayout grid;
   relative.PlaceWidget(&grid , 0 , LayoutRectangle(0.0 , 0.0 , 1.0 , 1.0));
   grid.ResizeGrid(4,3);

   EagleImage* ptr = win->LoadImageFromFile("Pointer.png");
   EAGLE_ASSERT(ptr && ptr->Valid());

   MousePointerInfo msinfo(POINTER_NORMAL , ptr , -1 , -1 , false);

   win->SetCustomPointer(msinfo);

   GuiButton mpbtns[12];

   for (int i = 0 ; i < 12 ; ++i) {
      mpbtns[i].SetFont(verdana);
      mpbtns[i].SetLabel(StringPrintF("%d" , i));
      mpbtns[i].SetButtonType(RECTANGLE_BTN , TOGGLE_BTN , BUTTON_CLASS_PLAIN);
      grid.PlaceWidget(&mpbtns[i] , i);
   }



//   testbtns[3].SetLabel("Lower right");
//   testbtns[3].SetButtonType(CIRCLE_BTN , SPRING_BTN);
//   testbtns[3].SetFont(verdana);
/*
   testbtns[0].SetBgDrawFunc(WidgetBorderPainter);
   testbtns[1].SetBgDrawFunc(WidgetBorderPainter);
   testbtns[2].SetBgDrawFunc(WidgetBorderPainter);
   testbtns[3].SetBgDrawFunc(WidgetBorderPainter);
*/


   hsp.PlaceWidget(&vsp1 , 0);
   hsp.PlaceWidget(&vsp2 , 1);
   vsp1.PlaceWidget(&testbtns[0] , 0);
   vsp1.PlaceWidget(&testbtns[1] , 1);
   vsp2.PlaceWidget(&testbtns[2] , 0);
   vsp2.PlaceWidget(&relative , 1);

//   vsp2.PlaceWidget(&testbtns[3] , 1 , false);

//   WidgetHandler gui;
   gui.SetDrawWindow(win);

   gui.SetupBuffer(800,600 , win);
   gui.WidgetBase::SetWidgetArea(Rectangle(0 , 0 , 800 , 600));

   gui.UseBackgroundImage(stallions2 , true);
   gui.SetBufferShrinkOnResize(true);

   RelativeLayout rl(StringPrintF("RelativeLayout rl at %p" , &rl));;
   rl.Resize(5);

///   EagleFont* font1 = win->LoadFont("Data/fonts/English.ttf" , -40 , DRAW_NORMAL);
///   EagleFont* font2 = win->LoadFont("Data/fonts/EnglishCn.ttf" , -40 , DRAW_NORMAL);
   EagleFont* font3 = win->LoadFont("Data/fonts/EnglishBold.ttf" , -40 , DRAW_NORMAL);
   EagleFont* font4 = win->LoadFont("Data/fonts/consola.ttf" , -40 , DRAW_NORMAL);
   EagleFont* font5 = win->LoadFont("Data/fonts/AlexBrush.ttf" , -40 , DRAW_NORMAL);

   BasicText dumbtext;
   dumbtext.SetText("Layout Test" , verdana);

   LinkText eaglelink;
   LinkText bitbucketlink;
   LinkText allegrolink;

///   eaglelink.SetText("https://sourceforge.net/projects/eagle5gui" , verdana);
   bitbucketlink.SetText("https://bitbucket.org/bugsquasher/eaglegui" , font5);
   eaglelink.SetText("https://members.allegro.cc/EdgarReynaldo/Eagle.html" , font4);
   allegrolink.SetText("http://www.allegro.cc" , font3);

   WidgetColorset wc;
   wc[TXTCOL] = wc[SDCOL];
   wc[HLCOL] = wc[SDCOL];
   dumbtext.UsePrivateColorset(true);
   eaglelink.UsePrivateColorset(true);
   bitbucketlink.UsePrivateColorset(true);
   allegrolink.UsePrivateColorset(true);
   dumbtext.SetPrivateColorset(wc);
   eaglelink.SetPrivateColorset(wc);
   bitbucketlink.SetPrivateColorset(wc);
   allegrolink.SetPrivateColorset(wc);

   rl.AddWidget(&eaglelink , LayoutRectangle( 0.05, 0.05 , 0.8 , 0.2));
   rl.AddWidget(&bitbucketlink , LayoutRectangle( 0.05, 0.85 , 0.8 , 0.15));
   rl.AddWidget(&allegrolink , LayoutRectangle(0.6 , 0.15 , 0.4 , 0.2));


   gui.SetRootLayout(&rl);

   hsp.WidgetBase::SetWidgetArea(Rectangle(100,100,600,375) , false);
   hsp.SetMarginsContractFromOuter(25,25,25,25);
   hsp.SetBackgroundDrawType(BG_DRAW_BACKGROUND_CONTRAST_BORDER);
///   hsp.SetBgDrawFunc(WidgetBorderPainterContrast);

   rl.PlaceWidget(&hsp , 3);
///   rl.RequestWidgetArea(&hsp , 100 , 100 , 600 , 375);/// This function call does nothing

//   gui.AddWidget(&hsp , false);
   gui.SetBackgroundColor(EagleColor(0,0,0,0));// Give gui a clear background
   hsp.WCols()[SDCOL] = EagleColor(255,127,0);

   hsp.SetDividerPercent(0.5);
   vsp1.SetDividerPercent(0.75);
   vsp2.SetDividerPercent(0.25);

   hsp.SetDividerSize(10);
   vsp1.SetDividerSize(10);
   vsp2.SetDividerSize(10);

   testbtns[0].WCols()[SDCOL] = EagleColor(255,0,0);
   testbtns[1].WCols()[SDCOL] = EagleColor(0,255,0);
   testbtns[2].WCols()[SDCOL] = EagleColor(0,0,255);
//   testbtns[3].WCols()[SDCOL] = EagleColor(0,0,0);

   testbtns[0].SetMarginsContractFromOuter(5,5,5,5);
   testbtns[1].SetMarginsContractFromOuter(10,10,10,10);
   testbtns[2].SetMarginsContractFromOuter(15,15,15,15);
//   testbtns[3].SetMarginsContractFromOuter(20,20,20,20);

   for (int i = 0 ; i < 12 ; ++i) {
      mpbtns[i].SetMarginsContractFromOuter(5,5,5,5);
   }


   sys->GetSystemTimer()->Start();

///   double etime = sys->GetProgramTime();
   bool redraw = true;
   bool quit = false;

   int oldindex = -1;
   int index = -1;
   int start_index = -1;
   bool olddrag = false;
   bool drag = false;
   int dragxstart = 0;
   int dragystart = 0;

   Rectangle start_rect;

   // NW,NE,SE,SW corners
   int x1 = 0;
   int y1 = 0;
   int x2 = 0;
   int y2 = 0;
   int x3 = 0;
   int y3 = 0;
   int x4 = 0;
   int y4 = 0;


   while (!quit) {

      if (test_runner->ShouldStop()) {
         break;
      }

      do {
         EagleEvent ev = sys->WaitForSystemEventAndUpdateState();
///         double et = sys->GetProgramTime();
///         double elapsed = et - etime;
///         etime = et;
///         EagleLog() << StringPrintF("ev.type = %s , ev.timestamp = %f , sys->GetProgramTime() = %f , since event was queued = %f , since last event = %f\n" ,
///                                     EagleEventName(ev.type).c_str() , ev.timestamp , etime , etime - ev.timestamp , elapsed);
         if (ev.type == EAGLE_EVENT_DISPLAY_RESIZE) {
            win->AcknowledgeResize();
            gui.SetWidgetDimensions(ev.display.width , ev.display.height);
         }


         if (ev.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         else if (ev.type == EAGLE_EVENT_KEY_DOWN && ev.keyboard.keycode == EAGLE_KEY_ESCAPE) {
            quit = true;
         }
         else if (ev.type == EAGLE_EVENT_TIMER) {
            gui.Update(ev.timer.eagle_timer_source->SecondsPerTick());
            redraw = true;
         }

         gui.HandleEvent(ev);

         if (ev.type == EAGLE_EVENT_KEY_DOWN && ev.keyboard.keycode == EAGLE_KEY_P) {
            EagleLog() << hsp.Area() << endl;
         }
         if (ev.type == EAGLE_EVENT_KEY_DOWN && ev.keyboard.keycode == EAGLE_KEY_R) {
            system("libtest-debug.exe");
            quit = true;
         }

         if (IsMouseEvent(ev)) {
            oldindex = index;
            index = -1;

            olddrag = drag;

            WidgetArea a = hsp.Area();

            MOUSE_POINTER_TYPE ptrs[9] = {
               POINTER_NWSE,
               POINTER_NS,
               POINTER_NESW,
               POINTER_EW,
               POINTER_GRAB,
               POINTER_EW,
               POINTER_NESW,
               POINTER_NS,
               POINTER_NWSE
            };

            index = a.GetCellIndex(ev.mouse.x , ev.mouse.y);

//            if (ev.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN && a.BorderContains(ev.mouse.x , ev.mouse.y)) {
            if (ev.type == EAGLE_EVENT_MOUSE_BUTTON_DOWN && index != -1) {
               EagleLog() << "Mouse button #" << ev.mouse.button << " pressed" << std::endl;
               if (ev.mouse.button == 1 || ev.mouse.button == 3) {
                  win->AcquireMousePointer(&hsp , ptrs[index] , true);
                  if ((ev.mouse.button == 1 && index != 4) ||
                      (ev.mouse.button == 3 && index == 4)) {
                     drag = true;
                     dragxstart = ev.mouse.x;
                     dragystart = ev.mouse.y;
                     start_index = index;
                     Rectangle r = a.OuterArea();
                     // NW,NE,SE,SW corners
                     x1 = r.X();
                     y1 = r.Y();
                     x2 = r.BRX();
                     y2 = r.Y();
                     x3 = r.BRX();
                     y3 = r.BRY();
                     x4 = r.X();
                     y4 = r.BRY();
                  }
               }
            }
            if (ev.type == EAGLE_EVENT_MOUSE_BUTTON_UP) {
               if (ev.mouse.button == 1 || ev.mouse.button == 3) {
                  drag = false;
                  if (!drag && olddrag) {
                     win->ReleaseMousePointer(&hsp);
                  }
               }
            }
            if (ev.type == EAGLE_EVENT_MOUSE_AXES) {
               if (index != oldindex) {
//               if (index != oldindex && index != -1) {

                  if (!drag) {
                     win->ReleaseMousePointer(&hsp);
      //               if (index != -1 && index != 4) {
                     if (index != -1) {
                        win->AcquireMousePointer(&hsp , ptrs[index] , true);
                     } else {
                        win->ReleaseMousePointer(&hsp);
                     }
                  }
               }
               if (drag) {
                  Rectangle newrect;
                  int dx = ev.mouse.x - dragxstart;
                  int dy = ev.mouse.y - dragystart;


                  switch((MARGIN_CELL)start_index) {
                     case MARGIN_CELL_OUTSIDE :
                        EagleLog() << "Error index = MARGIN_OUTSIDE" << endl;
                        break;
                     case MARGIN_CELL_TOPLEFT :
                        // Resizing NW corner - anchor SE corner (x3,y3)
                        newrect.SetCorners(x1 + dx , y1 + dy , x3 , y3);
                        break;
                     case MARGIN_CELL_TOPMIDDLE :
                        newrect.SetCorners(x1 , y1 + dy , x3 , y3);
                        break;
                     case MARGIN_CELL_TOPRIGHT :
                        newrect.SetCorners(x2 + dx , y2 + dy , x4 , y4);
                        break;
                     case MARGIN_CELL_MIDDLELEFT :
                        newrect.SetCorners(x1 + dx , y1 , x3 , y3);
                        break;
                     case MARGIN_CELL_MIDDLEMIDDLE :
                        newrect.SetCorners(x1 + dx , y1 + dy , x3 + dx , y3 + dy);
                        break;
                     case MARGIN_CELL_MIDDLERIGHT :
                        newrect.SetCorners(x1 , y1 , x3 + dx , y3);
                        break;
                     case MARGIN_CELL_BOTTOMLEFT :
                        newrect.SetCorners(x2 , y2 , x4 + dx , y4 + dy);
                        break;
                     case MARGIN_CELL_BOTTOMMIDLE :
                        newrect.SetCorners(x1 , y1 , x3 , y3 + dy);
                        break;
                     case MARGIN_CELL_BOTTOMRIGHT :
                        newrect.SetCorners(x1 , y1 , x3 + dx , y3 + dy);
                        break;
                     default :
                        EagleLog() << "Error (MARGIN_CELL)index = unknown" << endl;
                        break;
                  }

                  rl.SetLayoutRectangle(&hsp , LayoutRectangle(rl.InnerArea() , newrect));
///                  hsp.SetWidgetArea(newrect);

               }
            }
         }


         while (gui.HasMessages()) {
            WidgetMsg wmsg = gui.TakeNextMessage();
            EagleLog() << wmsg << endl;
            for (int i = 0 ; i < 11 ; ++i) {
               if (wmsg.from == &mpbtns[i] && wmsg.msgs == BUTTON_TOGGLED) {
                  EagleLog() << "Button # " << i << " toggled." << endl;
                  if (mpbtns[i].Up()) {
                     win->ReleaseMousePointer((void*)((char*)GuiTestMain2 + i));
                  }
                  else {
                     win->AcquireMousePointer((void*)((char*)GuiTestMain2 + i) , (MOUSE_POINTER_TYPE)i , true);
                  }
               }
            }
            if (wmsg.from == &mpbtns[11] && wmsg.msgs == BUTTON_TOGGLED) {
               EagleLog() << "Button # " << 11 << " toggled." << endl;
               if (mpbtns[11].Up()) {
                  win->ReleaseMousePointer((void*)((char*)GuiTestMain2 + 11));
               }
               else {
                  win->AcquireMousePointer((void*)((char*)GuiTestMain2 + 11) , POINTER_NORMAL , false);
               }
            }
         }

      } while (!sys->UpToDate());


      if (redraw) {

         al_set_blender(ALLEGRO_ADD , ALLEGRO_ALPHA , ALLEGRO_INVERSE_ALPHA);
         gui.Display(win , 0 , 0);

         win->FlipDisplay();
         redraw = false;
      }
   }

   return 0;

}






int GuiTestMain3(int argc , char** argv , TestRunner* test_runner) {

   (void)argc;
   (void)argv;

   (void)test_runner;

   Allegro5GraphicsContext* win = dynamic_cast<Allegro5GraphicsContext*>(GetAllegro5WindowManager()->CreateWindow(800,600,EAGLE_WINDOWED));

   EAGLE_ASSERT(win);

   Allegro5Font verdana("Data/fonts/verdana.ttf" , -20 , 0);

   EAGLE_ASSERT(verdana.Valid());

   win->Clear(EagleColor(255,0,0));

   win->DrawTextString(&verdana , "GuiTestMain #3 incomplete." , 50 , 50 , EagleColor(255,255,255));
   win->DrawTextString(&verdana , "Press a key." , 50 , 50 , EagleColor(255,255,255));

   win->FlipDisplay();

   sys->GetSystemQueue()->WaitForEvent(EAGLE_EVENT_KEY_DOWN , 0);

   return 0;
}






