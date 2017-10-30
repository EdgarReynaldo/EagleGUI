


#include "PillFont.hpp"



const int NCHAR = 17;



void DrawPill(EagleColor c) {
   EagleImage* dst = our_win->GetDrawingTarget();
   our_win->Clear(EagleColor(0,0,0,0));
   float cx = dst->W()/2.0f;
   float cy = dst->H()/2.0f;
   float cr = dst->W()/2.0f;
   our_win->DrawFilledCircle(cx , cy , cr  , EagleColor(0,0,0,255));
   our_win->DrawFilledCircle(cx , cy , cr - 3 , c);
}



void DrawPillOne(EagleColor c) {
   DrawPill(EagleColor(255,255,255));
   EagleImage* dst = our_win->GetDrawingTarget();
   double cx = dst->W()/2.0;
   double cy = dst->H()/2.0;
   double cr = 5.0*dst->W()/16;
   our_win->DrawFilledCircle((float)cx , cy , cr , c);
}



void DrawPillTwo(EagleColor c) {
   DrawPill(EagleColor(255,255,255));
   EagleImage* dst = our_win->GetDrawingTarget();
   double cx = dst->W()/2.0;
   double cy = dst->H()/2.0;
    double cr = 3.0*dst->W()/8;
   Pos2D topleft = Vector(Pos2D(cx,cy) , cr , -3*M_PI/4.0);
   double w = 2.0f*cr*(1.0f/sqrt(2.0f));
   our_win->DrawFilledRectangle((float)topleft.X() , topleft.Y() , w , w , c);
}



void DrawPillThree(EagleColor c) {
   DrawPill(EagleColor(255,255,255));
   EagleImage* dst = our_win->GetDrawingTarget();
   double cx = dst->W()/2.0;
   double cy = dst->H()/2.0;
   double cr = 3.0*dst->W()/8;
   Pos2D center(cx,cy);
   Pos2D p1 = Vector(center , cr , -M_PI/2);
   Pos2D p2 = Vector(center , cr , -M_PI/2 + 2*M_PI/3);
   Pos2D p3 = Vector(center , cr , -M_PI/2 - 2*M_PI/3);
   our_win->DrawFilledTriangle((float)p1.X() , p1.Y() , p2.X() , p2.Y() , p3.X() , p3.Y() , c);
}



void DrawPillFour(EagleColor c) {
   DrawPill(EagleColor(255,255,255));
   EagleImage* dst = our_win->GetDrawingTarget();
   double cx = dst->W()/2.0;
   double cy = dst->H()/2.0;
   double cr = 3.0*dst->W()/8;
   Pos2D center(cx,cy);
   Pos2D p1 = Vector(center , cr , -M_PI/2);
   Pos2D p2 = Vector(center , cr , -M_PI/2 + 2*M_PI/3);
   Pos2D p3 = Vector(center , cr , -M_PI/2 - 2*M_PI/3);
   our_win->DrawFilledTriangle((float)p1.X() , p1.Y() , p2.X() , p2.Y() , p3.X() , p3.Y() , c);
   Pos2D p4 = (p1 + p2)*0.5;
   Pos2D p5 = (p2 + p3)*0.5;
   Pos2D p6 = (p3 + p1)*0.5;
   our_win->DrawFilledTriangle((float)p4.X() , p4.Y() , p5.X() , p5.Y() , p6.X() , p6.Y() , EagleColor(255,255,255));
}



void DrawPillGlyph(int i) {

   if (i < 0) {i = 0;}
   if (i >= NCHAR) {i = NCHAR - 1;}
   
   if (i == (NCHAR - 1)) {
      DrawPill(EagleColor(255,255,255));
      return;
   }

   PILL_DRAW_FUNC drawfuncs[4] = {DrawPillOne , DrawPillTwo , DrawPillThree , DrawPillFour};
   EagleColor colors[4] = {EagleColor(255,0,0) , EagleColor(0,255,0) , EagleColor(255,255,0) , EagleColor(0,0,255)};
   drawfuncs[(i/4)](colors[i%4]);
   
}



EagleImage* CreatePillFontImage(int w , int h) {
   
///   return our_win->LoadImageFromFile("PillImage.png");
   
   int cw = w + 8;
   int ch = h + 8;
   int tw = cw*NCHAR;
   int th = ch;
   
   EagleImage* font_img = our_win->CreateImage(tw , th);
   
   our_win->SetDrawingTarget(font_img);
   our_win->SetCopyBlender();
   /// Background color of image
   our_win->Clear(EagleColor(255,0,255,255));/// magic pink (wink wink)
   
   for (int i = 0 ; i < NCHAR ; ++i) {
      our_win->SetDrawingTarget(font_img);
      int x = i*cw + 2;
      int y = 2;
      /// Background color of glyph
      our_win->DrawFilledRectangle(x,y,w + 4,h + 4,EagleColor(0,255,0,255));
      EagleImage* sub = our_win->CreateSubImage(font_img , x + 2 , y + 2 , w , h);
      our_win->SetDrawingTarget(sub);
      our_win->Clear(EagleColor(0,0,0,0));
      DrawPillGlyph(i);
      our_win->FreeImage(sub);
   }
   our_win->RestoreLastBlendingState();
   
   return font_img;
}


EagleFont* CreatePillFont(int w , int h) {
   EagleImage* font_img = CreatePillFontImage(w,h);
   
   ALLEGRO_BITMAP* bmp = (dynamic_cast<Allegro5Image*>(font_img))->AllegroBitmap();
   
   int ranges[] = {'0' , '9' , 'a' , 'f' , ' ' , ' '};
   
   ALLEGRO_FONT* font = al_grab_font_from_bitmap(bmp , 3 , ranges);
   EAGLE_ASSERT(font);
   
   EagleFont* new_font = new Allegro5Font(font);
   
   our_win->FreeImage(font_img);
   
   return new_font;
}





