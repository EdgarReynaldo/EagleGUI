


#ifndef TitleScene_HPP
#define TitleScene_HPP



#include "Scene.hpp"
#include "Eagle.hpp"



class TitleScene : public Scene {
   
protected :

   EagleGraphicsContext* our_window;

   EagleImage* buffer;
   EagleImage* landscape;
   EagleFont* krampus_font;
   
   WidgetHandler gui;
   RelativeLayout layout;
   BasicText text;
   
   double hold_time;
   double fade_time;
   double scene_time;
   double time_elapsed;
   
   
public :
///   TitleScene();
   TitleScene() :
         our_window(0),
         buffer(0),
         landscape(0),
         krampus_font(0),
         gui(0),
         layout(),
         text(),
         hold_time(0.0),
         fade_time(0.0),
         scene_time(0.0),
         time_elapsed(0.0)
   {}
   
   
   virtual bool Init(EagleGraphicsContext* window);
   virtual void Free(EagleGraphicsContext* window);

   
   virtual void Display(EagleGraphicsContext* win);
   virtual int HandleEvent(EagleEvent ev);
   virtual int Update(double tsec);

   
};




#endif // TitleScene_HPP

