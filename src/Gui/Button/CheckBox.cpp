


#include "Eagle/Gui/Button/CheckBox.hpp"



void DefaultCBDrawFunc(EagleGraphicsContext* win , Rectangle area , int xpos , int ypos , const WidgetColorset& wc , bool up , bool hover) {
   /// BG color
   area.MoveBy(xpos,ypos);
   win->DrawFilledRectangle(area , wc[BGCOL]);
   
   /// Shadow box
   area.Shrink(2);
   win->DrawFilledRectangle(area , wc[SDCOL]);
   
   /// FG box
   area.Shrink(4);
   if (up) {
      area.MoveBy(-2,-2);
   }
   win->DrawFilledRectangle(area , hover?wc[HVRCOL]:wc[FGCOL]);
   
   if (!up) {
      Pos2D p1(area.W() , area.H());
      Pos2D pcenter = p1*0.5;
      Pos2D left(p1.X()/4.0 , pcenter.Y());
      Pos2D bottom(pcenter.X() , p1.Y()*0.75);
      Pos2D topright(p1.X()*0.95 , p1.Y()*0.05);
      
      left.MoveBy(area.X() , area.Y());
      bottom.MoveBy(area.X() , area.Y());
      topright.MoveBy(area.X() , area.Y());
      
      float thickness = 0.08*(p1.X()>p1.Y()?p1.X():p1.Y());
      win->DrawLine(left.X() , left.Y() , bottom.X() , bottom.Y() , thickness , wc[MGCOL]);
      win->DrawLine(topright.X() , topright.Y() , bottom.X() , bottom.Y() , thickness , wc[MGCOL]);
   }
}



void BasicCheckBox::RefreshImages(EagleGraphicsContext* win) {
   FreeImages();
   if (draw_func) {
      for (int i = 0 ; i < 4 ; ++i) {
         bool up = (i%2)==0;
         bool hover = i/2;
         EagleImage* img = win->CreateImage(OuterArea().W() , OuterArea().H());
         win->PushDrawingTarget(img);
         draw_func(win , Rectangle(0 , 0 , img->W() , img->H()) , 0 , 0 , WCols() , up , hover);
         cb_images[i] = img;
         win->PopDrawingTarget();
         IconButton::SetImages(cb_images[0] , cb_images[1] , cb_images[2] , cb_images[3]);
      }
   }
   current = true;
}



void BasicCheckBox::FreeImages() {
   for (int i = 0 ; i < 4 ; ++i) {
      EagleImage* img = cb_images[i];
      if (img) {
         delete img;
      }
      cb_images[i] = 0;
   }
   current = false;
}



void BasicCheckBox::PrivateDisplay(EagleGraphicsContext* win , int xpos , int ypos) {
   if (!current) {
      RefreshImages(win);
   }
   IconButton::PrivateDisplay(win,xpos,ypos);
}



BasicCheckBox::BasicCheckBox(std::string objname) :
      IconButton("BasicCheckBox" , objname),
      draw_func(DefaultCBDrawFunc),
      cb_images(),
      current(false)
{
   for (int i = 0 ; i < 4 ; ++i) {
      cb_images[i] = 0;
   }
   SetButtonType(TOGGLE_BTN);
}



BasicCheckBox::~BasicCheckBox() {
   FreeImages();
}



void BasicCheckBox::SetWidgetArea(int xpos , int ypos , int width , int height , bool notify_layout) {
   int oldw = OuterArea().W();
   int oldh = OuterArea().H();
   IconButton::SetWidgetArea(xpos,ypos,width,height,notify_layout);
   if ((oldw != OuterArea().W()) || (oldh != OuterArea().H())) {
      if (draw_func) {
         current = false;
      }
   }
}



void BasicCheckBox::SetDrawFunc(CHECKBOXDRAWFUNC cbdrawfunc) {
   if (cbdrawfunc) {
      if (draw_func) {
         FreeImages();
      }
      draw_func = cbdrawfunc;
      current = false;
   }
}



void BasicCheckBox::SetImages(EagleImage* upimage , EagleImage* downimage , EagleImage* hoverupimage , EagleImage* hoverdownimage) {
   draw_func = 0;
   current = true;
   IconButton::SetImages(upimage , downimage , hoverupimage , hoverdownimage);
}



void BasicCheckBox::ToggleChecked() {
   SetButtonState(Hover() , !Up());
}



void BasicCheckBox::SetChecked(bool checked) {
   SetButtonState(Hover() , !checked);
}







