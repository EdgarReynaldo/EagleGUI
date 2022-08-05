



#include "Eagle/backends/Allegro5Backend.hpp"




int main(int argc , char** argv) {
   
   (void)argc;
   (void)argv;
   
   EagleSystem* sys = GetAllegro5System();
   
   EAGLE_ASSERT(sys);
   
   int ret = sys->Initialize(EAGLE_FULL_SETUP);
   if (ret != EAGLE_FULL_SETUP) {
      EagleLog() << "Failed to initialized subsystem. Continuing." << std::endl;
   }
   
   int sw = 1024;
   int sh = 768;
   
   EagleGraphicsContext* win = sys->CreateGraphicsContext("win" , sw , sh , EAGLE_OPENGL | EAGLE_WINDOWED | EAGLE_NOFRAME);
   EAGLE_ASSERT(win && win->Valid());
   
   WidgetHandler gui(win , "GUI" , "Main GUI");
   
   gui.SetWidgetArea(Rectangle(0 , 0 , sw , sh) , false);
   
   gui.SetupBuffer(sw , sh , win);
   
   EagleImage* orangenp = win->LoadImageFromFile("Data/Images/OrangeYellowNP.png");
   EagleImage* whitenp = win->LoadImageFromFile("Data/Images/AllPurposeNP.png");
   EagleImage* blacknp = win->LoadImageFromFile("Data/Images/AllPurpose2NP.png");
   
   NPRAREA npr(Rectangle(0,0,orangenp->W(),orangenp->H()) , 0.33 , 0.33);
   NPAREA npa = npr.GetNP();
   
   NPAREA npa2(Rectangle(0,0,whitenp->W(),whitenp->H()) , BOXAREA(32,32,32,32));
   NPAREA npa3(Rectangle(0,0,whitenp->W(),whitenp->H()) , BOXAREA(250,250,250,250));
   
   NinePatch* np = win->CreateNinePatchSubImages(orangenp , npa);
   NinePatch* npw = win->CreateNinePatchSubImages(whitenp , npa2);
   NinePatch* npb = win->CreateNinePatchSubImages(blacknp , npa3);
   
   
   bool quit = false;
   bool redraw = true;
   
   sys->GetSystemTimer()->Start();
   
   while (!quit) {
      if (redraw) {
         win->SetDrawingTarget(win->GetBackBuffer());
         win->Clear();
         gui.Display(win , 0 , 0);
         win->FlipDisplay();
         redraw = false;
      }
      do {
         EagleEvent e = sys->WaitForSystemEventAndUpdateState();
      } while (!sys->UpToDate());
         
   };
   
   
   return 0;
}
