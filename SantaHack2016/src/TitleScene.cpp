



#include "TitleScene.hpp"



bool TitleScene::Init(EagleGraphicsContext* window) {
   EAGLE_ASSERT(window && window->Valid());
   
   our_window = window;
   
///   WidgetBase::DisplayWidgetArea(true);
   
   krampus_font = window->LoadFont("data/fonts/olde_english_regular.ttf" , -160 , LOAD_FONT_MONOCHROME);
   EAGLE_ASSERT(krampus_font && krampus_font->Valid());
   text.SetText("Krampus\nHack\n2016!");
   text.SetFont(krampus_font);
   text.Realign(HALIGN_CENTER , VALIGN_CENTER);
   landscape = window->LoadImageFromFile("data/images/SnowyLandscape1024x768.jpg");
   EAGLE_ASSERT(landscape && landscape->Valid());
   
   buffer = window->CreateImage(window->Width() , window->Height());
   EAGLE_ASSERT(buffer && buffer->Valid());
   
   lolbunnylogo_image = window->LoadImageFromFile("data/images/bunnies/ascii/BunnyLogo320x64.png");
   EAGLE_ASSERT(lolbunnylogo_image && lolbunnylogo_image->Valid());
   
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
   layout.PlaceWidget(&text , 0 , LayoutRectangle(0.0, 0.15, 1.0 , 0.7));
   text.ScaleToFit(false);
   text.WCols()[TXTCOL] = EagleColor(255,0,0);
   
   hold_time = 3.0;
   fade_time = 6.0;
   scene_time = 9.0;
   lol_move_in_time = 11;
   lol_stay_time = 14;
   lol_move_right_time = 16;
   lol_finish_time = 18;
   time_elapsed = 0.0;
   
   lol_dest_x = (window->Width() - 384)/2;
   
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

   EagleColor logo_tint_color(0,255,0);

   if (time_elapsed <= hold_time) {
      gui.DrawToWindow(win , 0 , 0);
   }
   else if (time_elapsed <= fade_time) {
      double percent = ((fade_time - time_elapsed)/(fade_time - hold_time));
      int c = (int)(255.0*percent);
      EagleImage* guibuf = gui.BufferBitmap();
      win->DrawTinted(guibuf , 0 , 0 , EagleColor(c,c,c,c));
   }
   else if (time_elapsed <= scene_time) {
///      gui.DrawToWindow(win , 0 , 0);
   }
   else if (time_elapsed <= lol_move_in_time) {
      gui.DrawToWindow(win , 0 , 0);
      double percent = (time_elapsed - scene_time)/(lol_move_in_time - scene_time);

      double move_in_left_width = 384 + lol_dest_x;
      double lx = -384 + percent*move_in_left_width;

      double move_in_right_width = (win->Width() - lol_dest_x) + 384;
      double rx = win->Width() + 384 - percent*move_in_right_width;
      
      win->DrawTinted(lolbunnylogo_image , lx , text.InnerArea().BRY() + 10 , logo_tint_color);
      win->DrawTinted(lolbunnylogo_image , rx , text.InnerArea().Y() - (10 + lolbunnylogo_image->H()) , logo_tint_color);
   }
   else if (time_elapsed <= lol_stay_time) {
      gui.DrawToWindow(win , 0 , 0);
      win->DrawTinted(lolbunnylogo_image , lol_dest_x , text.InnerArea().BRY() + 10 , logo_tint_color);
      win->DrawTinted(lolbunnylogo_image , lol_dest_x , text.InnerArea().Y() - (10 + lolbunnylogo_image->H()) , logo_tint_color);
   }
   else if (time_elapsed <= lol_move_right_time) {
      gui.DrawToWindow(win , 0 , 0);
      double percent = (time_elapsed - lol_stay_time)/(lol_move_right_time - lol_stay_time);
      double move_out_left_width = 384 + lol_dest_x;
      double lx = lol_dest_x - percent*move_out_left_width;
      
      double move_out_right_width = (win->Width() - lol_dest_x) + 384;
      double rx = lol_dest_x + percent*move_out_right_width;
      win->DrawTinted(lolbunnylogo_image , rx , text.OuterArea().BRY() + 10 , logo_tint_color);
      win->DrawTinted(lolbunnylogo_image , lx , text.OuterArea().Y() - (10 + lolbunnylogo_image->H()) , logo_tint_color);
   }
   else if (time_elapsed <= lol_finish_time) {
      gui.DrawToWindow(win , 0 , 0);
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
            time_elapsed = fade_time;
         }
         else if (time_elapsed < scene_time) {
            time_elapsed = scene_time;
            ResetText();
         }
         else if (time_elapsed < lol_move_in_time) {
            time_elapsed = lol_move_in_time;
         }
         else if (time_elapsed < lol_stay_time) {
            time_elapsed = lol_stay_time;
         }
         else if (time_elapsed < lol_move_right_time) {
            time_elapsed = lol_move_right_time;
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
   if (((time_elapsed - tsec) < scene_time) && (time_elapsed >= scene_time)) {
      ResetText();
   }
   if (time_elapsed >= lol_finish_time) {
      return STATE_COMPLETE;
   }
   return STATE_RUNNING;
}


void TitleScene::ResetText() {
   text.SetText("A LOL Bunny\nProduction...");
   text.ShrinkWrap();
   gui.SetFullRedraw();
   gui.DrawBuffer(our_window);
}
