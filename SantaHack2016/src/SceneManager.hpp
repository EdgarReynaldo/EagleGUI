




#ifndef SceneManager_HPP
#define SceneManager_HPP

#include "Scene.hpp"

#include "TitleScene.hpp"


///   Scene* NextScene(Scene* current);


class SceneManager : public Scene {
   
protected :
   
   EagleGraphicsContext* current_window;
   Scene* current_scene;
   
   
   
public :
   
///   SceneManager();
   SceneManager() :
         current_window(0),
         current_scene(0)
   {}
   
   virtual ~SceneManager() {}
   
   virtual Scene* NextScene(Scene* current);

   virtual bool Init(EagleGraphicsContext* window);
   virtual void Free(EagleGraphicsContext* window);
   
   virtual void Display(EagleGraphicsContext* win);
   virtual int HandleEvent(EagleEvent ev);
   virtual int Update(double tsec);
   
   virtual void Run();

};








#endif // SceneManager_HPP

