


#ifndef TitleScene_HPP
#define TitleScene_HPP



#include "Scene.hpp"
#include "Eagle.hpp"



class TitleScene : public Scene {
   
protected :

   EagleGraphicsContext* our_window;

   EagleImage* buffer;
   EagleImage* landscape;
   EagleImage* lolbunnylogo_image;
   EagleFont* krampus_font;
   
   WidgetHandler gui;
   RelativeLayout layout;
   BasicText text;
   
   double hold_time;
   double fade_time;
   double scene_time;
   double lol_move_in_time;
   double lol_stay_time;
   double lol_move_right_time;
   double lol_finish_time;
   
   double lol_dest_x;
   
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
         lol_move_in_time(0.0),
         lol_stay_time(0.0),
         lol_move_right_time(0.0),
         lol_dest_x(0.0),
         time_elapsed(0.0)
   {}
   
   
   virtual bool Init(EagleGraphicsContext* window);
   virtual void Free(EagleGraphicsContext* window);

   
   virtual void Display(EagleGraphicsContext* win);
   virtual int HandleEvent(EagleEvent ev);
   virtual int Update(double tsec);

   void ResetText();
};




#endif // TitleScene_HPP

