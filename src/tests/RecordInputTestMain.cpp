

#include "RecordInputTestMain.hpp"

#include "Eagle/backends/Allegro5Backend.hpp"

#include <sstream>
using std::ostringstream;
#include <string>
using std::string;

int RecordInputTestMain(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
//   if (!SendOutputToFile("RecordInputTestMain.txt" , "Record input test\n\n" , false)) {return 1;}

   Allegro5System sys;
   
   int start_state = EAGLE_FULL_SETUP;
   if (sys.Initialize(start_state) != start_state) {
      return -1;
   }

   int SCREEN_WIDTH = 800;
   int SCREEN_HEIGHT = 600;
   EagleGraphicsContext* win = sys.CreateGraphicsContext(SCREEN_WIDTH , SCREEN_HEIGHT , EAGLE_WINDOWED | EAGLE_OPENGL);
   
   if (!win->Valid()) {
      return -2;
   }
   
   EagleFont* font40 = win->LoadFont("Data/fonts/verdana.ttf" , 40 , 0 , VIDEO_IMAGE);
   EagleFont* font20 = win->LoadFont("Data/fonts/verdana.ttf" , 20 , 0 , VIDEO_IMAGE);
   
   if (!font40->Valid() || !font20->Valid()) {
      return -3;
   }

   int count = 0;
   int scount = 0;
   bool quit = false;
   bool redraw = true;
   int fps = 0;
   int framecount = 0;

   Input press;
   Input held;
   InputGroup ig;
   
   sys.GetSystemTimer()->Start();

   do {

      if (redraw) {
         
         ostringstream oss;
         oss.str("");
         ig.ShowLogic(oss);
         
         
         win->Clear(EagleColor(0,0,0));
         win->DrawTextString(font40 , StringPrintF("FPS : %i" , fps) , 40 , 40 , EagleColor(0,255,255));
         win->DrawTextString(font40 , StringPrintF("framecount = %i" , framecount + 1) , 40 , 80 , EagleColor(0,255,255));
         win->DrawTextString(font20 , oss.str() , 400 , 300 , EagleColor(255,255,255) , HALIGN_CENTER , VALIGN_TOP);
         win->FlipDisplay();
         ++framecount;
         redraw = false;
      }

      do {
         EagleEvent ee = sys.UpdateSystemState();
         
         if (AnyInputPressed(&press)) {
            if (ModifierHeld(&held)) {
               ig = press && held;
            }
            else {
               ig = InputGroup(press);
            }
            redraw = true;
         }
         
         if (ee.type == EAGLE_EVENT_TIMER && true/*ee.timer.source == sys.GetSystemTimer()*/) {
            count += 1;
            if (count == 60) {
               scount += 1;
               count = 0;
               fps = framecount;
               framecount = 0;
            }
//            fps = count;
            redraw = true;
         }
         else if (ee.type == EAGLE_EVENT_KEY_DOWN) {
            if (ee.keyboard.keycode == EAGLE_KEY_ESCAPE) {
               quit = true;
               break;
            }
         }
         if (ee.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
            break;
         }
         
      } while (!sys.UpToDate());
   } while (!quit);


   return 0;
}






