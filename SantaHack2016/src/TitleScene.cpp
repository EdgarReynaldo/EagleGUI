



#include "TitleScene.hpp"



bool TitleScene::Init(EagleGraphicsContext* window) {
   EAGLE_ASSERT(window && window->Valid());
   krampus_font = window->LoadFont("data/fonts/olde_english_regular.ttf" , -160 , LOAD_FONT_MONOCHROME);
   EAGLE_ASSERT(krampus_font && krampus_font->Valid());
   text.SetText("Krampus\nHack\n2016!");
   text.SetFont(krampus_font);
   text.Realign(HALIGN_CENTER , VALIGN_CENTER);
   landscape = window->LoadImageFromFile("data/images/SnowyLandscape1024x768.jpg");
   EAGLE_ASSERT(landscape && landscape->Valid());
   
   buffer = window->CreateImage(window->Width() , window->Height());
   EAGLE_ASSERT(buffer && buffer->Valid());
   
   window->SetDrawingTarget(buffer);
   window->SetCopyBlender();
   window->Clear();
   window->DrawStretchedRegion(landscape , 0 , 0 , landscape->W() , landscape->H() , 0 , 0 , window->Width() , window->Height());
   window->RestoreLastBlendingState();
   
   gui.SetDrawWindow(window);
   gui.SetWidgetArea(0 , 0 , window->Width() , window->Height());
   gui.SetRootLayout(&layout);
   gui.SetBackgroundColor(EagleColor(0,0,0,0));
   layout.Resize(1);
   layout.PlaceWidget(&text , 0 , LayoutRectangle(0.1, 0.1, 0.8 , 0.8));
   text.WCols()[TXTCOL] = EagleColor(255,0,0);
   
   hold_time = 3.0;
   fade_time = 6.0;
   scene_time = 9.0;
   time_elapsed = 0.0;
   eagle_system->GetSystemTimer()->ClearTicks();

   return true;
}



void TitleScene::Free(EagleGraphicsContext* window) {
   EAGLE_ASSERT(window);
   if (window) {
      text.SetFont(0);
      window->FreeFont(krampus_font);
      window->FreeImage(buffer);
      window->FreeImage(landscape);
      krampus_font = 0;
      buffer = 0;
      landscape = 0;
      gui.ClearLayout();
      gui.SetupBuffer(0 , 0 , window);
   }
}



void TitleScene::Display(EagleGraphicsContext* win) {
   win->SetDrawingTarget(win->GetBackBuffer());
   win->SetCopyBlender();
   win->Clear();
   win->Draw(buffer , 0 , 0);
   win->RestoreLastBlendingState();
//**
   gui.DrawBuffer(win);
   win->SetPMAlphaBlender();
   if (time_elapsed <= hold_time) {
      static int i = 1;
      if (i) {
         EagleLog() << "Time less than hold time" << std::endl;
         i = 0;
      }
      gui.DrawToWindow(win , 0 , 0);
   }
   else if (time_elapsed <= fade_time) {
      static int i = 1;
      if (i) {
         EagleLog() << "Time less than fade time" << std::endl;
         i = 0;
      }
      double percent = ((fade_time - time_elapsed)/(fade_time - hold_time));
      int c = (int)(255.0*percent);
      EagleImage* guibuf = gui.BufferBitmap();
      win->DrawTinted(guibuf , 0 , 0 , EagleColor(c,c,c,c));
   }
   else if (time_elapsed <= scene_time) {
      static int i = 1;
      if (i) {
         EagleLog() << "Time less than scene time" << std::endl;
         i = 0;
      }
   }
   win->RestoreLastBlendingState();
//*/
   win->FlipDisplay();
}



int TitleScene::HandleEvent(EagleEvent ev) {
   if (ev.type == EAGLE_EVENT_KEY_DOWN) {
      if (ev.keyboard.keycode == EAGLE_KEY_ESCAPE) {
         return STATE_COMPLETE;
      }
      else {
         if (time_elapsed < hold_time) {
            time_elapsed = hold_time;
         }
         else if (time_elapsed < fade_time) {
            time_elapsed = scene_time;
         }
         else {
            return STATE_COMPLETE;
         }
      }
   }
   return STATE_RUNNING;
}



int TitleScene::Update(double tsec) {
   time_elapsed += tsec;
   if (time_elapsed > scene_time) {
      return STATE_COMPLETE;
   }
   return STATE_RUNNING;
}
