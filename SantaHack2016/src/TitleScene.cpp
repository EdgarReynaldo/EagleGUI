



#include "TitleScene.hpp"



bool TitleScene::Init(EagleGraphicsContext* window) {
   EAGLE_ASSERT(window);
   our_window = window;
   landscape = our_window->LoadImageFromFile("Data/Images/SnowyLandscape800x600.jpg");
   return text_display.Init(our_window) && landscape->Valid();
}



void TitleScene::Display(EagleGraphicsContext* win) {
   win->Clear(EagleColor(0,0,0));
   win->DrawStretchedRegion(landscape , 0 , 0 , landscape->W() , landscape->H() , 0 , 0 , win->Width() , win->Height());
   text_display.DrawText(win);
   win->FlipDisplay();
}



int TitleScene::HandleEvent(EagleEvent ev) {
   if (ev.type == EAGLE_EVENT_KEY_DOWN) {
      return SCENE_COMPLETE;
   }
   return SCENE_RUNNING;
}



int TitleScene::Update(double tsec) {
   return SCENE_RUNNING;
}
