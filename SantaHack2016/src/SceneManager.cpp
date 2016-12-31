

#include "PresentationScene.hpp"
#include "TitleScene.hpp"
#include "Lol.hpp"
#include "MapScene.hpp"


#include "SceneManager.hpp"


TitleScene title_scene;

PresentationScene presentation_scene;

LolMaker lol_maker;

MapScene map_scene;

Scene* SceneManager::NextScene(Scene* current) {
   Scene* next_scene = 0;
   if (current) {
      current->Free(current_window);
   }
   if (!current) {
      next_scene = &presentation_scene;
///      next_scene = &title_scene;
   }
   if (current == &presentation_scene) {
      next_scene = &title_scene;
   }
   if (current == &title_scene) {
      next_scene = &map_scene;
///      next_scene = &lol_maker;
   }
   if (current == &map_scene) {
      next_scene = 0;
   }
///   if (current == &lol_maker) {
///      next_scene = 0;
///   }
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
   else if (state == STATE_QUIT) {
      return STATE_QUIT;
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
   else if (state == STATE_QUIT) {
      return STATE_QUIT;
   }
   return STATE_RUNNING;
}




void SceneManager::Run() {
   while (!quit) {

      eagle_system->GetSystemQueue()->Clear();
      eagle_system->GetSystemTimer()->Start();

      while (!quit) {
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
      while (!next && !quit) {
      
         int state = STATE_RUNNING;
         current_scene->Display(current_window);
         do {
            EagleEvent ev;
            ev = eagle_system->WaitForSystemEventAndUpdateState();
            if (ev.type == EAGLE_EVENT_TIMER && ev.timer.eagle_timer_source == eagle_system->GetSystemTimer()) {
               int state = current_scene->Update(ev.timer.eagle_timer_source->SPT());
               if (state == STATE_COMPLETE) {
                  current_scene = NextScene(current_scene);
                  if (!current_scene) {
                     return;
                  }
                  else {
                     next = true;
                     break;
                  }
               }
               else if (state == STATE_QUIT) {
                  quit = true;
               }
            }
            else {
               state = current_scene->HandleEvent(ev);
               if (state == STATE_COMPLETE) {
                  current_scene = NextScene(current_scene);
                  if (!current_scene) {
                     return;
                  }
                  else {
                     next = true;
                     break;
                  }
               }
               else if (state == STATE_QUIT) {
                  quit = true;
               }
            }
         } while (!eagle_system->UpToDate());
      }
   }
}








