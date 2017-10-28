


#ifndef AnimationData_HPP
#define AnimationData_HPP

#include "Eagle/AnimationBase.hpp"


class EagleGraphicsContext;


class Animation : public AnimationBase {

protected :
   virtual void OnSetAnimationPercent()=0;

   virtual void OnLoopComplete()=0;
   virtual void OnComplete()=0;

public :
   
   Animation();
   
   virtual void Draw(EagleGraphicsContext* win , int x , int y)=0;

};


#include "Eagle/Position.hpp"


typedef Pos2D(*POS2D_FUNC)(double);



class XYAnimation : public Animation {

protected :
   POS2D_FUNC posfunc;
   
   Pos2D pos;
   
   void OnSetAnimationPercent();

public :
   XYAnimation(); 
   XYAnimation(POS2D_FUNC pfunc); 
   
   void Create(POS2D_FUNC pfunc);
   
   virtual void Draw(EagleGraphicsContext* win , int x , int y)=0;
};

#include <string>

class EagleFont;


typedef EagleColor(*COLOR_FUNC)(double);


class XYTextAnimation : public XYAnimation {

protected :
   EagleFont* text_font;
   std::string text_string;
   EagleColor text_color;
   
   COLOR_FUNC colfunc;

   virtual void OnSetAnimationPercent();

public :
   
   XYTextAnimation();
   XYTextAnimation(EagleFont* f , std::string s , COLOR_FUNC cfunc , POS2D_FUNC pfunc);

   void Create(EagleFont* f , std::string s , COLOR_FUNC cfunc , POS2D_FUNC pfunc);

   
   virtual void Draw(EagleGraphicsContext* win , int x , int y);

   void OnLoopComplete();
   void OnComplete();
};

#include "Eagle/Transforms.hpp"

typedef Transform (*TRANS_FUNC) (double);

class TextAnimation : public XYTextAnimation {
   
protected :
   TRANS_FUNC tfunc;
   
   Transform trans;
   
   virtual void OnSetAnimationPercent();
public :
   
   TextAnimation();

   TextAnimation(EagleFont* f , std::string s , TRANS_FUNC transfunc , COLOR_FUNC cfunc , POS2D_FUNC pfunc);

   void Create(EagleFont* f , std::string s , TRANS_FUNC transfunc , COLOR_FUNC cfunc , POS2D_FUNC pfunc);
   
   virtual void Draw(EagleGraphicsContext* win , int x , int y);

};



#endif // AnimationData_HPP

