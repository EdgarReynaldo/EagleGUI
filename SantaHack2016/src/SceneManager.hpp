




#ifndef SceneManager_HPP
#define SceneManager_HPP

#include "Scene.hpp"

#include "TitleScene.hpp"



class SceneManager : public Scene {
   
protected :
   
   TitleScene title_scene;
   
   Scene* current_scene;
   
   
///   Scene* NextScene(Scene* current);
Scene* NextScene(Scene* current) {
   if (current == &title_scene) {
      return NULL;
   }
   return NULL;
}
   
public :
   
   virtual ~SceneManager() {}
   
   virtual bool Init(EagleGraphicsContext* window);
   
   virtual void Display(EagleGraphicsContext* win);
   virtual int HandleEvent(EagleEvent ev);
   virtual int Update(double tsec);
   
   
   
};



#endif // SceneManager_HPP

