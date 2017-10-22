
#include "Animation.hpp"
#include "Intro.hpp"
#include "Eagle/backends/Allegro5Backend.hpp"
#include "allegro5/allegro_color.h"


int window_width = 1024;
int window_height = 768;

EagleFont* our_font = 0;

EagleGraphicsContext* our_win = 0;

#define M_PI 3.14159265

EagleColor Hue(double hue);
EagleColor Hue(double hue) {
   hue = 360.0*(hue - (float)(int)hue);
   float r,g,b;
   al_color_hsv_to_rgb(hue , 1 , 1 , &r , &g , &b);
   return EagleColor(r,g,b,1.0);
}

EagleColor Red        (double pct) {return Hue(pct);}
EagleColor Orange     (double pct) {return Hue(pct + 1.0/12.0);}
EagleColor Yellow     (double pct) {return Hue(pct + 2.0/12.0);}
EagleColor YellowGreen(double pct) {return Hue(pct + 3.0/12.0);}
EagleColor Green      (double pct) {return Hue(pct + 4.0/12.0);}
EagleColor GreenBlue  (double pct) {return Hue(pct + 5.0/12.0);}
EagleColor Cyan       (double pct) {return Hue(pct + 6.0/12.0);}
EagleColor BlueGreen  (double pct) {return Hue(pct + 7.0/12.0);}
EagleColor Blue       (double pct) {return Hue(pct + 8.0/12.0);}
EagleColor Purple     (double pct) {return Hue(pct + 9.0/12.0);}
EagleColor HotPink    (double pct) {return Hue(pct + 10.0/12.0);}
EagleColor Magenta    (double pct) {return Hue(pct + 11.0/12.0);}



Pos2d Upper(double percent) {
   float z = our_font->Height()*sin(2*M_PI*percent);
   return Pos2d(window_width / 2 , window_height/4 - z);
}
Pos2d Lower(double percent) {
   float z = our_font->Height()*sin(2*M_PI*percent);
   return Pos2d(window_width / 2 , window_height*3/4 + z);
}
Pos2d Left(double percent) {
   float z = our_font->Height()*sin(2*M_PI*percent);
   return Pos2d(window_width/4 - z , window_height/2);
}
Pos2d Right(double percent) {
   float z = our_font->Height()*sin(2*M_PI*percent);
   return Pos2d(window_width*3/4 + z , window_height/2);
}
Pos2d UpperLeft(double percent) {
   float z = our_font->Height()*sin(2*M_PI*percent);
   return Pos2d(window_width/4 - z , window_height/4 - z);
}
Pos2d LowerLeft(double percent) {
   float z = our_font->Height()*sin(2*M_PI*percent);
   return Pos2d(window_width/4 - z , window_height*3/4 + z);
}
Pos2d UpperRight(double percent) {
   float z = our_font->Height()*sin(2*M_PI*percent);
   return Pos2d(window_width*3/4 + z, window_height/4 - z);
}
Pos2d LowerRight(double percent) {
   float z = our_font->Height()*sin(2*M_PI*percent);
   return Pos2d(window_width*3/4 + z , window_height*3/4 + z);
}

Pos2d SpiralPos(double percent) {
   if (percent < 0.5) {
      percent *= 2;
   }
   else {
      percent = 2.0*(1.0 - percent);
   }
   double r = our_font->Height()*percent;
   double theta = 2.0*M_PI*percent;
   return Pos2d(r*cos(theta) , r*sin(theta));
}

Pos2d Center(double percent) {
   percent = (float)(percent - (int)percent);
   return Pos2d(our_win->Width()*percent/2.0 , our_win->Height()*percent/2.0);
}
Pos2d CenterOffset(double percent) {
   return Center(percent)+ Pos2d(5,5);
}
Transform UpperRightTransform(double percent) {\
   Transform t = our_win->GetTransformer()->CreateTransform();
   t.GetIdentityMatrix();
   Pos2d spos = SpiralPos(percent);
   Pos2d center = Center(percent);
   t.Translate(-center.X() , -center.Y());
   t.Rotate(2.0*M_PI*percent , 0 , 0 , 1);
   t.Scale(percent , percent);
   t.Translate(center.X() , center.Y());
   t.Translate(spos.X() , spos.Y());
   return t;
}


Transform CenterTransform(double percent) {\
   Transform t = our_win->GetTransformer()->CreateTransform();
   t.GetIdentityMatrix();
//   Pos2d spos = SpiralPos(.99);
   Pos2d center = Center(.99);
///   t.Translate(-center.X() , -center.Y());
   t.Rotate(2.0*M_PI*.99 , 0 , 0 , 1);
   t.Scale(percent*8 , percent*8);
   t.Translate(center.X() , center.Y());
//   t.Translate(spos.X() , spos.Y());
   return t;
}

Pos2d Zero(double percent) {return Pos2d(0,0);}
Pos2d ZeroOffset(double percent) {return Pos2d(8,8);}

Transform StretchTransform(double percent) {
   Transform t = our_win->GetTransformer()->CreateTransform();
   t.GetIdentityMatrix();
   t.Scale(1.0 , percent);
   Pos2d center = Center(0.99);
   t.Translate(center.X() , center.Y());
   return t;
}

EagleColor White(double percent) {return EagleColor((float)percent,percent,percent,percent);}      
EagleColor Black(double percent) {return EagleColor(0,0,0,255);}      


EagleImage* RunIntro() {
   
   Allegro5System* a5sys = GetAllegro5System();
   
   our_win = a5sys->GetWindowManager()->CreateWindow("TINS:our_win" , window_width , window_height , EAGLE_OPENGL | EAGLE_WINDOWED);
   
   EagleImage* img = our_win->CreateImage(our_win->Width() , our_win->Height());
   
   EAGLE_ASSERT(our_win && our_win->Valid());
   
   our_font = our_win->LoadFont("Verdana.ttf" , -96);
   
   EAGLE_ASSERT(our_font && our_font->Valid());
   
   bool corner = false;
   
   XYTextAnimation xyt_upper(our_font , "Welcome\nto\nTINS 2017!\n" , Red , Upper);
   XYTextAnimation xyt_lower(our_font , "Welcome\nto\nTINS 2017!\n" , Orange , Lower);
   XYTextAnimation xyt_right(our_font , "Welcome\nto\nTINS 2017!\n" , Yellow, Right);
   XYTextAnimation xyt_left(our_font , "Welcome\nto\nTINS 2017!\n" , Magenta, Left);
   
   XYTextAnimation xyt_upper_right(our_font , "Welcome\nto\nTINS 2017!\n" , Green , UpperRight);
   XYTextAnimation xyt_lower_right(our_font , "Welcome\nto\nTINS 2017!\n" , Cyan , LowerRight);
   XYTextAnimation xyt_upper_left(our_font , "Welcome\nto\nTINS 2017!\n" , Blue , UpperLeft);
   XYTextAnimation xyt_lower_left(our_font , "Welcome\nto\nTINS 2017!\n" , Purple , LowerLeft);
   
   TextAnimation spinny_thing(our_font , "Welcome\nto\nTINS 2017!\n" , UpperRightTransform , Red , Center);
   TextAnimation spinny_thing2(our_font , "Welcome\nto\nTINS 2017!\n" , CenterTransform , Red , Zero);
   
   EagleFont* title_font = our_win->LoadFont("Verdana.ttf" , -64);

   TextAnimation title(title_font , "Dr. Von Chicken Pox" , StretchTransform , White , Zero);
   TextAnimation title_shadow(title_font , "Dr. Von Chicken Pox" , StretchTransform , Black , ZeroOffset);
   
   bool quit = false;
   bool redraw = true;
   
   double old_pct = 0.0;
   double pct = 0.0;
   
   do {
      if (redraw) {
         our_win->DrawToBackBuffer();
         our_win->Clear(EagleColor(0,0,0));
         
         old_pct = pct;
         pct = ProgramTime::Elapsed()/3;
         
         if (pct < 1.0) {
            spinny_thing.SetAnimationPercent(pct*.99);
            spinny_thing.Draw(our_win , 0 , 0);
         }
         else if (pct < 2.0) {
            spinny_thing.SetAnimationPercent(.99);
            spinny_thing.Draw(our_win , 0 , 0);
         }
         else if (pct < 4.0) {
            spinny_thing2.SetAnimationPercent(pct - 2);
            spinny_thing2.Draw(our_win , 0 , 0);
         }
         else if (pct < 8.0) {
            pct = (pct - 4)*2 + 4;
            spinny_thing2.SetAnimationPercent(pct - 2);
            spinny_thing2.Draw(our_win , 0 , 0);
            title.SetAnimationPercent(pct - 4);
            title_shadow.SetAnimationPercent(pct - 4);
            title_shadow.Draw(our_win , 0 , 0);
            title.Draw(our_win , 0 , 0);
         }
         else {
            our_win->SetDrawingTarget(img);
            our_win->SetCopyBlender();
            our_win->Draw(our_win->GetBackBuffer() , 0 , 0);
            quit = true;
         }
/*         
         if ((int)pct - (int)old_pct) {
            corner = !corner;
         }
         xyt_upper.SetAnimationPercent(pct);
         xyt_lower.SetAnimationPercent(pct);
         xyt_right.SetAnimationPercent(pct);
         xyt_left.SetAnimationPercent(pct);
         xyt_upper_right.SetAnimationPercent(pct);
         xyt_lower_right.SetAnimationPercent(pct);
         xyt_upper_left.SetAnimationPercent(pct);
         xyt_lower_left.SetAnimationPercent(pct);
         
         if (!corner) {
            xyt_upper.Draw(our_win , 0 , 0);
            xyt_lower.Draw(our_win , 0 , 0);
            xyt_right.Draw(our_win , 0 , 0);
            xyt_left.Draw(our_win , 0 , 0);
         }
         else {
            xyt_upper_right.Draw(our_win , 0 , 0);
            xyt_lower_right.Draw(our_win , 0 , 0);
            xyt_upper_left.Draw(our_win , 0 , 0);
            xyt_lower_left.Draw(our_win , 0 , 0);
         }
*/
         
         our_win->FlipDisplay();
         
         redraw = false;
      }
      
      do {
         EagleEvent e = a5sys->GetSystemQueue()->WaitForEvent(0);
         
         if (e.type == EAGLE_EVENT_TIMER) {
            redraw = true;
         }
         if (e.type == EAGLE_EVENT_DISPLAY_CLOSE || (e.type == EAGLE_EVENT_KEY_DOWN && e.keyboard.keycode == EAGLE_KEY_ESCAPE)) {
            quit = true;
         }
         
      } while (a5sys->GetSystemQueue()->HasEvent(0));
      
      
   } while (!quit);
   
   return img;
}

