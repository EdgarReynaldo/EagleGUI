

#include "Allegro5TestMain.hpp"

#include "Eagle/backends/Allegro5Backend.hpp"


#include <cstdio>

DEFINE_TEST(Allegro5TestMain , "Simple Allegro 5 Graphics Test");


int Allegro5TestMain(int argc , char** argv , TestRunner* test_runner) {

   (void)test_runner;

   if (!al_init()) {return -1;}
   
   double t1 = 0.0 , t2 = 0.0;
   
   (void)argc; 
   (void)argv;
   
   int SCREEN_WIDTH = 800;
   int SCREEN_HEIGHT = 600;
   t1 = al_get_time();
   EagleGraphicsContext* win = sys->CreateGraphicsContext("WIN" , SCREEN_WIDTH , SCREEN_HEIGHT , EAGLE_WINDOWED);
   t2 = al_get_time();
   printf("sys->CreateGraphicsContext(default window) took %f seconds\n" , t2 - t1);
   
   if (!win) {return -1;}
   if (!win->Valid()) {return -1;}
   
   win->Clear(EagleColor(0,0,0));

   t1 = al_get_time();
   EagleImage* splash_img = win->LoadImageFromFile("Data/SquireImages/GoodVsEvil.jpg" , VIDEO_IMAGE);
   t2 = al_get_time();
   printf("win->LoadImageFromFile took %f seconds\n" , t2 - t1);
   
   if (!splash_img->Valid()) {
      return -2;
   }

   win->Draw(splash_img , (SCREEN_WIDTH - splash_img->W())/2 , (SCREEN_HEIGHT - splash_img->H())/2 , DRAW_NORMAL);
   
   win->FlipDisplay();
   
   al_rest(3.0);

   t1 = al_get_time();
   EagleFont* font = win->LoadFont("Data/fonts/verdana.ttf" , -70 , LOAD_FONT_NORMAL  , VIDEO_IMAGE);
   t2 = al_get_time();
   printf("win->LoadFont took %f seconds\n" , t2 - t1);
   
   if (!font->Valid()) {
      return -3;
   }

   win->Clear(EagleColor(0,0,0));
   
   win->DrawTextString(font , "Hello verdana.ttf!!!111~~~" , SCREEN_WIDTH/2 , SCREEN_HEIGHT/2 , EagleColor(255,127,255),
                 HALIGN_CENTER , VALIGN_CENTER);
                 
   win->FlipDisplay();

   al_rest(3.0);

   return 0;
}
