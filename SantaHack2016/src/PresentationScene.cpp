


#include "PresentationScene.hpp"

#include "Avatars.hpp"


   
PresentationScene::PresentationScene() :
      edgar_image(0),
      edgar_font(0),
      gui(0),
      relative("Relative Layout for presentation scene"),
      edgar_icon("Edgar Avatar icon"),
      edgar_text("Edgar Reynaldo text"),
      present_text("Present text"),
      fade_time(0.0),
      display_time(0.0),
      elapsed_time(0.0)
{}



bool PresentationScene::Init(EagleGraphicsContext* window) {
   EAGLE_ASSERT(window && window->Valid());
   edgar_image = dynamic_cast<Allegro5Image*>(window->EmptyImage());
   edgar_image->ReferenceBitmap(GetAvatar(160 , false , 7));
   edgar_font = dynamic_cast<Allegro5Font*>(window->LoadFont("data/fonts/olde_english_regular.ttf" , -100 , LOAD_FONT_MONOCHROME));
   EAGLE_ASSERT(edgar_font && edgar_font->Valid());
   
   gui.SetDrawWindow(window);
   gui.SetBackgroundColor(EagleColor(0,0,0,0));
   gui.SetWidgetArea(0 , 0 , window->Width() , window->Height());
   gui.SetRootLayout(&relative);
   
   relative.Resize(3);
   
   edgar_icon.SetImage(edgar_image);
   edgar_icon.SetStretch(true);
   
   edgar_text.SetFont(edgar_font);
   edgar_text.SetText("Edgar Reynaldo");
   edgar_text.Realign(HALIGN_CENTER , VALIGN_CENTER);
   
   present_text.SetFont(edgar_font);
   present_text.SetText("Presents!!!");
   present_text.Realign(HALIGN_CENTER , VALIGN_CENTER);
   
   relative.PlaceWidget(&edgar_text , 0 , LayoutRectangle(0.1 , 0.1 , 0.8 , 0.2));
   relative.PlaceWidget(&edgar_icon , 1 , LayoutRectangle(0.35 , 0.35 , 0.35 , 0.35));
   relative.PlaceWidget(&present_text , 2 , LayoutRectangle(0.1 , 0.75 , 0.8 , 0.2));
   
   fade_time = 3.0;
   display_time = 6.0;
   elapsed_time = 0.0;
   return true;
}



void PresentationScene::Free(EagleGraphicsContext* window) {
   
   edgar_text.SetFont(0);
   present_text.SetFont(0);
   
   edgar_icon.SetImage(0);
   
   gui.ClearLayout();
   
   window->FreeFont(edgar_font);
   window->FreeImage(edgar_image);
   
}



void PresentationScene::Display(EagleGraphicsContext* window) {
   
   gui.DrawBuffer(window);
   
   EagleImage* guibuf = gui.BufferBitmap();
   
   window->Clear();
   
   if (elapsed_time <= fade_time) {
      int c = (int)(255.0*(elapsed_time/fade_time));
      window->DrawTinted(guibuf , 0 , 0 , EagleColor(c,c,c,c));
   }
   else {
      window->Draw(guibuf , 0 , 0);
   }
   window->FlipDisplay();
}



int PresentationScene::HandleEvent(EagleEvent ev) {
   
   if (ev.type == EAGLE_EVENT_KEY_DOWN) {
      if (ev.keyboard.keycode == EAGLE_KEY_ESCAPE) {
         return STATE_COMPLETE;
      }
      else {
         if (elapsed_time < fade_time) {
            elapsed_time = fade_time;
         }
         else {
            return STATE_COMPLETE;
         }
      }
   } 
   return STATE_RUNNING;
}



int PresentationScene::Update(double tsec) {
   elapsed_time += tsec;
   if (elapsed_time > display_time) {
      return STATE_COMPLETE;
   }
   return STATE_RUNNING;   
}



