





#include "Eagle.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"

#include "Avatars.hpp"

#include "SceneManager.hpp"

using namespace std;

int main(int argc , char** argv) {
   (void)argc;
   (void)argv;
   
   SendOutputToFile("SantaHackLog.txt" , "" , false);
   
   Allegro5System* sys = new Allegro5System();
   
   if (sys->Initialize(EAGLE_FULL_SETUP) != EAGLE_FULL_SETUP) {
      return 1;
   }

   Allegro5GraphicsContext* win = dynamic_cast<Allegro5GraphicsContext*>(sys->CreateGraphicsContext(1024,768 , EAGLE_WINDOWED | EAGLE_OPENGL));
   if (!win || !win->Valid()) {return 2;}
   
   if (!PopulateAvatars()) {
      EagleInfo() << "Failed to populate avatars." << endl;
   }
   
   SceneManager sm;
   
   sm.Init(win);
   
   sm.Run();
   
/**
   WidgetHandler gui(win);

   RelativeLayout relative;
   
   relative.Resize(4);
   
   GridLayout grid1;
   GridLayout grid2;
   GridLayout grid3;
   GridLayout grid4;
   
   relative.PlaceWidget(&grid1 , 0 , LayoutRectangle(0 , 0 , 1 , 0.5));
   relative.PlaceWidget(&grid2 , 1 , LayoutRectangle(0 , 0.5 , 1 , 0.3));
   relative.PlaceWidget(&grid3 , 2 , LayoutRectangle(0 , 0.8 , 1 , 0.1));
   relative.PlaceWidget(&grid4 , 3 , LayoutRectangle(0 , 0.9 , 1 , 0.1));
   
   gui.SetWidgetArea(0 , 0 , win->Width() , win->Height());

   gui.SetRootLayout(&relative);
   
   grid1.ResizeGrid(NUM_AVATARS/2, 2);
   grid2.ResizeGrid(NUM_AVATARS/2, 2);
   grid3.ResizeGrid(NUM_AVATARS, 1);
   grid4.ResizeGrid(NUM_AVATARS, 1);

   
   bool stretch = true;
   GridLayout* g = 0;
   BasicIcon icons[4][NUM_AVATARS];
   for (int j = 0 ; j < 4 ; ++j) {
      int sz = 0;
      bool bw = false;
      switch (j) {
         case 0 :
            sz = 160;
            g = &grid1;
            break;
         case 1 : 
            sz = 64;
            g = &grid2;
            break;
         case 2 : 
            sz = 32;
            g = &grid3;
            break;
         case 3 :
            sz = 32;
            bw = true;
            g = &grid4;
             break;
      }
      int x = 0;
      win->Clear(EagleColor(0,0,0));
      Allegro5Image img;
      for (int i = 0 ; i < NUM_AVATARS ; ++i) {
         icons[j][i].SetName(StringPrintF("BasicIcon row %d column %d , avatar %s , size %d x %d %s",
                                          j , i , avatar_names[i] , sz , sz , bw?"bw":""));
         icons[j][i].SetStretch(stretch);
         ALLEGRO_BITMAP* avatar = GetAvatar(sz , bw , i);
         img.ReferenceBitmap(avatar);
         win->Draw(&img , x , 0);
         x += win->Width()/NUM_AVATARS;
         
         EAGLE_ASSERT(avatar);
         icons[j][i].SetImage(win->ReferenceImage(avatar));
         g->PlaceWidget(&(icons[j][i]) , i);
      }
      win->FlipDisplay();
      sys->GetSystemQueue()->WaitForEvent(EAGLE_EVENT_KEY_DOWN);
   }
   
   sys->GetSystemTimer()->Start();
   
   bool quit = false;
   bool redraw = true;
   bool clip = true;
   
   EagleLog() << gui << endl << endl;
   
   WidgetBase* hover = 0;
   WidgetBase* old_hover = 0;
   while (!quit) {
      if (redraw) {
         gui.Display(win , 0 , 0);
         win->DrawTextString(win->DefaultFont() , StringPrintF("Clip(%s) : Hover is %p (%s)" , clip?"Y":"N" ,
                                                               hover , hover?hover->GetName().c_str():"NULL"),
                             10 , win->Height() - win->DefaultFont()->Height() - 10 , EagleColor(255,255,255));
         win->FlipDisplay();
         redraw = false;
      }
      do {

         EagleEvent ee = sys->WaitForSystemEventAndUpdateState();
         
         gui.HandleEvent(ee);
         
         hover = gui.CurrentHover();
         if (hover != old_hover) {gui.SetFullRedraw();redraw = true;}
         old_hover = hover;

         if (ee.type == EAGLE_EVENT_TIMER) {
            redraw = true;
         }
         if (ee.type == EAGLE_EVENT_DISPLAY_CLOSE) {
            quit = true;
         }
         if (ee.type == EAGLE_EVENT_KEY_DOWN && ee.keyboard.keycode == EAGLE_KEY_ESCAPE) {
            quit = true;
         }
         if (ee.type == EAGLE_EVENT_KEY_DOWN && ee.keyboard.keycode == EAGLE_KEY_C) {
            WidgetBase::ClipWidgets(clip = !clip);
            gui.SetFullRedraw();
            redraw = true;
         }
         if (ee.type == EAGLE_EVENT_KEY_DOWN && ee.keyboard.keycode == EAGLE_KEY_S) {
            stretch = !stretch;
            for (int i = 0 ; i < 12 ; ++i) {
               for (int j = 0 ; j < 4 ; ++j) {
                  icons[j][i].SetStretch(stretch);
               }
            }
            gui.SetFullRedraw();
            redraw = true;
         }
         
      } while (!sys->UpToDate());

   }

//*/

   return 0;
}
