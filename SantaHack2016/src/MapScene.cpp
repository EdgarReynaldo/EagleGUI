



#include "MapScene.hpp"






bool MapScene::Init(EagleGraphicsContext* window) {
   if (!CreateTiles(window)) {
      return false;
   }
   game_map.Load(default_game_map);
   return true;
}



void MapScene::Free(EagleGraphicsContext* window) {
   
}



void MapScene::Display(EagleGraphicsContext* window) {
   window->Clear(EagleColor(0,255,0));
   game_map.Draw(window , 0 , 0 , 1024 , 768);
   window->FlipDisplay();
}



int MapScene::HandleEvent(EagleEvent ev) {
   if (ev.type == EAGLE_EVENT_KEY_DOWN && ev.keyboard.keycode == EAGLE_KEY_ESCAPE) {
      return STATE_COMPLETE;
   }
   if (ev.type == EAGLE_EVENT_MOUSE_AXES) {
      if (ev.mouse.x < 5) {
         map_vx = -64;
      }
      else if (ev.mouse.x > our_window
   }
   return STATE_RUNNING;
}



int MapScene::Update(double tsec) {
   time_elapsed += tsec;
   return STATE_RUNNING;
}



