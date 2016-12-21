


#include "TitleScene.hpp"
#include "SceneManager.hpp"

TitleScene title_scene;




Scene* SceneManager::NextScene(Scene* current) {
   Scene* next_scene = 0;
   if (current) {
      current->Free(current_window);
   }
   if (!current) {
      next_scene = &title_scene;
   }
   if (current == &title_scene) {
      next_scene = 0;
   }
   if (next_scene) {
      next_scene->Init(current_window);
   }
   return next_scene;
}


   
bool SceneManager::Init(EagleGraphicsContext* window) {
   current_window = window;
   current_scene = NextScene(0);
   if (current_scene) {
      return true;
   }
   return false;
}



void SceneManager::Free(EagleGraphicsContext* window) {
   (void)window;
   current_window = 0;
}



void SceneManager::Display(EagleGraphicsContext* win) {
   EAGLE_ASSERT(current_scene);
   current_scene->Display(win);
}



int SceneManager::HandleEvent(EagleEvent ev) {
   EAGLE_ASSERT(current_scene);
   int state = current_scene->HandleEvent(ev);
   if (state == STATE_COMPLETE) {
      current_scene = NextScene(current_scene);
      if (!current_scene) {
         return STATE_COMPLETE;
      }
   }
   return state;
}



int SceneManager::Update(double tsec) {
   EAGLE_ASSERT(current_scene);
   int state = current_scene->Update(tsec);
   if (state == STATE_COMPLETE) {
      current_scene = NextScene(current_scene);
      if (!current_scene) {
         return STATE_COMPLETE;
      }
   }
   return STATE_RUNNING;
}




void SceneManager::Run() {
   eagle_system->GetSystemQueue()->Clear();
   eagle_system->GetSystemTimer()->Start();
   while (1) {
      SceneManager* sm = dynamic_cast<SceneManager*>(current_scene);
      if (!sm) {
         break;
      }
      sm->Run();
      current_scene = NextScene(current_scene);
      if (!current_scene) {
         return;
      }
      continue;
   }
   bool next = false;
   while (!next) {
   
      current_scene->Display(current_window);
      do {
         EagleEvent ev;
         ev = eagle_system->WaitForSystemEventAndUpdateState();
         if (ev.type == EAGLE_EVENT_TIMER && ev.timer.eagle_timer_source == eagle_system->GetSystemTimer()) {
            if (current_scene->Update(ev.timer.eagle_timer_source->SPT()) == STATE_COMPLETE) {
               current_scene = NextScene(current_scene);
               if (!current_scene) {
                  return;
               }
               else {
                  next = true;
                  break;
               }
            }
         }
         else {
            if (current_scene->HandleEvent(ev) == STATE_COMPLETE) {
               current_scene = NextScene(current_scene);
               if (!current_scene) {
                  return;
               }
               else {
                  next = true;
                  break;
               }
            }
         }
      } while (!eagle_system->UpToDate());
   }
}








