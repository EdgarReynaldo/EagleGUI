



#include "SceneManager.hpp"


bool SceneManager::Init(EagleGraphicsContext* window) {
   title_scene.Init(window);
   return true;
}



void SceneManager::Display(EagleGraphicsContext* win) {
   current_scene->Display(win);
}



int SceneManager::HandleEvent(EagleEvent ev) {
   if (ev.type == EAGLE_EVENT_DISPLAY_CLOSE) {
      return SCENE_COMPLETE;
   }
   if (current_scene->HandleEvent(ev) == SCENE_COMPLETE) {
      current_scene = NextScene();
   }
}



int SceneManager::Update(double tsec) {
   current_scene->Update(tsec);
   return SCENE_RUNNING;
}





